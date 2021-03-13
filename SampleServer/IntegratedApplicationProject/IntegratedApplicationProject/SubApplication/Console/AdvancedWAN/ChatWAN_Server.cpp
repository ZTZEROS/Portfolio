#include "stdafx.h"
#include "AdvancedWAN.h"
#include "BaseWAN_Server.h"
#include "ChatWAN_Server.h"

AdvancedWAN_ClientPlayerAddressL ChatWAN_Server::AWAN_CPAL;
SRWLOCK ChatWAN_Server::AWAN_CPAL_LOCK;

AdvancedWAN_Sector ChatWAN_Server::AWAN_SECTOR[ADVANCED_WAN_CHAT_SECTOR_HEIGHT][ADVANCED_WAN_CHAT_SECTOR_WIDTH];

ChatWAN_Server::ChatWAN_Server()
{
	CurrentClientCount = 0;
	ConnectionCount = 0;
	DisconnectionCount = 0;

	WorkLFQ_CurrentCount = 0;
	WorkLFQ_EnqueueCount = 0;
	WorkLFQ_DequeueCount = 0;
	
	ReceiveCount = 0;
	ReceivedSize = 0;
	SendCount = 0;
	SendedSize = 0;

	FOR(i, sizeof(CheckPoint))
	{
		CheckPoint[i] = 0;
	}

	FOR(i, ADVANCED_WAN_CHAT_SECTOR_WIDTH)
	{
		FOR(j, ADVANCED_WAN_CHAT_SECTOR_HEIGHT)
		{
			InitializeSRWLock(&AWAN_SECTOR[j][i].Lock);
		}
	}
}

ChatWAN_Server::~ChatWAN_Server() {}

bool ChatWAN_Server::Initialize()
{
	BaseWAN_Server::Initialize();

	CurrentClientCount = 0;
	ConnectionCount = 0;
	DisconnectionCount = 0;

	WorkLFQ_CurrentCount = 0;
	WorkLFQ_EnqueueCount = 0;
	WorkLFQ_DequeueCount = 0;

	ReceiveCount = 0;
	ReceivedSize = 0;
	SendCount = 0;
	SendedSize = 0;

	FOR(i, sizeof(CheckPoint) / sizeof(UINT))
	{
		CheckPoint[i] = 0;
	}

	FOR(i, ADVANCED_WAN_CHAT_SECTOR_WIDTH)
	{
		FOR(j, ADVANCED_WAN_CHAT_SECTOR_HEIGHT)
		{
			InitializeSRWLock(&AWAN_SECTOR[j][i].Lock);
		}
	}

	return 0;
}

