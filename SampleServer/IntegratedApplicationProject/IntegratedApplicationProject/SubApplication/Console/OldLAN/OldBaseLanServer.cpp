#include "stdafx.h"
#include "Lan.h"
#include "BaseLanServer.h"

LanServerSession BaseLanServer::LSS;
LanClientSessionV BaseLanServer::LCSV;
LanClientSessionAddressM BaseLanServer:: LCSAM;
LanSubThreadV BaseLanServer::LAN_SUB_THREAD_V;

BOOL BaseLanServer::LS_SHUTDOWN;

UINT BaseLanServer::CURRENT_SESSION_KEY;

HANDLE BaseLanServer::LSCP;

SRWLOCK BaseLanServer::LCSAM_LOCK;

BaseLanServer* BaseLanServer::THIS;




BaseLanServer::BaseLanServer() {}
BaseLanServer::~BaseLanServer() {}

bool BaseLanServer::Initialize()
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
	LanSubThread subThread;

	ZeroMemory(&LSS, sizeof(LSS)); //warning, need to reinitialize
	LCSV.resize(TOTAL_LAN_CLIENT_COUNT);
	LAN_SUB_THREAD_V.resize(LAN_SUB_THREAD_COUNT);

	{
		CHECK_ERROR(WSAStartup(0x0202, &wsaDatum));

		LSS.Socket = WSASocket(PF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED);
		CHECK_INVALID_SOCKET(LSS.Socket);

		//lingeringTime.l_onoff = 1;
		//lingeringTime.l_linger = 0;
		//CHECK_SOCKET_ERROR(setsockopt(IOCP_CHAT_SERVER.Socket, SOL_SOCKET, SO_LINGER, (char*)&lingeringTime, sizeof(lingeringTime)));

		//reusable = 1;
		//CHECK_SOCKET_ERROR(setsockopt(IOCP_CHAT_SERVER.Socket, SOL_SOCKET, SO_REUSEADDR, (char*)&reusable, sizeof(reusable)));

		//keepAliveOn = 1;
		//CHECK_SOCKET_ERROR(setsockopt(IOCP_CHAT_SERVER.Socket, SOL_SOCKET, SO_KEEPALIVE, (char*)&keepAliveOn, sizeof(keepAliveOn)));

		nagleOff = 1;
		CHECK_SOCKET_ERROR(setsockopt(LSS.Socket, IPPROTO_TCP, TCP_NODELAY, (char*)&nagleOff, sizeof(nagleOff)));

		//socketReceiveBufferSize = 0;
		//CHECK_SOCKET_ERROR(setsockopt(IOCP_CHAT_SERVER.Socket, SOL_SOCKET, SO_RCVBUF, (char*)&socketReceiveBufferSize, sizeof(socketReceiveBufferSize)));

		socketSendBufferSize = 0;
		CHECK_SOCKET_ERROR(setsockopt(LSS.Socket, SOL_SOCKET, SO_SNDBUF, (char*)&socketSendBufferSize, sizeof(socketSendBufferSize)));

		//nonblocked = 1;
		//CHECK_SOCKET_ERROR(ioctlsocket(IOCP_CHAT_SERVER.Socket, FIONBIO, &nonblocked));

		LSS.Address.sin_family = AF_INET;
		LSS.Address.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
		//InetPton(AF_INET, ipAddress, &LSS.Address.sin_addr); //inet_addr(INADDR_ANY); //InetNtop();
		//WSAAddressToString();
		//WSAStringToAddress();
		LSS.Address.sin_port = htons(LAN_SERVER_PORT);
		CHECK_SOCKET_ERROR(bind(LSS.Socket, (SOCKADDR*)&LSS.Address, sizeof(LSS.Address)));
		CHECK_SOCKET_ERROR(listen(LSS.Socket, SOMAXCONN));
	}

	timeBeginPeriod(1);

	TIMER.Initialize();

	//srand(time(nullptr));
	srand(2705);

	GetSystemInfo(&systemInformation);

	LS_SHUTDOWN = 0;
	CURRENT_SESSION_KEY = 0xffffffff;
	THIS = this;

	{
		LSCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, LAN_SUB_THREAD_WORK4 - LAN_SUB_THREAD_WORK1 + 1);
		//LS_RECEIVE_CP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 1);
		//LS_UPDATE_CP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 1);
		//LS_SEND_CP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 1);
		//LS_DISCONNECT_CP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 1);

		subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, AcceptLanClient, THIS, 0, (unsigned int*)&subThread.IdentificationNumber);
		LAN_SUB_THREAD_V[LAN_SUB_THREAD_ACCEPT] = subThread;

		//subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, ReceiveIOCP_ChatClientPacket, nullptr, 0, (unsigned int*)&subThread.IdentificationNumber);
		//IOCP_CHAT_SUB_THREAD_V[IOCP_CHAT_SUB_THREAD_RECEIVE] = subThread;

		//subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, ReceiveIOCP_ChatClientPacket, nullptr, 0, (unsigned int*)&subThread.IdentificationNumber);
		//IOCP_CHAT_SUB_THREAD_V[IOCP_CHAT_SUB_THREAD_UPDATE] = subThread;

		//subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, SendIOCP_ChatServerPacket, nullptr, 0, (unsigned int*)&subThread.IdentificationNumber);
		//IOCP_CHAT_SUB_THREAD_V[IOCP_CHAT_SUB_THREAD_SEND] = subThread;

		//subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, DisconnectLanClient, nullptr, 0, (unsigned int*)&subThread.IdentificationNumber);
		//LAN_SUB_THREAD_V[LAN_SUB_THREAD_DISCONNECT] = subThread;

		subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, WorkLanServer, THIS, 0, (unsigned int*)&subThread.IdentificationNumber);
		LAN_SUB_THREAD_V[LAN_SUB_THREAD_WORK1] = subThread;

		subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, WorkLanServer, THIS, 0, (unsigned int*)&subThread.IdentificationNumber);
		LAN_SUB_THREAD_V[LAN_SUB_THREAD_WORK2] = subThread;

		subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, WorkLanServer, THIS, 0, (unsigned int*)&subThread.IdentificationNumber);
		LAN_SUB_THREAD_V[LAN_SUB_THREAD_WORK3] = subThread;

		subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, WorkLanServer, THIS, 0, (unsigned int*)&subThread.IdentificationNumber);
		LAN_SUB_THREAD_V[LAN_SUB_THREAD_WORK4] = subThread;
	}

	{
		FOR(i, TOTAL_LAN_CLIENT_COUNT)
		{
			LCSV[i].Socket = INVALID_SOCKET;
		
			ZeroMemory(&LCSV[i].Address, sizeof(LCSV[i].Address));
			ZeroMemory(&LCSV[i].OverlappedReceive, sizeof(LCSV[i].OverlappedReceive));
			ZeroMemory(&LCSV[i].OverlappedSend, sizeof(LCSV[i].OverlappedSend));
		
			LCSV[i].CurrentPhaseIndex = LAN_CLIENT_PHASE_ACCEPT;
		}
	}

	InitializeSRWLock(&LCSAM_LOCK);

	return 0;
}

