#include "stdafx.h"
#include "JurassicWarServer.h"

unsigned int WINAPI AcceptJurassicWarClient(LPVOID argument)
{
	JurassicWarClient jurassicWarClient;
	int addressLength;

	addressLength = sizeof(jurassicWarClient.Address);

	while (!JURASSIC_WAR_SERVER_SHUTDOWN)
	{
		ZeroMemory(&jurassicWarClient, sizeof(jurassicWarClient));

		jurassicWarClient.Socket = WSAAccept(JURASSIC_WAR_SERVER.Socket, (SOCKADDR*)&jurassicWarClient.Address, &addressLength, nullptr, NULL);
		CHECK_INVALID_SOCKET(jurassicWarClient.Socket)
		//IF_INVALID_SOCKET(iocpChatClient.Socket)
		//ELSE
		{
			FOR(i, JURASSIC_WAR_CLIENT_COUNT)
			{
				if (JURASSIC_WAR_CLIENT_V[i].CurrentStepIndex == JURASSIC_WAR_CLIENT_PHASE_ACCEPT)
				{
					IF_NULL(CreateIoCompletionPort((HANDLE)jurassicWarClient.Socket, JURASSIC_WAR_SERVER_IOCP, i, JURASSIC_WAR_SUB_THREAD_WORK4 - JURASSIC_WAR_SUB_THREAD_WORK1 + 1))
					ELSE
					{
						JURASSIC_WAR_CLIENT_V[i].Socket = jurassicWarClient.Socket;
						JURASSIC_WAR_CLIENT_V[i].Address = jurassicWarClient.Address;
						JURASSIC_WAR_CLIENT_V[i].Connected = 1;
						JURASSIC_WAR_CLIENT_V[i].Sendable = 1;
						JURASSIC_WAR_CLIENT_V[i].ReceiveCount = 0;
						JURASSIC_WAR_CLIENT_V[i].SendCount = 0;

						ZeroMemory(&JURASSIC_WAR_CLIENT_V[i].OverlappedReceiving, sizeof(JURASSIC_WAR_CLIENT_V[i].OverlappedReceiving));
						ZeroMemory(&JURASSIC_WAR_CLIENT_V[i].OverlappedSending, sizeof(JURASSIC_WAR_CLIENT_V[i].OverlappedSending));

						JURASSIC_WAR_CLIENT_V[i].CurrentStepIndex = JURASSIC_WAR_CLIENT_PHASE_UPDATE;

						//InterlockedIncrement(&JURASSIC_WAR_CLIENT_V[i].ReceiveCount);
						++JURASSIC_WAR_CLIENT_V[i].ReceiveCount;

						PostQueuedCompletionStatus(JURASSIC_WAR_SERVER_IOCP, NULL, i, &JURASSIC_WAR_CLIENT_V[i].OverlappedReceiving);

						break;
					}
				}
			}
		}
	}

	return 0;
}

unsigned int WINAPI DisconnectJurassicWarClient(LPVOID argument)
{
	DWORD transferredSize;
	ULONG_PTR completionKey;
	OVERLAPPED* overlapped;
	UINT clientIndex;

	while (!JURASSIC_WAR_SERVER_SHUTDOWN)
	{
		transferredSize = 0;
		completionKey = 0;
		overlapped = nullptr;
		clientIndex = 0xffffffff;

		if (GetQueuedCompletionStatus(JURASSIC_WAR_SERVER_DISCONNECT_CP, &transferredSize, &completionKey, &overlapped, INFINITE))
		{
			clientIndex = completionKey;

			if (JURASSIC_WAR_CLIENT_V[clientIndex].CurrentStepIndex == JURASSIC_WAR_CLIENT_PHASE_DISCONNECT)
			{
				JURASSIC_WAR_CLIENT_V[clientIndex].Connected = 0;
				JURASSIC_WAR_CLIENT_V[clientIndex].Sendable = 0;
				JURASSIC_WAR_CLIENT_V[clientIndex].ReceiveBQ.Initialize();
				JURASSIC_WAR_CLIENT_V[clientIndex].SendBQ.Initialize();

				closesocket(JURASSIC_WAR_CLIENT_V[clientIndex].Socket);

				JURASSIC_WAR_CLIENT_V[clientIndex].Socket = INVALID_SOCKET;

				JURASSIC_WAR_CLIENT_V[clientIndex].CurrentStepIndex = JURASSIC_WAR_CLIENT_PHASE_ACCEPT;
			}
		}
	}

	return 0;
}


