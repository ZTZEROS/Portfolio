#include "stdafx.h"
#include "AdvancedWAN.h"
#include "BaseWAN_Server.h"

AdvancedWAN_ServerSession BaseWAN_Server::AWAN_SS;
AdvancedWAN_ClientSessionV BaseWAN_Server::AWAN_CSV;
AdvancedWAN_SubThreadV BaseWAN_Server::AWAN_SUB_THREAD_V;

BOOL BaseWAN_Server::SUB_THREAD_ON;
UINT BaseWAN_Server::CURRENT_ACCEPTION_COUNT;

HANDLE BaseWAN_Server::AWAN_S_COMMON_CP;
//HANDLE BaseWAN_Server::AWAN_S_ACCEPT_CP;
//HANDLE BaseWAN_Server::AWAN_S_RECEIVE_CP;
//HANDLE BaseWAN_Server::AWAN_S_SEND_CP;
//HANDLE BaseWAN_Server::AWAN_S_UPDATE_CP;
//HANDLE BaseWAN_Server::AWAN_S_SEND_CP;
HANDLE BaseWAN_Server::AWAN_S_DISCONNECT_CP;

BaseWAN_Server* BaseWAN_Server::THIS;

ThreadLocalPool<SerialPacketQueue<AdvancedWAN_PacketHeader>> BaseWAN_Server::PACKET_SPQ_TLP;

UINT BaseWAN_Server::SPQ_ALLOCATION_COUNT;
UINT BaseWAN_Server::SPQ_DEALLOCATION_COUNT;

BOOL BaseWAN_Server::THREAD_CONTEXT_SWITCH_ON;
BOOL BaseWAN_Server::INDICATOR_ON;

BaseWAN_Server::BaseWAN_Server() {}
BaseWAN_Server::~BaseWAN_Server() {}

