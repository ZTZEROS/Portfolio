#include "stdafx.h"
#include "AdvancedWAN.h"
#include "BaseWAN_Server.h"

unsigned int BaseWAN_Server::ReceivePacket(UINT64 sessionKey, SerialPacketQueue<AdvancedWAN_PacketHeader>* receivedPacketSPQ_Address)
{
	AdvancedWAN_PacketHeader receivedPacketHeader;
	UINT sessionIndex;

	UINT result;

	sessionIndex = sessionKey >> ADVANCED_WAN_CLIENT_INDEX_SHIFTED_BIT_COUNT;
	//if (1 < InterlockedIncrement(&AWAN_CSV[sessionIndex].IO_Count))
	{
		//if (AWAN_CSV[sessionIndex].Connected && AWAN_CSV[sessionIndex].Key == sessionKey)
		{
			result = AWAN_CSV[sessionIndex].ReceiveBQ.Peek((char*)&receivedPacketHeader, sizeof(receivedPacketHeader));
			if (sizeof(receivedPacketHeader) == result)
			{
				result = AWAN_CSV[sessionIndex].ReceiveBQ.Peek(receivedPacketSPQ_Address->GetQueueAddress(), sizeof(receivedPacketHeader) + receivedPacketHeader.BodySize);
				if (sizeof(receivedPacketHeader) + receivedPacketHeader.BodySize == result)
				{
					receivedPacketSPQ_Address->SetHeader(receivedPacketHeader);
					AWAN_CSV[sessionIndex].ReceiveBQ.DecreaseCurrentSize(sizeof(receivedPacketHeader));

					receivedPacketSPQ_Address->IncreaseCurrentSize(receivedPacketHeader.BodySize);
					AWAN_CSV[sessionIndex].ReceiveBQ.DecreaseCurrentSize(receivedPacketHeader.BodySize);
				}
				else result = 0;
			}
			else result = 0;
		}
		//else result = 0;
	}
	//else result = 0;

	//InterlockedDecrement(&AWAN_CSV[sessionIndex].IO_Count);

	return result;
}

unsigned int BaseWAN_Server::SendPacket(UINT64 sessionKey, SerialPacketQueue<AdvancedWAN_PacketHeader>* sendingPacketSPQ_Address)
{
	AdvancedWAN_PacketHeader sendingPacketHeader;
	UINT sessionIndex;

	UINT result;

	sessionIndex = sessionKey >> ADVANCED_WAN_CLIENT_INDEX_SHIFTED_BIT_COUNT;
	if (1 < InterlockedIncrement(&AWAN_CSV[sessionIndex].IO_Count))
	{
		Sleep(0);

		if (AWAN_CSV[sessionIndex].Connected && AWAN_CSV[sessionIndex].Key == sessionKey)
		{
			Sleep(0);

			result = sendingPacketSPQ_Address->GetHeaderSize();
			if (sizeof(sendingPacketHeader) == result)
			{
				sendingPacketHeader = sendingPacketSPQ_Address->GetHeader();

				result = sendingPacketSPQ_Address->GetTotalSize(); //GetCurrentSize();
				if (sizeof(sendingPacketHeader) + sendingPacketHeader.BodySize == result)
				{
					if (AWAN_CSV[sessionIndex].SendQ.Enqueue(sendingPacketSPQ_Address) == sendingPacketSPQ_Address) sendingPacketSPQ_Address->IncreaseReferenceCount();
					else result = 0;
				}
				else result = 0;
			}
			else result = 0;
		}
		else result = 0;

		{
			//if(!AWAN_CSV[sessionIndex].ReceiveCount && !AWAN_CSV[sessionIndex].SendCount)
			if (!InterlockedDecrement(&AWAN_CSV[sessionIndex].IO_Count))
			{
				if (InterlockedExchange(&AWAN_CSV[sessionIndex].Connected, 0)) // && AWAN_CSV[sessionIndex].Key == sessionKey)
				{
					AWAN_CSV[sessionIndex].CurrentPhaseIndex = ADVANCED_WAN_CLIENT_PHASE_DISCONNECT;

					PostQueuedCompletionStatus(AWAN_S_DISCONNECT_CP, NULL, AWAN_CSV[sessionIndex].Key, &AWAN_CSV[sessionIndex].OverlappedSend);
				}

				//THIS->PostDisconnect(AWAN_CSV[sessionIndex].Key);
				//THIS->Disconnect(AWAN_CSV[sessionIndex].Key);
			}
		}
	}
	else //result = 0;
	{
		InterlockedDecrement(&AWAN_CSV[sessionIndex].IO_Count);

		result = 0;
	}

	return result;
}