bool ChatWAN_Server::Update()
{
	if (TIMER.CheckUpdate())
	{
		if (GetAsyncKeyState(VK_F1)) THREAD_CONTEXT_SWITCH_ON = !THREAD_CONTEXT_SWITCH_ON;
		if (GetAsyncKeyState(VK_F2)) INDICATOR_ON = !INDICATOR_ON;
		if (GetAsyncKeyState(VK_F4)) return 1;
	}

	while(1)
	{
		AdvancedWAN_ClientPlayer* clientPlayerAddress;
		AdvancedWAN_Work receivedWork; //clientWork
		AdvancedWAN_Work sendingWork; //serverWork
		WORD clientRequestIndex; //requestWorkType //receivedRequest
		WORD serverResponseIndex; //responseWorkType //sendingResponse
		
		clientPlayerAddress = nullptr;

		clientRequestIndex = 0;
		serverResponseIndex = 0;

		if (WorkLFQ.GetCurrentCount())
		{
			InterlockedDecrement(&WorkLFQ_CurrentCount);
			++WorkLFQ_DequeueCount;
			receivedWork = WorkLFQ.Dequeue();

			//slow part
			AcquireSRWLockShared(&AWAN_CPAL_LOCK);
			for (AdvancedWAN_ClientPlayerAddressLI playerAddressLI = AWAN_CPAL.begin(); playerAddressLI != AWAN_CPAL.end(); ++playerAddressLI)
			{
				if ((*playerAddressLI)->SessionKey == receivedWork.SessionKey)
				{
					clientPlayerAddress = *playerAddressLI;

					break;
				}
			}
			ReleaseSRWLockShared(&AWAN_CPAL_LOCK);

			if (clientPlayerAddress)
			{
				serverResponseIndex = ProcessRequest(clientPlayerAddress, receivedWork);

				if (serverResponseIndex)
				{
					sendingWork = ProcessResponse(clientPlayerAddress, serverResponseIndex);

					switch (serverResponseIndex)
					{
					case ADVANCED_WAN_CHAT_SERVER_RESPONSE_LOGIN:
						Singlecast(sendingWork);
						break;
					case ADVANCED_WAN_CHAT_SERVER_RESPONSE_SECTOR_MOVE:
						Singlecast(sendingWork);
						break;
					case ADVANCED_WAN_CHAT_SERVER_RESPONSE_MESSAGE:
						//Singlecast(sendingWork);
						Broadcast(sendingWork, clientPlayerAddress->SectorIndexX, clientPlayerAddress->SectorIndexY);
						break;
					case ADVANCED_WAN_CHAT_SERVER_RESPONSE_HEARTBEAT:
						{
							if (!sendingWork.PacketSPQ_Address->DecreaseReferenceCount())
							{
								InterlockedIncrement(&SPQ_DEALLOCATION_COUNT);
								//delete sendingWork.PacketSPQ_Address;
								PACKET_SPQ_TLP.Deallocate(sendingWork.PacketSPQ_Address);
							}
						}
						break;
					case ADVANCED_WAN_CHAT_SERVER_RESPONSE_LOGOUT:
						{
							if (!sendingWork.PacketSPQ_Address->DecreaseReferenceCount())
							{
								InterlockedIncrement(&SPQ_DEALLOCATION_COUNT);
								//delete sendingWork.PacketSPQ_Address;
								PACKET_SPQ_TLP.Deallocate(sendingWork.PacketSPQ_Address);
							}
						}
						break;
					default:
						{
							Disconnect(sendingWork.SessionKey);

							if (!sendingWork.PacketSPQ_Address->DecreaseReferenceCount())
							{
								InterlockedIncrement(&SPQ_DEALLOCATION_COUNT);
								//delete sendingWork.PacketSPQ_Address;
								PACKET_SPQ_TLP.Deallocate(sendingWork.PacketSPQ_Address);
							}
						}
						break;
					}
				}
				else break;
			}
			else break;
		}
		else break;
		//else Sleep(0);
	}

	return 0;
}

bool ChatWAN_Server::Render()
{
	if (TIMER.ShowCounter()) //CheckRender()
	{
		printf(
			"CurrentClientCount : %u" CRALF
			"ConnectionCount : %u" CRALF
			"DisconnectionCount : %u" CRALF
			CRALF
			"WorkLFQ_CurrentCount : %u" CRALF
			"WorkLFQ_EnqueueCount : %u" CRALF
			"WorkLFQ_DequeueCount : %u" CRALF
			CRALF
			"ReceiveCount : %u" CRALF
			"ReceivedSize : %u" CRALF
			"SendCount : %u" CRALF
			"SendedSize : %u" CRALF
			CRALF
			"SPQ_ALLOCATION_COUNT : %u" CRALF
			"SPQ_DEALLOCATION_COUNT : %u" CRALF
			CRALF
			,
			CurrentClientCount,
			ConnectionCount,
			DisconnectionCount,

			WorkLFQ_CurrentCount,
			WorkLFQ_EnqueueCount,
			WorkLFQ_DequeueCount,
			
			ReceiveCount,
			ReceivedSize,
			SendCount,
			SendedSize,
			
			SPQ_ALLOCATION_COUNT,
			SPQ_DEALLOCATION_COUNT
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
	}

	return 0;
}

bool ChatWAN_Server::Terminalize()
{
	BaseWAN_Server::Terminalize();

	return 0;
}