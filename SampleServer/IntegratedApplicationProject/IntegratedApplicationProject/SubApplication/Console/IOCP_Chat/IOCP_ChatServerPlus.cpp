#include "stdafx.h"
#include "IOCP_ChatServer.h"

bool InitializeIOCP_ChatServer()
{
	WSADATA	wsaDatum;
	//WSAPROTOCOL_INFO wsaProtocolInformation;
	//LINGER lingeringTime;
	//BOOL reusable;
	//BOOL keepAliveOn;
	BOOL nagleOff;
	//INT socketReceiveBufferSize;
	INT socketSendBufferSize;
	//u_long nonblocked;
	SYSTEM_INFO systemInformation;
	IOCP_ChatSubThread subThread;

	ZeroMemory(&IOCP_CHAT_SERVER, sizeof(IOCP_CHAT_SERVER)); //warning
	IOCP_CHAT_CLIENT_V.resize(TOTAL_IOCP_CHAT_CLIENT_COUNT);
	IOCP_CHAT_SUB_THREAD_V.resize(IOCP_CHAT_SUB_THREAD_COUNT);

	{
		CHECK_ERROR(WSAStartup(0x0202, &wsaDatum));

		IOCP_CHAT_SERVER.Socket = WSASocket(PF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED);
		CHECK_INVALID_SOCKET(IOCP_CHAT_SERVER.Socket);

		//lingeringTime.l_onoff = 1;
		//lingeringTime.l_linger = 0;
		//CHECK_SOCKET_ERROR(setsockopt(IOCP_CHAT_SERVER.Socket, SOL_SOCKET, SO_LINGER, (char*)&lingeringTime, sizeof(lingeringTime)));

		//reusable = 1;
		//CHECK_SOCKET_ERROR(setsockopt(IOCP_CHAT_SERVER.Socket, SOL_SOCKET, SO_REUSEADDR, (char*)&reusable, sizeof(reusable)));

		//keepAliveOn = 1;
		//CHECK_SOCKET_ERROR(setsockopt(IOCP_CHAT_SERVER.Socket, SOL_SOCKET, SO_KEEPALIVE, (char*)&keepAliveOn, sizeof(keepAliveOn)));

		nagleOff = 1;
		CHECK_SOCKET_ERROR(setsockopt(IOCP_CHAT_SERVER.Socket, IPPROTO_TCP, TCP_NODELAY, (char*)&nagleOff, sizeof(nagleOff)));

		//socketReceiveBufferSize = 0;
		//CHECK_SOCKET_ERROR(setsockopt(IOCP_CHAT_SERVER.Socket, SOL_SOCKET, SO_RCVBUF, (char*)&socketReceiveBufferSize, sizeof(socketReceiveBufferSize)));

		socketSendBufferSize = 0;
		CHECK_SOCKET_ERROR(setsockopt(IOCP_CHAT_SERVER.Socket, SOL_SOCKET, SO_SNDBUF, (char*)&socketSendBufferSize, sizeof(socketSendBufferSize)));

		//nonblocked = 1;
		//CHECK_SOCKET_ERROR(ioctlsocket(IOCP_CHAT_SERVER.Socket, FIONBIO, &nonblocked));

		IOCP_CHAT_SERVER.Address.sin_family = AF_INET;
		IOCP_CHAT_SERVER.Address.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
		//InetPton(AF_INET, INADDR_ANY, &IOCP_CHAT_SERVER.Address.sin_addr); //inet_addr(INADDR_ANY); //InetNtop();
		//WSAAddressToString();
		//WSAStringToAddress();
		IOCP_CHAT_SERVER.Address.sin_port = htons(IOCP_CHAT_SERVER_PORT);
		CHECK_SOCKET_ERROR(bind(IOCP_CHAT_SERVER.Socket, (SOCKADDR*)&IOCP_CHAT_SERVER.Address, sizeof(IOCP_CHAT_SERVER.Address)));
		CHECK_SOCKET_ERROR(listen(IOCP_CHAT_SERVER.Socket, SOMAXCONN));
	}

	timeBeginPeriod(1);

	TIMER.Initialize();

	//srand(time(nullptr));
	srand(2705);

	GetSystemInfo(&systemInformation);

	IOCP_CHAT_SERVER_SHUTDOWN = 0;

	{
		IOCP_CHAT_SERVER_IOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, IOCP_CHAT_SUB_THREAD_WORK4 - IOCP_CHAT_SUB_THREAD_WORK1 + 1);
		IOCP_CHAT_SERVER_RECEIVE_CP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 1);
		IOCP_CHAT_SERVER_UPDATE_CP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 1);
		IOCP_CHAT_SERVER_SEND_CP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 1);
		IOCP_CHAT_SERVER_DISCONNECT_CP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 1);

		subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, AcceptIOCP_ChatClient, nullptr, 0, (unsigned int*)&subThread.IdentificationNumber);
		IOCP_CHAT_SUB_THREAD_V[IOCP_CHAT_SUB_THREAD_ACCEPT] = subThread;

		//subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, ReceiveIOCP_ChatClientPacket, nullptr, 0, (unsigned int*)&subThread.IdentificationNumber);
		//IOCP_CHAT_SUB_THREAD_V[IOCP_CHAT_SUB_THREAD_RECEIVE] = subThread;

		//subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, ReceiveIOCP_ChatClientPacket, nullptr, 0, (unsigned int*)&subThread.IdentificationNumber);
		//IOCP_CHAT_SUB_THREAD_V[IOCP_CHAT_SUB_THREAD_UPDATE] = subThread;
		
		//subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, SendIOCP_ChatServerPacket, nullptr, 0, (unsigned int*)&subThread.IdentificationNumber);
		//IOCP_CHAT_SUB_THREAD_V[IOCP_CHAT_SUB_THREAD_SEND] = subThread;
		
		subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, DisconnectIOCP_ChatClient, nullptr, 0, (unsigned int*)&subThread.IdentificationNumber);
		IOCP_CHAT_SUB_THREAD_V[IOCP_CHAT_SUB_THREAD_DISCONNECT] = subThread;

		subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, WorkIOCP_ChatServer, nullptr, 0, (unsigned int*)&subThread.IdentificationNumber);
		IOCP_CHAT_SUB_THREAD_V[IOCP_CHAT_SUB_THREAD_WORK1] = subThread;

		subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, WorkIOCP_ChatServer, nullptr, 0, (unsigned int*)&subThread.IdentificationNumber);
		IOCP_CHAT_SUB_THREAD_V[IOCP_CHAT_SUB_THREAD_WORK2] = subThread;

		subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, WorkIOCP_ChatServer, nullptr, 0, (unsigned int*)&subThread.IdentificationNumber);
		IOCP_CHAT_SUB_THREAD_V[IOCP_CHAT_SUB_THREAD_WORK3] = subThread;

		subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, WorkIOCP_ChatServer, nullptr, 0, (unsigned int*)&subThread.IdentificationNumber);
		IOCP_CHAT_SUB_THREAD_V[IOCP_CHAT_SUB_THREAD_WORK4] = subThread;
	}

	{
		FOR(i, TOTAL_IOCP_CHAT_CLIENT_COUNT)
		{
			IOCP_CHAT_CLIENT_V[i].Socket = INVALID_SOCKET;
			
			ZeroMemory(&IOCP_CHAT_CLIENT_V[i].Address, sizeof(IOCP_CHAT_CLIENT_V[i].Address));
			ZeroMemory(&IOCP_CHAT_CLIENT_V[i].OverlappedReceive, sizeof(IOCP_CHAT_CLIENT_V[i].OverlappedReceive));
			ZeroMemory(&IOCP_CHAT_CLIENT_V[i].OverlappedSend, sizeof(IOCP_CHAT_CLIENT_V[i].OverlappedSend));

			IOCP_CHAT_CLIENT_V[i].CurrentStepIndex = IOCP_CHAT_CLIENT_STEP_ACCEPT;
		}
	}

	return 0;
}

