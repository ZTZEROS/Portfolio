#include "stdafx.h"
#include "AdvancedLAN.h"
#include "BaseLAN_Server.h"

AdvancedLAN_ServerSession BaseLAN_Server::ALAN_SS; //AdvancedLAN_ServerSession;
AdvancedLAN_ClientSessionV BaseLAN_Server::ALAN_CSV; //AdvancedLAN_ClientSessionV;
AdvancedLAN_SubThreadV BaseLAN_Server::ALAN_SUB_THREAD_V; //AdvancedLAN_SubThreadV;

BOOL BaseLAN_Server::ALAN_S_SHUTDOWN;
UINT BaseLAN_Server::CURRENT_ACCEPTION_COUNT;

HANDLE BaseLAN_Server::ALAN_S_COMMON_CP;
//HANDLE BaseLAN_Server::ALAN_S_ACCEPT_CP;
//HANDLE BaseLAN_Server::ALAN_S_RECEIVE_CP;
//HANDLE BaseLAN_Server::ALAN_S_UPDATE_CP;
//HANDLE BaseLAN_Server::ALAN_S_SEND_CP;
HANDLE BaseLAN_Server::ALAN_S_DISCONNECT_CP;

BaseLAN_Server* BaseLAN_Server::THIS;

UINT BaseLAN_Server::SPQ_ALLOCATION_COUNT;
UINT BaseLAN_Server::SPQ_DEALLOCATION_COUNT;

//UINT BaseLAN_Server::RCV_DISCONNECTION_COUNT;
//UINT BaseLAN_Server::SND_DISCONNECTION_COUNT;

//UINT BaseLAN_Server::RCV_GHOST_SESSION_COUNT;
//UINT BaseLAN_Server::SND_GHOST_SESSION_COUNT;

//UINT BaseLAN_Server::DISCONNECTION_LEAK_COUNT;
//UINT BaseLAN_Server::RCV_LEAK_COUNT;
//UINT BaseLAN_Server::SND_LEAK_COUNT;

//UINT BaseLAN_Server::SLOW_DISCONNECTION_COUNT;

//UINT BaseLAN_Server::PACKET_ERROR;

BaseLAN_Server::BaseLAN_Server() {}
BaseLAN_Server::~BaseLAN_Server() {}

