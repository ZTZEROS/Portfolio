#include "stdafx.h"
#include "AnotherLAN.h"
#include "BaseLAN_Server.h"

AnotherLAN_ServerSession BaseLAN_Server::ALAN_SS;
AnotherLAN_ClientSessionV BaseLAN_Server::ALAN_CSV;
AnotherLAN_SubThreadV BaseLAN_Server::ALAN_SUB_THREAD_V;

BOOL BaseLAN_Server::ALAN_S_SHUTDOWN;
UINT BaseLAN_Server::CURRENT_SESSION_KEY;

HANDLE BaseLAN_Server::ALAN_S_COMMON_CP;
HANDLE BaseLAN_Server::ALAN_S_DISCONNECT_CP;

SRWLOCK BaseLAN_Server::ALAN_CSAM_LOCK;

BaseLAN_Server* BaseLAN_Server::THIS;



UINT BaseLAN_Server::SBQ_ALLOCATION_COUNT;
UINT BaseLAN_Server::SBQ_DEALLOCATION_COUNT;

UINT BaseLAN_Server::GQCS_DISCONNECTION_COUNT;
UINT BaseLAN_Server::RCV_DISCONNECTION_COUNT;
UINT BaseLAN_Server::SND_DISCONNECTION_COUNT;

UINT BaseLAN_Server::ACCEPT_LEAK_COUNT;
UINT BaseLAN_Server::DISCONNECT_LEAK_COUNT;
UINT BaseLAN_Server::RCV_LEAK;
UINT BaseLAN_Server::SND_LEAK;
UINT BaseLAN_Server::RCV_GHOST_SESSION_COUNT;
UINT BaseLAN_Server::SND_GHOST_SESSION_COUNT;
UINT BaseLAN_Server::PACKET_ERROR;
UINT BaseLAN_Server::SLOW_DISCONNECTION_COUNT;



BaseLAN_Server::BaseLAN_Server() {}
BaseLAN_Server::~BaseLAN_Server() {}

