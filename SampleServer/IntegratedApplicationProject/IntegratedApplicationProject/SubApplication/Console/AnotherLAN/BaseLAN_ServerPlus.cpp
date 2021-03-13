#include "stdafx.h"
#include "AnotherLAN.h"
#include "BaseLAN_Server.h"

unsigned int WINAPI BaseLAN_Server::AcceptAnotherLAN_Client(LPVOID instanceAddress)
{
	AnotherLAN_ClientSession anotherLAN_ClientSession;
	INT addressLength;
	UINT64 sessionKey;

	addressLength = sizeof(anotherLAN_ClientSession);

	if(!THIS) THIS = (BaseLAN_Server*)instanceAddress;

	while (!ALAN_S_SHUTDOWN)
	{
		anotherLAN_ClientSession.Socket = WSAAccept(ALAN_SS.Socket, (SOCKADDR*)&anotherLAN_ClientSession.Address, &addressLength, nullptr, NULL);
		CHECK_INVALID_SOCKET(anotherLAN_ClientSession.Socket)
		//IF_INVALID_SOCKET(advancedLanClientSession.Socket)
		//ELSE
		{
			THIS->OnConnectionRequest(anotherLAN_ClientSession.Address, anotherLAN_ClientSession.Address.sin_port);

			FOR(i, TOTAL_ANOTHER_LAN_CLIENT_COUNT)
			{
				if (ALAN_CSV[i].CurrentPhaseIndex == ANOTHER_LAN_CLIENT_PHASE_ACCEPT)
				{
					sessionKey = i;
					sessionKey = sessionKey << ANOTHER_LAN_CLIENT_INDEX_SHIFTED_BIT_COUNT;
					sessionKey += ++CURRENT_SESSION_KEY;

					IF_NULL(CreateIoCompletionPort((HANDLE)anotherLAN_ClientSession.Socket, ALAN_S_COMMON_CP, sessionKey, ANOTHER_LAN_SUB_THREAD_WORK4 - ANOTHER_LAN_SUB_THREAD_WORK1 + 1))
					ELSE
					{
						ALAN_CSV[i].Socket = anotherLAN_ClientSession.Socket;
						ALAN_CSV[i].Address = anotherLAN_ClientSession.Address;
						ALAN_CSV[i].Key = sessionKey;
						ALAN_CSV[i].Connected = 1;
						ALAN_CSV[i].Sendable = 1;
						ALAN_CSV[i].ReceiveCount = 0;
						ALAN_CSV[i].SendCount = 0;

						ZeroMemory(&ALAN_CSV[i].OverlappedReceive, sizeof(ALAN_CSV[i].OverlappedReceive));
						ZeroMemory(&ALAN_CSV[i].OverlappedSend, sizeof(ALAN_CSV[i].OverlappedSend));

						//ALAN_CSV[i].ReceiveQ.Initialize(); //not finished function
						//ALAN_CSV[i].SendQ.Initialize(); //not finished function
						//ALAN_CSV[i].ReceiveBQ.Initialize();
						//ALAN_CSV[i].SendBQ.Initialize();

						if (ALAN_CSV[i].SendQ.GetCurrentCount())
						{
							ACCEPT_LEAK_COUNT += ALAN_CSV[i].SendQ.GetCurrentCount();
						}

						ALAN_CSV[i].IO_Count = 2; //not danger?
						++ALAN_CSV[i].ReceiveCount;

						InitializeSRWLock(&ALAN_CSV[i].LAN_ClientLock);

						ALAN_CSV[i].CurrentPhaseIndex = ANOTHER_LAN_CLIENT_PHASE_UPDATE;

						THIS->OnClientJoin(ALAN_CSV[i].Address, ALAN_CSV[i].Key);

						PostQueuedCompletionStatus(ALAN_S_COMMON_CP, NULL, ALAN_CSV[i].Key, &ALAN_CSV[i].OverlappedReceive);

						break;
					}
				}

				if (i == TOTAL_ANOTHER_LAN_CLIENT_COUNT - 1)
				{
					i = 0;

					++SLOW_DISCONNECTION_COUNT;
					Sleep(0);
				}
				//if (i == TOTAL_ANOTHER_LAN_CLIENT_COUNT - 1) closesocket(anotherLAN_ClientSession.Socket);
			}
		}
	}

	return 0;
}

