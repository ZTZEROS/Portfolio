#include "stdafx.h"
#include "AdvancedLAN.h"
#include "BaseLAN_Server.h"

unsigned int BaseLAN_Server::ReceivePacket(UINT64 sessionKey, SerialPacketQueue<AdvancedLAN_PacketHeader>* receivedPacketSPQ_Address)
{
	AdvancedLAN_PacketHeader receivedPacketHeader;
	UINT sessionIndex;
	
	UINT result;

	sessionIndex = sessionKey >> ADVANCED_LAN_CLIENT_INDEX_SHIFTED_BIT_COUNT;
	//if (1 < InterlockedIncrement(&ALAN_CSV[sessionIndex].IO_Count))
	{
		//if (ALAN_CSV[sessionIndex].Connected && ALAN_CSV[sessionIndex].Key == sessionKey)
		{
			result = ALAN_CSV[sessionIndex].ReceiveBQ.Peek((char*)&receivedPacketHeader, sizeof(receivedPacketHeader));
			if (sizeof(receivedPacketHeader) == result)
			{
				result = ALAN_CSV[sessionIndex].ReceiveBQ.Peek(receivedPacketSPQ_Address->GetQueueAddress(), sizeof(receivedPacketHeader) + receivedPacketHeader.BodySize);
				if (sizeof(receivedPacketHeader) + receivedPacketHeader.BodySize == result)
				{
					receivedPacketSPQ_Address->SetHeader(receivedPacketHeader);
					ALAN_CSV[sessionIndex].ReceiveBQ.DecreaseCurrentSize(sizeof(receivedPacketHeader));

					receivedPacketSPQ_Address->IncreaseCurrentSize(result);
					ALAN_CSV[sessionIndex].ReceiveBQ.DecreaseCurrentSize(result);
				}
				else result = 0;

				//result = ALAN_CSV[sessionIndex].ReceiveBQ.Peek(receivedPacketSPQ.GetQueueAddress(), sizeof(receivedPacketHeader) + receivedPacketHeader.BodySize);
				//if (sizeof(receivedPacketHeader) + receivedPacketHeader.BodySize == result)
				//{
				//	receivedPacketSPQ.SetHeader(receivedPacketHeader);
				//	receivedPacketSPQ.IncreaseCurrentSize(receivedPacketHeader.BodySize);
				//
				//	result = ALAN_CSV[sessionIndex].ReceiveBQ.DecreaseCurrentSize(sizeof(receivedPacketHeader) + receivedPacketHeader.BodySize);
				//	//ALAN_CSV[sessionIndex].ReceiveBQ.Dequeue(receivedPacketSPQ.GetQueueAddress(), sizeof(receivedPacketHeader) + receivedPacketHeader.BodySize);
				//}
				//else result = 0;
			}
			else result = 0;
		}
		//else result = 0;
	}
	//else result = 0;

	//InterlockedDecrement(&AWAN_CSV[sessionIndex].IO_Count);
	
	return result;
}

unsigned int BaseLAN_Server::SendPacket(UINT64 sessionKey, SerialPacketQueue<AdvancedLAN_PacketHeader>* sendingPacketSPQ_Address)
{
	AdvancedLAN_PacketHeader sendingPacketHeader;
	UINT sessionIndex;

	UINT result;

	sessionIndex = sessionKey >> ADVANCED_LAN_CLIENT_INDEX_SHIFTED_BIT_COUNT;
	if (1 < InterlockedIncrement(&ALAN_CSV[sessionIndex].IO_Count))
	{
		if (ALAN_CSV[sessionIndex].Connected && ALAN_CSV[sessionIndex].Key == sessionKey)
		{
			result = sendingPacketSPQ_Address->GetHeaderSize();
			if (sizeof(sendingPacketHeader) == result)
			{
				sendingPacketHeader = sendingPacketSPQ_Address->GetHeader();

				result = sendingPacketSPQ_Address->GetTotalSize(); //GetCurrentrSize();
				if (sizeof(sendingPacketHeader) + sendingPacketHeader.BodySize == result)
				{
					if (ALAN_CSV[sessionIndex].SendQ.Enqueue(sendingPacketSPQ_Address) == sendingPacketSPQ_Address) sendingPacketSPQ_Address->IncreaseReferenceCount();
					else result = 0;
				}
				else result = 0;
			}
			else result = 0;
		}
		else result = 0;

		{
			//if(!AWAN_CSV[sessionIndex].ReceiveCount && !AWAN_CSV[sessionIndex].SendCount)
			if (!InterlockedDecrement(&ALAN_CSV[sessionIndex].IO_Count))
			{
				//if (InterlockedExchange(&AWAN_CSV[sessionIndex].Connected, 0) && AWAN_CSV[sessionIndex].Key == sessionKey)
				{
					ALAN_CSV[sessionIndex].CurrentPhaseIndex = ADVANCED_LAN_CLIENT_PHASE_DISCONNECT;

					PostQueuedCompletionStatus(ALAN_S_DISCONNECT_CP, NULL, ALAN_CSV[sessionIndex].Key, &ALAN_CSV[sessionIndex].OverlappedSend);
				}

				//THIS->PostDisconnect(AWAN_CSV[sessionIndex].Key);
				//THIS->Disconnect(AWAN_CSV[sessionIndex].Key);
			}
		}
	}
	else //result = 0;
	{
		InterlockedDecrement(&ALAN_CSV[sessionIndex].IO_Count);

		result = 0;
	}

	return result;
}