bool BaseWAN_Server::Initialize()
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
	AdvancedWAN_SubThread subThread;

	{
		{
			ZeroMemory(&AWAN_SS, sizeof(AWAN_SS));
			AWAN_CSV.resize(TOTAL_ADVANCED_WAN_CLIENT_COUNT);
			AWAN_SUB_THREAD_V.resize(ADVANCED_WAN_SUB_THREAD_COUNT);

			FOR(i, TOTAL_ADVANCED_WAN_CLIENT_COUNT)
			{
				AWAN_CSV[i].Socket = INVALID_SOCKET;

				ZeroMemory(&AWAN_CSV[i].Address, sizeof(AWAN_CSV[i].Address));
				ZeroMemory(&AWAN_CSV[i].OverlappedReceive, sizeof(AWAN_CSV[i].OverlappedReceive));
				ZeroMemory(&AWAN_CSV[i].OverlappedSend, sizeof(AWAN_CSV[i].OverlappedSend));

				AWAN_CSV[i].ReceiveQ.Resize(BUFFER_SIZE);
				AWAN_CSV[i].SendQ.Resize(BUFFER_SIZE);

				AWAN_CSV[i].CurrentPhaseIndex = ADVANCED_WAN_CLIENT_PHASE_ACCEPT;

				AWAN_CSV[i].AccumulatedUseCount = 0;
			}

			SUB_THREAD_ON = 1;
			CURRENT_ACCEPTION_COUNT = 0;
			
			AWAN_S_COMMON_CP = nullptr;
			AWAN_S_DISCONNECT_CP = nullptr;

			THIS = this;
		}

		{
			//PACKET_SPQ_TLP.Initialize();
			
			SPQ_ALLOCATION_COUNT = 0;
			SPQ_DEALLOCATION_COUNT = 0;

			THREAD_CONTEXT_SWITCH_ON = 0;
			INDICATOR_ON = 0;
		}
	}

	{
		CHECK_ERROR(WSAStartup(0x0202, &wsaDatum));

		AWAN_SS.Socket = WSASocket(PF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED);
		CHECK_INVALID_SOCKET(AWAN_SS.Socket);

		//{
		//	lingeringTime.l_onoff = 1;
		//	lingeringTime.l_linger = 0;
		//	CHECK_SOCKET_ERROR(setsockopt(AWAN_SS.Socket, SOL_SOCKET, SO_LINGER, (char*)&lingeringTime, sizeof(lingeringTime)));
		//}

		//{
		//	addressReuseOn = 1;
		//	CHECK_SOCKET_ERROR(setsockopt(AWAN_SS.Socket, SOL_SOCKET, SO_REUSEADDR, (char*)&addressReuseOn, sizeof(addressReuseOn)));
		//}

		//{
		//	keepAliveOn = 1;
		//	CHECK_SOCKET_ERROR(setsockopt(AWAN_SS.Socket, SOL_SOCKET, SO_KEEPALIVE, (char*)&keepAliveOn, sizeof(keepAliveOn)));
		//}

		{
			nagleAlgorithmOff = 1;
			CHECK_SOCKET_ERROR(setsockopt(AWAN_SS.Socket, IPPROTO_TCP, TCP_NODELAY, (char*)&nagleAlgorithmOff, sizeof(nagleAlgorithmOff)));
		}

		//{
		//	socketReceiveBufferSize = 0;
		//	CHECK_SOCKET_ERROR(setsockopt(AWAN_SS.Socket, SOL_SOCKET, SO_RCVBUF, (char*)&socketReceiveBufferSize, sizeof(socketReceiveBufferSize)));
		//}

		{
			socketSendBufferSize = 0;
			CHECK_SOCKET_ERROR(setsockopt(AWAN_SS.Socket, SOL_SOCKET, SO_SNDBUF, (char*)&socketSendBufferSize, sizeof(socketSendBufferSize)));
		}

		//{
		//	nonblocked = 1;
		//	CHECK_SOCKET_ERROR(ioctlsocket(AWAN_SS.Socket, FIONBIO, &nonblocked));
		//}

		AWAN_SS.Address.sin_family = AF_INET;
		AWAN_SS.Address.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
		//InetPton(AF_INET, ipAddress, &AWAN_SS.Address.sin_addr); //inet_addr(INADDR_ANY); //InetNtop();
		//WSAAddressToString();
		//WSAStringToAddress();
		AWAN_SS.Address.sin_port = htons(ADVANCED_WAN_CHAT_SERVER_PORT);

		CHECK_SOCKET_ERROR(bind(AWAN_SS.Socket, (SOCKADDR*)&AWAN_SS.Address, sizeof(AWAN_SS.Address)));
		CHECK_SOCKET_ERROR(listen(AWAN_SS.Socket, SOMAXCONN));
	}

	{
		timeBeginPeriod(1);

		TIMER.Initialize();

		srand(time(nullptr));
		//srand(2705);

		GetSystemInfo(&systemInformation);
	}

	{
		AWAN_S_COMMON_CP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, NULL, ADVANCED_WAN_SUB_THREAD_COUNT - ADVANCED_WAN_SUB_THREAD_WORK1);
		//AWAN_S_ACCEPT_CP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, NULL, 1);
		//AWAN_S_RECEIVE_CP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, NULL, 1);
		//AWAN_S_UPDATE_CP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, NULL, 1);
		//AWAN_S_SEND_CP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, NULL, 1);
		AWAN_S_DISCONNECT_CP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, NULL, 1);

		subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, AcceptAdvancedWAN_Client, THIS, 0, &subThread.IdentificationNumber);
		AWAN_SUB_THREAD_V[ADVANCED_WAN_SUB_THREAD_ACCEPT] = subThread;

		//subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, ReceiveAdvancedWAN_Client, THIS, 0, &subThread.IdentificationNumber);
		//AWAN_SUB_THREAD_V[ADVANCED_WAN_SUB_THREAD_RECEIVE] = subThread;
		//
		//subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, UpdateAdvancedWAN_Client, THIS, 0, &subThread.IdentificationNumber);
		//AWAN_SUB_THREAD_V[ADVANCED_WAN_SUB_THREAD_UPDATE] = subThread;
		//
		//subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, SendAdvancedWAN_Client, THIS, 0, &subThread.IdentificationNumber);
		//AWAN_SUB_THREAD_V[ADVANCED_WAN_SUB_THREAD_SEND] = subThread;

		subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, DisconnectAdvancedWAN_Client, THIS, 0, &subThread.IdentificationNumber);
		AWAN_SUB_THREAD_V[ADVANCED_WAN_SUB_THREAD_DISCONNECT] = subThread;

		subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, WorkAdvancedWAN_Server, THIS, 0, &subThread.IdentificationNumber);
		AWAN_SUB_THREAD_V[ADVANCED_WAN_SUB_THREAD_WORK1] = subThread;

		subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, WorkAdvancedWAN_Server, THIS, 0, &subThread.IdentificationNumber);
		AWAN_SUB_THREAD_V[ADVANCED_WAN_SUB_THREAD_WORK2] = subThread;

		subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, WorkAdvancedWAN_Server, THIS, 0, &subThread.IdentificationNumber);
		AWAN_SUB_THREAD_V[ADVANCED_WAN_SUB_THREAD_WORK3] = subThread;

		subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, WorkAdvancedWAN_Server, THIS, 0, &subThread.IdentificationNumber);
		AWAN_SUB_THREAD_V[ADVANCED_WAN_SUB_THREAD_WORK4] = subThread;

		//subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, WorkAdvancedWAN_Server, THIS, 0, &subThread.IdentificationNumber);
		//AWAN_SUB_THREAD_V[ADVANCED_LAN_SUB_THREAD_WORK5] = subThread;
		//
		//subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, WorkAdvancedWAN_Server, THIS, 0, &subThread.IdentificationNumber);
		//AWAN_SUB_THREAD_V[ADVANCED_LAN_SUB_THREAD_WORK6] = subThread;
	}

	return 0;
}