unsigned int BaseWAN_Server::PostAccept(UINT64 sessionKey) //deprecated
{
	UINT sessionIndex;

	UINT result;

	sessionIndex = sessionKey >> ADVANCED_WAN_CLIENT_INDEX_SHIFTED_BIT_COUNT;

	return result;
}

unsigned int BaseWAN_Server::PostReceive(UINT64 sessionKey) //deprecated
{
	UINT sessionIndex;

	UINT result;

	sessionIndex = sessionKey >> ADVANCED_WAN_CLIENT_INDEX_SHIFTED_BIT_COUNT;
	
	result = AWAN_CSV[sessionIndex].ReceiveBQ.GetCurrentSize();
	if (result)
	{
		//if (InterlockedExchange(&AWAN_CSV[sessionIndex].Receivable, 0))
		{
			InterlockedIncrement(&AWAN_CSV[sessionIndex].IO_Count);
			++AWAN_CSV[sessionIndex].ReceiveCount;

			AWAN_CSV[sessionIndex].CurrentPhaseIndex = ADVANCED_WAN_CLIENT_PHASE_RECEIVE;

			PostQueuedCompletionStatus(AWAN_S_COMMON_CP, NULL, AWAN_CSV[sessionIndex].Key, &AWAN_CSV[sessionIndex].OverlappedReceive);
		}
		//else result = 0;
	}
	//else result = 0;

	return result;
}

unsigned int BaseWAN_Server::PostSend(UINT64 sessionKey)
{
	UINT sessionIndex;
	
	UINT result;

	sessionIndex = sessionKey >> ADVANCED_WAN_CLIENT_INDEX_SHIFTED_BIT_COUNT;
	
	result = AWAN_CSV[sessionIndex].SendQ.GetCurrentCount();
	if (result)
	{
		if (InterlockedExchange(&AWAN_CSV[sessionIndex].Sendable, 0))
		{
			InterlockedIncrement(&AWAN_CSV[sessionIndex].IO_Count);
			++AWAN_CSV[sessionIndex].SendCount;

			AWAN_CSV[sessionIndex].CurrentPhaseIndex = ADVANCED_WAN_CLIENT_PHASE_SEND;

			PostQueuedCompletionStatus(AWAN_S_COMMON_CP, NULL, AWAN_CSV[sessionIndex].Key, &AWAN_CSV[sessionIndex].OverlappedSend);
		}
		else result = 0;
	}
	else result = 0;

	return result;
}

unsigned int BaseWAN_Server::PostDisconnect(UINT64 sessionKey)
{
	UINT sessionIndex;

	UINT result;

	sessionIndex = sessionKey >> ADVANCED_WAN_CLIENT_INDEX_SHIFTED_BIT_COUNT;
	if (InterlockedExchange(&AWAN_CSV[sessionIndex].Connected, 0) && AWAN_CSV[sessionIndex].Key == sessionKey)
	{
		AWAN_CSV[sessionIndex].CurrentPhaseIndex = ADVANCED_WAN_CLIENT_PHASE_DISCONNECT;

		PostQueuedCompletionStatus(AWAN_S_DISCONNECT_CP, NULL, AWAN_CSV[sessionIndex].Key, &AWAN_CSV[sessionIndex].OverlappedSend);
	}
	
	result = 0;

	return result;
}

bool BaseWAN_Server::Start(LPCWSTR ipAddress, UINT port, UINT workThreadCount, BOOL nagleOff, UINT maximumLAN_ClientCount)
{
	Initialize();
}

void BaseWAN_Server::Stop()
{
	Terminalize();
}

