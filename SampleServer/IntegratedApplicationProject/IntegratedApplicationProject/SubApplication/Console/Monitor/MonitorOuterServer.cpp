#include "stdafx.h"
#include "Monitor.h"
#include "MonitorOuterServer.h"

MonitorOuterServer::MonitorOuterServer()
{
	CurrentClientCount = 0;
	ConnectedClientCount = 0;
	DisconnectedClientCount = 0;

	CurrentWorkCount = 0;
	EnqueuedWorkCount = 0;
	DequeuedWorkCount = 0;

	ReceiveCount = 0;
	ReceivedSize = 0;
	SendCount = 0;
	SendedSize = 0;

	FOR(i, sizeof(CheckPoint) / sizeof(UINT))
	{
		CheckPoint[i] = 0;
	}

	InitializeSRWLock(&MCAL_LOCK);
}

MonitorOuterServer::~MonitorOuterServer() {}

bool MonitorOuterServer::Initialize()
{
	{
		CurrentClientCount = 0;
		ConnectedClientCount = 0;
		DisconnectedClientCount = 0;

		CurrentWorkCount = 0;
		EnqueuedWorkCount = 0;
		DequeuedWorkCount = 0;

		ReceiveCount = 0;
		ReceivedSize = 0;
		SendCount = 0;
		SendedSize = 0;

		FOR(i, sizeof(CheckPoint) / sizeof(UINT))
		{
			CheckPoint[i] = 0;
		}

		InitializeSRWLock(&MCAL_LOCK);
	}

	{
		FILE* jsonFile;
		GenericDocument<UTF16LE<>> jsonDocument;
		WCHAR* jsonBuffer;
		UINT jsonFileSize;

		_wfopen_s(&jsonFile, L"MonitorServer.ini", L"r, ccs=UTF-16LE");

		jsonFileSize = _filelength(_fileno(jsonFile));

		jsonBuffer = (WCHAR*)malloc(jsonFileSize);
		ZeroMemory(jsonBuffer, jsonFileSize);
		fread_s(jsonBuffer, jsonFileSize, jsonFileSize, 1, jsonFile);
		fclose(jsonFile);

		jsonDocument.Parse(jsonBuffer);

		{
			wstring ipAddress;
			UINT port;
			UINT workThreadCount;
			UINT maximumClientCount;

			ipAddress = jsonDocument[L"MonitorOuterServer"][L"IP_Address"].GetString();
			port = jsonDocument[L"MonitorOuterServer"][L"Port"].GetUint();
			workThreadCount = jsonDocument[L"MonitorOuterServer"][L"WorkThreadCount"].GetUint();
			maximumClientCount = jsonDocument[L"MonitorOuterServer"][L"MaximumClientCount"].GetUint();

			LoginKey = jsonDocument[L"MonitorOuterServer"][L"LoginKey"].GetString();

			Start(ipAddress.c_str(), port, workThreadCount, 1, maximumClientCount); //IOCP_Server::Initialize();
		}

		free(jsonBuffer);
	}

	{
		//MC_TLP.Initialize();
		//MW_TLP.Initialize();

		TLDH.Initialize();
		//TLDH.Connect();

		InnerServer.Initialize();
	}

	return 0;
}