bool BaseWAN_Server::Terminalize()
{
	HANDLE subThreadHandleA[ADVANCED_WAN_SUB_THREAD_COUNT];

	{
		closesocket(AWAN_SS.Socket); //close accept thread

		FOR(i, TOTAL_ADVANCED_WAN_CLIENT_COUNT) //induct disconnection
		{
			shutdown(AWAN_CSV[i].Socket, SD_BOTH);
		}

		FOR(i, TOTAL_ADVANCED_WAN_CLIENT_COUNT) //confirm disconnection
		{
			if (AWAN_CSV[i].CurrentPhaseIndex != ADVANCED_WAN_CLIENT_PHASE_ACCEPT)
			{
				--i;

				Sleep(0);
			}
		}

		SUB_THREAD_ON = 0;

		FOR(i, ADVANCED_WAN_SUB_THREAD_COUNT - ADVANCED_WAN_SUB_THREAD_WORK1) //close work thread
		{
			PostQueuedCompletionStatus(AWAN_S_COMMON_CP, NULL, NULL, nullptr);
		}

		PostQueuedCompletionStatus(AWAN_S_DISCONNECT_CP, NULL, NULL, nullptr); //close disconnect thread

		FOR(i, ADVANCED_WAN_SUB_THREAD_COUNT)
		{
			subThreadHandleA[i] = AWAN_SUB_THREAD_V[i].Handle;
		}

		WaitForMultipleObjects(AWAN_SUB_THREAD_V.size(), subThreadHandleA, 1, INFINITE); //confirm thread closed

		CloseHandle(AWAN_S_COMMON_CP);
		//CloseHandle(AWAN_S_ACCEPT_CP);
		//CloseHandle(AWAN_S_RECEIVE_CP);
		//CloseHandle(AWAN_S_UPDATE_CP);
		//CloseHandle(AWAN_S_SEND_CP);
		CloseHandle(AWAN_S_DISCONNECT_CP);

		FOR(i, ADVANCED_WAN_SUB_THREAD_COUNT)
		{
			CloseHandle(AWAN_SUB_THREAD_V[i].Handle);
		}

		AWAN_CSV.clear();
		AWAN_SUB_THREAD_V.clear();
	}

	{
		timeEndPeriod(1);

		TIMER.Termimalize();

		PACKET_SPQ_TLP.Terminalize();
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