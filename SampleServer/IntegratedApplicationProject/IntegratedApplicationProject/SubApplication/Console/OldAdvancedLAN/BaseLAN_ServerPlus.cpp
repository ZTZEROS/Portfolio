#include "stdafx.h"
#include "AdvancedLAN.h"
#include "BaseLAN_Server.h"

unsigned int WINAPI BaseLAN_Server::AcceptAdvancedLAN_Client(LPVOID instanceAddress)
{
	AdvancedLAN_ClientSession advancedLAN_ClientSession;
	INT addressLength;
	UINT acceptedALAN_CS_Index;

	addressLength = sizeof(advancedLAN_ClientSession);
	acceptedALAN_CS_Index = 0xffffffff;

	if (!THIS) THIS = (BaseLAN_Server*)instanceAddress;

	while (!ALAN_S_SHUTDOWN)
	{
		advancedLAN_ClientSession.Socket = WSAAccept(ALAN_SS.Socket, (SOCKADDR*)&advancedLAN_ClientSession.Address, &addressLength, nullptr, NULL);
		CHECK_INVALID_SOCKET(advancedLAN_ClientSession.Socket)
		//IF_INVALID_SOCKET(advancedLAN_ClientSession.Socket)
		//ELSE
		{
			THIS->OnConnectionRequest(advancedLAN_ClientSession.Address, advancedLAN_ClientSession.Address.sin_port);

			acceptedALAN_CS_Index = ACCEPTABLE_ALAN_CS_INDEX_LFS.Pop(); //디스커넥트 처리가 늦으면 실패 데이터를 계속 리턴해 오류 위험

			//if (ALCSV[acceptedALCS_Index].CurrentPhaseIndex == ADVANCED_LAN_CLIENT_PHASE_ACCEPT)
			{
				IF_NULL(CreateIoCompletionPort((HANDLE)advancedLAN_ClientSession.Socket, ALAN_S_COMMON_CP, acceptedALAN_CS_Index, ADVANCED_LAN_SUB_THREAD_WORK4 - ADVANCED_LAN_SUB_THREAD_WORK1 + 1))
				ELSE
				{
					ALAN_CSV[acceptedALAN_CS_Index].Socket = advancedLAN_ClientSession.Socket;
					ALAN_CSV[acceptedALAN_CS_Index].Address = advancedLAN_ClientSession.Address;
					ALAN_CSV[acceptedALAN_CS_Index].Connected = 1;
					ALAN_CSV[acceptedALAN_CS_Index].Sendable = 1;
					ALAN_CSV[acceptedALAN_CS_Index].ReceiveCount = 0;
					ALAN_CSV[acceptedALAN_CS_Index].SendCount = 0;
					ALAN_CSV[acceptedALAN_CS_Index].SessionKey = acceptedALAN_CS_Index;
					ALAN_CSV[acceptedALAN_CS_Index].SessionKey = ALAN_CSV[acceptedALAN_CS_Index].SessionKey << ADVANCED_LAN_CLIENT_INDEX_SHIFTED_BIT_COUNT;
					ALAN_CSV[acceptedALAN_CS_Index].SessionKey += ++CURRENT_SESSION_KEY;

					ZeroMemory(&ALAN_CSV[acceptedALAN_CS_Index].OverlappedReceive, sizeof(ALAN_CSV[acceptedALAN_CS_Index].OverlappedReceive));
					ZeroMemory(&ALAN_CSV[acceptedALAN_CS_Index].OverlappedSend, sizeof(ALAN_CSV[acceptedALAN_CS_Index].OverlappedSend));

					//ALCSV[acceptedSessionIndex].ReceiveQ.Initialize(); //not finished function
					//ALCSV[acceptedSessionIndex].SendQ.Initialize(); //not finished function
					//ALCSV[acceptedSessionIndex].ReceiveBQ.Initialize();
					//ALCSV[acceptedSessionIndex].SendBQ.Initialize();

					ALAN_CSV[acceptedALAN_CS_Index].IO_Count = 1;
					++ALAN_CSV[acceptedALAN_CS_Index].ReceiveCount;

					InitializeSRWLock(&ALAN_CSV[acceptedALAN_CS_Index].LAN_ClientLock);

					ALAN_CSV[acceptedALAN_CS_Index].CurrentPhaseIndex = ADVANCED_LAN_CLIENT_PHASE_UPDATE;
					++ALAN_CSV[acceptedALAN_CS_Index].AccumulatedUseCount;

					THIS->OnClientJoin(ALAN_CSV[acceptedALAN_CS_Index].Address, ALAN_CSV[acceptedALAN_CS_Index].SessionKey);

					PostQueuedCompletionStatus(ALAN_S_COMMON_CP, NULL, acceptedALAN_CS_Index, &ALAN_CSV[acceptedALAN_CS_Index].OverlappedReceive);
				}
			}
		}
	}

	return 0;
}