bool BaseWAN_Server::Disconnect(UINT64 sessionKey)
{
	UINT sessionIndex;

	sessionIndex = sessionKey >> ADVANCED_WAN_CLIENT_INDEX_SHIFTED_BIT_COUNT;
	if (1 < InterlockedIncrement(&AWAN_CSV[sessionIndex].IO_Count))
	{
		if (AWAN_CSV[sessionIndex].Connected && AWAN_CSV[sessionIndex].Key == sessionKey)
		{
			shutdown(AWAN_CSV[sessionIndex].Socket, SD_BOTH);
		}

		{
			//if(!AWAN_CSV[sessionIndex].ReceiveCount && !AWAN_CSV[sessionIndex].SendCount)
			if (!InterlockedDecrement(&AWAN_CSV[sessionIndex].IO_Count))
			{
				//if (InterlockedExchange(&AWAN_CSV[sessionIndex].Connected, 0) && AWAN_CSV[sessionIndex].Key == sessionKey)
				{
					AWAN_CSV[sessionIndex].CurrentPhaseIndex = ADVANCED_WAN_CLIENT_PHASE_DISCONNECT;

					PostQueuedCompletionStatus(AWAN_S_DISCONNECT_CP, NULL, AWAN_CSV[sessionIndex].Key, &AWAN_CSV[sessionIndex].OverlappedSend);
				}

				//THIS->PostDisconnect(AWAN_CSV[sessionIndex].Key);
				//THIS->Disconnect(AWAN_CSV[sessionIndex].Key);
			}
		}
	}
	else
	{
		InterlockedDecrement(&AWAN_CSV[sessionIndex].IO_Count);
	}

	//if (InterlockedExchange(&AWAN_CSV[sessionIndex].Connected, 0) && AWAN_CSV[sessionIndex].Key == sessionKey)
	//{
	//	AWAN_CSV[sessionIndex].CurrentPhaseIndex = ADVANCED_WAN_CLIENT_PHASE_DISCONNECT;
	//
	//	if (AWAN_CSV[sessionIndex].CurrentPhaseIndex == ADVANCED_WAN_CLIENT_PHASE_DISCONNECT)
	//	{
	//		//ProcessRemainedDatum
	//		{
	//
	//		}
	//
	//		THIS->OnClientLeave(sessionKey);
	//
	//		closesocket(AWAN_CSV[sessionIndex].Socket);
	//
	//		AWAN_CSV[sessionIndex].Socket = INVALID_SOCKET;
	//		AWAN_CSV[sessionIndex].Address;
	//
	//		AWAN_CSV[sessionIndex].Key = 0xffffffffffffffff;
	//		AWAN_CSV[sessionIndex].Index = 0xffffffff;
	//
	//		AWAN_CSV[sessionIndex].Connected = 0;
	//		AWAN_CSV[sessionIndex].Sendable = 0;
	//		AWAN_CSV[sessionIndex].ReceiveCount = 0;
	//		AWAN_CSV[sessionIndex].SendCount = 0;
	//
	//		ZeroMemory(&AWAN_CSV[sessionIndex].OverlappedReceive, sizeof(AWAN_CSV[sessionIndex].OverlappedReceive));
	//		ZeroMemory(&AWAN_CSV[sessionIndex].OverlappedSend, sizeof(AWAN_CSV[sessionIndex].OverlappedSend));
	//
	//		while (1)
	//		{
	//			SerialPacketQueue<AdvancedWAN_PacketHeader>* sendedPacketSPQ_Address;
	//
	//			sendedPacketSPQ_Address = AWAN_CSV[sessionIndex].SendQ.Dequeue();
	//
	//			if (!sendedPacketSPQ_Address) break;
	//
	//			if (!sendedPacketSPQ_Address->DecreaseReferenceCount())
	//			{
	//				InterlockedIncrement(&SPQ_DEALLOCATION_COUNT);
	//				//delete sendedPacketSPQ_Address;
	//				PACKET_SPQ_TLP.Deallocate(sendedPacketSPQ_Address);
	//			}
	//		}
	//
	//		AWAN_CSV[sessionIndex].ReceiveQ.Clear();
	//		AWAN_CSV[sessionIndex].SendQ.Clear();
	//		AWAN_CSV[sessionIndex].ReceiveBQ.Clear();
	//		AWAN_CSV[sessionIndex].SendBQ.Clear();
	//
	//		//AWAN_CSV[sessionIndex].ReceiveQ.Terminalize();
	//		//AWAN_CSV[sessionIndex].SendQ.Terminalize();
	//		//AWAN_CSV[sessionIndex].ReceiveBQ.Terminalize();
	//		//AWAN_CSV[sessionIndex].SendBQ.Terminalize();
	//
	//		AWAN_CSV[sessionIndex].IO_Count = 0;
	//
	//		AWAN_CSV[sessionIndex].CurrentPhaseIndex = ADVANCED_WAN_CLIENT_PHASE_ACCEPT;
	//	}
	//}

	return 0;
}