bool BaseLAN_Server::Initialize()
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
	AnotherLAN_SubThread subThread;

	ZeroMemory(&ALAN_SS, sizeof(ALAN_SS)); //warning, need to reinitialize
	ALAN_CSV.resize(TOTAL_ANOTHER_LAN_CLIENT_COUNT);
	ALAN_SUB_THREAD_V.resize(ANOTHER_LAN_SUB_THREAD_COUNT);

	{
		CHECK_ERROR(WSAStartup(0x0202, &wsaDatum));

		ALAN_SS.Socket = WSASocket(PF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED);
		CHECK_INVALID_SOCKET(ALAN_SS.Socket);

		//lingeringTime.l_onoff = 1;
		//lingeringTime.l_linger = 0;
		//CHECK_SOCKET_ERROR(setsockopt(IOCP_CHAT_SERVER.Socket, SOL_SOCKET, SO_LINGER, (char*)&lingeringTime, sizeof(lingeringTime)));

		//reusable = 1;
		//CHECK_SOCKET_ERROR(setsockopt(IOCP_CHAT_SERVER.Socket, SOL_SOCKET, SO_REUSEADDR, (char*)&reusable, sizeof(reusable)));

		//keepAliveOn = 1;
		//CHECK_SOCKET_ERROR(setsockopt(IOCP_CHAT_SERVER.Socket, SOL_SOCKET, SO_KEEPALIVE, (char*)&keepAliveOn, sizeof(keepAliveOn)));

		nagleOff = 1;
		CHECK_SOCKET_ERROR(setsockopt(ALAN_SS.Socket, IPPROTO_TCP, TCP_NODELAY, (char*)&nagleOff, sizeof(nagleOff)));

		//socketReceiveBufferSize = 0;
		//CHECK_SOCKET_ERROR(setsockopt(IOCP_CHAT_SERVER.Socket, SOL_SOCKET, SO_RCVBUF, (char*)&socketReceiveBufferSize, sizeof(socketReceiveBufferSize)));

		socketSendBufferSize = 0;
		CHECK_SOCKET_ERROR(setsockopt(ALAN_SS.Socket, SOL_SOCKET, SO_SNDBUF, (char*)&socketSendBufferSize, sizeof(socketSendBufferSize)));

		//nonblocked = 1;
		//CHECK_SOCKET_ERROR(ioctlsocket(IOCP_CHAT_SERVER.Socket, FIONBIO, &nonblocked));

		ALAN_SS.Address.sin_family = AF_INET;
		ALAN_SS.Address.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
		//InetPton(AF_INET, ipAddress, &LSS.Address.sin_addr); //inet_addr(INADDR_ANY); //InetNtop();
		//WSAAddressToString();
		//WSAStringToAddress();
		ALAN_SS.Address.sin_port = htons(ANOTHER_LAN_SERVER_PORT);
		CHECK_SOCKET_ERROR(bind(ALAN_SS.Socket, (SOCKADDR*)&ALAN_SS.Address, sizeof(ALAN_SS.Address)));
		CHECK_SOCKET_ERROR(listen(ALAN_SS.Socket, SOMAXCONN));
	}

	ALAN_S_SHUTDOWN = 0;
	CURRENT_SESSION_KEY = 0xffffffff;
	THIS = this;



	SBQ_ALLOCATION_COUNT = 0;
	SBQ_DEALLOCATION_COUNT = 0;
	GQCS_DISCONNECTION_COUNT = 0;
	RCV_DISCONNECTION_COUNT = 0;
	
	DISCONNECT_LEAK_COUNT = 0;
	RCV_LEAK = 0;
	SND_LEAK = 0;
	RCV_GHOST_SESSION_COUNT = 0;
	SND_GHOST_SESSION_COUNT = 0;



	timeBeginPeriod(1);

	TIMER.Initialize();

	//srand(time(nullptr));
	srand(2705);

	GetSystemInfo(&systemInformation);

	{
		FOR(i, TOTAL_ANOTHER_LAN_CLIENT_COUNT)
		{
			ALAN_CSV[i].Socket = INVALID_SOCKET;

			ZeroMemory(&ALAN_CSV[i].Address, sizeof(ALAN_CSV[i].Address));
			ZeroMemory(&ALAN_CSV[i].OverlappedReceive, sizeof(ALAN_CSV[i].OverlappedReceive));
			ZeroMemory(&ALAN_CSV[i].OverlappedSend, sizeof(ALAN_CSV[i].OverlappedSend));

			ALAN_CSV[i].ReceiveQ.Resize(BUFFER_SIZE);
			ALAN_CSV[i].SendQ.Resize(BUFFER_SIZE);

			ALAN_CSV[i].CurrentPhaseIndex = ANOTHER_LAN_CLIENT_PHASE_ACCEPT;
			ALAN_CSV[i].AccumulatedUseCount = 0;
		}
	}

	{
		ALAN_S_COMMON_CP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, ANOTHER_LAN_SUB_THREAD_WORK4 - ANOTHER_LAN_SUB_THREAD_WORK1 + 1);
		//ALS_ACCEPT_CP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 1);
		//ALS_RECEIVE_CP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 1);
		//ALS_UPDATE_CP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 1);
		//ALS_SEND_CP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 1);
		ALAN_S_DISCONNECT_CP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 1);

		subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, AcceptAnotherLAN_Client, THIS, 0, (unsigned int*)&subThread.IdentificationNumber);
		ALAN_SUB_THREAD_V[ANOTHER_LAN_SUB_THREAD_ACCEPT] = subThread;

		//subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, ReceiveIOCP_ChatClientPacket, nullptr, 0, (unsigned int*)&subThread.IdentificationNumber);
		//AL_SUB_THREAD_V[IOCP_CHAT_SUB_THREAD_RECEIVE] = subThread;

		//subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, ReceiveIOCP_ChatClientPacket, nullptr, 0, (unsigned int*)&subThread.IdentificationNumber);
		//AL_SUB_THREAD_V[IOCP_CHAT_SUB_THREAD_UPDATE] = subThread;

		//subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, SendIOCP_ChatServerPacket, nullptr, 0, (unsigned int*)&subThread.IdentificationNumber);
		//AL_SUB_THREAD_V[IOCP_CHAT_SUB_THREAD_SEND] = subThread;

		subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, DisconnectAnotherLAN_Client, THIS, 0, (unsigned int*)&subThread.IdentificationNumber);
		ALAN_SUB_THREAD_V[ANOTHER_LAN_SUB_THREAD_DISCONNECT] = subThread;

		subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, WorkAnotherLAN_Server, THIS, 0, (unsigned int*)&subThread.IdentificationNumber);
		ALAN_SUB_THREAD_V[ANOTHER_LAN_SUB_THREAD_WORK1] = subThread;
		
		subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, WorkAnotherLAN_Server, THIS, 0, (unsigned int*)&subThread.IdentificationNumber);
		ALAN_SUB_THREAD_V[ANOTHER_LAN_SUB_THREAD_WORK2] = subThread;
		
		subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, WorkAnotherLAN_Server, THIS, 0, (unsigned int*)&subThread.IdentificationNumber);
		ALAN_SUB_THREAD_V[ANOTHER_LAN_SUB_THREAD_WORK3] = subThread;
		
		subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, WorkAnotherLAN_Server, THIS, 0, (unsigned int*)&subThread.IdentificationNumber);
		ALAN_SUB_THREAD_V[ANOTHER_LAN_SUB_THREAD_WORK4] = subThread;
	}



	InitializeSRWLock(&ALAN_CSAM_LOCK);

	return 0;
}