bool BaseLAN_Server::Initialize()
{
	WSADATA wsaDatum;
	WSAPROTOCOL_INFO wsaProtocolInformation;
	LINGER lingeringTime;
	BOOL addressReuseOn;
	BOOL keepAliveOn;
	BOOL nagleAlgorithmOff;
	INT socketReceiveBufferSize;
	INT socketSendBufferSize;
	u_long nonblocked;
	
	SYSTEM_INFO systemInformation;
	AdvancedLAN_SubThread subThread;

	{
		{
			ZeroMemory(&ALAN_SS, sizeof(ALAN_SS));
			ALAN_CSV.resize(TOTAL_ADVANCED_LAN_CLIENT_COUNT);
			ALAN_SUB_THREAD_V.resize(ADVANCED_LAN_SUB_THREAD_COUNT);

			FOR(i, TOTAL_ADVANCED_LAN_CLIENT_COUNT)
			{
				ALAN_CSV[i].Socket = INVALID_SOCKET;

				ZeroMemory(&ALAN_CSV[i].Address, sizeof(ALAN_CSV[i].Address));
				ZeroMemory(&ALAN_CSV[i].OverlappedReceive, sizeof(ALAN_CSV[i].OverlappedReceive));
				ZeroMemory(&ALAN_CSV[i].OverlappedSend, sizeof(ALAN_CSV[i].OverlappedSend));

				ALAN_CSV[i].ReceiveQ.Resize(BUFFER_SIZE);
				ALAN_CSV[i].SendQ.Resize(BUFFER_SIZE);

				ALAN_CSV[i].CurrentPhaseIndex = ADVANCED_LAN_CLIENT_PHASE_ACCEPT;

				ALAN_CSV[i].AccumulatedUseCount = 0;
			}

			ALAN_S_SHUTDOWN = 0;
			CURRENT_ACCEPTION_COUNT = 0;

			ALAN_S_COMMON_CP = nullptr;
			ALAN_S_DISCONNECT_CP = nullptr;

			THIS = this;
		}

		{
			SPQ_ALLOCATION_COUNT = 0;
			SPQ_DEALLOCATION_COUNT = 0;

			//RCV_DISCONNECTION_COUNT = 0;
			//SND_DISCONNECTION_COUNT = 0;

			//RCV_GHOST_SESSION_COUNT = 0;
			//SND_GHOST_SESSION_COUNT = 0;

			//DISCONNECTION_LEAK_COUNT = 0;
			//RCV_LEAK_COUNT = 0;
			//SND_LEAK_COUNT = 0;

			//SLOW_DISCONNECTION_COUNT = 0;

			//PACKET_ERROR = 0;
		}
	}

	{
		CHECK_ERROR(WSAStartup(0x0202, &wsaDatum));

		ALAN_SS.Socket = WSASocket(PF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED);
		CHECK_INVALID_SOCKET(ALAN_SS.Socket);

		//{
		//	lingeringTime.l_onoff = 1;
		//	lingeringTime.l_linger = 0;
		//	CHECK_SOCKET_ERROR(setsockopt(ALAN_SS.Socket, SOL_SOCKET, SO_LINGER, (char*)&lingeringTime, sizeof(lingeringTime)));
		//}
		
		//{
		//	addressReuseOn = 1;
		//	CHECK_SOCKET_ERROR(setsockopt(ALAN_SS.Socket, SOL_SOCKET, SO_REUSEADDR, (char*)&addressReuseOn, sizeof(addressReuseOn)));
		//}

		//{
		//	keepAliveOn = 1;
		//	CHECK_SOCKET_ERROR(setsockopt(ALAN_SS.Socket, SOL_SOCKET, SO_KEEPALIVE, (char*)&keepAliveOn, sizeof(keepAliveOn)));
		//}

		{
			nagleAlgorithmOff = 1;
			CHECK_SOCKET_ERROR(setsockopt(ALAN_SS.Socket, IPPROTO_TCP, TCP_NODELAY, (char*)&nagleAlgorithmOff, sizeof(nagleAlgorithmOff)));
		}

		//{
		//	socketReceiveBufferSize = 0;
		//	CHECK_SOCKET_ERROR(setsockopt(ALAN_SS.Socket, SOL_SOCKET, SO_RCVBUF, (char*)&socketReceiveBufferSize, sizeof(socketReceiveBufferSize)));
		//}

		{
			socketSendBufferSize = 0;
			CHECK_SOCKET_ERROR(setsockopt(ALAN_SS.Socket, SOL_SOCKET, SO_SNDBUF, (char*)&socketSendBufferSize, sizeof(socketSendBufferSize)));
		}

		//{
		//	nonblocked = 1;
		//	CHECK_SOCKET_ERROR(ioctlsocket(ALAN_SS.Socket, FIONBIO, &nonblocked));
		//}

		ALAN_SS.Address.sin_family = AF_INET;
		ALAN_SS.Address.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
		//InetPton(AF_INET, ipAddress, &ALAN_SS.Address.sin_addr); //inet_addr(INADDR_ANY); //InetNtop();
		//WSAAddressToString();
		//WSAStringToAddress();
		ALAN_SS.Address.sin_port = htons(ADVANCED_LAN_SERVER_PORT);

		CHECK_SOCKET_ERROR(bind(ALAN_SS.Socket, (SOCKADDR*)&ALAN_SS.Address, sizeof(ALAN_SS.Address)));
		CHECK_SOCKET_ERROR(listen(ALAN_SS.Socket, SOMAXCONN));
	}

	{
		timeBeginPeriod(1);
		
		TIMER.Initialize();

		srand(time(nullptr));
		//srand(2705);

		GetSystemInfo(&systemInformation);
	}

	{
		ALAN_S_COMMON_CP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, NULL, ADVANCED_LAN_SUB_THREAD_COUNT - ADVANCED_LAN_SUB_THREAD_WORK1);
		//ALAN_S_ACCEPT_CP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 1);
		//ALAN_S_RECEIVE_CP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 1);
		//ALAN_S_UPDATE_CP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 1);
		//ALAN_S_SEND_CP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 1);
		ALAN_S_DISCONNECT_CP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, NULL, 1);

		subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, AcceptAdvancedLAN_Client, THIS, 0, &subThread.IdentificationNumber);
		ALAN_SUB_THREAD_V[ADVANCED_LAN_SUB_THREAD_ACCEPT] = subThread;

		//subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, ReceiveAdvancedLAN_Client, THIS, 0, &subThread.IdentificationNumber);
		//ALAN_SUB_THREAD_V[ADVANCED_LAN_SUB_THREAD_RECEIVE] = subThread;
		//
		//subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, UpdateAdvancedLAN_Client, THIS, 0, &subThread.IdentificationNumber);
		//ALAN_SUB_THREAD_V[ADVANCED_LAN_SUB_THREAD_UPDATE] = subThread;
		//
		//subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, SendAdvancedLAN_Client, THIS, 0, &subThread.IdentificationNumber);
		//ALAN_SUB_THREAD_V[ADVANCED_LAN_SUB_THREAD_SEND] = subThread;

		subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, DisconnectAdvancedLAN_Client, THIS, 0, &subThread.IdentificationNumber);
		ALAN_SUB_THREAD_V[ADVANCED_LAN_SUB_THREAD_DISCONNECT] = subThread;

		subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, WorkAdvancedLAN_Server, THIS, 0, &subThread.IdentificationNumber);
		ALAN_SUB_THREAD_V[ADVANCED_LAN_SUB_THREAD_WORK1] = subThread;

		subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, WorkAdvancedLAN_Server, THIS, 0, &subThread.IdentificationNumber);
		ALAN_SUB_THREAD_V[ADVANCED_LAN_SUB_THREAD_WORK2] = subThread;

		subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, WorkAdvancedLAN_Server, THIS, 0, &subThread.IdentificationNumber);
		ALAN_SUB_THREAD_V[ADVANCED_LAN_SUB_THREAD_WORK3] = subThread;

		subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, WorkAdvancedLAN_Server, THIS, 0, &subThread.IdentificationNumber);
		ALAN_SUB_THREAD_V[ADVANCED_LAN_SUB_THREAD_WORK4] = subThread;
		
		//subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, WorkAdvancedLAN_Server, THIS, 0, &subThread.IdentificationNumber);
		//ALAN_SUB_THREAD_V[ADVANCED_LAN_SUB_THREAD_WORK5] = subThread;
		//
		//subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, WorkAdvancedLAN_Server, THIS, 0, &subThread.IdentificationNumber);
		//ALAN_SUB_THREAD_V[ADVANCED_LAN_SUB_THREAD_WORK6] = subThread;
	}

	return 0;
}

