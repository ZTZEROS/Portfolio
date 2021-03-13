#include "pch.h"
#include "MMO.h"
#include "MMO_BaseClient.h"
#include "MMO_DerivedClient.h"
#include "MMO_BaseServer.h"

unsigned int MMO_BaseServer::AcceptThread()
{
	MMO_ClientSession clientSession;
	INT addressLength;

	addressLength = sizeof(clientSession.Address);

	while (SUB_THREAD_ON)
	{
		clientSession.Socket = WSAAccept(SS.Socket, (SOCKADDR*)&clientSession.Address, &addressLength, nullptr, NULL);
		CHECK_INVALID_SOCKET(clientSession.Socket);
		//IF_INVALID_SOCKET(clientSession.Socket);
		//ELSE
		{
			SerialPacketQueue<IOCP_PacketHeader>* acceptedClientSPQ_Address;

			acceptedClientSPQ_Address = SPQ_TLP_Allocate();

			*acceptedClientSPQ_Address << clientSession.Socket;
			*acceptedClientSPQ_Address << clientSession.Address;

			acceptedClientSPQ_Address->IncreaseReferenceCount();
			
			while (ACCEPTED_CLIENT_Q.Enqueue(acceptedClientSPQ_Address) != acceptedClientSPQ_Address)
			{
				UINT sleepTime;
				
				sleepTime = ACCEPTED_CLIENT_Q.GetCurrentCount();

				sleepTime /= 1000;

				Sleep(sleepTime);
			}

			OnConnect();

			SPQ_TLP_Deallocate(acceptedClientSPQ_Address);
		}
	}

	return 0;
}

