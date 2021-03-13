#include "stdafx.h"
#include "AdvancedWAN.h"
#include "BaseWAN_Server.h"

unsigned int WINAPI BaseWAN_Server::AcceptAdvancedWAN_Client(LPVOID selfInstanceAddress)
{
	AdvancedWAN_ClientSession acceptedClientSession;
	INT addressLength;
	UINT64 sessionKey;

	addressLength = sizeof(acceptedClientSession.Address);

	if (!THIS) THIS = (BaseWAN_Server*)selfInstanceAddress;

	while (SUB_THREAD_ON)
	{
		acceptedClientSession.Socket = WSAAccept(AWAN_SS.Socket, (SOCKADDR*)&acceptedClientSession.Address, &addressLength, nullptr, NULL);
		CHECK_INVALID_SOCKET(acceptedClientSession.Socket);
		//IF_INVALID_SOCKET(acceptedclientSession.Socket);
		//ELSE
		{
			if (THIS->OnConnectionRequest(acceptedClientSession.Address, acceptedClientSession.Address.sin_port));

			FOR(i, TOTAL_ADVANCED_WAN_CLIENT_COUNT)
			{
				if (AWAN_CSV[i].CurrentPhaseIndex == ADVANCED_WAN_CLIENT_PHASE_ACCEPT)
				{
					++CURRENT_ACCEPTION_COUNT;
					sessionKey = i;
					sessionKey = sessionKey << ADVANCED_WAN_CLIENT_INDEX_SHIFTED_BIT_COUNT;
					sessionKey += CURRENT_ACCEPTION_COUNT;

					IF_NULL(CreateIoCompletionPort((HANDLE)acceptedClientSession.Socket, AWAN_S_COMMON_CP, sessionKey, NULL))
					ELSE
					{
						AWAN_CSV[i].Socket = acceptedClientSession.Socket;
						AWAN_CSV[i].Address = acceptedClientSession.Address;
						AWAN_CSV[i].Key = sessionKey;
						AWAN_CSV[i].Index = i;
						AWAN_CSV[i].Connected = 1;
						AWAN_CSV[i].Sendable = 1;
						AWAN_CSV[i].ReceiveCount = 1;
						AWAN_CSV[i].SendCount = 0;

						ZeroMemory(&AWAN_CSV[i].OverlappedReceive, sizeof(AWAN_CSV[i].OverlappedReceive));
						ZeroMemory(&AWAN_CSV[i].OverlappedSend, sizeof(AWAN_CSV[i].OverlappedSend));

						AWAN_CSV[i].IO_Count = 1;

						InitializeSRWLock(&AWAN_CSV[i].Lock);

						AWAN_CSV[i].CurrentPhaseIndex = ADVANCED_WAN_CLIENT_PHASE_UPDATE;

						++AWAN_CSV[i].AccumulatedUseCount;

						THIS->OnClientJoin(AWAN_CSV[i].Address, sessionKey);

						PostQueuedCompletionStatus(AWAN_S_COMMON_CP, NULL, sessionKey, &AWAN_CSV[i].OverlappedReceive);
						//PostQueuedCompletionStatus(AWAN_S_RECEIVE_CP, NULL, sessionKey, &AWAN_CSV[i].OverlappedReceive);

						break;
					}
				}

				if (i == TOTAL_ADVANCED_WAN_CLIENT_COUNT - 1)
				{
					i = 0;

					Sleep(0);
				}
			}
		}
	}

	return 0;
}