bool BaseLanServer::Terminalize() //uncorrected
{
	HANDLE subThreadHandle[LAN_SUB_THREAD_COUNT];
	UINT clientCount;

	LS_SHUTDOWN = 1;

	closesocket(LSS.Socket);

	clientCount = LCSV.size();
	
	//<<<<<<<<<<<<<<<<<<uncorrected code
	AcquireSRWLockExclusive(&LCSAM_LOCK);
	for (LanClientSessionAddressMI lanClientSessionAddressMI = LCSAM.begin(); lanClientSessionAddressMI != LCSAM.end();)
	{
		LanClientSession* lanClientSessionAddress;

		lanClientSessionAddress = lanClientSessionAddressMI->second;

		if (lanClientSessionAddress->Connected && lanClientSessionAddress->CurrentPhaseIndex == LAN_CLIENT_PHASE_UPDATE)
		{
			lanClientSessionAddress->CurrentPhaseIndex = LAN_CLIENT_PHASE_DISCONNECT;
			lanClientSessionAddress->Connected = 0;

			//uncorrected

			PostQueuedCompletionStatus(LSCP, NULL, lanClientSessionAddress->SessionKey, &lanClientSessionAddress->OverlappedReceive);
			PostQueuedCompletionStatus(LSCP, NULL, lanClientSessionAddress->SessionKey, &lanClientSessionAddress->OverlappedSend);

			LCSAM.erase(lanClientSessionAddressMI);
		}
	}
	ReleaseSRWLockExclusive(&LCSAM_LOCK);
	//>>>>>>>>>>>>>>>>>uncorrected code

	FOR(i, LAN_SUB_THREAD_COUNT)
	{
		subThreadHandle[i] = LAN_SUB_THREAD_V[i].Handle;
	}

	WaitForMultipleObjects(LAN_SUB_THREAD_V.size(), subThreadHandle, 1, INFINITE);


	//<<<<<<<<<<<<<<<<<<<<<uncorrected code
	FOR(i, clientCount)
	{
		if (LCSV[i].Connected)
		{
			LCSV[i].CurrentPhaseIndex = LAN_CLIENT_PHASE_DISCONNECT;
			LCSV[i].Connected = 0;

			//shutdown(LCSV[i].Socket, SD_BOTH);
			closesocket(LCSV[i].Socket);
		}
	}
	//>>>>>>>>>>>>>>>>>>>>uncorrected code


	CloseHandle(LSCP);

	FOR(i, LAN_SUB_THREAD_V.size())
	{
		CloseHandle(LAN_SUB_THREAD_V[i].Handle);
	}

	timeEndPeriod(1);

	TIMER.Termimalize();

	WSACleanup();

	return 0;
}

