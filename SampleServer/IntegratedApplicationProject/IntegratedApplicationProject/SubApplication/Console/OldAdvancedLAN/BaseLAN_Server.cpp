#include "stdafx.h"
#include "AdvancedLAN.h"
#include "BaseLAN_Server.h"

AdvancedLAN_ServerSession BaseLAN_Server::ALAN_SS;
AdvancedLAN_ClientSessionV BaseLAN_Server::ALAN_CSV;
AdvancedLAN_SubThreadV BaseLAN_Server::ALAN_SUB_THREAD_V;
LockFreeStack<UINT> BaseLAN_Server::ACCEPTABLE_ALAN_CS_INDEX_LFS;

BOOL BaseLAN_Server::ALAN_S_SHUTDOWN;
UINT BaseLAN_Server::CURRENT_SESSION_KEY;

HANDLE BaseLAN_Server::ALAN_S_COMMON_CP;
HANDLE BaseLAN_Server::ALAN_S_DISCONNECT_CP;

SRWLOCK BaseLAN_Server::ALAN_CSAM_LOCK;

BaseLAN_Server* BaseLAN_Server::THIS;



UINT BaseLAN_Server::SPQ_ALLOCATION_COUNT;
UINT BaseLAN_Server::SPQ_DEALLOCATION_COUNT;



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
	AdvancedLAN_SubThread subThread;

	ZeroMemory(&ALAN_SS, sizeof(ALAN_SS)); //warning, need to reinitialize
	ALAN_CSV.resize(TOTAL_ADVANCED_LAN_CLIENT_COUNT);
	ALAN_SUB_THREAD_V.resize(ADVANCED_LAN_SUB_THREAD_COUNT);

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
		ALAN_SS.Address.sin_port = htons(ADVANCED_LAN_SERVER_PORT);
		CHECK_SOCKET_ERROR(bind(ALAN_SS.Socket, (SOCKADDR*)&ALAN_SS.Address, sizeof(ALAN_SS.Address)));
		CHECK_SOCKET_ERROR(listen(ALAN_SS.Socket, SOMAXCONN));
	}

	ALAN_S_SHUTDOWN = 0;
	CURRENT_SESSION_KEY = 0xffffffff;
	THIS = this;



	SPQ_ALLOCATION_COUNT = 0;
	SPQ_DEALLOCATION_COUNT = 0;



	timeBeginPeriod(1);

	TIMER.Initialize();

	//srand(time(nullptr));
	srand(2705);

	GetSystemInfo(&systemInformation);

	{
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

			ACCEPTABLE_ALAN_CS_INDEX_LFS.Push(i);
			//ACCEPTABLE_ALAN_CS_INDEX_LFS.Push(TOTAL_ADVANCED_LAN_CLIENT_COUNT - i);
		}
	}

	{
		ALAN_S_COMMON_CP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, ADVANCED_LAN_SUB_THREAD_WORK4 - ADVANCED_LAN_SUB_THREAD_WORK1 + 1);
		//ALAN_S_ACCEPT_CP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 1);
		//ALAN_S_RECEIVE_CP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 1);
		//ALAN_S_UPDATE_CP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 1);
		//ALAN_S_SEND_CP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 1);
		ALAN_S_DISCONNECT_CP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 1);

		subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, AcceptAdvancedLAN_Client, THIS, 0, (unsigned int*)&subThread.IdentificationNumber);
		ALAN_SUB_THREAD_V[ADVANCED_LAN_SUB_THREAD_ACCEPT] = subThread;

		//subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, ReceiveIOCP_ChatClientPacket, nullptr, 0, (unsigned int*)&subThread.IdentificationNumber);
		//ALAN_SUB_THREAD_V[IOCP_CHAT_SUB_THREAD_RECEIVE] = subThread;

		//subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, ReceiveIOCP_ChatClientPacket, nullptr, 0, (unsigned int*)&subThread.IdentificationNumber);
		//ALAN_SUB_THREAD_V[IOCP_CHAT_SUB_THREAD_UPDATE] = subThread;

		//subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, SendIOCP_ChatServerPacket, nullptr, 0, (unsigned int*)&subThread.IdentificationNumber);
		//ALAN_SUB_THREAD_V[IOCP_CHAT_SUB_THREAD_SEND] = subThread;

		subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, DisconnectAdvancedLAN_Client, THIS, 0, (unsigned int*)&subThread.IdentificationNumber);
		ALAN_SUB_THREAD_V[ADVANCED_LAN_SUB_THREAD_DISCONNECT] = subThread;

		subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, WorkAdvancedLAN_Server, THIS, 0, (unsigned int*)&subThread.IdentificationNumber);
		ALAN_SUB_THREAD_V[ADVANCED_LAN_SUB_THREAD_WORK1] = subThread;

		subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, WorkAdvancedLAN_Server, THIS, 0, (unsigned int*)&subThread.IdentificationNumber);
		ALAN_SUB_THREAD_V[ADVANCED_LAN_SUB_THREAD_WORK2] = subThread;

		subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, WorkAdvancedLAN_Server, THIS, 0, (unsigned int*)&subThread.IdentificationNumber);
		ALAN_SUB_THREAD_V[ADVANCED_LAN_SUB_THREAD_WORK3] = subThread;

		subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, WorkAdvancedLAN_Server, THIS, 0, (unsigned int*)&subThread.IdentificationNumber);
		ALAN_SUB_THREAD_V[ADVANCED_LAN_SUB_THREAD_WORK4] = subThread;
	}

	InitializeSRWLock(&ALAN_CSAM_LOCK);

	return 0;
}