unsigned int WINAPI BaseWAN_Server::DisconnectAdvancedWAN_Client(LPVOID selfInstanceAddress)
{
	DWORD transferredSize;
	ULONG_PTR completionKey;
	LPOVERLAPPED overlapped;
	UINT64 sessionKey;
	UINT sessionIndex;

	while (SUB_THREAD_ON)
	{
		transferredSize = 0;
		completionKey = 0;
		overlapped = nullptr;
		sessionKey = 0xffffffffffffffff;
		sessionIndex = 0xffffffff;

		GetQueuedCompletionStatus(AWAN_S_DISCONNECT_CP, &transferredSize, &completionKey, &overlapped, INFINITE);

		sessionKey = completionKey;
		sessionIndex = sessionKey >> ADVANCED_WAN_CLIENT_INDEX_SHIFTED_BIT_COUNT;

		if (overlapped)
		{
			//if (InterlockedExchange(&AWAN_CSV[sessionIndex].Connected, 0) && AWAN_CSV[sessionIndex].Key == sessionKey)
			{
				if (AWAN_CSV[sessionIndex].CurrentPhaseIndex == ADVANCED_WAN_CLIENT_PHASE_DISCONNECT)
				{
					//ProcessRemainedDatum
					{

					}

					THIS->OnClientLeave(sessionKey);

					closesocket(AWAN_CSV[sessionIndex].Socket);

					AWAN_CSV[sessionIndex].Socket = INVALID_SOCKET;
					AWAN_CSV[sessionIndex].Address;

					AWAN_CSV[sessionIndex].Key = 0xffffffffffffffff;
					AWAN_CSV[sessionIndex].Index = 0xffffffff;

					AWAN_CSV[sessionIndex].Connected = 0;
					AWAN_CSV[sessionIndex].Sendable = 0;
					AWAN_CSV[sessionIndex].ReceiveCount = 0;
					AWAN_CSV[sessionIndex].SendCount = 0;

					ZeroMemory(&AWAN_CSV[sessionIndex].OverlappedReceive, sizeof(AWAN_CSV[sessionIndex].OverlappedReceive));
					ZeroMemory(&AWAN_CSV[sessionIndex].OverlappedSend, sizeof(AWAN_CSV[sessionIndex].OverlappedSend));

					while (1)
					{
						SerialPacketQueue<AdvancedWAN_PacketHeader>* sendedPacketSPQ_Address;

						sendedPacketSPQ_Address = AWAN_CSV[sessionIndex].SendQ.Dequeue();

						if (!sendedPacketSPQ_Address) break;

						if (!sendedPacketSPQ_Address->DecreaseReferenceCount())
						{
							InterlockedIncrement(&SPQ_DEALLOCATION_COUNT);
							//delete sendedPacketSPQ_Address;
							PACKET_SPQ_TLP.Deallocate(sendedPacketSPQ_Address);
						}
					}

					AWAN_CSV[sessionIndex].ReceiveQ.Clear();
					AWAN_CSV[sessionIndex].SendQ.Clear();
					AWAN_CSV[sessionIndex].ReceiveBQ.Clear();
					AWAN_CSV[sessionIndex].SendBQ.Clear();

					//AWAN_CSV[sessionIndex].ReceiveQ.Terminalize();
					//AWAN_CSV[sessionIndex].SendQ.Terminalize();
					//AWAN_CSV[sessionIndex].ReceiveBQ.Terminalize();
					//AWAN_CSV[sessionIndex].SendBQ.Terminalize();

					AWAN_CSV[sessionIndex].IO_Count = 0;

					AWAN_CSV[sessionIndex].CurrentPhaseIndex = ADVANCED_WAN_CLIENT_PHASE_ACCEPT;
				}
			}
		}
		else
		{
			if (GetLastError() == ERROR_ABANDONED_WAIT_0) break;

			if (completionKey || transferredSize) continue;
			else break;
		}
	}

	return 0;
}