unsigned int WINAPI BaseLAN_Server::DisconnectAnotherLAN_Client(LPVOID instanceAddress)
{
	DWORD transferredSize;
	ULONG_PTR completionKey;
	OVERLAPPED* overlapped;
	ULONG64 sessionKey;
	UINT clientIndex;

	while (!ALAN_S_SHUTDOWN)
	{
		transferredSize = 0;
		completionKey = 0;
		overlapped = nullptr;
		clientIndex = 0xffffffff;

		if(GetQueuedCompletionStatus(ALAN_S_DISCONNECT_CP, &transferredSize, &completionKey, &overlapped, INFINITE))
		{
			sessionKey = completionKey;
			clientIndex = sessionKey >> ANOTHER_LAN_CLIENT_INDEX_SHIFTED_BIT_COUNT;
			
			//if (ALAN_CSV[clientIndex].CurrentPhaseIndex == ANOTHER_LAN_CLIENT_PHASE_DISCONNECT)
			{
				THIS->OnClientLeave(ALAN_CSV[clientIndex].Key);

				closesocket(ALAN_CSV[clientIndex].Socket);
				//shutdown(ALAN_CSV[clientIndex].Socket, SD_BOTH); //closesocket 사용시, 센드 중 디스커넥트 되고 다시 어셉트를 받으면서 같은 소켓을 할당받는 경우 새 접속자에게 이전 접속자의 패킷을 보내는 경우가 생길 수 있을까?

				//closesocket 사용시 세션이 재사용 될 때 재사용되는 소켓이 뒤늦게 닫힐 수 있다
				//TIME_WAIT은 로컬 아이피와 포트, 리모트 아이피와 포트간의 상태. 같은 아이피와 포트로 재접속을 시도하면 TIME_WAIT 상태라 실패함
				//상대방이 FIN 에 대한 ACK를 주지 않으면 계속 리시브를 대기한다

				//삭제할 세션과 생성할 세션을 따로 모아서 일괄처리하는 방식도 가능

				ALAN_CSV[clientIndex].Socket = INVALID_SOCKET;
				ALAN_CSV[clientIndex].Address;
				ALAN_CSV[clientIndex].Connected = 0;
				ALAN_CSV[clientIndex].Sendable = 0;
				ALAN_CSV[clientIndex].ReceiveCount = 0;
				ALAN_CSV[clientIndex].SendCount = 0;

				ALAN_CSV[clientIndex].Key = 0xffffffffffffffff;

				ALAN_CSV[clientIndex].OverlappedReceive;
				ALAN_CSV[clientIndex].OverlappedSend;

				if (ALAN_CSV[clientIndex].SendQ.GetCurrentCount())
				{
					DISCONNECT_LEAK_COUNT += ALAN_CSV[clientIndex].SendQ.GetCurrentCount();
				}

				while (1)
				{
					SerialPacketQueue<AnotherLAN_PacketHeader>* sendedPacketSPQ_Address;
				
					sendedPacketSPQ_Address = ALAN_CSV[clientIndex].SendQ.Dequeue();
				
					if (!sendedPacketSPQ_Address) break;
				
					//correct to check referenceCount of SBQ
					InterlockedIncrement(&SBQ_DEALLOCATION_COUNT);
					delete sendedPacketSPQ_Address;
				}

				//check size and deal last packet
				ALAN_CSV[clientIndex].ReceiveQ.Clear();
				ALAN_CSV[clientIndex].SendQ.Clear(); //memory leak?
				ALAN_CSV[clientIndex].ReceiveBQ.Clear();
				ALAN_CSV[clientIndex].SendBQ.Clear();
				//check size and deal last packet

				ALAN_CSV[clientIndex].IO_Count = 0;

				ALAN_CSV[clientIndex].CurrentPhaseIndex = ANOTHER_LAN_CLIENT_PHASE_ACCEPT;
				++ALAN_CSV[clientIndex].AccumulatedUseCount;
			}
		}
		else break;
	}

	return 0;
}

