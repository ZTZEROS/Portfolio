#include "stdafx.h"
#include "IOCP_ChatServer.h"

unsigned int WINAPI AcceptIOCP_ChatClient(LPVOID argument)
{
	IOCP_ChatClient iocpChatClient;
	int addressLength;

	addressLength = sizeof(iocpChatClient.Address);

	while (!IOCP_CHAT_SERVER_SHUTDOWN)
	{
		ZeroMemory(&iocpChatClient, sizeof(iocpChatClient));

		iocpChatClient.Socket = WSAAccept(IOCP_CHAT_SERVER.Socket, (SOCKADDR*)&iocpChatClient.Address, &addressLength, nullptr, NULL);
		CHECK_INVALID_SOCKET(iocpChatClient.Socket)
		//IF_INVALID_SOCKET(iocpChatClient.Socket)
		//ELSE
		{
			FOR(i, TOTAL_IOCP_CHAT_CLIENT_COUNT)
			{
				if (IOCP_CHAT_CLIENT_V[i].CurrentStepIndex == IOCP_CHAT_CLIENT_STEP_ACCEPT)
				{
					IF_NULL(CreateIoCompletionPort((HANDLE)iocpChatClient.Socket, IOCP_CHAT_SERVER_IOCP, i, IOCP_CHAT_SUB_THREAD_WORK4 - IOCP_CHAT_SUB_THREAD_WORK1 + 1))
					ELSE
					{
						IOCP_CHAT_CLIENT_V[i].Socket = iocpChatClient.Socket;
						IOCP_CHAT_CLIENT_V[i].Address = iocpChatClient.Address;
						IOCP_CHAT_CLIENT_V[i].Connected = 1;
						IOCP_CHAT_CLIENT_V[i].Sendable = 1;
						IOCP_CHAT_CLIENT_V[i].ReceiveCount = 0;
						IOCP_CHAT_CLIENT_V[i].SendCount = 0;

						ZeroMemory(&IOCP_CHAT_CLIENT_V[i].OverlappedReceive, sizeof(IOCP_CHAT_CLIENT_V[i].OverlappedReceive));
						ZeroMemory(&IOCP_CHAT_CLIENT_V[i].OverlappedSend, sizeof(IOCP_CHAT_CLIENT_V[i].OverlappedSend));

						InterlockedIncrement(&IOCP_CHAT_CLIENT_V[i].IO_Count);
						++IOCP_CHAT_CLIENT_V[i].ReceiveCount;
						
						IOCP_CHAT_CLIENT_V[i].CurrentStepIndex = IOCP_CHAT_CLIENT_STEP_UPDATE;

						PostQueuedCompletionStatus(IOCP_CHAT_SERVER_IOCP, NULL, i, &IOCP_CHAT_CLIENT_V[i].OverlappedReceive);

						break;
					}
				}

				if (i == TOTAL_IOCP_CHAT_CLIENT_COUNT) closesocket(iocpChatClient.Socket);
			}
		}
	}

	return 0;
}

unsigned int WINAPI DisconnectIOCP_ChatClient(LPVOID argument)
{
	DWORD transferredSize;
	ULONG_PTR completionKey;
	OVERLAPPED* overlapped;
	UINT clientIndex;

	while (!IOCP_CHAT_SERVER_SHUTDOWN)
	{
		transferredSize = 0;
		completionKey = 0;
		overlapped = nullptr;
		clientIndex = 0xffffffff;

		if (GetQueuedCompletionStatus(IOCP_CHAT_SERVER_DISCONNECT_CP, &transferredSize, &completionKey, &overlapped, INFINITE))
		{
			clientIndex = completionKey;

			if (IOCP_CHAT_CLIENT_V[clientIndex].CurrentStepIndex == IOCP_CHAT_CLIENT_STEP_DISCONNECT)
			{
				IOCP_CHAT_CLIENT_V[clientIndex].Connected = 0;
				IOCP_CHAT_CLIENT_V[clientIndex].Sendable = 0;
				IOCP_CHAT_CLIENT_V[clientIndex].ReceiveBQ.Initialize();
				IOCP_CHAT_CLIENT_V[clientIndex].SendBQ.Initialize();

				closesocket(IOCP_CHAT_CLIENT_V[clientIndex].Socket);

				IOCP_CHAT_CLIENT_V[clientIndex].Socket = INVALID_SOCKET;

				IOCP_CHAT_CLIENT_V[clientIndex].CurrentStepIndex = IOCP_CHAT_CLIENT_STEP_ACCEPT;
			}
			else
			{
				printf("Disconnection Fail" CRALF);
			}
		}
	}

	return 0;
}