bool BaseLAN_Server::Terminalize()
{
	HANDLE subThreadHandle[ANOTHER_LAN_SUB_THREAD_COUNT];

	ALAN_S_SHUTDOWN = 1;

	closesocket(ALAN_SS.Socket);

	//uncorrected;
	FOR(i, TOTAL_ANOTHER_LAN_CLIENT_COUNT)
	{
		if (ALAN_CSV[i].Connected)
		{
			ALAN_CSV[i].CurrentPhaseIndex = ANOTHER_LAN_CLIENT_PHASE_DISCONNECT;
			ALAN_CSV[i].Connected = 0;
			ALAN_CSV[i].Sendable = 0;

			PostQueuedCompletionStatus(ALAN_S_COMMON_CP, NULL, i, &ALAN_CSV[i].OverlappedReceive); //???
			PostQueuedCompletionStatus(ALAN_S_COMMON_CP, NULL, i, &ALAN_CSV[i].OverlappedSend); //???
		}
	}
	//uncorrected;

	ALAN_CSV.clear();

	FOR(i, ANOTHER_LAN_SUB_THREAD_COUNT)
	{
		subThreadHandle[i] = ALAN_SUB_THREAD_V[i].Handle;
	}

	WaitForMultipleObjects(ALAN_SUB_THREAD_V.size(), subThreadHandle, 1, INFINITE);

	CloseHandle(ALAN_S_COMMON_CP);
	//CloseHandle(ALS_ACCEPT_CP);
	//CloseHandle(ALS_RECEIVE_CP);
	//CloseHandle(ALS_UPDATE_CP);
	//CloseHandle(ALS_SEND_CP);
	CloseHandle(ALAN_S_DISCONNECT_CP);

	FOR(i, ANOTHER_LAN_SUB_THREAD_COUNT)
	{
		CloseHandle(ALAN_SUB_THREAD_V[i].Handle);
	}

	ALAN_SUB_THREAD_V.clear();

	timeEndPeriod(1);

	TIMER.Termimalize();

	WSACleanup();

	return 0;
}