bool BaseLanServer::Start(LPCWSTR ipAddress, unsigned int port, unsigned int workThreadCount, bool nagleOff, unsigned int maximumLanClientCount)
{
	Initialize();

	return 0;
}

void BaseLanServer::Stop()
{
	Terminalize();
}



bool BaseLanServer::Disconnect(unsigned int sessionKey)
{
	LanClientSessionAddressMI lanClientSessionAddressMI;
	LanClientSession* lanClientSessionAddress;

	lanClientSessionAddress = nullptr;

	AcquireSRWLockExclusive(&LCSAM_LOCK);
	lanClientSessionAddressMI = LCSAM.find(sessionKey);
	if (lanClientSessionAddressMI == LCSAM.end())
	{
		//printf("[Disconnect] Session %d is deleted aleady." CRALF, sessionKey);
	}
	else
	{
		lanClientSessionAddress = lanClientSessionAddressMI->second;

		LCSAM.erase(lanClientSessionAddressMI);
	}
	ReleaseSRWLockExclusive(&LCSAM_LOCK);

	if (!lanClientSessionAddress) return 1;

	if (lanClientSessionAddress->SessionKey != sessionKey)
	{
		printf("Session %d is reused aleady." CRALF, sessionKey);
	}

	if (lanClientSessionAddress->CurrentPhaseIndex == LAN_CLIENT_PHASE_DISCONNECT)
	{
		//shutdown(lanClientSessionAddress->Socket, SD_BOTH);

		if (closesocket(lanClientSessionAddress->Socket) == SOCKET_ERROR)
		{
			printf("closesocket error : %d", WSAGetLastError());
		}
		
		//세션이 재사용 될 때 재사용되는 소켓이 뒤늦게 닫힐 수 있다
		//TIME_WAIT은 로컬 아이피와 포트, 리모트 아이피와 포트간의 상태. 같은 아이피와 포트로 재접속을 시도하면 TIME_WAIT 상태라 실패함
		//상대방이 FIN 에 대한 ACK를 주지 않으면 계속 리시브를 대기한다

		//삭제할 세션과 생성할 세션을 따로 모아서 일괄처리하는 방식도 가능

		lanClientSessionAddress->Socket = INVALID_SOCKET;
		lanClientSessionAddress->Address;
		lanClientSessionAddress->Connected = 0;
		lanClientSessionAddress->Sendable = 0;
		lanClientSessionAddress->ReceiveCount = 0;
		lanClientSessionAddress->SendCount = 0;

		lanClientSessionAddress->SessionKey = 0xffffffff;

		lanClientSessionAddress->OverlappedReceive;
		lanClientSessionAddress->OverlappedSend;

		lanClientSessionAddress->ReceiveBQ.Initialize();
		lanClientSessionAddress->SendBQ.Initialize();

		lanClientSessionAddress->IO_Count = 0;

		lanClientSessionAddress->CurrentPhaseIndex = LAN_CLIENT_PHASE_ACCEPT;

		THIS->OnClientLeave(sessionKey);
	}
	
	return 0;
}