unsigned int WINAPI ReceiveIOCP_ChatClientPacket(LPVOID argument)
{
	UINT iocpChatClientCount;
	WSABUF currentBuffer;
	DWORD flag;
	INT returnValue;
	DWORD receivedSize;

	while (1)
	{
		iocpChatClientCount = IOCP_CHAT_CLIENT_V.size();

		FOR(i, iocpChatClientCount)
		{
			if (IOCP_CHAT_CLIENT_STEP_RECEIVE == IOCP_CHAT_CLIENT_V[i].CurrentStepIndex)
			{
				//IOCP_CHAT_CLIENT_V[i].ReceiveBQ.WSA_Receive();

				currentBuffer.buf = IOCP_CHAT_CLIENT_V[i].ReceiveBQ.GetNextAddress();
				currentBuffer.len = IOCP_CHAT_CLIENT_V[i].ReceiveBQ.GetLeftSize();
				flag = 0;

				//++IOCount;

				returnValue = WSARecv(IOCP_CHAT_CLIENT_V[i].Socket, &currentBuffer, 1, &receivedSize, &flag, &IOCP_CHAT_CLIENT_V[i].OverlappedReceive, nullptr);
				//if (returnValue < 1)
				if (returnValue == SOCKET_ERROR || returnValue == 0)
				{
					if (WSAGetLastError() == WSA_IO_PENDING);
					else
					{
						//--IOCount;
						IOCP_CHAT_CLIENT_V[i].CurrentStepIndex = IOCP_CHAT_CLIENT_STEP_DISCONNECT;
					}
				}
				else IOCP_CHAT_CLIENT_V[i].CurrentStepIndex = IOCP_CHAT_CLIENT_STEP_UPDATE;
			}
		}
	}

	return 0;
}

unsigned int WINAPI SendIOCP_ChatServerPacket(LPVOID argument)
{
	UINT iocpChatClientCount;
	WSABUF currentBuffer;
	DWORD flag;
	INT returnValue;
	DWORD sendedSize; //sentSize;

	while (1)
	{
		iocpChatClientCount = IOCP_CHAT_CLIENT_V.size();

		FOR(i, iocpChatClientCount)
		{
			if (IOCP_CHAT_CLIENT_STEP_SEND == IOCP_CHAT_CLIENT_V[i].CurrentStepIndex)
			{
				//IOCP_CHAT_CLIENT_V[i].WSASend();

				currentBuffer.buf = IOCP_CHAT_CLIENT_V[i].SendBQ.GetFrontAddress();
				currentBuffer.len = IOCP_CHAT_CLIENT_V[i].SendBQ.GetCurrentSize();
				flag = 0;

				//++IO_Count;

				returnValue = WSASend(IOCP_CHAT_CLIENT_V[i].Socket, &currentBuffer, 1, &sendedSize, flag, &IOCP_CHAT_CLIENT_V[i].OverlappedSend, nullptr);
				//if(returnValue < 1)
				if (returnValue == SOCKET_ERROR)
				{
					if (WSAGetLastError() == WSA_IO_PENDING); //소켓버퍼 0으로 두고 pending 확인
					//else
					//{
					//	//--IO_Count;
					//	closesocket(IOCP_CHAT_CLIENT_V[i].Socket);
					//	IOCP_CHAT_CLIENT_V[i].CurrentStepIndex = IOCP_CHAT_CLIENT_STEP_DISCONNECT;
					//}
				}

				IOCP_CHAT_CLIENT_V[i].SendBQ.DecreaseCurrentSize(sendedSize);

				IOCP_CHAT_CLIENT_V[i].CurrentStepIndex = IOCP_CHAT_CLIENT_STEP_RECEIVE;
			}
		}
	}

	return 0;
}

