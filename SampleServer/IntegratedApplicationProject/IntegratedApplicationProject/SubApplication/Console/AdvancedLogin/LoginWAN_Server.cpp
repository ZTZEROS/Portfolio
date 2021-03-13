#include "stdafx.h"
#include "AdvancedLogin.h"
#include "LoginWAN_Server.h"

AdvancedLoginClientPlayerAddressL LoginWAN_Server::ALCPAL;
SRWLOCK LoginWAN_Server::ALCPAL_LOCK;

ThreadLocalPool<AdvancedLoginClientPlayer> LoginWAN_Server::ALCP_TLP;
ThreadLocalPool<AdvancedLoginWork> LoginWAN_Server::ALW_TLP;

ThreadLocalDatabaseHandler LoginWAN_Server::TLDH;

LoginWAN_Server::LoginWAN_Server()
{
	GameServerIP_Address.clear();
	GameServerPort = 0;
	ChatServerIP_Address.clear();
	ChatServerPort = 0;

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
}

LoginWAN_Server::~LoginWAN_Server() {}

bool LoginWAN_Server::Initialize()
{
	{
		GameServerIP_Address.clear();
		GameServerPort = 0;
		ChatServerIP_Address.clear();
		ChatServerPort = 0;

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
	}

	{
		FILE* jsonFile;
		GenericDocument<UTF16LE<>> jsonDocument;
		WCHAR* jsonBuffer;
		UINT jsonFileSize;

		_wfopen_s(&jsonFile, L"LoginWAN.ini", L"r, ccs=UTF-16LE");

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

			ipAddress = jsonDocument[L"IP_Address"].GetString();
			port = jsonDocument[L"Port"].GetUint();
			workThreadCount = jsonDocument[L"WorkThreadCount"].GetUint();
			maximumClientCount = jsonDocument[L"MaximumClientCount"].GetUint();

			GameServerIP_Address = jsonDocument[L"GameServerIP_Address"].GetString();
			GameServerPort = jsonDocument[L"GameServerPort"].GetUint();
			ChatServerIP_Address = jsonDocument[L"ChatServerIP_Address"].GetString();
			ChatServerPort = jsonDocument[L"ChatServerPort"].GetUint();

			ConnectorSessionKeyQ.Resize(maximumClientCount);
			DisconnectorSessionKeyQ.Resize(maximumClientCount);

			Start(ipAddress.c_str(), port, workThreadCount, 1, maximumClientCount); //IOCP_Server::Initialize();
		}

		free(jsonBuffer);
	}

	{
		//ALCP_TLP.Initialize();
		TLDH.Initialize();
		TLDH.Connect();

		LAN_Client.Initialize();
	}

	return 0;
}

