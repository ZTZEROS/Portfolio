#include "stdafx.h"
#include "JurassicWarServer.h"

bool InitializeJurassicWarServer()
{
	WSADATA	wsaDatum;
	//WSAPROTOCOL_INFO wsaProtocolInformation;
	LINGER lingeringTime;
	//BOOL reusable;
	//BOOL keepAliveOn;
	BOOL nagleOff;
	//INT socketReceiveBufferSize;
	INT socketSendBufferSize;
	//u_long nonblocked;
	SYSTEM_INFO systemInformation;
	JurassicWarSubThread subThread;

	ZeroMemory(&JURASSIC_WAR_SERVER, sizeof(JURASSIC_WAR_SERVER));
	JURASSIC_WAR_CLIENT_V.resize(TOTAL_JURASSIC_WAR_CLIENT_COUNT);
	JURASSIC_WAR_SUB_THREAD_V.resize(JURASSIC_WAR_SUB_THREAD_COUNT);


	{
		CHECK_ERROR(WSAStartup(0x0202, &wsaDatum));

		JURASSIC_WAR_SERVER.Socket = WSASocket(PF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED);
		CHECK_INVALID_SOCKET(JURASSIC_WAR_SERVER.Socket);

		//lingeringTime.l_onoff = 1;
		//lingeringTime.l_linger = 0;
		//CHECK_SOCKET_ERROR(setsockopt(IOCP_CHAT_SERVER.Socket, SOL_SOCKET, SO_LINGER, (char*)&lingeringTime, sizeof(lingeringTime)));

		//reusable = 1;
		//CHECK_SOCKET_ERROR(setsockopt(IOCP_CHAT_SERVER.Socket, SOL_SOCKET, SO_REUSEADDR, (char*)&reusable, sizeof(reusable)));

		//keepAliveOn = 1;
		//CHECK_SOCKET_ERROR(setsockopt(IOCP_CHAT_SERVER.Socket, SOL_SOCKET, SO_KEEPALIVE, (char*)&keepAliveOn, sizeof(keepAliveOn)));

		nagleOff = 1;
		CHECK_SOCKET_ERROR(setsockopt(JURASSIC_WAR_SERVER.Socket, IPPROTO_TCP, TCP_NODELAY, (char*)&nagleOff, sizeof(nagleOff)));

		//socketReceiveBufferSize = 0;
		//CHECK_SOCKET_ERROR(setsockopt(IOCP_CHAT_SERVER.Socket, SOL_SOCKET, SO_RCVBUF, (char*)&socketReceiveBufferSize, sizeof(socketReceiveBufferSize)));

		socketSendBufferSize = 0;
		CHECK_SOCKET_ERROR(setsockopt(JURASSIC_WAR_SERVER.Socket, SOL_SOCKET, SO_SNDBUF, (char*)&socketSendBufferSize, sizeof(socketSendBufferSize)));

		//nonblocked = 1;
		//CHECK_SOCKET_ERROR(ioctlsocket(IOCP_CHAT_SERVER.Socket, FIONBIO, &nonblocked));

		JURASSIC_WAR_SERVER.Address.sin_family = AF_INET;
		JURASSIC_WAR_SERVER.Address.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
		//InetPton(AF_INET, INADDR_ANY, &IOCP_CHAT_SERVER.Address.sin_addr); //inet_addr(INADDR_ANY); //InetNtop();
		//WSAAddressToString();
		//WSAStringToAddress();
		JURASSIC_WAR_SERVER.Address.sin_port = htons(FREE_WIZARD_NET_SERVER_PORT);
		CHECK_SOCKET_ERROR(bind(JURASSIC_WAR_SERVER.Socket, (SOCKADDR*)&JURASSIC_WAR_SERVER.Address, sizeof(JURASSIC_WAR_SERVER.Address)));
		CHECK_SOCKET_ERROR(listen(JURASSIC_WAR_SERVER.Socket, SOMAXCONN));
	}

	timeBeginPeriod(1);

	TIMER.Initialize();

	//srand(time(nullptr));
	srand(2705);

	GetSystemInfo(&systemInformation);

	JURASSIC_WAR_SERVER_SHUTDOWN = 0;

	{
		JURASSIC_WAR_SERVER_IOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, JURASSIC_WAR_SUB_THREAD_WORK4 - JURASSIC_WAR_SUB_THREAD_WORK1 + 1);
		JURASSIC_WAR_SERVER_RECEIVE_CP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 1);
		JURASSIC_WAR_SERVER_UPDATE_CP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 1);
		JURASSIC_WAR_SERVER_SEND_CP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 1);
		JURASSIC_WAR_SERVER_DISCONNECT_CP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 1);

		subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, AcceptJurassicWarClient, nullptr, 0, (unsigned int*)&subThread.IdentificationNumber);
		JURASSIC_WAR_SUB_THREAD_V[JURASSIC_WAR_SUB_THREAD_ACCEPT] = subThread;

		//subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, ReceiveIOCP_ChatClientPacket, nullptr, 0, (unsigned int*)&subThread.IdentificationNumber);
		//IOCP_CHAT_SUB_THREAD_V[IOCP_CHAT_SUB_THREAD_RECEIVE] = subThread;

		//subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, ReceiveIOCP_ChatClientPacket, nullptr, 0, (unsigned int*)&subThread.IdentificationNumber);
		//IOCP_CHAT_SUB_THREAD_V[IOCP_CHAT_SUB_THREAD_UPDATE] = subThread;

		//subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, SendIOCP_ChatServerPacket, nullptr, 0, (unsigned int*)&subThread.IdentificationNumber);
		//IOCP_CHAT_SUB_THREAD_V[IOCP_CHAT_SUB_THREAD_SEND] = subThread;

		subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, DisconnectJurassicWarClient, nullptr, 0, (unsigned int*)&subThread.IdentificationNumber);
		JURASSIC_WAR_SUB_THREAD_V[JURASSIC_WAR_SUB_THREAD_DISCONNECT] = subThread;

		subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, WorkJurassicWarServer, nullptr, 0, (unsigned int*)&subThread.IdentificationNumber);
		JURASSIC_WAR_SUB_THREAD_V[JURASSIC_WAR_SUB_THREAD_WORK1] = subThread;

		subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, WorkJurassicWarServer, nullptr, 0, (unsigned int*)&subThread.IdentificationNumber);
		JURASSIC_WAR_SUB_THREAD_V[JURASSIC_WAR_SUB_THREAD_WORK2] = subThread;

		subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, WorkJurassicWarServer, nullptr, 0, (unsigned int*)&subThread.IdentificationNumber);
		JURASSIC_WAR_SUB_THREAD_V[JURASSIC_WAR_SUB_THREAD_WORK3] = subThread;

		subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, WorkJurassicWarServer, nullptr, 0, (unsigned int*)&subThread.IdentificationNumber);
		JURASSIC_WAR_SUB_THREAD_V[JURASSIC_WAR_SUB_THREAD_WORK4] = subThread;
	}

	{
		FOR(i, TOTAL_JURASSIC_WAR_CLIENT_COUNT)
		{
			JURASSIC_WAR_CLIENT_V[i].Socket = INVALID_SOCKET;

			ZeroMemory(&JURASSIC_WAR_CLIENT_V[i].Address, sizeof(JURASSIC_WAR_CLIENT_V[i].Address));
			ZeroMemory(&JURASSIC_WAR_CLIENT_V[i].OverlappedReceiving, sizeof(JURASSIC_WAR_CLIENT_V[i].OverlappedReceiving));
			ZeroMemory(&JURASSIC_WAR_CLIENT_V[i].OverlappedSending, sizeof(JURASSIC_WAR_CLIENT_V[i].OverlappedSending));

			JURASSIC_WAR_CLIENT_V[i].CurrentStepIndex = JURASSIC_WAR_CLIENT_PHASE_ACCEPT;
		}
	}

	return 0;
}

