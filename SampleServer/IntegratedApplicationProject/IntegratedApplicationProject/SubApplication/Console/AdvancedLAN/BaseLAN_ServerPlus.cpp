#include "stdafx.h"
#include "AdvancedLAN.h"
#include "BaseLAN_Server.h"

unsigned int WINAPI BaseLAN_Server::AcceptAdvancedLAN_Client(LPVOID selfInstanceAddress)
{
	AdvancedLAN_ClientSession acceptedClientSession;
	INT addressLength;
	UINT64 sessionKey;
	//SessionUniqueKey sessionUniqueKey;

	addressLength = sizeof(acceptedClientSession.Address);

	if (!THIS) THIS = (BaseLAN_Server*)selfInstanceAddress;

	while (!ALAN_S_SHUTDOWN)
	{
		acceptedClientSession.Socket = WSAAccept(ALAN_SS.Socket, (SOCKADDR*)&acceptedClientSession.Address, &addressLength, nullptr, NULL);
		CHECK_INVALID_SOCKET(acceptedClientSession.Socket);
		//IF_INVALID_SOCKET(acceptedClientSession.Socket);
		//ELSE
		{
			if(THIS->OnConnectionRequest(acceptedClientSession.Address, acceptedClientSession.Address.sin_port));

			FOR(i, TOTAL_ADVANCED_LAN_CLIENT_COUNT)
			{
				if (ALAN_CSV[i].CurrentPhaseIndex == ADVANCED_LAN_CLIENT_PHASE_ACCEPT)
				{
					++CURRENT_ACCEPTION_COUNT;
					sessionKey = i;
					sessionKey = sessionKey << ADVANCED_LAN_CLIENT_INDEX_SHIFTED_BIT_COUNT;
					sessionKey += CURRENT_ACCEPTION_COUNT;

					IF_NULL(CreateIoCompletionPort((HANDLE)acceptedClientSession.Socket, ALAN_S_COMMON_CP, sessionKey, NULL))
					ELSE
					{
						ALAN_CSV[i].Socket = acceptedClientSession.Socket;
						ALAN_CSV[i].Address = acceptedClientSession.Address;
						ALAN_CSV[i].Key = sessionKey;
						ALAN_CSV[i].Index = i;
						ALAN_CSV[i].Connected = 1;
						ALAN_CSV[i].Sendable = 1;
						ALAN_CSV[i].ReceiveCount = 1;
						ALAN_CSV[i].SendCount = 0;

						ZeroMemory(&ALAN_CSV[i].OverlappedReceive, sizeof(ALAN_CSV[i].OverlappedReceive));
						ZeroMemory(&ALAN_CSV[i].OverlappedSend, sizeof(ALAN_CSV[i].OverlappedSend));

						ALAN_CSV[i].IO_Count = 1;

						InitializeSRWLock(&ALAN_CSV[i].Lock);

						ALAN_CSV[i].CurrentPhaseIndex = ADVANCED_LAN_CLIENT_PHASE_UPDATE;

						++ALAN_CSV[i].AccumulatedUseCount;

						THIS->OnClientJoin(ALAN_CSV[i].Address, sessionKey);

						PostQueuedCompletionStatus(ALAN_S_COMMON_CP, NULL, sessionKey, &ALAN_CSV[i].OverlappedReceive);
						//PostQueuedCompletionStatus(ALAN_S_RECEIVE_CP, NULL, sessionKey, &ALAN_CSV[i].OverlappedReceive);

						break;
					}
				}

				if (i == TOTAL_ADVANCED_LAN_CLIENT_COUNT - 1)
				{
					i = 0;

					Sleep(0);
					//YieldProcessor(); //hyperthreading technology needed
				}
			}
		}
	}

	return 0;
}