unsigned int WINAPI BaseWAN_Server::WorkAdvancedWAN_Server(LPVOID selfInstanceAddress)
{
	DWORD transferredSize;
	ULONG_PTR completionKey;
	LPOVERLAPPED overlapped;
	UINT64 sessionKey;
	UINT sessionIndex;

	DWORD flag;
	INT result;

	if (!THIS) THIS = (BaseWAN_Server*)selfInstanceAddress;

	THIS->OnWorkerThreadBegin();

	while (SUB_THREAD_ON)
	{
		transferredSize = 0;
		completionKey = 0;
		overlapped = nullptr;
		sessionKey = 0xffffffffffffffff;
		sessionIndex = 0xffffffff;

		flag = 0;

		GetQueuedCompletionStatus(AWAN_S_COMMON_CP, &transferredSize, &completionKey, &overlapped, INFINITE);

		sessionKey = completionKey;
		sessionIndex = sessionKey >> ADVANCED_WAN_CLIENT_INDEX_SHIFTED_BIT_COUNT;

		if (overlapped)
		{
			//if(AWAN_CSV[sessionIndex].CurrentPhaseIndex == ADVANCED_WAN_CLIENT_PHASE_UPDATE)
			{
				if (&AWAN_CSV[sessionIndex].OverlappedReceive == overlapped)
				{
					ZeroMemory(&AWAN_CSV[sessionIndex].OverlappedReceive, sizeof(AWAN_CSV[sessionIndex].OverlappedReceive));

					if (transferredSize)
					{
						INT receivedSize;

						receivedSize = transferredSize;
						AWAN_CSV[sessionIndex].ReceiveBQ.IncreaseCurrentSize(receivedSize);

						THIS->OnRecv(AWAN_CSV[sessionIndex].Key, receivedSize);

						//AWAN_CSV[sessionIndex].CurrentPhaseIndex = ADVANCED_WAN_CLIENT_PHASE_UPDATE;
					}

					InterlockedIncrement(&AWAN_CSV[sessionIndex].IO_Count);
					++AWAN_CSV[sessionIndex].ReceiveCount;

					result = AWAN_CSV[sessionIndex].ReceiveBQ.WSAReceive(WSARecv, AWAN_CSV[sessionIndex].Socket, &flag, &AWAN_CSV[sessionIndex].OverlappedReceive);
					if (result == SOCKET_ERROR)
					{
						if (WSAGetLastError() == WSA_IO_PENDING);
						else
						{
							InterlockedDecrement(&AWAN_CSV[sessionIndex].IO_Count);
							--AWAN_CSV[sessionIndex].ReceiveCount;

							if(INDICATOR_ON) printf("[WSARecv] sessionIndex : %u, sessionKey : %llu, IO_Count : %u, WSAGetLastError() : %d" CRALF, sessionIndex, AWAN_CSV[sessionIndex].Key, AWAN_CSV[sessionIndex].IO_Count, WSAGetLastError());
						}
					}

					//InterlockedDecrement(&AWAN_CSV[sessionIndex].IO_Count);
					--AWAN_CSV[sessionIndex].ReceiveCount;
				}
			}

			if (&AWAN_CSV[sessionIndex].OverlappedSend == overlapped)
			{
				if (transferredSize)
				{
					INT sendedSize;

					sendedSize = transferredSize;

					while (0 < sendedSize)
					{
						SerialPacketQueue<AdvancedWAN_PacketHeader>* sendedPacketSPQ_Address;

						sendedPacketSPQ_Address = AWAN_CSV[sessionIndex].SendQ.Dequeue();

						sendedSize -= sendedPacketSPQ_Address->GetTotalSize();

						if (!sendedPacketSPQ_Address->DecreaseReferenceCount())
						{
							InterlockedIncrement(&SPQ_DEALLOCATION_COUNT);
							//delete sendedPacketSPQ_Address;
							PACKET_SPQ_TLP.Deallocate(sendedPacketSPQ_Address);
						}
					}
					
					if(!sendedSize) sendedSize = transferredSize;
					else sendedSize = transferredSize - sendedSize;

					THIS->OnSend(AWAN_CSV[sessionIndex].Key, sendedSize);

					AWAN_CSV[sessionIndex].CurrentPhaseIndex = ADVANCED_WAN_CLIENT_PHASE_UPDATE;

					//InterlockedExchange(&AWAN_CSV[sessionIndex].Sendable, 1);
				}
				//else
				//sndPost
				//if (InterlockedExchange(&AWAN_CSV[sessionIndex].Sendable, 0))
				{
					WSABUF sendingWSA_BufferA[ADVANCED_WAN_SERVER_WSABUF_COUNT];
					UINT sendingWSA_BufferCount;

					sendingWSA_BufferCount = AWAN_CSV[sessionIndex].SendQ.GetCurrentCount();
					if (ADVANCED_WAN_SERVER_WSABUF_COUNT < sendingWSA_BufferCount) sendingWSA_BufferCount = ADVANCED_WAN_SERVER_WSABUF_COUNT;

					FOR(i, sendingWSA_BufferCount)
					{
						SerialPacketQueue<AdvancedWAN_PacketHeader>* sendingPacketSPQ_Address;
						WSABUF wsaBuffer;

						sendingPacketSPQ_Address = AWAN_CSV[sessionIndex].SendQ.Peek(i); //Dequeue();
						wsaBuffer.buf = sendingPacketSPQ_Address->GetQueueAddress();
						wsaBuffer.len = sendingPacketSPQ_Address->GetTotalSize();

						sendingWSA_BufferA[i] = wsaBuffer;
					}

					if (!sendingWSA_BufferCount) InterlockedExchange(&AWAN_CSV[sessionIndex].Sendable, 1);
					else
					{
						InterlockedIncrement(&AWAN_CSV[sessionIndex].IO_Count);
						++AWAN_CSV[sessionIndex].SendCount;

						result = WSASend(AWAN_CSV[sessionIndex].Socket, sendingWSA_BufferA, sendingWSA_BufferCount, nullptr, flag, &AWAN_CSV[sessionIndex].OverlappedSend, nullptr);
						if (result == SOCKET_ERROR)
						{
							if (WSAGetLastError() == WSA_IO_PENDING);
							else
							{
								InterlockedDecrement(&AWAN_CSV[sessionIndex].IO_Count);
								--AWAN_CSV[sessionIndex].SendCount;

								if(INDICATOR_ON) printf("[WSASend] sessionIndex : %u, sessionKey : %llu, IO_Count : %u, WSAGetLastError() : %d" CRALF, sessionIndex, AWAN_CSV[sessionIndex].Key, AWAN_CSV[sessionIndex].IO_Count, WSAGetLastError());
							}
						}
					}
				}

				//InterlockedDecrement(&AWAN_CSV[sessionIndex].IO_Count);
				--AWAN_CSV[sessionIndex].SendCount;
			}

			//if(!AWAN_CSV[sessionIndex].ReceiveCount && !AWAN_CSV[sessionIndex].SendCount)
			if (!InterlockedDecrement(&AWAN_CSV[sessionIndex].IO_Count))
			{
				if (InterlockedExchange(&AWAN_CSV[sessionIndex].Connected, 0) && AWAN_CSV[sessionIndex].Key == sessionKey)
				{
					AWAN_CSV[sessionIndex].CurrentPhaseIndex = ADVANCED_WAN_CLIENT_PHASE_DISCONNECT;

					PostQueuedCompletionStatus(AWAN_S_DISCONNECT_CP, NULL, AWAN_CSV[sessionIndex].Key, overlapped);
				}

				//THIS->PostDisconnect(AWAN_CSV[sessionIndex].Key);
				//THIS->Disconnect(AWAN_CSV[sessionIndex].Key);
			}
		}
		else
		{
			if (GetLastError() == ERROR_ABANDONED_WAIT_0) break;

			if (completionKey || transferredSize) continue;
			else break;
		}
	}

	THIS->OnWorkerThreadEnd();

	return 0;
}