unsigned int WINAPI BaseLAN_Server::WorkAnotherLAN_Server(LPVOID instanceAddress)
{
	DWORD transferredSize;
	ULONG_PTR completionKey;
	LPOVERLAPPED overlapped;
	UINT64 sessionKey;
	UINT clientIndex;
	DWORD flag;
	int result;

	if(!THIS) THIS = (BaseLAN_Server*)instanceAddress;

	THIS->OnWorkerThreadBegin();

	while (!ALAN_S_SHUTDOWN)
	{
		transferredSize = 0;
		completionKey = 0;
		overlapped = nullptr;
		flag = 0;

		//@@@hotplace@@@
		//GetQueuedCompletionStatus(ALAN_S_COMMON_CP, &transferredSize, &completionKey, &overlapped, INFINITE);
		if (GetQueuedCompletionStatus(ALAN_S_COMMON_CP, &transferredSize, &completionKey, &overlapped, INFINITE))
		{
			if (overlapped)
			{
				sessionKey = completionKey;
				clientIndex = sessionKey >> ANOTHER_LAN_CLIENT_INDEX_SHIFTED_BIT_COUNT;
				if (!(clientIndex < TOTAL_ANOTHER_LAN_CLIENT_COUNT) || ALAN_CSV[clientIndex].Key != sessionKey)
				{
					continue;
				}

				if (&ALAN_CSV[clientIndex].OverlappedReceive == overlapped)
				{
					//rcvPost
					{
						ZeroMemory(&ALAN_CSV[clientIndex].OverlappedReceive, sizeof(ALAN_CSV[clientIndex].OverlappedReceive));

						if (0 < transferredSize)
						{
							ALAN_CSV[clientIndex].ReceiveBQ.IncreaseCurrentSize(transferredSize);
							//ALCSV[clientIndex].CurrentPhaseIndex = ADVANCED_LAN_CLIENT_PHASE_UPDATE;

							THIS->OnRecv(ALAN_CSV[clientIndex].Key, transferredSize);
							//disconnected session이더라도 끝까지 처리한다 
						}

						InterlockedIncrement(&ALAN_CSV[clientIndex].IO_Count);
						++ALAN_CSV[clientIndex].ReceiveCount;

						result = ALAN_CSV[clientIndex].ReceiveBQ.WSAReceive(WSARecv, ALAN_CSV[clientIndex].Socket, &flag, &ALAN_CSV[clientIndex].OverlappedReceive);
						if (result == SOCKET_ERROR)
						{
							if (WSAGetLastError() == WSA_IO_PENDING);
							else
							{
								InterlockedDecrement(&ALAN_CSV[clientIndex].IO_Count);
								--ALAN_CSV[clientIndex].ReceiveCount;

								//printf("[WSARecv] clientIndex : %u, sessionKey : %llu, IO_Count : %d, WSAGetLastError() : %d" CRALF, clientIndex, ALAN_CSV[clientIndex].SessionKey, ALAN_CSV[clientIndex].IO_Count, WSAGetLastError());
							}
						}

						//if(0 < --LCSV[clientIndex].ReceiveCount && 0 < LCSV[clientIndex].SendCount)
						if (!InterlockedDecrement(&ALAN_CSV[clientIndex].IO_Count))
						{
							if (ALAN_CSV[clientIndex].SendQ.GetCurrentCount()) InterlockedIncrement(&RCV_LEAK);
							
							//if(ALAN_CSV[clientIndex].ReceiveCount == 1 && ALAN_CSV[clientIndex].SendCount == 1)
							//{
							//	//InterlockedIncrement(&RCV_GHOST_SESSION_COUNT);
							//
							//	//while (1)
							//	//{
							//	//	SerialPacketQueue<AnotherLAN_PacketHeader>* sendedPacketSPQ_Address;
							//	//
							//	//	sendedPacketSPQ_Address = ALAN_CSV[clientIndex].SendQ.Dequeue();
							//	//
							//	//	if (!sendedPacketSPQ_Address) break;
							//	//
							//	//	//correct to check referenceCount of SBQ
							//	//	InterlockedIncrement(&SBQ_DEALLOCATION_COUNT);
							//	//	delete sendedPacketSPQ_Address;
							//	//}
							//	
							//	ALAN_CSV[clientIndex].CurrentPhaseIndex = ANOTHER_LAN_CLIENT_PHASE_DISCONNECT;
							//	
							//	InterlockedIncrement(&RCV_DISCONNECTION_COUNT);
							//	PostQueuedCompletionStatus(ALAN_S_DISCONNECT_CP, NULL, ALAN_CSV[clientIndex].Key, &ALAN_CSV[clientIndex].OverlappedReceive);
							//	//THIS->Disconnect(ALCSV[clientIndex].SessionKey);
							//}

							if (InterlockedExchange(&ALAN_CSV[clientIndex].Connected, 0))
							{
								if (ALAN_CSV[clientIndex].Key == sessionKey && ALAN_CSV[clientIndex].CurrentPhaseIndex != ANOTHER_LAN_CLIENT_PHASE_ACCEPT)
								{
									ALAN_CSV[clientIndex].CurrentPhaseIndex = ANOTHER_LAN_CLIENT_PHASE_DISCONNECT;
							
									InterlockedIncrement(&RCV_DISCONNECTION_COUNT);
									PostQueuedCompletionStatus(ALAN_S_DISCONNECT_CP, NULL, ALAN_CSV[clientIndex].Key, &ALAN_CSV[clientIndex].OverlappedReceive);
									//THIS->Disconnect(ALCSV[clientIndex].SessionKey);
								}
								else InterlockedExchange(&ALAN_CSV[clientIndex].Connected, 1);
							}
						}
						//else LCSV[clientIndex].CurrentPhaseIndex = LAN_CLIENT_PHASE_UPDATE;
					}
				}
				else
				{
					//sndPost
					{
						if (InterlockedExchange(&ALAN_CSV[clientIndex].Sendable, 0))
						{
							ZeroMemory(&ALAN_CSV[clientIndex].OverlappedSend, sizeof(ALAN_CSV[clientIndex].OverlappedSend));

							InterlockedIncrement(&ALAN_CSV[clientIndex].IO_Count);
							++ALAN_CSV[clientIndex].SendCount;

							if (0 < transferredSize)
							{
								INT sendedSize;

								sendedSize = transferredSize;

								while(0 < sendedSize)
								{
									SerialPacketQueue<AnotherLAN_PacketHeader>* sendedPacketSPQ_Address;

									sendedPacketSPQ_Address = ALAN_CSV[clientIndex].SendQ.Dequeue(); //non thread safe

									if (!sendedPacketSPQ_Address) break; //error

									sendedSize -= sendedPacketSPQ_Address->GetTotalSize();

									//correct to check referenceCount of SBQ
									InterlockedIncrement(&SBQ_DEALLOCATION_COUNT);
									delete sendedPacketSPQ_Address;
								}
								
								//ALAN_CSV[clientIndex].SendBQ.DecreaseCurrentSize(transferredSize);
								ALAN_CSV[clientIndex].CurrentPhaseIndex = ANOTHER_LAN_CLIENT_PHASE_UPDATE; //ADVANCED_LAN_CLIENT_PHASE_RECEIVE;

								THIS->OnSend(ALAN_CSV[clientIndex].Key, transferredSize);
							}
							else
							{
								WSABUF sendingWSABUF[ANOTHER_LAN_SERVER_WSABUF_COUNT];
								UINT currentSendingWSABUF_Count;

								ALAN_CSV[clientIndex].Sendable = 1;
								
								//InterlockedIncrement(&ALAN_CSV[clientIndex].IO_Count);
								//++ALAN_CSV[clientIndex].SendCount;



								if (ANOTHER_LAN_SERVER_WSABUF_COUNT < ALAN_CSV[clientIndex].SendQ.GetCurrentCount()) currentSendingWSABUF_Count = ANOTHER_LAN_SERVER_WSABUF_COUNT;
								else currentSendingWSABUF_Count = ALAN_CSV[clientIndex].SendQ.GetCurrentCount();

								FOR(i, currentSendingWSABUF_Count)
								{
									SerialPacketQueue<AnotherLAN_PacketHeader>* sendingPacketSPQ_Address;
									WSABUF wsabuf;

									sendingPacketSPQ_Address = ALAN_CSV[clientIndex].SendQ.Peek(i);
									
									if (!sendingPacketSPQ_Address) break; //error
									
									wsabuf.buf = sendingPacketSPQ_Address->GetQueueAddress();
									wsabuf.len = sendingPacketSPQ_Address->GetTotalSize();

									sendingWSABUF[i] = wsabuf;
								}
								
								result = WSASend(ALAN_CSV[clientIndex].Socket, sendingWSABUF, currentSendingWSABUF_Count, nullptr, flag, &ALAN_CSV[clientIndex].OverlappedSend, nullptr);
								//result = ALCSV[clientIndex].SendBQ.WSASend(WSASend, ALCSV[clientIndex].Socket, flag, &ALCSV[clientIndex].OverlappedSend);
								if (result == SOCKET_ERROR)
								{
									if (WSAGetLastError() == WSA_IO_PENDING);
									else
									{
										InterlockedDecrement(&ALAN_CSV[clientIndex].IO_Count);
										--ALAN_CSV[clientIndex].SendCount;

										printf("[WSASend] clientIndex : %u, sessionKey : %llu, IO_Count : %d, WSAGetLastError() : %d" CRALF, clientIndex, ALAN_CSV[clientIndex].Key, ALAN_CSV[clientIndex].IO_Count, WSAGetLastError());
									}
								}
							}

							//if(0 < --LCSV[clientIndex].SendCount && 0 < LCSV[clientIndex].ReceiveCount)
							if (!InterlockedDecrement(&ALAN_CSV[clientIndex].IO_Count))
							{
								if (ALAN_CSV[clientIndex].SendQ.GetCurrentCount()) InterlockedIncrement(&SND_LEAK);
								
								//if (ALAN_CSV[clientIndex].ReceiveCount == 1 || ALAN_CSV[clientIndex].SendCount == 1)
								//{
								//	//InterlockedIncrement(&SND_GHOST_SESSION_COUNT);
								//}

								if (InterlockedExchange(&ALAN_CSV[clientIndex].Connected, 0))
								{
									if (ALAN_CSV[clientIndex].Key == sessionKey && ALAN_CSV[clientIndex].CurrentPhaseIndex != ANOTHER_LAN_CLIENT_PHASE_ACCEPT)
									{
										ALAN_CSV[clientIndex].CurrentPhaseIndex = ANOTHER_LAN_CLIENT_PHASE_DISCONNECT;

										InterlockedIncrement(&SND_DISCONNECTION_COUNT);
										PostQueuedCompletionStatus(ALAN_S_DISCONNECT_CP, NULL, ALAN_CSV[clientIndex].Key, &ALAN_CSV[clientIndex].OverlappedSend);
										//THIS->Disconnect(ALCSV[clientIndex].SessionKey);
									}
								}
								else InterlockedExchange(&ALAN_CSV[clientIndex].Connected, 1);
							}
							//else LCSV[clientIndex].CurrentPhaseIndex = LAN_CLIENT_PHASE_RECEIVE;
						}
					}
				}
			}
			else break;
		}
		else
		{
			if (overlapped)
			{
				sessionKey = completionKey;
				clientIndex = sessionKey >> ANOTHER_LAN_CLIENT_INDEX_SHIFTED_BIT_COUNT;

				if (&ALAN_CSV[clientIndex].OverlappedReceive == overlapped)
				{
					if (!InterlockedDecrement(&ALAN_CSV[clientIndex].IO_Count))
					{
						if (InterlockedExchange(&ALAN_CSV[clientIndex].Connected, 0))
						{
							if (ALAN_CSV[clientIndex].Key == sessionKey && ALAN_CSV[clientIndex].CurrentPhaseIndex != ANOTHER_LAN_CLIENT_PHASE_ACCEPT)
							{
								ALAN_CSV[clientIndex].CurrentPhaseIndex = ANOTHER_LAN_CLIENT_PHASE_DISCONNECT;
				
								InterlockedIncrement(&GQCS_DISCONNECTION_COUNT);
								PostQueuedCompletionStatus(ALAN_S_DISCONNECT_CP, NULL, ALAN_CSV[clientIndex].Key, overlapped);
								//THIS->Disconnect(ALCSV[clientIndex].SessionKey);
							}
							else InterlockedExchange(&ALAN_CSV[clientIndex].Connected, 1);
						}
					}
				}
				else
				{
					{
						if (InterlockedExchange(&ALAN_CSV[clientIndex].Connected, 0))
						{
							if (ALAN_CSV[clientIndex].Key == sessionKey && ALAN_CSV[clientIndex].CurrentPhaseIndex != ANOTHER_LAN_CLIENT_PHASE_ACCEPT)
							{
								ALAN_CSV[clientIndex].CurrentPhaseIndex = ANOTHER_LAN_CLIENT_PHASE_DISCONNECT;
				
								InterlockedIncrement(&GQCS_DISCONNECTION_COUNT);
								PostQueuedCompletionStatus(ALAN_S_DISCONNECT_CP, NULL, ALAN_CSV[clientIndex].Key, overlapped);
								//THIS->Disconnect(ALCSV[clientIndex].SessionKey);
							}
							else InterlockedExchange(&ALAN_CSV[clientIndex].Connected, 1);
						}
					}
				}
			}
			else
			{
				if (GetLastError() == ERROR_ABANDONED_WAIT_0) break;
			}
		}
	}

	THIS->OnWorkerThreadEnd();

	return 0;
}