bool MonitorOuterServer::Update()
{
	{
		if (TIMER.CheckUpdate())
		{
			if (GetAsyncKeyState(VK_F1)) THREAD_CONTEXT_SWITCH_ON = !THREAD_CONTEXT_SWITCH_ON;
			if (GetAsyncKeyState(VK_F2)) INDICATOR_ON = !INDICATOR_ON;
			if (GetAsyncKeyState(VK_F4)) return 1;
		}
	}

	//while(1)
	{
		while (1) //while (ConnectorSessionKeyQ.GetCurrentCount())
		{
			MonitorClient* monitorClientAddress;
			UINT64 connectorSessionKey;
			
			connectorSessionKey = ConnectorSessionKeyQ.Dequeue();
			if (connectorSessionKey)
			{
				monitorClientAddress = MC_TLP.Allocate();

				ZeroMemory(monitorClientAddress, sizeof(*monitorClientAddress));

				monitorClientAddress->SessionKey = connectorSessionKey;
				InitializeSRWLock(&monitorClientAddress->Lock);

				MCAL.push_back(monitorClientAddress);
			}
			else break;
		}

		while (1) //while (DisconnectorSessionKeyQ.GetCurrentCount())
		{
			MonitorClient* monitorClientAddress;
			UINT64 disconnectorSessionKey;

			disconnectorSessionKey = DisconnectorSessionKeyQ.Dequeue();
			if (disconnectorSessionKey)
			{
				for (MonitorClientAddressLI mcaLI = MCAL.begin(); mcaLI != MCAL.end(); ++mcaLI)
				{
					if ((*mcaLI)->SessionKey == disconnectorSessionKey)
					{
						monitorClientAddress = (*mcaLI);

						MCAL.erase(mcaLI);
						MC_TLP.Deallocate(monitorClientAddress);

						break;
					}
				}
			}
			else break;
		}

		if (InnerServer.WorkLFQ.GetCurrentCount())
		{
			InnerWork receivedWork;
			MonitorWork work;
			MonitorWork sendingWork;

			InterlockedDecrement(&InnerServer.CurrentWorkCount);
			++InnerServer.DequeuedWorkCount;
			receivedWork = InnerServer.WorkLFQ.Dequeue();

			work.SessionKey = receivedWork.SessionKey;
			work.PacketSPQ_Address = receivedWork.PacketSPQ_Address;

			sendingWork = ProcessWork(nullptr, work);
		}

		if (WorkLFQ.GetCurrentCount()) //while (WorkLFQ.GetCurrentCount())
		{
			MonitorClient* monitorClientAddress;
			MonitorWork receivedWork;
			MonitorWork sendingWork;

			monitorClientAddress = nullptr;

			InterlockedDecrement(&CurrentWorkCount);
			++DequeuedWorkCount;
			receivedWork = WorkLFQ.Dequeue();

			for (MonitorClientAddressLI mcaLI = MCAL.begin(); mcaLI != MCAL.end(); ++mcaLI)
			{
				if ((*mcaLI)->SessionKey == receivedWork.SessionKey)
				{
					monitorClientAddress = (*mcaLI);

					break;
				}
			}

			if (monitorClientAddress) sendingWork = ProcessWork(monitorClientAddress, receivedWork);
			else
			{
				if (!receivedWork.PacketSPQ_Address->DecreaseReferenceCount())
				{
					InterlockedDecrement(&CURRENT_SPQ_COUNT);
					InterlockedIncrement(&DEALLOCATED_SPQ_COUNT);
					PACKET_SPQ_TLP.Deallocate(receivedWork.PacketSPQ_Address);
				}
			}
		}
		//else Sleep(0);
		//else break;

		//Check heartbeat
		//{
		//	DWORD currentHeartbeatTime;
		//
		//	currentHeartbeatTime = timeGetTime();
		//
		//	for (MonitorClientAddressLI mcaLI = MCAL.begin(); mcaLI != MCAL.end(); ++mcaLI)
		//	{
		//		if ((*mcaLI)->PreviousHeartbeatTime < currentHeartbeatTime - 30000)
		//		{
		//			Disconnect((*mcaLI)->SessionKey);
		//		}
		//	}
		//
		//	for (MonitorClientAddressLI mcaLI = MCAL.begin(); mcaLI != MCAL.end(); ++mcaLI)
		//	{
		//		if ((*mcaLI)->PreviousHeartbeatTime < currentHeartbeatTime - 30000)
		//		{
		//			Disconnect((*mcaLI)->SessionKey);
		//		}
		//	}
		//}
	}

	{
		InnerServer.Update();
	}

	Sleep(10);

	return 0;
}

bool MonitorOuterServer::Render()
{
	if (TIMER.ShowCounter())
	{
		printf(
			"====== MonitorOuterServer =====" CRALF
			"CurrentClientCount : %u" CRALF
			"ConnectedClientCount : %u" CRALF
			"DisconnectedClientCount : %u" CRALF
			CRALF
			"CurrentWorkCount : %u" CRALF
			"EnqueuedWorkCount : %u" CRALF
			"DequeuedWorkCount : %u" CRALF
			CRALF
			"ReceiveCount : %u" CRALF
			"ReceivedSize : %u" CRALF
			"SendCount : %u" CRALF
			"SendedSize : %u" CRALF
			CRALF
			"CURRENT_SPQ_COUNT : %u" CRALF
			"ALLOCATED_SPQ_COUNT : %u" CRALF
			"DEALLOCATED_SPQ_COUNT : %u" CRALF
			"===== ===== =====" CRALF
			CRALF
			,
			CurrentClientCount,
			ConnectedClientCount,
			DisconnectedClientCount,

			CurrentWorkCount,
			EnqueuedWorkCount,
			DequeuedWorkCount,

			ReceiveCount,
			ReceivedSize,
			SendCount,
			SendedSize,

			CURRENT_SPQ_COUNT,
			ALLOCATED_SPQ_COUNT,
			DEALLOCATED_SPQ_COUNT
			);

		if (INDICATOR_ON)
		{
			printf(
				"CheckPoint[0] : %u" CRALF
				"CheckPoint[1] : %u" CRALF
				"CheckPoint[2] : %u" CRALF
				"CheckPoint[3] : %u" CRALF
				"CheckPoint[4] : %u" CRALF
				"CheckPoint[5] : %u" CRALF
				"CheckPoint[6] : %u" CRALF
				"CheckPoint[7] : %u" CRALF
				"CheckPoint[8] : %u" CRALF
				"CheckPoint[9] : %u" CRALF
				"CheckPoint[10] : %u" CRALF
				"CheckPoint[11] : %u" CRALF
				CRALF
				,
				CheckPoint[0],
				CheckPoint[1],
				CheckPoint[2],
				CheckPoint[3],
				CheckPoint[4],
				CheckPoint[5],
				CheckPoint[6],
				CheckPoint[7],
				CheckPoint[8],
				CheckPoint[9],
				CheckPoint[10],
				CheckPoint[11]
				);
		}

		InnerServer.Render();
	}

	{

	}

	return 0;
}

bool MonitorOuterServer::Terminalize()
{
	{
		Stop();
	}

	{
		TLDH.Disconnect();
		TLDH.Terminalize();
	}

	{
		InnerServer.Terminalize();
	}

	return 0;
}