bool BaseLAN_Server::ReceivePacket(UINT64 sessionKey, SerialPacketQueue<AnotherLAN_PacketHeader>& receivedPacketSPQ)
{
	AnotherLAN_PacketHeader receivedPacketHeader;
	UINT clientIndex;
	int result;

	clientIndex = sessionKey >> ANOTHER_LAN_CLIENT_INDEX_SHIFTED_BIT_COUNT;
	
	if (ALAN_CSV[clientIndex].Connected && ALAN_CSV[clientIndex].CurrentPhaseIndex == ANOTHER_LAN_CLIENT_PHASE_UPDATE)
	//if (ALAN_CSV[clientIndex].SessionKey == sessionKey && ALAN_CSV[clientIndex].CurrentPhaseIndex == ADVANCED_LAN_CLIENT_PHASE_UPDATE) //ADVANCED_LAN_CLIENT_PHASE_RECEIVE
	{
		result = ALAN_CSV[clientIndex].ReceiveBQ.Peek((char*)&receivedPacketHeader, sizeof(receivedPacketHeader));
		if (sizeof(receivedPacketHeader) == result)
		{
			//receivedPacketSPQ.SetHeader(receivedPacketHeader);
			//ALAN_CSV[clientIndex].ReceiveBQ.Dequeue((char*)&receivedPacketHeader, sizeof(receivedPacketHeader));

			result = ALAN_CSV[clientIndex].ReceiveBQ.Peek(receivedPacketSPQ.GetQueueAddress(), sizeof(receivedPacketHeader) + receivedPacketHeader.BodySize);
			if (sizeof(receivedPacketHeader) + receivedPacketHeader.BodySize == result)
			{
				receivedPacketSPQ.IncreaseCurrentSize(receivedPacketHeader.BodySize);
				//ALAN_CSV[clientIndex].ReceiveBQ.Dequeue(receivedPacketSPQ.GetQueueAddress(), sizeof(receivedPacketHeader) + receivedPacketHeader);

				//lanClientSessionAddress->ReceiveBQ.DecreaseCurrentSize(sizeof(receivedPacketHeader) + receivedPacketHeader); //ugly code
				
				//THIS->OnRecv(sessionKey, result);

				return 0;
			}
		}
	}

	return 1;
}

bool BaseLAN_Server::ReceivePacket(UINT64 sessionKey, SerialPacketQueue<AnotherLAN_PacketHeader>* receivedPacketSPQ_Address)
{
	return 0;
}

bool BaseLAN_Server::SendPacket(UINT64 sessionKey, SerialPacketQueue<AnotherLAN_PacketHeader>& sendingPacketSPQ)
{
	AnotherLAN_PacketHeader sendingPacketHeader;
	UINT clientIndex;
	int result;

	clientIndex = sessionKey >> ANOTHER_LAN_CLIENT_INDEX_SHIFTED_BIT_COUNT;
	
	if (ALAN_CSV[clientIndex].Connected && ALAN_CSV[clientIndex].CurrentPhaseIndex == ANOTHER_LAN_CLIENT_PHASE_UPDATE)
	//if (ALCSV[clientIndex].SessionKey == sessionKey && ALCSV[clientIndex].CurrentPhaseIndex == ADVANCED_LAN_CLIENT_PHASE_UPDATE)
	{
		result = sendingPacketSPQ.GetHeaderSize();
		if (sizeof(sendingPacketHeader) == result)
		{
			sendingPacketHeader = sendingPacketSPQ.GetHeader();

			result = ALAN_CSV[clientIndex].SendBQ.Enqueue(sendingPacketSPQ.GetQueueAddress(), sendingPacketSPQ.GetTotalSize());
			if (sizeof(sendingPacketHeader) + sendingPacketHeader.BodySize == result)
			{
				sendingPacketSPQ.DecreaseCurrentSize(sendingPacketHeader.BodySize); //not necessary

				ALAN_CSV[clientIndex].ReceiveBQ.DecreaseCurrentSize(sizeof(sendingPacketHeader) + sendingPacketHeader.BodySize); //ugly code
			}
		}

		if (ALAN_CSV[clientIndex].SendBQ.GetCurrentSize())
		{
			ALAN_CSV[clientIndex].CurrentPhaseIndex = ANOTHER_LAN_CLIENT_PHASE_SEND;
			ALAN_CSV[clientIndex].Sendable = 1;

			PostQueuedCompletionStatus(ALAN_S_COMMON_CP, NULL, ALAN_CSV[clientIndex].SessionKey, &ALAN_CSV[clientIndex].OverlappedSend);

			//THIS->OnSend(sessionKey, result);

			return 0;
		}
	}

	return 1;
}