unsigned int WINAPI WorkJurassicWarServer(LPVOID argument)
{
	DWORD transferredSize;
	ULONG_PTR completionKey;
	OVERLAPPED* overlapped;
	UINT clientIndex;
	char buffer[BUFFER_SIZE];
	DWORD flag;
	int returnValue;

	while (!JURASSIC_WAR_SERVER_SHUTDOWN)
	{
		transferredSize = 0;
		completionKey = 0;
		overlapped = nullptr;
		clientIndex = 0xffffffff;
		flag = 0;

		if (GetQueuedCompletionStatus(JURASSIC_WAR_SERVER_IOCP, &transferredSize, &completionKey, &overlapped, INFINITE))
		{
			if (overlapped)
			{
				clientIndex = completionKey;

				if (&JURASSIC_WAR_CLIENT_V[clientIndex].OverlappedReceiving == overlapped)
				{
					ZeroMemory(&JURASSIC_WAR_CLIENT_V[clientIndex].OverlappedReceiving, sizeof(JURASSIC_WAR_CLIENT_V[clientIndex].OverlappedReceiving));

					if (0 < transferredSize) JURASSIC_WAR_CLIENT_V[clientIndex].ReceiveBQ.IncreaseCurrentSize(transferredSize);

					//UpdateIOCP_ChatServer(); //exclude main loop update

					//rcvPost
					{
						//InterlockedIncrement(&JURASSIC_WAR_CLIENT_V[clientIndex].ReceiveCount);
						++JURASSIC_WAR_CLIENT_V[clientIndex].ReceiveCount;

						returnValue = JURASSIC_WAR_CLIENT_V[clientIndex].ReceiveBQ.WSAReceive(WSARecv, JURASSIC_WAR_CLIENT_V[clientIndex].Socket, &flag, &JURASSIC_WAR_CLIENT_V[clientIndex].OverlappedReceiving);
						if (returnValue == SOCKET_ERROR)
						{
							if (WSAGetLastError() == WSA_IO_PENDING);
							else
							{
								InterlockedDecrement(&JURASSIC_WAR_CLIENT_V[clientIndex].ReceiveCount);
								--JURASSIC_WAR_CLIENT_V[clientIndex].ReceiveCount;

								printf("[WSARecv] clientIndex : %d, %d" CRALF, clientIndex, WSAGetLastError());

								//shutdown(IOCP_CHAT_CLIENT_V[clientIndex].Socket, SD_SEND);
							}
						}

						//if (!InterlockedDecrement(&JURASSIC_WAR_CLIENT_V[clientIndex].ReceiveCount) && 0 < IOCP_CHAT_CLIENT_V[clientIndex].SendCount)
						if(0 < --JURASSIC_WAR_CLIENT_V[clientIndex].ReceiveCount && 0 < JURASSIC_WAR_CLIENT_V[clientIndex].SendCount)
						{
							JURASSIC_WAR_CLIENT_V[clientIndex].CurrentStepIndex = JURASSIC_WAR_CLIENT_PHASE_DISCONNECT;

							PostQueuedCompletionStatus(JURASSIC_WAR_SERVER_DISCONNECT_CP, NULL, clientIndex, &JURASSIC_WAR_CLIENT_V[clientIndex].OverlappedReceiving);
						}
						//else IOCP_CHAT_CLIENT_V[clientIndex].CurrentStepIndex = IOCP_CHAT_CLIENT_STEP_UPDATE;
					}
				}
				else
				{
					//sndPost
					{
						//if (InterlockedCompareExchange((unsigned int*)&IOCP_CHAT_CLIENT_V[clientIndex].Sendable, 0, 1))
						if (InterlockedExchange((unsigned int*)&JURASSIC_WAR_CLIENT_V[clientIndex].Sendable, 0))
						{
							ZeroMemory(&JURASSIC_WAR_CLIENT_V[clientIndex].OverlappedSending, sizeof(JURASSIC_WAR_CLIENT_V[clientIndex].OverlappedSending));

							if (0 < transferredSize)
							{
								JURASSIC_WAR_CLIENT_V[clientIndex].SendBQ.DecreaseCurrentSize(transferredSize);
								JURASSIC_WAR_CLIENT_V[clientIndex].CurrentStepIndex = JURASSIC_WAR_CLIENT_PHASE_UPDATE;
							}
							else
							{
								JURASSIC_WAR_CLIENT_V[clientIndex].Sendable = 1;

								//InterlockedIncrement(&JURASSIC_WAR_CLIENT_V[clientIndex].SendCount);
								++JURASSIC_WAR_CLIENT_V[clientIndex].SendCount;

								returnValue = JURASSIC_WAR_CLIENT_V[clientIndex].SendBQ.WSASend(WSASend, JURASSIC_WAR_CLIENT_V[clientIndex].Socket, flag, &JURASSIC_WAR_CLIENT_V[clientIndex].OverlappedSending);
								if (returnValue == SOCKET_ERROR)
								{
									if (WSAGetLastError() == WSA_IO_PENDING);
									else
									{
										//InterlockedDecrement(&IOCP_CHAT_CLIENT_V[clientIndex].SendCount);
										--JURASSIC_WAR_CLIENT_V[clientIndex].SendCount;

										printf("[WSASend] clientIndex : %d, %d" CRALF, clientIndex, WSAGetLastError());

										//shutdown(IOCP_CHAT_CLIENT_V[clientIndex].Socket, SD_RECEIVE);
									}
								}

								//if (!InterlockedDecrement(&JURASSIC_WAR_CLIENT_V[clientIndex].SendCount) && 0 < JURASSIC_WAR_CLIENT_V[clientIndex].ReceiveCount)
								if(0 < --JURASSIC_WAR_CLIENT_V[clientIndex].SendCount && 0 < JURASSIC_WAR_CLIENT_V[clientIndex].ReceiveCount)
								{
									JURASSIC_WAR_CLIENT_V[clientIndex].CurrentStepIndex = JURASSIC_WAR_CLIENT_PHASE_DISCONNECT;

									PostQueuedCompletionStatus(JURASSIC_WAR_SERVER_DISCONNECT_CP, NULL, clientIndex, &JURASSIC_WAR_CLIENT_V[clientIndex].OverlappedSending);
								}
								//else IOCP_CHAT_CLIENT_V[clientIndex].CurrentStepIndex = IOCP_CHAT_CLIENT_STEP_RECEIVE;
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
				clientIndex = completionKey;

				JURASSIC_WAR_CLIENT_V[clientIndex].CurrentStepIndex = JURASSIC_WAR_CLIENT_PHASE_DISCONNECT;

				PostQueuedCompletionStatus(JURASSIC_WAR_SERVER_DISCONNECT_CP, 0, clientIndex, overlapped);
			}
			else
			{
				if (GetLastError() == ERROR_ABANDONED_WAIT_0) break;
			}
		}
	}

	return 0;
}