bool BaseLanServer::ReceivePacket(unsigned int sessionKey, SerialByteQueue& receivedPacketSBQ)
{
	LanClientSessionAddressMI lanClientSessionAddressMI;
	LanClientSession* lanClientSessionAddress;

	WORD receivedPacketHeader;
	//SerialByteQueue receivedPacketSBQ;
	int result;

	lanClientSessionAddress = nullptr;

	AcquireSRWLockShared(&LCSAM_LOCK);
	lanClientSessionAddressMI = LCSAM.find(sessionKey);
	if (lanClientSessionAddressMI == LCSAM.end())
	{
		//printf("[ReceivePacket] Session %d is deleted aleady." CRALF, sessionKey);
	}
	else lanClientSessionAddress = lanClientSessionAddressMI->second;
	ReleaseSRWLockShared(&LCSAM_LOCK);

	if (!lanClientSessionAddress) return 1;

	if (lanClientSessionAddress->SessionKey != sessionKey)
	{
		//printf("Session %d is reused aleady." CRALF, sessionKey);
		return 1;
	}

	if (lanClientSessionAddress->Connected && lanClientSessionAddress->CurrentPhaseIndex == LAN_CLIENT_PHASE_UPDATE) //LAN_CLIENT_PHASE_RECEIVE
	{
		result = lanClientSessionAddress->ReceiveBQ.Peek((char*)&receivedPacketHeader, sizeof(receivedPacketHeader));
		if (sizeof(receivedPacketHeader) == result)
		{
			result = lanClientSessionAddress->ReceiveBQ.Peek(receivedPacketSBQ.GetQueueAddress(), sizeof(receivedPacketHeader) + receivedPacketHeader);
			if (sizeof(receivedPacketHeader) + receivedPacketHeader == result)
			{
				receivedPacketSBQ.IncreaseCurrentSize(receivedPacketHeader);

				//lanClientSessionAddress->ReceiveBQ.DecreaseCurrentSize(sizeof(receivedPacketHeader) + receivedPacketHeader);

				THIS->OnRecv(sessionKey, receivedPacketSBQ);

				return 0;
			}
		}
	}

	return 1;
}

bool BaseLanServer::SendPacket(unsigned int sessionKey, SerialByteQueue& sendingPacketSBQ)
{
	LanClientSessionAddressMI lanClientSessionAddressMI;
	LanClientSession* lanClientSessionAddress;

	WORD sendingPacketHeader;
	int result;

	//1. Lock LCSUM 
	//2. Find Session
	//3. Unlock LCSUM
	//4. Lock Session
	//5. Unlock Session

	lanClientSessionAddress = nullptr;

	AcquireSRWLockShared(&LCSAM_LOCK);
	lanClientSessionAddressMI = LCSAM.find(sessionKey);
	if (lanClientSessionAddressMI == LCSAM.end())
	{
		//printf("[SendPacket] Session %d is deleted aleady." CRALF, sessionKey);
	}
	else lanClientSessionAddress = lanClientSessionAddressMI->second;
	ReleaseSRWLockShared(&LCSAM_LOCK);

	if (!lanClientSessionAddress) return 1;

	if (lanClientSessionAddress->SessionKey != sessionKey)
	{
		printf("Session %d is reused aleady." CRALF, sessionKey);
	}

	result = sendingPacketSBQ.GetHeaderSize();
	if (sizeof(sendingPacketHeader) == result)
	{
		sendingPacketHeader = sendingPacketSBQ.GetHeader(sendingPacketHeader);

		result = lanClientSessionAddress->SendBQ.Enqueue(sendingPacketSBQ.GetQueueAddress(), sendingPacketSBQ.GetTotalSize());
		if (sizeof(sendingPacketHeader) + sendingPacketHeader == result)
		{
			sendingPacketSBQ.DecreaseCurrentSize(sendingPacketHeader);

			lanClientSessionAddress->ReceiveBQ.DecreaseCurrentSize(sizeof(sendingPacketHeader) + sendingPacketHeader); //ugly code
		}
	}
	
	if (lanClientSessionAddress->Connected && lanClientSessionAddress->CurrentPhaseIndex == LAN_CLIENT_PHASE_UPDATE) //LAN_CLIENT_PHASE_SEND
	{
		if (lanClientSessionAddress->SendBQ.GetCurrentSize())
		{
			lanClientSessionAddress->CurrentPhaseIndex = LAN_CLIENT_PHASE_SEND;
			lanClientSessionAddress->Sendable = 1;
		
			PostQueuedCompletionStatus(LSCP, NULL, sessionKey, &lanClientSessionAddress->OverlappedSend);

			THIS->OnSend(sessionKey, result); //sizeof(sendingPacketHeader) + sendingPacketHeader

			return 0;
		}
	}

	return 1;
}