unsigned int WINAPI BaseLAN_Server::DisconnectAdvancedLAN_Client(LPVOID instanceAddress)
{
	DWORD transferredSize;
	ULONG_PTR completionKey;
	OVERLAPPED* overlapped;
	ULONG64 sessionKey;
	UINT disconnectingALAN_CS_Index;

	while (!ALAN_S_SHUTDOWN)
	{
		transferredSize = 0;
		completionKey = 0;
		overlapped = nullptr;
		disconnectingALAN_CS_Index = 0xffffffff;
		//check duplicated disconnecting
		if (GetQueuedCompletionStatus(ALAN_S_DISCONNECT_CP, &transferredSize, &completionKey, &overlapped, INFINITE))
		{
			sessionKey = completionKey;

			disconnectingALAN_CS_Index = sessionKey >> ADVANCED_LAN_CLIENT_INDEX_SHIFTED_BIT_COUNT;

			//if (ALAN_CSV[clientIndex].CurrentPhaseIndex == ADVANCED_LAN_CLIENT_PHASE_DISCONNECT)
			if(ALAN_CSV[disconnectingALAN_CS_Index].SessionKey == sessionKey)
			{
				THIS->OnClientLeave(ALAN_CSV[disconnectingALAN_CS_Index].SessionKey);

				closesocket(ALAN_CSV[disconnectingALAN_CS_Index].Socket);
				//shutdown(ALAN_CSV[disconnectingALAN_CS_Index].Socket, SD_BOTH); //셧다운 사용시, 센드 중 디스커넥트 되고 다시 어셉트를 받으면서 같은 소켓을 할당받는 경우 새 접속자에게 이전 접속자의 패킷을 보내는 경우가 생길 수 있을까?

				//closesocket 사용시 세션이 재사용 될 때 재사용되는 소켓이 뒤늦게 닫힐 수 있다
				//TIME_WAIT은 로컬 아이피와 포트, 리모트 아이피와 포트간의 상태. 같은 아이피와 포트로 재접속을 시도하면 TIME_WAIT 상태라 실패함
				//상대방이 FIN 에 대한 ACK를 주지 않으면 계속 리시브를 대기한다

				//삭제할 세션과 생성할 세션을 따로 모아서 일괄처리하는 방식도 가능

				ALAN_CSV[disconnectingALAN_CS_Index].Socket = INVALID_SOCKET;
				ALAN_CSV[disconnectingALAN_CS_Index].Address;
				ALAN_CSV[disconnectingALAN_CS_Index].Connected = 0;
				ALAN_CSV[disconnectingALAN_CS_Index].Sendable = 0;
				ALAN_CSV[disconnectingALAN_CS_Index].ReceiveCount = 0;
				ALAN_CSV[disconnectingALAN_CS_Index].SendCount = 0;

				ALAN_CSV[disconnectingALAN_CS_Index].SessionKey = 0xffffffffffffffff;

				ALAN_CSV[disconnectingALAN_CS_Index].OverlappedReceive;
				ALAN_CSV[disconnectingALAN_CS_Index].OverlappedSend;

				while (1)
				{
					SerialPacketQueue<AdvancedLAN_PacketHeader>* sendedPacketSPQ_Address;
				
					sendedPacketSPQ_Address = ALAN_CSV[disconnectingALAN_CS_Index].SendQ.Dequeue();
					if (sendedPacketSPQ_Address)
					{
						//sendedSize -= sendedPacketSPQ_Address->GetTotalSize();
				
						//correct to check referenceCount of SBQ
						++SPQ_DEALLOCATION_COUNT;
						delete sendedPacketSPQ_Address;
					}
					else break;
				}

				//check size and deal last packet
				ALAN_CSV[disconnectingALAN_CS_Index].ReceiveQ.Clear();
				ALAN_CSV[disconnectingALAN_CS_Index].SendQ.Clear(); //memory leak?
				ALAN_CSV[disconnectingALAN_CS_Index].ReceiveBQ.Clear();
				ALAN_CSV[disconnectingALAN_CS_Index].SendBQ.Clear();
				//check size and deal last packet

				ALAN_CSV[disconnectingALAN_CS_Index].IO_Count = 0;

				ALAN_CSV[disconnectingALAN_CS_Index].CurrentPhaseIndex = ADVANCED_LAN_CLIENT_PHASE_ACCEPT;

				ACCEPTABLE_ALAN_CS_INDEX_LFS.Push(disconnectingALAN_CS_Index);
			}
		}
		else break;
	}

	return 0;
}