unsigned int WINAPI BaseLAN_Server::DisconnectAdvancedLAN_Client(LPVOID selfInstanceAddress)
{
	DWORD transferredSize;
	ULONG_PTR completionKey;
	LPOVERLAPPED overlapped;
	UINT64 sessionKey;
	UINT sessionIndex; 

	while (!ALAN_S_SHUTDOWN)
	{
		transferredSize = 0;
		completionKey = 0;
		overlapped = nullptr;
		sessionKey = 0xffffffffffffffff;
		sessionIndex = 0xffffffff;

		GetQueuedCompletionStatus(ALAN_S_DISCONNECT_CP, &transferredSize, &completionKey, &overlapped, INFINITE);
		
		sessionKey = completionKey;
		sessionIndex = sessionKey >> ADVANCED_LAN_CLIENT_INDEX_SHIFTED_BIT_COUNT;
		
		if (overlapped)
		{
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

unsigned int WINAPI BaseLAN_Server::WorkAdvancedLAN_Server(LPVOID selfInstanceAddress)
{
	DWORD transferredSize;
	ULONG_PTR completionKey;
	LPOVERLAPPED overlapped;
	UINT64 sessionKey;
	UINT sessionIndex;

	DWORD flag;
	INT result;

	if (!THIS) THIS = (BaseLAN_Server*)selfInstanceAddress;

	THIS->OnWorkerThreadBegin();

	while (!ALAN_S_SHUTDOWN)
	{
		transferredSize = 0;
		completionKey = 0;
		overlapped = nullptr;
		sessionKey = 0xffffffffffffffff;
		sessionIndex = 0xffffffff;

		flag = 0;

		GetQueuedCompletionStatus(ALAN_S_COMMON_CP, &transferredSize, &completionKey, &overlapped, INFINITE);

		sessionKey = completionKey;
		sessionIndex = sessionKey >> ADVANCED_LAN_CLIENT_INDEX_SHIFTED_BIT_COUNT;

		if (overlapped)
		{
			//if (ALAN_CSV[sessionIndex].CurrentPhaseIndex == ADVANCED_LAN_CLIENT_PHASE_UPDATE)
			{
				if (&ALAN_CSV[sessionIndex].OverlappedReceive == overlapped)
				{
					ZeroMemory(&ALAN_CSV[sessionIndex].OverlappedReceive, sizeof(ALAN_CSV[sessionIndex].OverlappedReceive));

					if (transferredSize)
					{
						INT receivedSize;

						receivedSize = transferredSize;
						ALAN_CSV[sessionIndex].ReceiveBQ.IncreaseCurrentSize(receivedSize);

						THIS->OnRecv(ALAN_CSV[sessionIndex].Key, receivedSize);

						//ALAN_CSV[sessionIndex].CurrentPhaseIndex = ADVANCED_LAN_CLIENT_PHASE_UPDATE;
					}

					InterlockedIncrement(&ALAN_CSV[sessionIndex].IO_Count);
					++ALAN_CSV[sessionIndex].ReceiveCount;

					result = ALAN_CSV[sessionIndex].ReceiveBQ.WSAReceive(WSARecv, ALAN_CSV[sessionIndex].Socket, &flag, &ALAN_CSV[sessionIndex].OverlappedReceive);
					if (result == SOCKET_ERROR)
					{
						if (WSAGetLastError() == WSA_IO_PENDING);
						else
						{
							InterlockedDecrement(&ALAN_CSV[sessionIndex].IO_Count);
							--ALAN_CSV[sessionIndex].ReceiveCount;

							//printf("[WSARecv] sessionIndex : %u, sessionKey : %llu, IO_Count : %d, WSAGetLastError() : %d" CRALF, sessionIndex, ALAN_CSV[sessionIndex].Key, ALAN_CSV[sessionIndex].IO_Count, WSAGetLastError());
						}
					}

					//InterlockedDecrement(&ALAN_CSV[sessionIndex].IO_Count);
					--ALAN_CSV[sessionIndex].ReceiveCount;
				}

				if (&ALAN_CSV[sessionIndex].OverlappedSend == overlapped)
				{
					if (transferredSize)
					{
						INT sendedSize;

						sendedSize = transferredSize;

						while (0 < sendedSize)
						{
							SerialPacketQueue<AdvancedLAN_PacketHeader>* sendedPacketSPQ_Address;

							sendedPacketSPQ_Address = ALAN_CSV[sessionIndex].SendQ.Dequeue();

							sendedSize -= sendedPacketSPQ_Address->GetTotalSize();

							//if (!sendedPacketSPQ_Address->DecreaseReferenceCount())
							{
								InterlockedIncrement(&SPQ_DEALLOCATION_COUNT);
								delete sendedPacketSPQ_Address;
							}
						}

						THIS->OnSend(ALAN_CSV[sessionIndex].Key, sendedSize);

						ALAN_CSV[sessionIndex].CurrentPhaseIndex = ADVANCED_LAN_CLIENT_PHASE_UPDATE;

						//InterlockedExchange(&ALAN_CSV[sessionIndex].Sendable, 1);
					}
					//else
					//sndPost
					//if (InterlockedExchange(&ALAN_CSV[sessionIndex].Sendable, 0))
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

					//InterlockedDecrement(&ALAN_CSV[sessionIndex].IO_Count);
					--ALAN_CSV[sessionIndex].SendCount;
				}

				//if(!ALAN_CSV[sessionIndex].ReceiveCount && !ALAN_CSV[sessionIndex].SendCount) //possible?
				if (!InterlockedDecrement(&ALAN_CSV[sessionIndex].IO_Count))
				{
					ALAN_CSV[sessionIndex].CurrentPhaseIndex = ADVANCED_LAN_CLIENT_PHASE_DISCONNECT;

					PostQueuedCompletionStatus(ALAN_S_DISCONNECT_CP, NULL, ALAN_CSV[sessionIndex].Key, overlapped);
					//THIS->PostDisconnect(ALAN_CSV[sessionIndex].Key);
					//THIS->Disconnect(ALAN_CSV[sessionIndex].Key);
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

	THIS->OnWorkerThreadEnd();

	return 0;
}

//Receive ready
//Receive pending
//Receive complete

//Send ready
//Send pending
//Send complete