unsigned int WINAPI BaseLanServer::AcceptLanClient(LPVOID argument)
{
	LanClientSession lanClientSession;
	int addressLength;

	addressLength = sizeof(lanClientSession);

	THIS = (BaseLanServer*)argument;

	while (!LS_SHUTDOWN)
	{
		lanClientSession.Socket = WSAAccept(LSS.Socket, (SOCKADDR*)&lanClientSession.Address, &addressLength, nullptr, NULL);
		CHECK_INVALID_SOCKET(lanClientSession.Socket)
		//IF_INVALID_SOCKET(iocpChatClient.Socket)
		//ELSE
		{
			THIS->OnConnectionRequest(lanClientSession.Address, lanClientSession.Address.sin_port);

			FOR(i, TOTAL_LAN_CLIENT_COUNT)
			{
				if (LCSV[i].CurrentPhaseIndex == LAN_CLIENT_PHASE_ACCEPT)
				{
					IF_NULL(CreateIoCompletionPort((HANDLE)lanClientSession.Socket, LSCP, ++CURRENT_SESSION_KEY, LAN_SUB_THREAD_WORK4 - LAN_SUB_THREAD_WORK1 + 1))
					ELSE
					{
						LCSV[i].Socket = lanClientSession.Socket;
						LCSV[i].Address = lanClientSession.Address;
						LCSV[i].Connected = 1;
						LCSV[i].Sendable = 1;
						LCSV[i].ReceiveCount = 0;
						LCSV[i].SendCount = 0;
						LCSV[i].SessionKey = CURRENT_SESSION_KEY;

						ZeroMemory(&LCSV[i].OverlappedReceive, sizeof(LCSV[i].OverlappedReceive));
						ZeroMemory(&LCSV[i].OverlappedSend, sizeof(LCSV[i].OverlappedSend));

						LCSV[i].CurrentPhaseIndex = LAN_CLIENT_PHASE_UPDATE;

						InterlockedIncrement(&LCSV[i].IO_Count);
						++LCSV[i].ReceiveCount;

						InitializeSRWLock(&LCSV[i].LanClientLock);



						AcquireSRWLockExclusive(&LCSAM_LOCK);
						LCSAM[LCSV[i].SessionKey] = &LCSV[i];
						ReleaseSRWLockExclusive(&LCSAM_LOCK);



						THIS->OnClientJoin(LCSV[i].Address, LCSV[i].SessionKey);

						PostQueuedCompletionStatus(LSCP, NULL, LCSV[i].SessionKey, &LCSV[i].OverlappedReceive);

						break;
					}
				}

				if (i == TOTAL_LAN_CLIENT_COUNT) closesocket(lanClientSession.Socket);
			}
		}
	}

	return 0;
}