bool UpdateIOCP_ChatServer()
{
	FOR(i, TOTAL_IOCP_CHAT_CLIENT_COUNT)
	{
		if(IOCP_CHAT_CLIENT_V[i].CurrentStepIndex == IOCP_CHAT_CLIENT_STEP_UPDATE)
		{
			while (1)
			{
				WORD receivedPacketHeader;
				WORD sendingPacketHeader;
				SerialByteQueue receivedPacketSBQ;
				SerialByteQueue sendingPacketSBQ;
				//sendingPacketSBQ.DecreaseCurrentSize(sendingPacketSBQ.GetCurrentSize());
				
				if (sizeof(receivedPacketHeader) == IOCP_CHAT_CLIENT_V[i].ReceiveBQ.Peek((char*)&receivedPacketHeader, sizeof(receivedPacketHeader)))
				{
					if (sizeof(receivedPacketHeader) + receivedPacketHeader == IOCP_CHAT_CLIENT_V[i].ReceiveBQ.Peek(receivedPacketSBQ.GetQueueAddress(), sizeof(receivedPacketHeader) + receivedPacketHeader))
					{
						receivedPacketSBQ.SetHeader(receivedPacketHeader);
						sendingPacketSBQ.SetHeader(sendingPacketHeader);

						receivedPacketSBQ.IncreaseCurrentSize(receivedPacketHeader);

						{
							sendingPacketHeader = receivedPacketHeader;
							sendingPacketSBQ = receivedPacketSBQ; //wrong

							sendingPacketSBQ.SetHeader(sendingPacketHeader);
						}

						//IOCP_CHAT_CLIENT_V[i].SendBQ.AquireEnqueueLock();
						if (sizeof(sendingPacketHeader) + sendingPacketHeader == IOCP_CHAT_CLIENT_V[i].SendBQ.Enqueue(sendingPacketSBQ.GetQueueAddress(), sendingPacketSBQ.GetTotalSize()))
						{
							IOCP_CHAT_CLIENT_V[i].ReceiveBQ.DecreaseCurrentSize(sizeof(receivedPacketHeader) + receivedPacketHeader);
						}
						else break;
						//IOCP_CHAT_CLIENT_V[i].SendBQ.ReleaseEnqueueLock();
					}
					else break;
					
					//{
					//	DWORD receivedSize;
					//
					//	receivedSize = IOCP_CHAT_CLIENT_V[i].ReceiveBQ.Peek(receivedPacketSBQ.GetQueueAddress(), IOCP_CHAT_CLIENT_V[i].ReceiveBQ.GetCurrentSize());
					//
					//	receivedPacketSBQ.IncreaseCurrentSize(receivedSize);
					//
					//	sendingPacketSBQ = receivedPacketSBQ;
					//
					//	if (receivedSize == IOCP_CHAT_CLIENT_V[i].SendBQ.Enqueue(sendingPacketSBQ.GetQueueAddress(), receivedSize))
					//	{
					//		IOCP_CHAT_CLIENT_V[i].ReceiveBQ.DecreaseCurrentSize(IOCP_CHAT_CLIENT_V[i].ReceiveBQ.GetCurrentSize());
					//	}
					//	else break;
					//}
				}
				else break;
			}

			if(IOCP_CHAT_CLIENT_V[i].SendBQ.GetCurrentSize())
			{
				IOCP_CHAT_CLIENT_V[i].CurrentStepIndex = IOCP_CHAT_CLIENT_STEP_SEND;
				IOCP_CHAT_CLIENT_V[i].Sendable = 1;

				PostQueuedCompletionStatus(IOCP_CHAT_SERVER_IOCP, NULL, i, &IOCP_CHAT_CLIENT_V[i].OverlappedSend);
			}

			//HasOverlappedIoCompleted(IOCP_CHAT_CLIENT_V[i].OverlappedSend);
			//CancelIo(IOCP_CHAT_SERVER_IOCP);
		}
	}

	if (GetAsyncKeyState('Q')) return 1;

	return 0;
}