/*
unsigned int BaseLAN_Server::SendPacket(UINT64 sessionKey, SerialPacketQueue<AdvancedLAN_PacketHeader>* sendingPacketSPQ_Address)
{
	AdvancedLAN_PacketHeader sendingPacketHeader;
	UINT sessionIndex;

	DWORD flag;
	UINT result;

	sessionIndex = sessionKey >> ADVANCED_LAN_CLIENT_INDEX_SHIFTED_BIT_COUNT;
	flag = 0;

	result = sendingPacketSPQ_Address->GetHeaderSize();
	if (sizeof(sendingPacketHeader) == result)
	{
		sendingPacketHeader = sendingPacketSPQ_Address->GetHeader();

		result = sendingPacketSPQ_Address->GetTotalSize();
		if (sizeof(sendingPacketHeader) + sendingPacketHeader.BodySize == result)
		{
			if (ALAN_CSV[sessionIndex].SendQ.Enqueue(sendingPacketSPQ_Address) == sendingPacketSPQ_Address);
			else result = 0;
		}
	}

	if (ALAN_CSV[sessionIndex].SendQ.GetCurrentCount())
	{
		//sndPost
		if (InterlockedExchange(&ALAN_CSV[sessionIndex].Sendable, 0))
		{
			WSABUF sendingWSA_BufferA[ADVANCED_LAN_SERVER_WSABUF_COUNT];
			UINT sendingWSA_BufferCount;

			sendingWSA_BufferCount = ALAN_CSV[sessionIndex].SendQ.GetCurrentCount();
			if (ADVANCED_LAN_SERVER_WSABUF_COUNT < sendingWSA_BufferCount) sendingWSA_BufferCount = ADVANCED_LAN_SERVER_WSABUF_COUNT;

			FOR(i, sendingWSA_BufferCount)
			{
				SerialPacketQueue<AdvancedLAN_PacketHeader>* sendingPacketSPQ_Address;
				WSABUF wsaBuffer;

				sendingPacketSPQ_Address = ALAN_CSV[sessionIndex].SendQ.Peek(i);
				wsaBuffer.buf = sendingPacketSPQ_Address->GetQueueAddress();
				wsaBuffer.len = sendingPacketSPQ_Address->GetTotalSize();

				sendingWSA_BufferA[i] = wsaBuffer;
			}

			if (!sendingWSA_BufferCount) InterlockedExchange(&ALAN_CSV[sessionIndex].Sendable, 1);
			else
			{
				InterlockedIncrement(&ALAN_CSV[sessionIndex].IO_Count);
				++ALAN_CSV[sessionIndex].SendCount;

				result = WSASend(ALAN_CSV[sessionIndex].Socket, sendingWSA_BufferA, sendingWSA_BufferCount, nullptr, flag, &ALAN_CSV[sessionIndex].OverlappedSend, nullptr);
				if (result == SOCKET_ERROR)
				{
					if (WSAGetLastError() == WSA_IO_PENDING);
					else
					{
						InterlockedDecrement(&ALAN_CSV[sessionIndex].IO_Count);
						--ALAN_CSV[sessionIndex].SendCount;

						//printf("[WSASend] sessionIndex : %u, sessionKey : %llu, IO_Count : %d, WSAGetLastError() : %d" CRALF, sessionIndex, ALAN_CSV[sessionIndex].Key, ALAN_CSV[sessionIndex].IO_Count, WSAGetLastError());
					}
				}
			}
		}
	}

	return result;
}
*/

unsigned int BaseLAN_Server::PostAccept(UINT64 sessionKey) //deprecated
{
	UINT sessionIndex;

	UINT result;

	sessionIndex = sessionKey >> ADVANCED_LAN_CLIENT_INDEX_SHIFTED_BIT_COUNT;

	return result;
}