bool LoginWAN_Server::Update()
{
	if (TIMER.CheckUpdate());

	{
		{
			if (GetAsyncKeyState(VK_F5)) THREAD_CONTEXT_SWITCH_ON = !THREAD_CONTEXT_SWITCH_ON;
			if (GetAsyncKeyState(VK_F6)) INDICATOR_ON = !INDICATOR_ON;
			if (GetAsyncKeyState(VK_F8)) return 1;
		}

		LAN_Client.Update();
	}

	{
		while (1) //while (ConnectorSessionKeyQ.GetCurrentCount())
		{
			AdvancedLoginClientPlayer* clientPlayerAddress;
			UINT64 connectorSessionKey;

			connectorSessionKey = ConnectorSessionKeyQ.Dequeue();
			if (connectorSessionKey)
			{
				clientPlayerAddress = ALCP_TLP.Allocate();

				ZeroMemory(clientPlayerAddress, sizeof(*clientPlayerAddress));

				clientPlayerAddress->SessionKey = connectorSessionKey;
				InitializeSRWLock(&clientPlayerAddress->Lock);

				ALCPAL.push_back(clientPlayerAddress);
			}
			else break;
		}

		while (1) //while (DisconnectorSessionKeyQ.GetCurrentCount());
		{
			AdvancedLoginClientPlayer* clientPlayerAddress;
			UINT64 disconnectorSessionKey;

			disconnectorSessionKey = DisconnectorSessionKeyQ.Dequeue();
			if (disconnectorSessionKey)
			{
				for (AdvancedLoginClientPlayerAddressLI cpaLI = ALCPAL.begin(); cpaLI != ALCPAL.end(); ++cpaLI)
				{
					if ((*cpaLI)->SessionKey == disconnectorSessionKey)
					{
						clientPlayerAddress = (*cpaLI);

						ALCPAL.erase(cpaLI);
						ALCP_TLP.Deallocate(clientPlayerAddress);

						break;
					}
				}
			}
			else break;
		}

		if (LAN_Client.WorkLFQ.GetCurrentCount())
		{
			while (1) //while (ConnectorSessionKeyQ.GetCurrentCount())
			{
				AdvancedLoginClientPlayer* clientPlayerAddress;
				UINT64 connectorSessionKey;

				connectorSessionKey = ConnectorSessionKeyQ.Dequeue();
				if (connectorSessionKey)
				{
					clientPlayerAddress = ALCP_TLP.Allocate();

					ZeroMemory(clientPlayerAddress, sizeof(*clientPlayerAddress));

					clientPlayerAddress->SessionKey = connectorSessionKey;
					InitializeSRWLock(&clientPlayerAddress->Lock);

					ALCPAL.push_back(clientPlayerAddress);
				}
				else break;
			}

			while (1) //while (DisconnectorSessionKeyQ.GetCurrentCount());
			{
				AdvancedLoginClientPlayer* clientPlayerAddress;
				UINT64 disconnectorSessionKey;

				disconnectorSessionKey = DisconnectorSessionKeyQ.Dequeue();
				if (disconnectorSessionKey)
				{
					for (AdvancedLoginClientPlayerAddressLI cpaLI = ALCPAL.begin(); cpaLI != ALCPAL.end(); ++cpaLI)
					{
						if ((*cpaLI)->SessionKey == disconnectorSessionKey)
						{
							clientPlayerAddress = (*cpaLI);

							ALCPAL.erase(cpaLI);
							ALCP_TLP.Deallocate(clientPlayerAddress);

							break;
						}
					}
				}
				else break;
			}

			{
				AdvancedLoginWork receivedWork;
				AdvancedLoginWork sendingWork;

				InterlockedDecrement(&LAN_Client.CurrentWorkCount);
				++LAN_Client.DequeuedWorkCount;
				receivedWork = LAN_Client.WorkLFQ.Dequeue();

				sendingWork = ProcessWork(receivedWork);
			}
		}

		//while (WorkLFQ.GetCurrentCount())
		if (WorkLFQ.GetCurrentCount())
		{
			while (1) //while (ConnectorSessionKeyQ.GetCurrentCount())
			{
				AdvancedLoginClientPlayer* clientPlayerAddress;
				UINT64 connectorSessionKey;

				connectorSessionKey = ConnectorSessionKeyQ.Dequeue();
				if (connectorSessionKey)
				{
					clientPlayerAddress = ALCP_TLP.Allocate();

					ZeroMemory(clientPlayerAddress, sizeof(*clientPlayerAddress));

					clientPlayerAddress->SessionKey = connectorSessionKey;
					InitializeSRWLock(&clientPlayerAddress->Lock);

					ALCPAL.push_back(clientPlayerAddress);
				}
				else break;
			}

			while (1) //while (DisconnectorSessionKeyQ.GetCurrentCount());
			{
				AdvancedLoginClientPlayer* clientPlayerAddress;
				UINT64 disconnectorSessionKey;

				disconnectorSessionKey = DisconnectorSessionKeyQ.Dequeue();
				if (disconnectorSessionKey)
				{
					for (AdvancedLoginClientPlayerAddressLI cpaLI = ALCPAL.begin(); cpaLI != ALCPAL.end(); ++cpaLI)
					{
						if ((*cpaLI)->SessionKey == disconnectorSessionKey)
						{
							clientPlayerAddress = (*cpaLI);

							ALCPAL.erase(cpaLI);
							ALCP_TLP.Deallocate(clientPlayerAddress);

							break;
						}
					}
				}
				else break;
			}

			{
				AdvancedLoginClientPlayer* clientPlayerAddress;
				AdvancedLoginWork receivedWork;
				AdvancedLoginWork sendingWork;

				clientPlayerAddress = nullptr;

				InterlockedDecrement(&CurrentWorkCount);
				++DequeuedWorkCount;
				receivedWork = WorkLFQ.Dequeue();

				for (AdvancedLoginClientPlayerAddressLI cpaLI = ALCPAL.begin(); cpaLI != ALCPAL.end(); ++cpaLI)
				{
					if ((*cpaLI)->SessionKey == receivedWork.SessionKey)
					{
						clientPlayerAddress = (*cpaLI);

						break;
					}
				}

				if (clientPlayerAddress) sendingWork = LAN_Client.ProcessWork(clientPlayerAddress, receivedWork);
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
		}
		//else Sleep(0);
		//else break;

		//Check heartbeat
		//{
		//	DWORD currentHeartbeatTime;
		//
		//	currentHeartbeatTime = timeGetTime();
		//
		//	for (AdvancedLoginClientPlayerAddressLI cpaLI = ALCPAL.begin(); cpaLI != ALCPAL.end(); ++cpaLI)
		//	{
		//		if ((*cpaLI)->PreviousHeartbeatTime < currentHeartbeatTime - 30000)
		//		{
		//			Disconnect((*cpaLI)->SessionKey);
		//		}
		//	}
		//}
	}

	{
		//LAN_Client.Update();
	}

	return 0;
}

bool LoginWAN_Server::Render()
{
	if (TIMER.ShowCounter()) //CheckRender()
	{
		printf(
			"===== LoginWAN_Server ======" CRALF
			"CurrentClientCount : %u" CRALF
			"ConnectionCount : %u" CRALF
			"DisconnectionCount : %u" CRALF
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

		LAN_Client.Render();
	}

	{
		//LAN_Client.Render(); //use after repair TIMER
	}

	return 0;
}

bool LoginWAN_Server::Terminalize()
{
	{
		Stop(); //IOCP_Server::Terminalize();
	}

	{
		TLDH.Disconnect();
		TLDH.Terminalize();
	}

	{
		LAN_Client.Terminalize();
	}

	return 0;
}