unsigned int WINAPI BaseLAN_Server::WorkAdvancedLAN_Server(LPVOID instanceAddress)
{
	DWORD transferredSize;
	ULONG_PTR completionKey;
	LPOVERLAPPED overlapped;
	UINT currentALAN_CS_Index;
	DWORD flag;
	int result;

	if (!THIS) THIS = (BaseLAN_Server*)instanceAddress;

	THIS->OnWorkerThreadBegin();

	while (!ALAN_S_SHUTDOWN)
	{
		transferredSize = 0;
		completionKey = 0;
		overlapped = nullptr;
		flag = 0;

		//GetQueuedCompletionStatus(ALAN_S_COMMON_CP, &transferredSize, &completionKey, &overlapped, INFINITE);
		if (GetQueuedCompletionStatus(ALAN_S_COMMON_CP, &transferredSize, &completionKey, &overlapped, INFINITE))
		{
			if (overlapped)
			{
				currentALAN_CS_Index = completionKey;

				if (&ALAN_CSV[currentALAN_CS_Index].OverlappedReceive == overlapped)
				{
					//rcvPost
					{
						ZeroMemory(&ALAN_CSV[currentALAN_CS_Index].OverlappedReceive, sizeof(ALAN_CSV[currentALAN_CS_Index].OverlappedReceive));

						if (0 < transferredSize)
						{
							INT receivedSize;

							receivedSize = transferredSize;

							ALAN_CSV[currentALAN_CS_Index].ReceiveBQ.IncreaseCurrentSize(receivedSize);
							//ALCSV[currentALCS_Index].CurrentPhaseIndex = ADVANCED_LAN_CLIENT_PHASE_UPDATE;

							THIS->OnRecv(ALAN_CSV[currentALAN_CS_Index].SessionKey, receivedSize);
						}

						InterlockedIncrement(&ALAN_CSV[currentALAN_CS_Index].IO_Count);
						++ALAN_CSV[currentALAN_CS_Index].ReceiveCount;

						result = ALAN_CSV[currentALAN_CS_Index].ReceiveBQ.WSAReceive(WSARecv, ALAN_CSV[currentALAN_CS_Index].Socket, &flag, &ALAN_CSV[currentALAN_CS_Index].OverlappedReceive);
						if (result == SOCKET_ERROR)
						{
							if (WSAGetLastError() == WSA_IO_PENDING);
							else
							{
								InterlockedDecrement(&ALAN_CSV[currentALAN_CS_Index].IO_Count);
								--ALAN_CSV[currentALAN_CS_Index].ReceiveCount;

								printf("[WSARecv] sessionKey : %llu, IO_Count : %d, WSAGetLastError() : %d" CRALF, ALAN_CSV[currentALAN_CS_Index].SessionKey, ALAN_CSV[currentALAN_CS_Index].IO_Count, WSAGetLastError());
							}
						}

						//if(0 < --LCSV[clientIndex].ReceiveCount && 0 < LCSV[clientIndex].SendCount)
						if (!InterlockedDecrement(&ALAN_CSV[currentALAN_CS_Index].IO_Count))
						{
							if (InterlockedExchange(&ALAN_CSV[currentALAN_CS_Index].Connected, 0))
							{
								ALAN_CSV[currentALAN_CS_Index].CurrentPhaseIndex = ADVANCED_LAN_CLIENT_PHASE_DISCONNECT;

								PostQueuedCompletionStatus(ALAN_S_DISCONNECT_CP, NULL, ALAN_CSV[currentALAN_CS_Index].SessionKey, &ALAN_CSV[currentALAN_CS_Index].OverlappedReceive);
								//THIS->Disconnect(ALAN_CSV[currentALAN_CS_Index].SessionKey);
							}
						}
						//else LCSV[clientIndex].CurrentPhaseIndex = LAN_CLIENT_PHASE_UPDATE;
					}
				}
				else
				{
					//sndPost
					{
						//if (InterlockedExchange(&ALAN_CSV[currentALAN_CS_Index].Sendable, 0))
						{
							ZeroMemory(&ALAN_CSV[currentALAN_CS_Index].OverlappedSend, sizeof(ALAN_CSV[currentALAN_CS_Index].OverlappedSend));

							if (0 < transferredSize)
							{
								INT sendedSize;

								sendedSize = transferredSize;

								while (0 < sendedSize)
								{
									SerialPacketQueue<AdvancedLAN_PacketHeader>* sendedPacketSPQ_Address;

									sendedPacketSPQ_Address = ALAN_CSV[currentALAN_CS_Index].SendQ.Dequeue();
									
									sendedSize -= sendedPacketSPQ_Address->GetTotalSize();

									//correct to check referenceCount of SBQ
									++SPQ_DEALLOCATION_COUNT;
									delete sendedPacketSPQ_Address;
								}

								THIS->OnSend(ALAN_CSV[currentALAN_CS_Index].SessionKey, transferredSize);

								InterlockedExchange(&ALAN_CSV[currentALAN_CS_Index].Sendable, 1);

								if (ALAN_CSV[currentALAN_CS_Index].SendQ.GetCurrentCount())
								{
									if (InterlockedExchange(&ALAN_CSV[currentALAN_CS_Index].Sendable, 0))
									{
										//ALAN_CSV[currentALAN_CS_Index].CurrentPhaseIndex = ADVANCED_LAN_CLIENT_PHASE_SEND;
										//ALAN_CSV[currentALAN_CS_Index].Sendable = 1;

										PostQueuedCompletionStatus(ALAN_S_COMMON_CP, NULL, currentALAN_CS_Index, &ALAN_CSV[currentALAN_CS_Index].OverlappedSend);
									}
									//consider that using while and continue;
								}
								else
								{
									ALAN_CSV[currentALAN_CS_Index].CurrentPhaseIndex = ADVANCED_LAN_CLIENT_PHASE_UPDATE; //ADVANCED_LAN_CLIENT_PHASE_RECEIVE;
								}
							}
							else
							{
								WSABUF sendingWSABUF[ADVANCED_LAN_SERVER_WSABUF_COUNT];
								UINT currentSendingWSABUF_Count;

								//ALAN_CSV[currentALAN_CS_Index].Sendable = 1;

								InterlockedIncrement(&ALAN_CSV[currentALAN_CS_Index].IO_Count);
								++ALAN_CSV[currentALAN_CS_Index].SendCount;



								if (ADVANCED_LAN_SERVER_WSABUF_COUNT < ALAN_CSV[currentALAN_CS_Index].SendQ.GetCurrentCount()) currentSendingWSABUF_Count = ADVANCED_LAN_SERVER_WSABUF_COUNT;
								else currentSendingWSABUF_Count = ALAN_CSV[currentALAN_CS_Index].SendQ.GetCurrentCount();

								FOR(i, currentSendingWSABUF_Count)
								{
									SerialPacketQueue<AdvancedLAN_PacketHeader>* sendingPacketSPQ_Address;
									WSABUF wsabuf;

									sendingPacketSPQ_Address = ALAN_CSV[currentALAN_CS_Index].SendQ.Peek(i);
									wsabuf.buf = sendingPacketSPQ_Address->GetQueueAddress();
									wsabuf.len = sendingPacketSPQ_Address->GetTotalSize();

									sendingWSABUF[i] = wsabuf;
								}

								//WSAEINVAL //10022
								if (!currentSendingWSABUF_Count) InterlockedExchange(&ALAN_CSV[currentALAN_CS_Index].Sendable, 1);
								else
								{
									result = WSASend(ALAN_CSV[currentALAN_CS_Index].Socket, sendingWSABUF, currentSendingWSABUF_Count, nullptr, flag, &ALAN_CSV[currentALAN_CS_Index].OverlappedSend, nullptr);
									//result = ALCSV[clientIndex].SendBQ.WSASend(WSASend, ALCSV[clientIndex].Socket, flag, &ALCSV[clientIndex].OverlappedSend);
									if (result == SOCKET_ERROR)
									{
										if (WSAGetLastError() == WSA_IO_PENDING);
										else
										{
											InterlockedDecrement(&ALAN_CSV[currentALAN_CS_Index].IO_Count);
											--ALAN_CSV[currentALAN_CS_Index].SendCount;

											printf("[WSASend] sessionKey : %llu, IO_Count : %d, WSAGetLastError : %d" CRALF, ALAN_CSV[currentALAN_CS_Index].SessionKey, ALAN_CSV[currentALAN_CS_Index].IO_Count, WSAGetLastError());
										}
									}
								}

								//if(0 < --LCSV[clientIndex].SendCount && 0 < LCSV[clientIndex].ReceiveCount)
								if (!InterlockedDecrement(&ALAN_CSV[currentALAN_CS_Index].IO_Count))
								{
									if (InterlockedExchange(&ALAN_CSV[currentALAN_CS_Index].Connected, 0))
									{
										ALAN_CSV[currentALAN_CS_Index].CurrentPhaseIndex = ADVANCED_LAN_CLIENT_PHASE_DISCONNECT;

										PostQueuedCompletionStatus(ALAN_S_DISCONNECT_CP, NULL, ALAN_CSV[currentALAN_CS_Index].SessionKey, &ALAN_CSV[currentALAN_CS_Index].OverlappedSend);
										//THIS->Disconnect(ALAN_CSV[currentALAN_CS_Index].SessionKey);
									}
								}
								//else LCSV[clientIndex].CurrentPhaseIndex = LAN_CLIENT_PHASE_RECEIVE;
							}
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
				currentALAN_CS_Index = completionKey;
		
				if (InterlockedExchange(&ALAN_CSV[currentALAN_CS_Index].Connected, 0))
				{
					ALAN_CSV[currentALAN_CS_Index].CurrentPhaseIndex = ADVANCED_LAN_CLIENT_PHASE_DISCONNECT;
		
					PostQueuedCompletionStatus(ALAN_S_DISCONNECT_CP, NULL, ALAN_CSV[currentALAN_CS_Index].SessionKey, overlapped);
					//THIS->Disconnect(ALAN_CSV[currentALAN_CS_Index].SessionKey);
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