#include "stdafx.h"
#include "AdvancedChat.h"
#include "ChatWAN_Server.h"

AdvancedChatClientPlayerAddressL ChatWAN_Server::ACCPAL;
SRWLOCK ChatWAN_Server::ACCPAL_LOCK;

AdvancedChatSector ChatWAN_Server::AC_SECTOR_A[ADVANCED_CHAT_SECTOR_HEIGHT][ADVANCED_CHAT_SECTOR_WIDTH];

ThreadLocalPool<AdvancedChatClientPlayer> ChatWAN_Server::ACCP_TLP;
ThreadLocalPool<AdvancedChatWork> ChatWAN_Server::ACW_TLP;

ThreadLocalDatabaseHandler ChatWAN_Server::TLDH;

ChatWAN_Server::ChatWAN_Server()
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

	FOR(i, ADVANCED_CHAT_SECTOR_HEIGHT)
	{
		FOR(j, ADVANCED_CHAT_SECTOR_WIDTH)
		{
			InitializeSRWLock(&AC_SECTOR_A[i][j].Lock);
		}
	}
}

ChatWAN_Server::~ChatWAN_Server() {}

bool ChatWAN_Server::Initialize()
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

		FOR(i, ADVANCED_CHAT_SECTOR_HEIGHT)
		{
			FOR(j, ADVANCED_CHAT_SECTOR_WIDTH)
			{
				InitializeSRWLock(&AC_SECTOR_A[i][j].Lock);
			}
		}
	}

	{
		FILE* jsonFile;
		GenericDocument<UTF16LE<>> jsonDocument;
		WCHAR* jsonBuffer;
		UINT jsonFileSize;

		_wfopen_s(&jsonFile, L"ChatWAN.ini", L"r, ccs=UTF-16LE");

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

			ConnectorSessionKeyQ.Resize(maximumClientCount);
			DisconnectorSessionKeyQ.Resize(maximumClientCount);

			Start(ipAddress.c_str(), port, workThreadCount, 1, maximumClientCount); //IOCP_Server::Initialize();
		}

		free(jsonBuffer);
	}

	{
		//ACCP_TLP.Initialize();
		TLDH.Initialize();
		//TLDH.Connect();

		LAN_Server.Initialize();
	}

	return 0;
}