unsigned int BaseLAN_Server::PostReceive(UINT64 sessionKey) //deprecated
{
	UINT sessionIndex;

	UINT result;

	sessionIndex = sessionKey >> ADVANCED_LAN_CLIENT_INDEX_SHIFTED_BIT_COUNT;

	result = ALAN_CSV[sessionIndex].ReceiveBQ.GetCurrentSize();
	if (result)
	{
		//if (InterlockedExchange(&ALAN_CSV[sessionIndex].Receivable, 0))
		{
			InterlockedIncrement(&ALAN_CSV[sessionIndex].IO_Count);
			++ALAN_CSV[sessionIndex].ReceiveCount;

			ALAN_CSV[sessionIndex].CurrentPhaseIndex = ADVANCED_LAN_CLIENT_PHASE_RECEIVE;

			PostQueuedCompletionStatus(ALAN_S_COMMON_CP, NULL, ALAN_CSV[sessionIndex].Key, &ALAN_CSV[sessionIndex].OverlappedReceive);
		}
		//else result = 0;
	}
	//else result = 0;

	return result;
}

unsigned int BaseLAN_Server::PostSend(UINT64 sessionKey)
{
	UINT sessionIndex;

	UINT result;

	sessionIndex = sessionKey >> ADVANCED_LAN_CLIENT_INDEX_SHIFTED_BIT_COUNT;

	result = ALAN_CSV[sessionIndex].SendQ.GetCurrentCount();
	if (result)
	{
		if (InterlockedExchange(&ALAN_CSV[sessionIndex].Sendable, 0))
		{
			InterlockedIncrement(&ALAN_CSV[sessionIndex].IO_Count);
			++ALAN_CSV[sessionIndex].SendCount;
	
			ALAN_CSV[sessionIndex].CurrentPhaseIndex = ADVANCED_LAN_CLIENT_PHASE_SEND;
	
			PostQueuedCompletionStatus(ALAN_S_COMMON_CP, NULL, ALAN_CSV[sessionIndex].Key, &ALAN_CSV[sessionIndex].OverlappedSend);
		}
		else result = 0;
	}
	else result = 0;

	//{
	//	AdvancedLAN_PacketHeader sendingPacketHeader;
	//	UINT sessionIndex;
	//
	//	DWORD flag;
	//	UINT result;
	//
	//	sessionIndex = sessionKey >> ADVANCED_LAN_CLIENT_INDEX_SHIFTED_BIT_COUNT;
	//	flag = 0;
	//
	//	if (ALAN_CSV[sessionIndex].SendQ.GetCurrentCount())
	//	{
	//		//sndPost
	//		if (InterlockedExchange(&ALAN_CSV[sessionIndex].Sendable, 0))
	//		{
	//			WSABUF sendingWSA_BufferA[ADVANCED_LAN_SERVER_WSABUF_COUNT];
	//			UINT sendingWSA_BufferCount;
	//
	//			sendingWSA_BufferCount = ALAN_CSV[sessionIndex].SendQ.GetCurrentCount();
	//			if (ADVANCED_LAN_SERVER_WSABUF_COUNT < sendingWSA_BufferCount) sendingWSA_BufferCount = ADVANCED_LAN_SERVER_WSABUF_COUNT;
	//
	//			FOR(i, sendingWSA_BufferCount)
	//			{
	//				SerialPacketQueue<AdvancedLAN_PacketHeader>* sendingPacketSPQ_Address;
	//				WSABUF wsaBuffer;
	//
	//				sendingPacketSPQ_Address = ALAN_CSV[sessionIndex].SendQ.Peek(i);
	//				wsaBuffer.buf = sendingPacketSPQ_Address->GetQueueAddress();
	//				wsaBuffer.len = sendingPacketSPQ_Address->GetTotalSize();
	//
	//				sendingWSA_BufferA[i] = wsaBuffer;
	//			}
	//
	//			if (!sendingWSA_BufferCount) InterlockedExchange(&ALAN_CSV[sessionIndex].Sendable, 1);
	//			else
	//			{
	//				InterlockedIncrement(&ALAN_CSV[sessionIndex].IO_Count);
	//				++ALAN_CSV[sessionIndex].SendCount;
	//
	//				result = WSASend(ALAN_CSV[sessionIndex].Socket, sendingWSA_BufferA, sendingWSA_BufferCount, nullptr, flag, &ALAN_CSV[sessionIndex].OverlappedSend, nullptr);
	//				if (result == SOCKET_ERROR)
	//				{
	//					if (WSAGetLastError() == WSA_IO_PENDING);
	//					else
	//					{
	//						InterlockedDecrement(&ALAN_CSV[sessionIndex].IO_Count);
	//						--ALAN_CSV[sessionIndex].SendCount;
	//
	//						//printf("[WSASend] sessionIndex : %u, sessionKey : %llu, IO_Count : %d, WSAGetLastError() : %d" CRALF, sessionIndex, ALAN_CSV[sessionIndex].Key, ALAN_CSV[sessionIndex].IO_Count, WSAGetLastError());
	//					}
	//				}
	//			}
	//		}
	//	}
	//}

	return result;
}