bool BaseLAN_Server::Terminalize()
{
	HANDLE subThreadHandle[ADVANCED_LAN_SUB_THREAD_COUNT];

	ALAN_S_SHUTDOWN = 1;

	closesocket(ALAN_SS.Socket);

	//uncorrected;
	FOR(i, TOTAL_ADVANCED_LAN_CLIENT_COUNT)
	{
		if (ALAN_CSV[i].Connected)
		{
			ALAN_CSV[i].CurrentPhaseIndex = ADVANCED_LAN_CLIENT_PHASE_DISCONNECT;
			ALAN_CSV[i].Connected = 0;
			ALAN_CSV[i].Sendable = 0;

			PostQueuedCompletionStatus(ALAN_S_COMMON_CP, NULL, i, &ALAN_CSV[i].OverlappedReceive); //???
			PostQueuedCompletionStatus(ALAN_S_COMMON_CP, NULL, i, &ALAN_CSV[i].OverlappedSend); //???
		}
	}
	//uncorrected;

	ALAN_CSV.clear();

	FOR(i, ADVANCED_LAN_SUB_THREAD_COUNT)
	{
		subThreadHandle[i] = ALAN_SUB_THREAD_V[i].Handle;
	}

	WaitForMultipleObjects(ALAN_SUB_THREAD_V.size(), subThreadHandle, 1, INFINITE);

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

	ALAN_SUB_THREAD_V.clear();

	timeEndPeriod(1);

	TIMER.Termimalize();

	WSACleanup();

	return 0;
}

unsigned int BaseLAN_Server::ReceivePacket(UINT64 sessionKey, SerialPacketQueue<AdvancedLAN_PacketHeader>& receivedPacketSPQ)
{
	AdvancedLAN_PacketHeader receivedPacketHeader;
	UINT currentALAN_CS_Index;
	UINT result;

	currentALAN_CS_Index = sessionKey >> ADVANCED_LAN_CLIENT_INDEX_SHIFTED_BIT_COUNT;
	
	result = ALAN_CSV[currentALAN_CS_Index].ReceiveBQ.Peek((char*)&receivedPacketHeader, sizeof(receivedPacketHeader));
	if (sizeof(receivedPacketHeader) == result)
	{
		//ALCSV[currentALCS_Index].ReceiveBQ.Dequeue((char*)&receivedPacketHeader, sizeof(receivedPacketHeader));
		//receivedPacketSPQ.SetHeader(receivedPacketHeader);

		result = ALAN_CSV[currentALAN_CS_Index].ReceiveBQ.Peek(receivedPacketSPQ.GetQueueAddress(), sizeof(receivedPacketHeader) + receivedPacketHeader.BodySize);
		if (sizeof(receivedPacketHeader) + receivedPacketHeader.BodySize == result)
		{
			receivedPacketSPQ.IncreaseCurrentSize(receivedPacketHeader.BodySize);

			ALAN_CSV[currentALAN_CS_Index].ReceiveBQ.Dequeue(receivedPacketSPQ.GetQueueAddress(), sizeof(receivedPacketHeader) + receivedPacketHeader.BodySize);
		}
	}

	return result;
}

unsigned int BaseLAN_Server::ReceivePacket(UINT64 sessionKey, SerialPacketQueue<AdvancedLAN_PacketHeader>* receivedPacketSPQ_Address)
{
	return 0;
}