bool ChatWAN_Server::Update()
{
	if (TIMER.CheckUpdate());

	{
		{
			if (GetAsyncKeyState(VK_F1)) THREAD_CONTEXT_SWITCH_ON = !THREAD_CONTEXT_SWITCH_ON;
			if (GetAsyncKeyState(VK_F2)) INDICATOR_ON = !INDICATOR_ON;
			if (GetAsyncKeyState(VK_F4)) return 1;
		}

		LAN_Server.Update();
	}

	//while(1)
	{
		while (1) //while (ConnectorSessionKeyQ.GetCurrentCount())
		{
			AdvancedChatClientPlayer* clientPlayerAddress;
			UINT64 connectorSessionKey;

			connectorSessionKey = ConnectorSessionKeyQ.Dequeue();
			if (connectorSessionKey)
			{
				clientPlayerAddress = ACCP_TLP.Allocate();

				ZeroMemory(clientPlayerAddress, sizeof(*clientPlayerAddress));

				clientPlayerAddress->SessionKey = connectorSessionKey;
				InitializeSRWLock(&clientPlayerAddress->Lock);

				ACCPAL.push_back(clientPlayerAddress);
			}
			else break;
		}

		while (1) //while (DisconnectorSessionKeyQ.GetCurrentCount())
		{
			AdvancedChatClientPlayer* clientPlayerAddress;
			UINT64 disconnectorSessionKey;

			disconnectorSessionKey = DisconnectorSessionKeyQ.Dequeue();
			if (disconnectorSessionKey)
			{
				for (AdvancedChatClientPlayerAddressLI cpaLI = ACCPAL.begin(); cpaLI != ACCPAL.end(); ++cpaLI)
				{
					if ((*cpaLI)->SessionKey == disconnectorSessionKey)
					{
						clientPlayerAddress = (*cpaLI);

						if (clientPlayerAddress->SectorIndexX < ADVANCED_CHAT_SECTOR_WIDTH && clientPlayerAddress->SectorIndexY < ADVANCED_CHAT_SECTOR_HEIGHT)
						{
							AdvancedChatClientPlayerAddressL* cpaL_Address;

							cpaL_Address = &AC_SECTOR_A[clientPlayerAddress->SectorIndexY][clientPlayerAddress->SectorIndexX].CPAL;

							for (AdvancedChatClientPlayerAddressLI cpaLI = cpaL_Address->begin(); cpaLI != cpaL_Address->end(); ++cpaLI)
							{
								if ((*cpaLI) == clientPlayerAddress)
								{
									cpaL_Address->erase(cpaLI);

									break;
								}
							}
						}

						ACCPAL.erase(cpaLI);
						ACCP_TLP.Deallocate(clientPlayerAddress);

						break;
					}
				}
			}
			else break;
		}

		if (LAN_Server.WorkLFQ.GetCurrentCount())
		{
			while (1) //while (ConnectorSessionKeyQ.GetCurrentCount())
			{
				AdvancedChatClientPlayer* clientPlayerAddress;
				UINT64 connectorSessionKey;

				connectorSessionKey = ConnectorSessionKeyQ.Dequeue();
				if (connectorSessionKey)
				{
					clientPlayerAddress = ACCP_TLP.Allocate();

					ZeroMemory(clientPlayerAddress, sizeof(*clientPlayerAddress));

					clientPlayerAddress->SessionKey = connectorSessionKey;
					InitializeSRWLock(&clientPlayerAddress->Lock);

					ACCPAL.push_back(clientPlayerAddress);
				}
				else break;
			}

			while (1) //while (DisconnectorSessionKeyQ.GetCurrentCount())
			{
				AdvancedChatClientPlayer* clientPlayerAddress;
				UINT64 disconnectorSessionKey;

				disconnectorSessionKey = DisconnectorSessionKeyQ.Dequeue();
				if (disconnectorSessionKey)
				{
					for (AdvancedChatClientPlayerAddressLI cpaLI = ACCPAL.begin(); cpaLI != ACCPAL.end(); ++cpaLI)
					{
						if ((*cpaLI)->SessionKey == disconnectorSessionKey)
						{
							clientPlayerAddress = (*cpaLI);

							if (clientPlayerAddress->SectorIndexX < ADVANCED_CHAT_SECTOR_WIDTH && clientPlayerAddress->SectorIndexY < ADVANCED_CHAT_SECTOR_HEIGHT)
							{
								AdvancedChatClientPlayerAddressL* cpaL_Address;

								cpaL_Address = &AC_SECTOR_A[clientPlayerAddress->SectorIndexY][clientPlayerAddress->SectorIndexX].CPAL;

								for (AdvancedChatClientPlayerAddressLI cpaLI = cpaL_Address->begin(); cpaLI != cpaL_Address->end(); ++cpaLI)
								{
									if ((*cpaLI) == clientPlayerAddress)
									{
										cpaL_Address->erase(cpaLI);

										break;
									}
								}
							}

							ACCPAL.erase(cpaLI);
							ACCP_TLP.Deallocate(clientPlayerAddress);

							break;
						}
					}
				}
				else break;
			}

			{
				AdvancedChatWork receivedWork;
				AdvancedChatWork sendingWork;

				InterlockedDecrement(&LAN_Server.CurrentWorkCount);
				++LAN_Server.DequeuedWorkCount;
				receivedWork = LAN_Server.WorkLFQ.Dequeue();

				LAN_Server.ProcessWork(receivedWork);
			}
		}

		if (WorkLFQ.GetCurrentCount()) //while (WorkLFQ.GetCurrentCount())
		{
			while (1) //while (ConnectorSessionKeyQ.GetCurrentCount())
			{
				AdvancedChatClientPlayer* clientPlayerAddress;
				UINT64 connectorSessionKey;

				connectorSessionKey = ConnectorSessionKeyQ.Dequeue();
				if (connectorSessionKey)
				{
					clientPlayerAddress = ACCP_TLP.Allocate();

					ZeroMemory(clientPlayerAddress, sizeof(*clientPlayerAddress));

					clientPlayerAddress->SessionKey = connectorSessionKey;
					InitializeSRWLock(&clientPlayerAddress->Lock);

					ACCPAL.push_back(clientPlayerAddress);
				}
				else break;
			}

			while (1) //while (DisconnectorSessionKeyQ.GetCurrentCount())
			{
				AdvancedChatClientPlayer* clientPlayerAddress;
				UINT64 disconnectorSessionKey;

				disconnectorSessionKey = DisconnectorSessionKeyQ.Dequeue();
				if (disconnectorSessionKey)
				{
					for (AdvancedChatClientPlayerAddressLI cpaLI = ACCPAL.begin(); cpaLI != ACCPAL.end(); ++cpaLI)
					{
						if ((*cpaLI)->SessionKey == disconnectorSessionKey)
						{
							clientPlayerAddress = (*cpaLI);

							if (clientPlayerAddress->SectorIndexX < ADVANCED_CHAT_SECTOR_WIDTH && clientPlayerAddress->SectorIndexY < ADVANCED_CHAT_SECTOR_HEIGHT)
							{
								AdvancedChatClientPlayerAddressL* cpaL_Address;

								cpaL_Address = &AC_SECTOR_A[clientPlayerAddress->SectorIndexY][clientPlayerAddress->SectorIndexX].CPAL;

								for (AdvancedChatClientPlayerAddressLI cpaLI = cpaL_Address->begin(); cpaLI != cpaL_Address->end(); ++cpaLI)
								{
									if ((*cpaLI) == clientPlayerAddress)
									{
										cpaL_Address->erase(cpaLI);
										
										break;
									}
								}
							}

							ACCPAL.erase(cpaLI);
							ACCP_TLP.Deallocate(clientPlayerAddress);

							break;
						}
					}
				}
				else break;
			}

			{
				AdvancedChatClientPlayer* clientPlayerAddress;
				AdvancedChatWork receivedWork;
				AdvancedChatWork sendingWork;

				clientPlayerAddress = nullptr;

				InterlockedDecrement(&CurrentWorkCount);
				++DequeuedWorkCount;
				receivedWork = WorkLFQ.Dequeue();

				for (AdvancedChatClientPlayerAddressLI cpaLI = ACCPAL.begin(); cpaLI != ACCPAL.end(); ++cpaLI)
				{
					if ((*cpaLI)->SessionKey == receivedWork.SessionKey)
					{
						clientPlayerAddress = (*cpaLI);

						break;
					}
				}

				if (clientPlayerAddress) sendingWork = ProcessWork(clientPlayerAddress, receivedWork);
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
		//	for (AdvancedChatClientPlayerAddressLI cpaLI = LAN_Server.ACCPAL.begin(); cpaLI != LAN_Server.ACCPAL.end(); ++cpaLI)
		//	{
		//		if ((*cpaLI)->PreviousHeartbeatTime < currentHeartbeatTime - 30000)
		//		{
		//			Disconnect((*cpaLI)->SessionKey);
		//		}
		//	}
		//
		//	for (AdvancedChatClientPlayerAddressLI cpaLI = ACCPAL.begin(); cpaLI != ACCPAL.end(); ++cpaLI)
		//	{
		//		if ((*cpaLI)->PreviousHeartbeatTime < currentHeartbeatTime - 30000)
		//		{
		//			Disconnect((*cpaLI)->SessionKey);
		//		}
		//	}
		//}
	}

	{
		//LAN_Server.Update();
	}

	return 0;
}

bool ChatWAN_Server::Render()
{
	if (TIMER.ShowCounter()) //CheckRender()
	{
		printf(
			"====== ChatWAN_Server =====" CRALF
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

		LAN_Server.Render();
	}

	{
		//LAN_Server.Render();
	}

	return 0;
}

bool ChatWAN_Server::Terminalize()
{
	{
		Stop(); //IOCP_Server::Terminalize();
	}

	{
		TLDH.Disconnect();
		TLDH.Terminalize();
	}

	{
		LAN_Server.Terminalize();
	}

	return 0;
}