unsigned int WINAPI BaseLanServer::WorkLanServer(LPVOID argument)
{
	DWORD transferredSize;
	ULONG_PTR completionKey;
	OVERLAPPED* overlapped;
	UINT sessionKey;
	char buffer[BUFFER_SIZE];
	DWORD flag;
	int result;

	THIS = (BaseLanServer*)argument;
	
	while (!LS_SHUTDOWN)
	{
		transferredSize = 0;
		completionKey = 0;
		overlapped = nullptr;
		sessionKey = 0xffffffff;
		flag = 0;

		THIS->OnWorkerThreadBegin();

		if (GetQueuedCompletionStatus(LSCP, &transferredSize, &completionKey, &overlapped, INFINITE))
		{
			if (overlapped)
			{
				LanClientSessionAddressMI lanClientSessionAddressMI;
				LanClientSession* lanClientSessionAddress;

				sessionKey = completionKey;

				lanClientSessionAddress = nullptr;

				AcquireSRWLockShared(&LCSAM_LOCK);
				lanClientSessionAddressMI = LCSAM.find(sessionKey);
				if (lanClientSessionAddressMI == LCSAM.end())
				{
					//printf("[WorkLanServer GQCS true] Session %d is deleted aleady." CRALF, sessionKey);
				}
				else lanClientSessionAddress = lanClientSessionAddressMI->second;
				ReleaseSRWLockShared(&LCSAM_LOCK);

				if (!lanClientSessionAddress) continue;

				if (lanClientSessionAddress->SessionKey != sessionKey)
				{
					printf("Session %d is reused aleady." CRALF, sessionKey);
				}

				if (&lanClientSessionAddress->OverlappedReceive == overlapped)
				{
					//rcvPost
					{
						ZeroMemory(&lanClientSessionAddress->OverlappedReceive, sizeof(lanClientSessionAddress->OverlappedReceive));

						if (0 < transferredSize) lanClientSessionAddress->ReceiveBQ.IncreaseCurrentSize(transferredSize);

						InterlockedIncrement(&lanClientSessionAddress->IO_Count);
						++lanClientSessionAddress->ReceiveCount;

						result = lanClientSessionAddress->ReceiveBQ.WSAReceive(WSARecv, lanClientSessionAddress->Socket, &flag, &lanClientSessionAddress->OverlappedReceive);
						if (result == SOCKET_ERROR)
						{
							if (WSAGetLastError() == WSA_IO_PENDING);
							else
							{
								InterlockedDecrement(&lanClientSessionAddress->IO_Count);
								--lanClientSessionAddress->ReceiveCount;

								printf("[WSARecv] sessionKey : %d, %d" CRALF, sessionKey, WSAGetLastError());
							}
						}

						//if(0 < --LCSV[clientIndex].ReceiveCount && 0 < LCSV[clientIndex].SendCount)
						//if (!InterlockedDecrement(&lanClientSessionAddress->IO_Count))
						//{
						//	lanClientSessionAddress->CurrentPhaseIndex = LAN_CLIENT_PHASE_DISCONNECT;
						//
						//	THIS->Disconnect(lanClientSessionAddress->SessionKey); //PostQueuedCompletionStatus(LS_DISCONNECT_CP, NULL, clientIndex, &LCSV[clientIndex].OverlappedReceive);
						//}
						//else LCSV[clientIndex].CurrentPhaseIndex = LAN_CLIENT_PHASE_UPDATE;
					}
				}
				else
				{
					//sndPost
					{
						if (InterlockedExchange((unsigned int*)&lanClientSessionAddress->Sendable, 0))
						{
							ZeroMemory(&lanClientSessionAddress->OverlappedSend, sizeof(lanClientSessionAddress->OverlappedSend));

							if (0 < transferredSize)
							{
								lanClientSessionAddress->SendBQ.DecreaseCurrentSize(transferredSize);
								lanClientSessionAddress->CurrentPhaseIndex = LAN_CLIENT_PHASE_UPDATE;
							}
							else
							{
								lanClientSessionAddress->Sendable = 1;

								InterlockedIncrement(&lanClientSessionAddress->IO_Count);
								++lanClientSessionAddress->SendCount;

								result = lanClientSessionAddress->SendBQ.WSASend(WSASend, lanClientSessionAddress->Socket, flag, &lanClientSessionAddress->OverlappedSend);
								if (result == SOCKET_ERROR)
								{
									if (WSAGetLastError() == WSA_IO_PENDING);
									else
									{
										InterlockedDecrement(&lanClientSessionAddress->IO_Count);
										--lanClientSessionAddress->SendCount;

										printf("[WSASend] sessionKey : %d, %d" CRALF, sessionKey, WSAGetLastError());
									}
								}

								//if(0 < --LCSV[clientIndex].SendCount && 0 < LCSV[clientIndex].ReceiveCount)
								//if (!InterlockedDecrement(&lanClientSessionAddress->IO_Count))
								//{
								//	lanClientSessionAddress->CurrentPhaseIndex = LAN_CLIENT_PHASE_DISCONNECT;
								//
								//	THIS->Disconnect(lanClientSessionAddress->SessionKey); //PostQueuedCompletionStatus(LS_DISCONNECT_CP, NULL, clientIndex, &LCSV[clientIndex].OverlappedSend);
								//}
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
				LanClientSessionAddressMI lanClientSessionAddressMI;
				LanClientSession* lanClientSessionAddress;

				sessionKey = completionKey;

				lanClientSessionAddress = nullptr;

				AcquireSRWLockShared(&LCSAM_LOCK);
				lanClientSessionAddressMI = LCSAM.find(sessionKey);
				if (lanClientSessionAddressMI == LCSAM.end())
				{
					//printf("[WorkLanServer GQCS false] Session %d is deleted aleady." CRALF, sessionKey);
				}
				else lanClientSessionAddress = lanClientSessionAddressMI->second;
				ReleaseSRWLockShared(&LCSAM_LOCK);

				if (!lanClientSessionAddress) continue;

				if (lanClientSessionAddress->SessionKey != sessionKey)
				{
					printf("Session %d is reused aleady." CRALF, sessionKey);
				}
			
				lanClientSessionAddress->CurrentPhaseIndex = LAN_CLIENT_PHASE_DISCONNECT;
			
				THIS->Disconnect(lanClientSessionAddress->SessionKey);
			}
			else
			{
				if (GetLastError() == ERROR_ABANDONED_WAIT_0) break;
			}
		}

		THIS->OnWorkerThreadEnd();
	}

	return 0;
}