bool BaseLAN_Server::Terminalize()
{
	HANDLE subThreadHandleA[ADVANCED_LAN_SUB_THREAD_COUNT];

	{
		closesocket(ALAN_SS.Socket); //close accept thread

		FOR(i, TOTAL_ADVANCED_LAN_CLIENT_COUNT) //induct disconnection
		{
			shutdown(ALAN_CSV[i].Socket, SD_BOTH);
		}
		
		FOR(i, TOTAL_ADVANCED_LAN_CLIENT_COUNT) //confirm disconnection
		{
			if (ALAN_CSV[i].CurrentPhaseIndex != ADVANCED_LAN_CLIENT_PHASE_ACCEPT)
			{
				--i;

				Sleep(0);
			}
		}

		ALAN_S_SHUTDOWN = 1;

		FOR(i, ADVANCED_LAN_SUB_THREAD_COUNT - ADVANCED_LAN_SUB_THREAD_WORK1) //close work thread
		{
			PostQueuedCompletionStatus(ALAN_S_COMMON_CP, NULL, NULL, nullptr);
		}

		PostQueuedCompletionStatus(ALAN_S_DISCONNECT_CP, NULL, NULL, nullptr); //close disconnect thread

		FOR(i, ADVANCED_LAN_SUB_THREAD_COUNT)
		{
			subThreadHandleA[i] = ALAN_SUB_THREAD_V[i].Handle;
		}

		WaitForMultipleObjects(ALAN_SUB_THREAD_V.size(), subThreadHandleA, 1, INFINITE); //confirm thread closing

		CloseHandle(ALAN_S_COMMON_CP);
		//CloseHandle(ALAN_S_ACCEPT_CP);
		//CloseHandle(ALAN_S_RECEIVE_CP);
		//CloseHandle(ALAN_S_UPDATE_CP);
		//CloseHandle(ALAN_S_SEND_CP);
		CloseHandle(ALAN_S_DISCONNECT_CP);

		FOR(i, ADVANCED_LAN_SUB_THREAD_COUNT)
		{
			CloseHandle(ALAN_SUB_THREAD_V[i].Handle);
		}

		ALAN_CSV.clear();
		ALAN_SUB_THREAD_V.clear();
	}

	{
		timeEndPeriod(1);

		TIMER.Termimalize();
	}

	{
		WSACleanup();
	}

	{
		printf(CRALF);
		printf("SPQ_ALLOCATION_COUNT : %u" CRALF, SPQ_ALLOCATION_COUNT);
		printf("SPQ_DEALLOCATION_COUNT : %u" CRALF, SPQ_DEALLOCATION_COUNT);
	}

	return 0;
}