bool UpdateJurassicWarServer()
{
	FOR(i, TOTAL_JURASSIC_WAR_CLIENT_COUNT)
	{
		if (JURASSIC_WAR_CLIENT_V[i].CurrentStepIndex == JURASSIC_WAR_CLIENT_PHASE_UPDATE)
		{
			if (JURASSIC_WAR_CLIENT_V[i].ReceiveBQ.GetCurrentSize())
			{
				char receiveBuffer[BUFFER_SIZE];
				char sendingBuffer[BUFFER_SIZE];

				ZeroMemory(receiveBuffer, sizeof(receiveBuffer));

				JURASSIC_WAR_CLIENT_V[i].ReceiveBQ.Dequeue(receiveBuffer, JURASSIC_WAR_CLIENT_V[i].ReceiveBQ.GetCurrentSize());

				printf(receiveBuffer);

				scanf_s("%s", sendingBuffer);
				
				JURASSIC_WAR_CLIENT_V[i].ReceiveBQ.Enqueue(sendingBuffer, strlen(sendingBuffer));
			}

			while (0)
			{
				WORD receivedPacketHeader;
				WORD sendingPacketHeader;
				SerialByteQueue receivedPacketSBQ;
				SerialByteQueue sendingPacketSBQ;
				//sendingPacketSBQ.DecreaseCurrentSize(sendingPacketSBQ.GetCurrentSize());

				if (sizeof(receivedPacketHeader) == JURASSIC_WAR_CLIENT_V[i].ReceiveBQ.Peek((char*)&receivedPacketHeader, sizeof(receivedPacketHeader)))
				{
					if (sizeof(receivedPacketHeader) + receivedPacketHeader == JURASSIC_WAR_CLIENT_V[i].ReceiveBQ.Peek(receivedPacketSBQ.GetQueueAddress(), sizeof(receivedPacketHeader) + receivedPacketHeader))
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
						if (sizeof(sendingPacketHeader) + sendingPacketHeader == JURASSIC_WAR_CLIENT_V[i].SendBQ.Enqueue(sendingPacketSBQ.GetQueueAddress(), sendingPacketSBQ.GetTotalSize()))
						{
							JURASSIC_WAR_CLIENT_V[i].ReceiveBQ.DecreaseCurrentSize(sizeof(receivedPacketHeader) + receivedPacketHeader);
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

			if (JURASSIC_WAR_CLIENT_V[i].SendBQ.GetCurrentSize())
			{
				JURASSIC_WAR_CLIENT_V[i].CurrentStepIndex = JURASSIC_WAR_CLIENT_PHASE_SEND;
				JURASSIC_WAR_CLIENT_V[i].Sendable = 1;

				PostQueuedCompletionStatus(JURASSIC_WAR_SERVER_IOCP, NULL, i, &JURASSIC_WAR_CLIENT_V[i].OverlappedSending);
			}

			//HasOverlappedIoCompleted(IOCP_CHAT_CLIENT_V[i].OverlappedSend);
			//CancelIo(IOCP_CHAT_SERVER_IOCP);
		}
	}

	if (GetAsyncKeyState('Q')) return 1;

	return 0;
}

bool RenderJurassicWarServer()
{
	return 0;
}

bool TerminalizeJurassicWarServer()
{
	return 0;
}