unsigned int BaseLAN_Server::PostDisconnect(UINT64 sessionKey)
{
	UINT sessionIndex;

	UINT result;

	sessionIndex = sessionKey >> ADVANCED_LAN_CLIENT_INDEX_SHIFTED_BIT_COUNT;

	ALAN_CSV[sessionIndex].CurrentPhaseIndex = ADVANCED_LAN_CLIENT_PHASE_DISCONNECT;

	//PostQueuedCompletionStatus(ALAN_S_DISCONNECT_CP, NULL, ALAN_CSV[sessionIndex].Key, nullptr);
	result = PostQueuedCompletionStatus(ALAN_S_DISCONNECT_CP, NULL, ALAN_CSV[sessionIndex].Key, &ALAN_CSV[sessionIndex].OverlappedSend);

	return result;
}

bool BaseLAN_Server::Start(LPCWSTR ipAddress, UINT port, UINT workThreadCount, BOOL nagleOff, UINT maximumLAN_ClientCount)
{
	Initialize();

	return 0;
}

void BaseLAN_Server::Stop()
{
	Terminalize();
}

bool BaseLAN_Server::Disconnect(UINT64 sessionKey)
{
	UINT sessionIndex;

	sessionIndex = sessionKey >> ADVANCED_LAN_CLIENT_INDEX_SHIFTED_BIT_COUNT;

	ALAN_CSV[sessionIndex].CurrentPhaseIndex = ADVANCED_LAN_CLIENT_PHASE_DISCONNECT;

	if (ALAN_CSV[sessionIndex].CurrentPhaseIndex == ADVANCED_LAN_CLIENT_PHASE_DISCONNECT)
	{
		//ProcessRemainedDatum
		{
			//THIS->OnRecv(sessionKey, 0); //not necessary, processed already
		}

		THIS->OnClientLeave(sessionKey);

		closesocket(ALAN_CSV[sessionIndex].Socket);

		//shutdown(ALAN_CSV[sessionIndex].Socket, SD_BOTH);
		//WSASendDisconnect();
		//WSARecvDisconnect();

		ALAN_CSV[sessionIndex].Socket = INVALID_SOCKET;
		ALAN_CSV[sessionIndex].Address;

		ALAN_CSV[sessionIndex].Key = 0xffffffffffffffff;
		ALAN_CSV[sessionIndex].Index = 0xffffffff;

		ALAN_CSV[sessionIndex].Connected = 0;
		ALAN_CSV[sessionIndex].Sendable = 0;
		ALAN_CSV[sessionIndex].ReceiveCount = 0;
		ALAN_CSV[sessionIndex].SendCount = 0;

		ZeroMemory(&ALAN_CSV[sessionIndex].OverlappedReceive, sizeof(ALAN_CSV[sessionIndex].OverlappedReceive));
		ZeroMemory(&ALAN_CSV[sessionIndex].OverlappedSend, sizeof(ALAN_CSV[sessionIndex].OverlappedSend));

		while (1)
		{
			SerialPacketQueue<AdvancedLAN_PacketHeader>* sendedPacketSPQ_Address;

			sendedPacketSPQ_Address = ALAN_CSV[sessionIndex].SendQ.Dequeue();

			if (!sendedPacketSPQ_Address) break;

			//if (!sendedPacketSPQ_Address->DecreaseReferenceCount())
			{
				InterlockedIncrement(&SPQ_DEALLOCATION_COUNT);
				delete sendedPacketSPQ_Address;
			}
		}

		ALAN_CSV[sessionIndex].ReceiveQ.Clear();
		ALAN_CSV[sessionIndex].SendQ.Clear();
		ALAN_CSV[sessionIndex].ReceiveBQ.Clear();
		ALAN_CSV[sessionIndex].SendBQ.Clear();

		//ALAN_CSV[sessionIndex].ReceiveQ.Terminalize();
		//ALAN_CSV[sessionIndex].SendQ.Terminalize();
		//ALAN_CSV[sessionIndex].ReceiveBQ.Terminalize();
		//ALAN_CSV[sessionIndex].SendBQ.Terminalize();

		ALAN_CSV[sessionIndex].IO_Count = 0;

		ALAN_CSV[sessionIndex].CurrentPhaseIndex = ADVANCED_LAN_CLIENT_PHASE_ACCEPT;
	}

	return 0;
}