bool BaseLAN_Server::SendPacket(UINT64 sessionKey, SerialPacketQueue<AnotherLAN_PacketHeader>* sendingPacketSPQ_Address)
{
	WSABUF sendingPacketWSABUF;
	AnotherLAN_PacketHeader sendingPacketHeader;
	UINT clientIndex;
	int result;

	clientIndex = sessionKey >> ANOTHER_LAN_CLIENT_INDEX_SHIFTED_BIT_COUNT;

	if (ALAN_CSV[clientIndex].Connected && ALAN_CSV[clientIndex].CurrentPhaseIndex == ANOTHER_LAN_CLIENT_PHASE_UPDATE)
	{
		result = sendingPacketSPQ_Address->GetHeaderSize();
		if (sizeof(sendingPacketHeader) == result)
		{
			sendingPacketHeader = sendingPacketSPQ_Address->GetHeader();
			
			result = sendingPacketSPQ_Address->GetTotalSize();
			if (sizeof(sendingPacketHeader) + sendingPacketHeader.BodySize == result)
			{
				ALAN_CSV[clientIndex].SendQ.Enqueue(sendingPacketSPQ_Address);

				ALAN_CSV[clientIndex].ReceiveBQ.DecreaseCurrentSize(sizeof(sendingPacketHeader) + sendingPacketHeader.BodySize); //ugly code
			}
		}

		if (ALAN_CSV[clientIndex].SendQ.GetCurrentSize())
		{
			ALAN_CSV[clientIndex].CurrentPhaseIndex = ANOTHER_LAN_CLIENT_PHASE_SEND;
			ALAN_CSV[clientIndex].Sendable = 1;

			PostQueuedCompletionStatus(ALAN_S_COMMON_CP, NULL, ALAN_CSV[clientIndex].SessionKey, &ALAN_CSV[clientIndex].OverlappedSend);

			//THIS->OnSend(sessionKey, result);

			return 0;
		}

		//HasOverlappedIoCompleted(IOCP_CHAT_CLIENT_V[i].OverlappedSend);
		//CancelIo(IOCP_CHAT_SERVER_IOCP);
	}

	return 0;
}

bool BaseLAN_Server::Start(LPCWSTR ipAddress, unsigned int port, unsigned int workThreadCount, bool nagleOff, unsigned int maximumLanClientCount)
{
	Initialize();

	return 0;
}

void BaseLAN_Server::Stop()
{
	Terminalize();
}

bool BaseLAN_Server::Disconnect(UINT64 sessionKey)
{
	UINT clientIndex;
	int result;

	clientIndex = sessionKey >> ANOTHER_LAN_CLIENT_INDEX_SHIFTED_BIT_COUNT;

	ALAN_CSV[clientIndex].CurrentPhaseIndex = ANOTHER_LAN_CLIENT_PHASE_DISCONNECT;

	PostQueuedCompletionStatus(ALAN_S_DISCONNECT_CP, NULL, ALAN_CSV[clientIndex].SessionKey, &ALAN_CSV[clientIndex].OverlappedSend);

	return 0;
}