unsigned int BaseLAN_Server::SendPacket(UINT64 sessionKey, SerialPacketQueue<AdvancedLAN_PacketHeader>& sendingPacketSPQ)
{
	AdvancedLAN_PacketHeader sendingPacketHeader;
	UINT currentALAN_CS_Index;
	UINT result;

	currentALAN_CS_Index = sessionKey >> ADVANCED_LAN_CLIENT_INDEX_SHIFTED_BIT_COUNT;
	//check invalid session
	result = sendingPacketSPQ.GetHeaderSize();
	if (sizeof(sendingPacketHeader) == result)
	{
		sendingPacketHeader = sendingPacketSPQ.GetHeader();

		result = ALAN_CSV[currentALAN_CS_Index].SendBQ.Enqueue(sendingPacketSPQ.GetQueueAddress(), sendingPacketSPQ.GetTotalSize());
		if (sizeof(sendingPacketHeader) + sendingPacketHeader.BodySize == result)
		{
			sendingPacketSPQ.DecreaseCurrentSize(sendingPacketHeader.BodySize);
		}
	}

	if (ALAN_CSV[currentALAN_CS_Index].SendBQ.GetCurrentSize())
	{
		ALAN_CSV[currentALAN_CS_Index].CurrentPhaseIndex = ADVANCED_LAN_CLIENT_PHASE_SEND;
		ALAN_CSV[currentALAN_CS_Index].Sendable = 1;

		PostQueuedCompletionStatus(ALAN_S_COMMON_CP, NULL, currentALAN_CS_Index, &ALAN_CSV[currentALAN_CS_Index].OverlappedSend);
	}
	
	return result;
}

unsigned int BaseLAN_Server::SendPacket(UINT64 sessionKey, SerialPacketQueue<AdvancedLAN_PacketHeader>* sendingPacketSPQ_Address)
{
	WSABUF sendingPacketWSABUF;
	AdvancedLAN_PacketHeader sendingPacketHeader;
	UINT currentALAN_CS_Index;
	UINT result;

	currentALAN_CS_Index = sessionKey >> ADVANCED_LAN_CLIENT_INDEX_SHIFTED_BIT_COUNT;

	result = sendingPacketSPQ_Address->GetHeaderSize();
	if (sizeof(sendingPacketHeader) == result)
	{
		sendingPacketHeader = sendingPacketSPQ_Address->GetHeader();

		result = sendingPacketSPQ_Address->GetTotalSize();
		if (sizeof(sendingPacketHeader) + sendingPacketHeader.BodySize == result)
		{
			//ALAN_CSV[currentALAN_CS_Index].SendQ.Enqueue(sendingPacketSPQ_Address);

			//result = ALAN_CSV[currentALAN_CS_Index].SendQ.Enqueue(sendingPacketSPQ_Address);
			//if (sizeof(sendingPacketHeader) + sendingPacketHeader.BodySize == result)
			if (sendingPacketSPQ_Address == ALAN_CSV[currentALAN_CS_Index].SendQ.Enqueue(sendingPacketSPQ_Address));
			else
			{
				result = 0;
			}
		}
	}

	if (ALAN_CSV[currentALAN_CS_Index].SendQ.GetCurrentCount())
	{
		//ALAN_CSV[currentALAN_CS_Index].CurrentPhaseIndex = ADVANCED_LAN_CLIENT_PHASE_SEND;
		//ALAN_CSV[currentALAN_CS_Index].Sendable = 1;

		//PostQueuedCompletionStatus(ALAN_S_COMMON_CP, NULL, currentALAN_CS_Index, &ALAN_CSV[currentALAN_CS_Index].OverlappedSend);

		{
			if (InterlockedExchange(&ALAN_CSV[currentALAN_CS_Index].Sendable, 0))
			{
				ZeroMemory(&ALAN_CSV[currentALAN_CS_Index].OverlappedSend, sizeof(ALAN_CSV[currentALAN_CS_Index].OverlappedSend));

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
					if(!currentSendingWSABUF_Count) InterlockedExchange(&ALAN_CSV[currentALAN_CS_Index].Sendable, 1);
					else
					{
						result = WSASend(ALAN_CSV[currentALAN_CS_Index].Socket, sendingWSABUF, currentSendingWSABUF_Count, nullptr, 0, &ALAN_CSV[currentALAN_CS_Index].OverlappedSend, nullptr);
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
							//THIS->Disconnect(ALCSV[clientIndex].SessionKey);
						}
					}
					//else LCSV[clientIndex].CurrentPhaseIndex = LAN_CLIENT_PHASE_RECEIVE;
				}
			}
		}
	}

	//HasOverlappedIoCompleted(IOCP_CHAT_CLIENT_V[i].OverlappedSend);
	//CancelIo(IOCP_CHAT_SERVER_IOCP);

	return result;
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
	UINT currentALCS_Index;
	
	currentALCS_Index = sessionKey >> ADVANCED_LAN_CLIENT_INDEX_SHIFTED_BIT_COUNT;
	
	ALAN_CSV[currentALCS_Index].CurrentPhaseIndex = ADVANCED_LAN_CLIENT_PHASE_DISCONNECT;
	
	PostQueuedCompletionStatus(ALAN_S_DISCONNECT_CP, NULL, currentALCS_Index, &ALAN_CSV[currentALCS_Index].OverlappedSend);

	return 0;
}