bool RenderIOCP_ChatServer()
{
	return 0;
}

bool TerminalizeIOCP_ChatServer()
{
	HANDLE subThreadHandle[IOCP_CHAT_SUB_THREAD_COUNT];
	UINT clientCount;

	IOCP_CHAT_SERVER_SHUTDOWN = 1;

	closesocket(IOCP_CHAT_SERVER.Socket);

	clientCount = IOCP_CHAT_CLIENT_V.size();

	//==================uncorrected code

	FOR(i, clientCount)
	{
		if (IOCP_CHAT_CLIENT_V[i].Connected)
		{
			IOCP_CHAT_CLIENT_V[i].CurrentStepIndex = IOCP_CHAT_CLIENT_STEP_DISCONNECT;

			//shutdown(IOCP_CHAT_CLIENT_V[i].Socket, SD_BOTH);
			closesocket(IOCP_CHAT_CLIENT_V[i].Socket);

			PostQueuedCompletionStatus(IOCP_CHAT_SERVER_IOCP, NULL, i, &IOCP_CHAT_CLIENT_V[i].OverlappedReceive);
			PostQueuedCompletionStatus(IOCP_CHAT_SERVER_IOCP, NULL, i, &IOCP_CHAT_CLIENT_V[i].OverlappedSend);
		}
	}

	//wrong
	CloseHandle(IOCP_CHAT_SERVER_IOCP);
	CloseHandle(IOCP_CHAT_SERVER_RECEIVE_CP);
	CloseHandle(IOCP_CHAT_SERVER_UPDATE_CP);
	CloseHandle(IOCP_CHAT_SERVER_SEND_CP);
	CloseHandle(IOCP_CHAT_SERVER_DISCONNECT_CP);

	//====================uncorrected code

	FOR(i, IOCP_CHAT_SUB_THREAD_COUNT)
	{
		subThreadHandle[i] = IOCP_CHAT_SUB_THREAD_V[i].Handle;
	}

	WaitForMultipleObjects(IOCP_CHAT_SUB_THREAD_COUNT, subThreadHandle, 1, INFINITE);

	CloseHandle(IOCP_CHAT_SERVER_IOCP);
	CloseHandle(IOCP_CHAT_SERVER_RECEIVE_CP);
	CloseHandle(IOCP_CHAT_SERVER_UPDATE_CP);
	CloseHandle(IOCP_CHAT_SERVER_SEND_CP);
	CloseHandle(IOCP_CHAT_SERVER_DISCONNECT_CP);

	FOR(i, IOCP_CHAT_SUB_THREAD_COUNT)
	{
		CloseHandle(IOCP_CHAT_SUB_THREAD_V[i].Handle);
	}

	timeEndPeriod(1);

	TIMER.Termimalize();

	WSACleanup();

	return 0;
}