unsigned int WINAPI ProcessReceivedIOCP_ChatClientPacket(LPVOID argument)
{
	while (1)
	{

	}

	return 0;
}

unsigned int WINAPI ProcessSendingIOCP_ChatServerPacket(LPVOID argument)
{
	while (1)
	{

	}

	return 0;
}



unsigned int WINAPI WorkIOCP_ChatServer(LPVOID argument)
{
	DWORD transferredSize;
	ULONG_PTR completionKey;
	OVERLAPPED* overlapped;
	UINT clientIndex;
	char buffer[BUFFER_SIZE];
	DWORD flag;
	int returnValue;

	while (!IOCP_CHAT_SERVER_SHUTDOWN)
	{
		transferredSize = 0;
		completionKey = 0;
		overlapped = nullptr;
		clientIndex = 0xffffffff;
		flag = 0;

		if (GetQueuedCompletionStatus(IOCP_CHAT_SERVER_IOCP, &transferredSize, &completionKey, &overlapped, INFINITE))
		{
			if (overlapped)
			{
				clientIndex = completionKey;

				if (&IOCP_CHAT_CLIENT_V[clientIndex].OverlappedReceive == overlapped)
				{
					ZeroMemory(&IOCP_CHAT_CLIENT_V[clientIndex].OverlappedReceive, sizeof(IOCP_CHAT_CLIENT_V[clientIndex].OverlappedReceive));

					if (0 < transferredSize) IOCP_CHAT_CLIENT_V[clientIndex].ReceiveBQ.IncreaseCurrentSize(transferredSize);

					//UpdateIOCP_ChatServer(); //exclude main loop update

					//rcvPost
					{
						InterlockedIncrement(&IOCP_CHAT_CLIENT_V[clientIndex].IO_Count);
						++IOCP_CHAT_CLIENT_V[clientIndex].ReceiveCount;

						returnValue = IOCP_CHAT_CLIENT_V[clientIndex].ReceiveBQ.WSAReceive(WSARecv, IOCP_CHAT_CLIENT_V[clientIndex].Socket, &flag, &IOCP_CHAT_CLIENT_V[clientIndex].OverlappedReceive);
						if (returnValue == SOCKET_ERROR)
						{
							if (WSAGetLastError() == WSA_IO_PENDING);
							else
							{
								InterlockedDecrement(&IOCP_CHAT_CLIENT_V[clientIndex].IO_Count);
								--IOCP_CHAT_CLIENT_V[clientIndex].ReceiveCount;

								printf("[WSARecv] clientIndex : %d, IO_Count : %d, WSAGetLastError() : %d" CRALF, clientIndex, IOCP_CHAT_CLIENT_V[clientIndex].IO_Count, WSAGetLastError());

								//shutdown(IOCP_CHAT_CLIENT_V[clientIndex].Socket, SD_SEND);
								//closesocket(IOCP_CHAT_CLIENT_V[clientIndex].Socket);
							}
						}

						//maybe unnecessary?
						//if(0 < --IOCP_CHAT_CLIENT_V[clientIndex].ReceiveCount && 0 < IOCP_CHAT_CLIENT_V[clientIndex].SendCount)
						if (!InterlockedDecrement(&IOCP_CHAT_CLIENT_V[clientIndex].IO_Count))
						{
							IOCP_CHAT_CLIENT_V[clientIndex].CurrentStepIndex = IOCP_CHAT_CLIENT_STEP_DISCONNECT;

							PostQueuedCompletionStatus(IOCP_CHAT_SERVER_DISCONNECT_CP, NULL, clientIndex, &IOCP_CHAT_CLIENT_V[clientIndex].OverlappedReceive);
						}
						//else IOCP_CHAT_CLIENT_V[clientIndex].CurrentStepIndex = IOCP_CHAT_CLIENT_STEP_UPDATE;
					}
				}
				else
				{
					//sndPost
					{
						//if (InterlockedCompareExchange((unsigned int*)&IOCP_CHAT_CLIENT_V[clientIndex].Sendable, 0, 1))
						if (InterlockedExchange((unsigned int*)&IOCP_CHAT_CLIENT_V[clientIndex].Sendable, 0))
						{
							ZeroMemory(&IOCP_CHAT_CLIENT_V[clientIndex].OverlappedSend, sizeof(IOCP_CHAT_CLIENT_V[clientIndex].OverlappedSend));

							if (0 < transferredSize)
							{
								IOCP_CHAT_CLIENT_V[clientIndex].SendBQ.DecreaseCurrentSize(transferredSize);
								IOCP_CHAT_CLIENT_V[clientIndex].CurrentStepIndex = IOCP_CHAT_CLIENT_STEP_UPDATE;
							}
							else
							{
								IOCP_CHAT_CLIENT_V[clientIndex].Sendable = 1;

								InterlockedIncrement(&IOCP_CHAT_CLIENT_V[clientIndex].IO_Count);
								++IOCP_CHAT_CLIENT_V[clientIndex].SendCount;

								returnValue = IOCP_CHAT_CLIENT_V[clientIndex].SendBQ.WSASend(WSASend, IOCP_CHAT_CLIENT_V[clientIndex].Socket, flag, &IOCP_CHAT_CLIENT_V[clientIndex].OverlappedSend);
								if (returnValue == SOCKET_ERROR)
								{
									if (WSAGetLastError() == WSA_IO_PENDING);
									else
									{
										InterlockedDecrement(&IOCP_CHAT_CLIENT_V[clientIndex].IO_Count);
										--IOCP_CHAT_CLIENT_V[clientIndex].SendCount;

										printf("[WSASend] clientIndex : %d, IO_Count : %d, WSAGetLastError() : %d" CRALF, clientIndex, IOCP_CHAT_CLIENT_V[clientIndex].IO_Count, WSAGetLastError());
										
										//shutdown(IOCP_CHAT_CLIENT_V[clientIndex].Socket, SD_RECEIVE);
										//closesocket(IOCP_CHAT_CLIENT_V[clientIndex].Socket);
									}
								}

								//maybe unnecessary?
								//if(0 < --IOCP_CHAT_CLIENT_V[clientIndex].SendCount && 0 < IOCP_CHAT_CLIENT_V[clientIndex].ReceiveCount)
								if (!InterlockedDecrement(&IOCP_CHAT_CLIENT_V[clientIndex].IO_Count))
								{
									IOCP_CHAT_CLIENT_V[clientIndex].CurrentStepIndex = IOCP_CHAT_CLIENT_STEP_DISCONNECT;

									PostQueuedCompletionStatus(IOCP_CHAT_SERVER_DISCONNECT_CP, NULL, clientIndex, &IOCP_CHAT_CLIENT_V[clientIndex].OverlappedSend);
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

				IOCP_CHAT_CLIENT_V[clientIndex].CurrentStepIndex = IOCP_CHAT_CLIENT_STEP_DISCONNECT;

				PostQueuedCompletionStatus(IOCP_CHAT_SERVER_DISCONNECT_CP, 0, clientIndex, overlapped);
			}
			else
			{
				if (GetLastError() == ERROR_ABANDONED_WAIT_0) break;
			}
		}
	}

	return 0;
}