unsigned int MMO_BaseServer::DisconnectThread()
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

		GetQueuedCompletionStatus(DISCONNECT_CP, &transferredSize, &completionKey, &overlapped, INFINITE);

		sessionKey = completionKey;
		sessionIndex = sessionKey >> IOCP_CLIENT_INDEX_SHIFTED_BIT_COUNT;

		if (overlapped)
		{
			//if (InterlockedExchange(&CSV[sessionIndex].Connected, 0) && CSV[sessionIndex].Key == sessionKey)
			{
				if (BCAV[sessionIndex]->CS.CurrentPhase == MMO_CLIENT_PHASE_DISCONNECT)
				{
					//ProcessRemainedDatum
					{

					}

					OnDisconnect();

					closesocket(BCAV[sessionIndex]->CS.Socket);

					BCAV[sessionIndex]->CS.Socket = INVALID_SOCKET;
					BCAV[sessionIndex]->CS.Address;

					BCAV[sessionIndex]->CS.Key = 0xffffffffffffffff;
					BCAV[sessionIndex]->CS.Index = 0xffffffff;

					BCAV[sessionIndex]->CS.Connected = 0;
					BCAV[sessionIndex]->CS.Sendable = 0;
					BCAV[sessionIndex]->CS.ReceiveCount = 0;
					BCAV[sessionIndex]->CS.SendCount = 0;

					ZeroMemory(&(BCAV[sessionIndex]->CS.OverlappedReceive), sizeof(BCAV[sessionIndex]->CS.OverlappedReceive));
					ZeroMemory(&(BCAV[sessionIndex]->CS.OverlappedSend), sizeof(BCAV[sessionIndex]->CS.OverlappedSend));

					while (1) //CSV[sessionIndex].ReceiveQ.GetCurrentCount();
					{
						SerialPacketQueue<IOCP_PacketHeader>* receivedPacketSPQ_Address;

						receivedPacketSPQ_Address = BCAV[sessionIndex]->CS.ReceiveQ.Dequeue();

						if (receivedPacketSPQ_Address) SPQ_TLP_Deallocate(receivedPacketSPQ_Address);
						else break;
					}

					while (1) //CSV[sessionIndex].SendQ.GetCurrentCount();
					{
						SerialPacketQueue<IOCP_PacketHeader>* sendedPacketSPQ_Address;

						sendedPacketSPQ_Address = BCAV[sessionIndex]->CS.SendQ.Dequeue();

						if (sendedPacketSPQ_Address) SPQ_TLP_Deallocate(sendedPacketSPQ_Address);
						else break;
					}

					BCAV[sessionIndex]->CS.ReceiveQ.Clear();
					BCAV[sessionIndex]->CS.SendQ.Clear();
					BCAV[sessionIndex]->CS.ReceiveBQ.Clear();
					BCAV[sessionIndex]->CS.SendBQ.Clear();

					//BCAV[sessionIndex]->CS.ReceiveQ.Terminalize();
					//BCAV[sessionIndex]->CS.SendQ.Terminalize();
					//BCAV[sessionIndex]->CS.ReceiveBQ.Terminalize();
					//BCAV[sessionIndex]->CS.SendBQ.Terminalize();

					//BCAV[sessionIndex]->CS.IO_Count = 0;

					BCAV[sessionIndex]->CS.CurrentPhase = MMO_CLIENT_PHASE_ACCEPT;
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

unsigned int MMO_BaseServer::SendThread()
{
	DWORD sleepTime;

	while (SUB_THREAD_ON)
	{
		sleepTime = MAXIMUM_CLIENT_COUNT;

		FOR(i, MAXIMUM_CLIENT_COUNT)
		{
			if (!BCAV[i]->CS.Disconnecting && ((BCAV[i]->CS.CurrentPhase) == MMO_CLIENT_PHASE_AUTHORIZE || (BCAV[i]->CS.CurrentPhase) == MMO_CLIENT_PHASE_GAME))
			{
				if (BCAV[i]->CS.SendQ.GetCurrentCount())
				{
					if (InterlockedExchange(&(BCAV[i]->CS.Sendable), 0))
					{
						InterlockedIncrement(&(BCAV[i]->CS.IO_Count));
						++(BCAV[i]->CS.SendCount);

						//PostQueuedCompletionStatus(WORK_CP, NULL, BCAV[i]->CS.Key, &BCAV[i]->CS.OverlappedSend);
						
						//WSASend
						{
							DWORD flag;
							INT result;
						
							WSABUF sendingWSA_BufferA[IOCP_SERVER_WSA_BUFFER_COUNT];
							UINT sendingWSA_BufferCount;
						
							flag = 0;
							result = 0;
						
							sendingWSA_BufferCount = BCAV[i]->CS.SendQ.GetCurrentCount();
							if (IOCP_SERVER_WSA_BUFFER_COUNT < sendingWSA_BufferCount) sendingWSA_BufferCount = IOCP_SERVER_WSA_BUFFER_COUNT;
						
							if (!sendingWSA_BufferCount) continue; //InterlockedExchange(&(BCAV[i]->CS.Sendable), 1);
							else
							{
								FOR(j, sendingWSA_BufferCount)
								{
									SerialPacketQueue<IOCP_PacketHeader>* sendingPacketSPQ_Address;
									WSABUF wsaBuffer;
						
									sendingPacketSPQ_Address = BCAV[i]->CS.SendQ.Peek(j); //Dequeue();
						
									wsaBuffer.buf = sendingPacketSPQ_Address->GetQueueAddress();
									wsaBuffer.len = sendingPacketSPQ_Address->GetTotalSize();
						
									sendingWSA_BufferA[j] = wsaBuffer;
								}
						
								InterlockedIncrement(&(BCAV[i]->CS.IO_Count));
								++(BCAV[i]->CS.SendCount);
						
								result = WSASend(BCAV[i]->CS.Socket, sendingWSA_BufferA, sendingWSA_BufferCount, nullptr, flag, &(BCAV[i]->CS.OverlappedSend), nullptr);
								if (result == SOCKET_ERROR)
								{
									if (WSAGetLastError() == WSA_IO_PENDING);
									else
									{
										InterlockedDecrement(&(BCAV[i]->CS.IO_Count));
										--(BCAV[i]->CS.SendCount);
						
										//if (INDICATOR_ON) printf("[WSASend] sessionIndex : %u, sessionKey : %llu, IO_Count : %u, WSAGetLastError() : %d" CRALF, sessionIndex, CSV[sessionIndex].Key, CSV[sessionIndex].IO_Count, WSAGetLastError());
									}
								}
							}
													
							//InterlockedDecrement(&BCAV[sessionIndex]->CS.IO_Count);
							--(BCAV[i]->CS.SendCount);
						}

						//if(!BCAV[sessionIndex]->CS.ReceiveCount && !BCAV[sessionIndex]->CS.SendCount)
						if (!InterlockedDecrement(&(BCAV[i]->CS.IO_Count)))
						{
							if (InterlockedExchange(&(BCAV[i]->CS.Connected), 0))// && BCAV[i]->CS.Key == sessionKey)
							{
								BCAV[i]->CS.Disconnecting = 1;

								//PostQueuedCompletionStatus(DISCONNECT_CP, NULL, BCAV[sessionIndex]->CS.Key, overlapped);
							}

							//PostDisconnect(BCAV[sessionIndex]->CS.Key);
							//Disconnect(BCAV[sessionIndex]->CS.Key);
						}

						--sleepTime;
					}
				}
			}
		}

		sleepTime /= 1000;

		Sleep(sleepTime);
	}

	return 0;
}

unsigned int MMO_BaseServer::AuthorizeThread()
{
	UINT64 sessionKey;
	UINT sessionIndex;
	DWORD sleepTime;

	sessionKey = 0xffffffffffffffff;
	sessionIndex = 0xffffffff;

	while (SUB_THREAD_ON)
	{
		sleepTime = MAXIMUM_CLIENT_COUNT;

		while (ACCEPTED_CLIENT_Q.GetCurrentCount())
		{
			SerialPacketQueue<IOCP_PacketHeader>* acceptedClientSPQ_Address;
			MMO_ClientSession clientSession;

			acceptedClientSPQ_Address = ACCEPTED_CLIENT_Q.Dequeue();

			*acceptedClientSPQ_Address >> clientSession.Socket;
			*acceptedClientSPQ_Address >> clientSession.Address;

			SPQ_TLP_Deallocate(acceptedClientSPQ_Address);

			while (1)
			{
				++sessionIndex;

				if (sessionIndex < MAXIMUM_CLIENT_COUNT);
				else sessionIndex = 0;

				if (BCAV[sessionIndex]->CS.CurrentPhase == MMO_CLIENT_PHASE_ACCEPT)
				{
					++CURRENT_ACCEPTION_COUNT;
					sessionKey = sessionIndex;
					sessionKey = sessionKey << IOCP_CLIENT_INDEX_SHIFTED_BIT_COUNT;
					sessionKey += CURRENT_ACCEPTION_COUNT;

					IF_NULL(CreateIoCompletionPort((HANDLE)clientSession.Socket, WORK_CP, sessionKey, NULL))
					ELSE
					{
						BCAV[sessionIndex]->CS.Socket = clientSession.Socket;
						BCAV[sessionIndex]->CS.Address = clientSession.Address;
						BCAV[sessionIndex]->CS.Key = sessionKey;
						BCAV[sessionIndex]->CS.Index = sessionIndex;
						BCAV[sessionIndex]->CS.Connected = 1;
						BCAV[sessionIndex]->CS.Sendable = 1;
						BCAV[sessionIndex]->CS.ReceiveCount = 1;
						BCAV[sessionIndex]->CS.SendCount = 0;

						BCAV[sessionIndex]->CS.Disconnecting = 0;
						BCAV[sessionIndex]->CS.FromAuthorizeToGame = 0;

						ZeroMemory(&(BCAV[sessionIndex]->CS.OverlappedReceive), sizeof(BCAV[sessionIndex]->CS.OverlappedReceive));
						ZeroMemory(&(BCAV[sessionIndex]->CS.OverlappedSend), sizeof(BCAV[sessionIndex]->CS.OverlappedSend));

						//CSV[sessionIndex].IO_Count = 1;
						InterlockedIncrement(&(BCAV[sessionIndex]->CS.IO_Count));

						InitializeSRWLock(&(BCAV[sessionIndex]->CS.Lock));

						BCAV[sessionIndex]->BS_Address = this;

						BCAV[sessionIndex]->OnAT_ClientJoin();

						PostQueuedCompletionStatus(WORK_CP, NULL, sessionKey, &(BCAV[sessionIndex]->CS.OverlappedReceive));
						//PostQueuedCompletionStatus(RECEIVE_CP, NULL, sessionKey, &BCAV[sessionIndex]->CS.OverlappedReceive);

						BCAV[sessionIndex]->CS.CurrentPhase = MMO_CLIENT_PHASE_AUTHORIZE;

						BCAL.push_back(BCAV[sessionIndex]);

						break;
					}
				}
			}
		}



		for (MMO_BaseClientAddressLI bcaLI = BCAL.begin(); bcaLI != BCAL.end(); ++bcaLI)
		{
			if ((*bcaLI)->CS.CurrentPhase == MMO_CLIENT_PHASE_AUTHORIZE)
			{
				(*bcaLI)->OnAT_ClientReceive();
			}

			--sleepTime;
		}

		OnAuthorizeThreadUpdate();

		for (MMO_BaseClientAddressLI bcaLI = BCAL.begin(); bcaLI != BCAL.end();)
		{
			if ((*bcaLI)->CS.CurrentPhase == MMO_CLIENT_PHASE_AUTHORIZE && (*bcaLI)->CS.Disconnecting)
			{
				(*bcaLI)->OnAT_ClientLeave();
				(*bcaLI)->CS.CurrentPhase = MMO_CLIENT_PHASE_FROM_AUTHORIZE_TO_DISCONNECT;

				bcaLI = BCAL.erase(bcaLI);

				continue;
			}

			if ((*bcaLI)->CS.CurrentPhase == MMO_CLIENT_PHASE_AUTHORIZE && (*bcaLI)->CS.FromAuthorizeToGame)
			{
				(*bcaLI)->OnAT_ClientLeave();
				(*bcaLI)->CS.CurrentPhase = MMO_CLIENT_PHASE_FROM_AUTHORIZE_TO_GAME;

				bcaLI = BCAL.erase(bcaLI);

				continue;
			}
			
			++bcaLI;
		}

		sleepTime /= 1000;

		Sleep(sleepTime);
	}

	return 0;
}

unsigned int MMO_BaseServer::GameThread()
{
	DWORD sleepTime;

	while (SUB_THREAD_ON)
	{
		sleepTime = MAXIMUM_CLIENT_COUNT;

		FOR(i, MAXIMUM_CLIENT_COUNT)
		{
			if (BCAV[i]->CS.CurrentPhase == MMO_CLIENT_PHASE_FROM_AUTHORIZE_TO_GAME)
			{
				BCAV[i]->OnGT_ClientJoin();
				BCAV[i]->CS.CurrentPhase = MMO_CLIENT_PHASE_GAME;
			}

			if (BCAV[i]->CS.CurrentPhase == MMO_CLIENT_PHASE_GAME)
			{
				BCAV[i]->OnGT_ClientReceive();
			}
		}

		OnGameThreadUpdate();

		FOR(i, MAXIMUM_CLIENT_COUNT)
		{
			if (BCAV[i]->CS.CurrentPhase == MMO_CLIENT_PHASE_GAME && BCAV[i]->CS.Disconnecting)
			{
				BCAV[i]->CS.CurrentPhase = MMO_CLIENT_PHASE_FROM_GAME_TO_DISCONNECT;
			}

			if (BCAV[i]->CS.CurrentPhase == MMO_CLIENT_PHASE_FROM_AUTHORIZE_TO_DISCONNECT || BCAV[i]->CS.CurrentPhase == MMO_CLIENT_PHASE_FROM_GAME_TO_DISCONNECT)
			{
				//if(!BCAV[i]->CS.Sendable)
				{
					BCAV[i]->OnGT_ClientLeave();
					BCAV[i]->CS.CurrentPhase = MMO_CLIENT_PHASE_WAIT_FOR_DISCONNECT;
				}
			}

			if (BCAV[i]->CS.CurrentPhase == MMO_CLIENT_PHASE_WAIT_FOR_DISCONNECT)
			{
				PostDisconnect(BCAV[i]->CS.Key);
			}
		}

		sleepTime /= 1000;

		Sleep(sleepTime);
	}

	return 0;
}

unsigned int MMO_BaseServer::WorkThread()
{
	IOCP_Server* thisInstanceAddress;
	DWORD transferredSize;
	ULONG_PTR completionKey;
	LPOVERLAPPED overlapped;
	UINT64 sessionKey;
	UINT sessionIndex;

	DWORD flag;
	INT result;
	UINT remainedSize;

	flag = 0;
	remainedSize = 0;

	while (SUB_THREAD_ON)
	{
		transferredSize = 0;
		completionKey = 0;
		overlapped = nullptr;
		sessionKey = 0xffffffffffffffff;
		sessionIndex = 0xffffffff;

		result = 0;

		GetQueuedCompletionStatus(WORK_CP, &transferredSize, &completionKey, &overlapped, INFINITE);

		sessionKey = completionKey;
		sessionIndex = sessionKey >> IOCP_CLIENT_INDEX_SHIFTED_BIT_COUNT;

		if (overlapped)
		{
			if (&(BCAV[sessionIndex]->CS.OverlappedReceive) == overlapped)
			{
				ZeroMemory(&(BCAV[sessionIndex]->CS.OverlappedReceive), sizeof(BCAV[sessionIndex]->CS.OverlappedReceive));

				if (transferredSize)
				{
					UINT receivedSize;

					receivedSize = transferredSize;

					BCAV[sessionIndex]->CS.ReceiveBQ.IncreaseCurrentSize(receivedSize);

					receivedSize += remainedSize;

					remainedSize = 0;

					while (receivedSize)
					{
						SerialPacketQueue<IOCP_PacketHeader>* receivedPacketSPQ_Address;

						receivedPacketSPQ_Address = SPQ_TLP_Allocate();

						if (BCAV[sessionIndex]->ReceivePacket(receivedPacketSPQ_Address))
						{
							receivedSize -= receivedPacketSPQ_Address->GetTotalSize();

							OnReceive(receivedPacketSPQ_Address->GetTotalSize());

							SPQ_TLP_Deallocate(receivedPacketSPQ_Address);
						}
						else
						{
							//리시브 큐가 모자라 실패한 경우의 대비가 안 되어 있음. 위험

							SPQ_TLP_Deallocate(receivedPacketSPQ_Address);

							remainedSize = receivedSize;

							break;
						}
					}

					if (receivedSize) receivedSize = transferredSize - receivedSize;
					else receivedSize = transferredSize;

					//OnReceive(receivedSize);
					{
						
					}
				}

				InterlockedIncrement(&(BCAV[sessionIndex]->CS.IO_Count));
				++BCAV[sessionIndex]->CS.ReceiveCount;

				result = BCAV[sessionIndex]->CS.ReceiveBQ.WSAReceive(WSARecv, BCAV[sessionIndex]->CS.Socket, &flag, &(BCAV[sessionIndex]->CS.OverlappedReceive));
				if (result == SOCKET_ERROR)
				{
					if (WSAGetLastError() == WSA_IO_PENDING);
					else
					{
						InterlockedDecrement(&(BCAV[sessionIndex]->CS.IO_Count));
						--BCAV[sessionIndex]->CS.ReceiveCount;

						//if (INDICATOR_ON) printf("[WSARecv] sessionIndex : %u, sessionKey : %llu, IO_Count : %u, WSAGetLastError() : %d" CRALF, sessionIndex, CSV[sessionIndex].Key, CSV[sessionIndex].IO_Count, WSAGetLastError());
					}
				}

				//InterlockedDecrement(&BCAV[sessionIndex]->CS.IO_Count);
				--(BCAV[sessionIndex]->CS.ReceiveCount);
			}

			if (&(BCAV[sessionIndex]->CS.OverlappedSend) == overlapped)
			{
				if (transferredSize)
				{
					UINT sendedSize;

					sendedSize = transferredSize;

					while (sendedSize)
					{
						SerialPacketQueue<IOCP_PacketHeader>* sendedPacketSPQ_Address;

						sendedPacketSPQ_Address = BCAV[sessionIndex]->CS.SendQ.Dequeue();

						sendedSize -= sendedPacketSPQ_Address->GetTotalSize();

						OnSend(sendedPacketSPQ_Address->GetTotalSize());

						SPQ_TLP_Deallocate(sendedPacketSPQ_Address);
					}

					if (sendedSize) sendedSize = transferredSize - sendedSize;
					else sendedSize = transferredSize;

					//OnSend(sendedSize);
					{

					}

					InterlockedExchange(&(BCAV[sessionIndex]->CS.Sendable), 1);
				}
				//else
				//sndPost
				//if (InterlockedExchange(&CSV[sessionIndex].Sendable, 0))
				//{
				//	WSABUF sendingWSA_BufferA[IOCP_SERVER_WSA_BUFFER_COUNT];
				//	UINT sendingWSA_BufferCount;
				//
				//	sendingWSA_BufferCount = BCAV[sessionIndex]->CS.SendQ.GetCurrentCount();
				//	if (IOCP_SERVER_WSA_BUFFER_COUNT < sendingWSA_BufferCount) sendingWSA_BufferCount = IOCP_SERVER_WSA_BUFFER_COUNT;
				//
				//	if (!sendingWSA_BufferCount) InterlockedExchange(&(BCAV[sessionIndex]->CS.Sendable), 1);
				//	else
				//	{
				//		FOR(i, sendingWSA_BufferCount)
				//		{
				//			SerialPacketQueue<IOCP_PacketHeader>* sendingPacketSPQ_Address;
				//			WSABUF wsaBuffer;
				//
				//			sendingPacketSPQ_Address = BCAV[sessionIndex]->CS.SendQ.Peek(i); //Dequeue();
				//			wsaBuffer.buf = sendingPacketSPQ_Address->GetQueueAddress();
				//			wsaBuffer.len = sendingPacketSPQ_Address->GetTotalSize();
				//
				//			sendingWSA_BufferA[i] = wsaBuffer;
				//		}
				//
				//		InterlockedIncrement(&(BCAV[sessionIndex]->CS.IO_Count));
				//		++(BCAV[sessionIndex]->CS.SendCount);
				//
				//		result = WSASend(BCAV[sessionIndex]->CS.Socket, sendingWSA_BufferA, sendingWSA_BufferCount, nullptr, flag, &(BCAV[sessionIndex]->CS.OverlappedSend), nullptr);
				//		if (result == SOCKET_ERROR)
				//		{
				//			if (WSAGetLastError() == WSA_IO_PENDING);
				//			else
				//			{
				//				InterlockedDecrement(&(BCAV[sessionIndex]->CS.IO_Count));
				//				--(BCAV[sessionIndex]->CS.SendCount);
				//
				//				//if (INDICATOR_ON) printf("[WSASend] sessionIndex : %u, sessionKey : %llu, IO_Count : %u, WSAGetLastError() : %d" CRALF, sessionIndex, CSV[sessionIndex].Key, CSV[sessionIndex].IO_Count, WSAGetLastError());
				//			}
				//		}
				//	}
				//}

				//InterlockedDecrement(&BCAV[sessionIndex]->CS.IO_Count);
				--(BCAV[sessionIndex]->CS.SendCount);
			}

			//if(!BCAV[sessionIndex]->CS.ReceiveCount && !BCAV[sessionIndex]->CS.SendCount)
			if (!InterlockedDecrement(&(BCAV[sessionIndex]->CS.IO_Count)))
			{
				if (InterlockedExchange(&(BCAV[sessionIndex]->CS.Connected), 0) && BCAV[sessionIndex]->CS.Key == sessionKey)
				{
					BCAV[sessionIndex]->CS.Disconnecting = 1;

					//PostQueuedCompletionStatus(DISCONNECT_CP, NULL, BCAV[sessionIndex]->CS.Key, overlapped);
				}

				//PostDisconnect(BCAV[sessionIndex]->CS.Key);
				//Disconnect(BCAV[sessionIndex]->CS.Key);
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

bool MMO_BaseServer::Disconnect(UINT64 sessionKey)
{
	UINT sessionIndex;

	sessionIndex = sessionKey >> IOCP_CLIENT_INDEX_SHIFTED_BIT_COUNT;
	if (1 < InterlockedIncrement(&(BCAV[sessionIndex]->CS.IO_Count)))
	{
		if (BCAV[sessionIndex]->CS.Connected && BCAV[sessionIndex]->CS.Key == sessionKey)
		{
			shutdown(BCAV[sessionIndex]->CS.Socket, SD_BOTH);

			PostQueuedCompletionStatus(WORK_CP, NULL, BCAV[sessionIndex]->CS.Key, &(BCAV[sessionIndex]->CS.OverlappedSend));
			//CancelIo((HANDLE)CSV[sessionIndex].Socket);
			//CancelIoEx((HANDLE)CSV[sessionIndex].Socket, &CSV[sessionIndex].OverlappedReceive);
		}

		//{
		//	//if(!CSV[sessionIndex].ReceiveCount & !CSV[sessionIndex].SendCount)
		//	if (!InterlockedDecrement(&CSV[sessionIndex].IO_Count))
		//	{
		//		if (InterlockedExchange(&CSV[sessionIndex].Connected, 0) && CSV[sessionIndex].Key == sessionKey)
		//		{
		//			CSV[sessionIndex].CurrentPhase = IOCP_CLIENT_PHASE_DISCONNECT;
		//			
		//			PostQueuedCompletionStatus(DISCONNECT_CP, NULL, CSV[sessionIndex].Key, &CSV[sessionIndex].OverlappedSend);
		//		}
		//
		//		//THIS->PostDisconnect(CSV[sessionIndex].Key);
		//		//THIS->Disconnect(CSV[sessionIndex].Key);
		//	}
		//}
	}
	else InterlockedDecrement(&(BCAV[sessionIndex]->CS.IO_Count));

	return 0;
}

unsigned int MMO_BaseServer::PostSend(UINT64 sessionKey)
{
	UINT sessionIndex;

	UINT result;

	sessionIndex = sessionKey >> IOCP_CLIENT_INDEX_SHIFTED_BIT_COUNT;

	result = BCAV[sessionIndex]->CS.SendQ.GetCurrentCount();
	if (result)
	{
		if (InterlockedExchange(&(BCAV[sessionIndex]->CS.Sendable), 0))
		{
			InterlockedIncrement(&(BCAV[sessionIndex]->CS.IO_Count));
			++(BCAV[sessionIndex]->CS.SendCount);

			PostQueuedCompletionStatus(WORK_CP, NULL, BCAV[sessionIndex]->CS.Key, &(BCAV[sessionIndex]->CS.OverlappedSend));
		}
		else result = 0;
	}
	else result = 0;

	return result;
}

unsigned int MMO_BaseServer::PostDisconnect(UINT64 sessionKey)
{
	UINT sessionIndex;

	UINT result;

	sessionIndex = sessionKey >> IOCP_CLIENT_INDEX_SHIFTED_BIT_COUNT;
	//if (InterlockedExchange(&(BCAV[sessionIndex]->CS.Connected), 0) && BCAV[sessionIndex]->CS.Key == sessionKey)
	{
		BCAV[sessionIndex]->CS.CurrentPhase = MMO_CLIENT_PHASE_DISCONNECT;

		result = PostQueuedCompletionStatus(DISCONNECT_CP, NULL, BCAV[sessionIndex]->CS.Key, &(BCAV[sessionIndex]->CS.OverlappedSend));
	}
	//else result = 0;

	return result;
}

SerialPacketQueue<IOCP_PacketHeader>* MMO_BaseServer::SPQ_TLP_Allocate()
{
	SerialPacketQueue<IOCP_PacketHeader>* serialPacketQueueAddress;

	serialPacketQueueAddress = SPQ_TLP.Allocate();

	InterlockedIncrement(&CURRENT_SPQ_COUNT);
	InterlockedIncrement(&ALLOCATED_SPQ_COUNT);

	CURRENT_SPQ_CHUNK_COUNT = SPQ_TLP.GetCurrentChunkCount();
	MAXIMUM_SPQ_CHUNK_COUNT = SPQ_TLP.GetMaximumChunkCount();

	return serialPacketQueueAddress;
}

unsigned int MMO_BaseServer::SPQ_TLP_Deallocate(SerialPacketQueue<IOCP_PacketHeader>* serialPacketQueueAddress)
{
	UINT result;

	result = serialPacketQueueAddress->DecreaseReferenceCount();

	if (!result)
	{
		SPQ_TLP.Deallocate(serialPacketQueueAddress);

		InterlockedDecrement(&CURRENT_SPQ_COUNT);
		InterlockedIncrement(&DEALLOCATED_SPQ_COUNT);

		CURRENT_SPQ_CHUNK_COUNT = SPQ_TLP.GetCurrentChunkCount();
		MAXIMUM_SPQ_CHUNK_COUNT = SPQ_TLP.GetMaximumChunkCount();
	}

	return result;
}
