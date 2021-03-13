#include "pch.h"
#include "MMO.h"
#include "MMO_BaseClient.h"
#include "MMO_DerivedClient.h"
#include "MMO_BaseServer.h"

MMO_BaseServer::MMO_BaseServer()
{
	{
		IP_ADDRESS = L"127.0.0.1";
		PORT = 40000;
		WORK_THREAD_COUNT = 4;
		MAXIMUM_CLIENT_COUNT = MMO_CLIENT_COUNT;
	}

	{
		BCAV.resize(MAXIMUM_CLIENT_COUNT);
		SUB_THREAD_V.resize(WORK_THREAD_COUNT);

		{
			SS.Socket = INVALID_SOCKET;

			ZeroMemory(&SS.Address, sizeof(SS.Address));

			SS.Key = 0;
			SS.Index = 0;
			SS.Connected = 0;
			SS.Sendable = 0;
			SS.ReceiveCount = 0;
			SS.SendCount = 0;

			ZeroMemory(&SS.OverlappedReceive, sizeof(SS.OverlappedReceive));
			ZeroMemory(&SS.OverlappedSend, sizeof(SS.OverlappedSend));

			InitializeSRWLock(&SS.Lock);
		}

		{
			FOR(i, MAXIMUM_CLIENT_COUNT)
			{
				BCAV[i] = nullptr;
			}

			SUB_THREAD_ON = 1;
			CURRENT_ACCEPTION_COUNT = 0;

			DISCONNECT_CP = nullptr;

			SEND_CP = nullptr;

			WORK_CP = nullptr;
		}

		{
			//MMOW_TLP.Initialize();
			//PACKET_SPQ_TLP.Initialize();

			CURRENT_SPQ_COUNT = 0;
			ALLOCATED_SPQ_COUNT = 0;
			DEALLOCATED_SPQ_COUNT = 0;

			THREAD_CONTEXT_SWITCH_ON = 0;
			INDICATOR_ON = 0;
		}
	}
}

MMO_BaseServer::~MMO_BaseServer()
{

}

bool MMO_BaseServer::Initialize()
{
	{
		//BCAV.resize(MAXIMUM_CLIENT_COUNT);
		SUB_THREAD_V.resize(MMO_SERVER_SUB_THREAD_COUNT + WORK_THREAD_COUNT);

		{
			SS.Socket = INVALID_SOCKET;

			ZeroMemory(&SS.Address, sizeof(SS.Address));

			SS.Key = 0;
			SS.Index = 0;
			SS.Connected = 0;
			SS.Sendable = 0;
			SS.ReceiveCount = 0;
			SS.SendCount = 0;

			ZeroMemory(&SS.OverlappedReceive, sizeof(SS.OverlappedReceive));
			ZeroMemory(&SS.OverlappedSend, sizeof(SS.OverlappedSend));

			InitializeSRWLock(&SS.Lock);
		}

		{
			//FOR(i, MAXIMUM_CLIENT_COUNT)
			//{
			//	BCAV[i] = nullptr;
			//}

			SUB_THREAD_ON = 1;
			CURRENT_ACCEPTION_COUNT = 0;

			DISCONNECT_CP = nullptr;

			SEND_CP = nullptr;

			WORK_CP = nullptr;
		}

		{
			//MMOW_TLP.Initialize();
			//PACKET_SPQ_TLP.Initialize();

			CURRENT_SPQ_COUNT = 0;
			ALLOCATED_SPQ_COUNT = 0;
			DEALLOCATED_SPQ_COUNT = 0;

			CURRENT_SPQ_CHUNK_COUNT = 0;
			MAXIMUM_SPQ_CHUNK_COUNT = 0;

			THREAD_CONTEXT_SWITCH_ON = 0;
			INDICATOR_ON = 0;
		}
	}

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

		CHECK_ERROR(WSAStartup(0x0202, &wsaDatum));

		SS.Socket = WSASocket(PF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED);
		CHECK_INVALID_SOCKET(SS.Socket);

		//{
		//	lingeringTime.l_onoff = 1;
		//	lingeringTime.l_linger = 0;
		//	CHECK_SOCKET_ERROR(setsockopt(SS.Socket, SOL_SOCKET, SO_LINGER, (char*)&lingeringTime, sizeof(lingeringTime)));
		//}

		//{
		//	addressReuseOn = 1;
		//	CHECK_SOCKET_ERROR(setsockopt(SS.Socket, SOL_SOCKET, SO_REUSEADDR, (char*)&addressReuseOn, sizeof(addressReuseOn)));
		//}

		{
			keepAliveOn = 1;
			CHECK_SOCKET_ERROR(setsockopt(SS.Socket, SOL_SOCKET, SO_KEEPALIVE, (char*)&keepAliveOn, sizeof(keepAliveOn)));
		}

		{
			nagleAlgorithmOff = 1;
			CHECK_SOCKET_ERROR(setsockopt(SS.Socket, IPPROTO_TCP, TCP_NODELAY, (char*)&nagleAlgorithmOff, sizeof(nagleAlgorithmOff)));
		}

		//{
		//	socketReceiveBufferSize = 0;
		//	CHECK_SOCKET_ERROR(setsockopt(SS.Socket, SOL_SOCKET, SO_RCVBUF, (char*)&socketReceiveBufferSize, sizeof(socketReceiveBufferSize)));
		//}

		//{
		//	socketSendBufferSize = 0;
		//	CHECK_SOCKET_ERROR(setsockopt(SS.Socket, SOL_SOCKET, SO_SNDBUF, (char*)&socketSendBufferSize, sizeof(socketSendBufferSize)));
		//}

		//{
		//	nonblocked = 1;
		//	CHECK_SOCKET_ERROR(ioctlsocket(SS.Socket, FIONBIO, &nonblocked));
		//}

		SS.Address.sin_family = AF_INET;
		SS.Address.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
		//InetPton(AF_INET, ipAddress, &SS.Address.sin_addr); //inet_addr(INADDR_ANY); //InetNtop();
		//WSAAddressToString();
		//WSAStringToAddress();
		SS.Address.sin_port = htons(PORT);

		CHECK_SOCKET_ERROR(bind(SS.Socket, (SOCKADDR*)&SS.Address, sizeof(SS.Address)));
		CHECK_SOCKET_ERROR(listen(SS.Socket, SOMAXCONN));
	}

	{
		TIMER.Initialize();
		//CPU_PROFILER.Initialize();
		//PDH_PROFILER.Initialize();

		srand(time(nullptr));
		//srand(2705);
	}

	{
		SYSTEM_INFO systemInformation;
		IOCP_SubThread subThread;

		GetSystemInfo(&systemInformation);

		if (SUB_THREAD_V.size() == MMO_SERVER_SUB_THREAD_COUNT);
		else SUB_THREAD_V.resize(MMO_SERVER_SUB_THREAD_COUNT + systemInformation.dwNumberOfProcessors * 2);

		//ACCEPT_CP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, NULL, 1);
		DISCONNECT_CP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, NULL, 1);
		
		//RECEIVE_CP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, NULL, 1);
		//UPDATE_CP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, NULL, 1);
		SEND_CP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, NULL, 1);
		
		//AUTHORIZE_CP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, NULL, 1);
		//GAME_CP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, NULL, 1);

		WORK_CP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, NULL, SUB_THREAD_V.size() - MMO_SERVER_SUB_THREAD_COUNT);

		subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, AcceptThread, this, 0, &subThread.IdentificationNumber);
		SUB_THREAD_V[MMO_SERVER_SUB_THREAD_ACCEPT] = subThread;

		subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, DisconnectThread, this, 0, &subThread.IdentificationNumber);
		SUB_THREAD_V[MMO_SERVER_SUB_THREAD_DISCONNECT] = subThread;

		//subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, ReceiveThread, this, 0, &subThread.IdentificationNumber);
		//SUB_THREAD_V[MMO_SERVER_SUB_THREAD_RECEIVE] = subThread;
		
		//subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, UpdateThread, this, 0, &subThread.IdentificationNumber);
		//SUB_THREAD_V[MMO_SERVER_SUB_THREAD_UPDATE] = subThread;
		
		subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, SendThread, this, 0, &subThread.IdentificationNumber);
		SUB_THREAD_V[MMO_SERVER_SUB_THREAD_SEND] = subThread;

		subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, AuthorizeThread, this, 0, &subThread.IdentificationNumber);
		SUB_THREAD_V[MMO_SERVER_SUB_THREAD_SEND] = subThread;

		subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, GameThread, this, 0, &subThread.IdentificationNumber);
		SUB_THREAD_V[MMO_SERVER_SUB_THREAD_SEND] = subThread;

		FOR(i, SUB_THREAD_V.size() - MMO_SERVER_SUB_THREAD_COUNT)
		{
			subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, WorkThread, this, 0, &subThread.IdentificationNumber);
			SUB_THREAD_V[MMO_SERVER_SUB_THREAD_COUNT + i] = subThread;
		}
	}
}

bool MMO_BaseServer::Update()
{
	return 0;
}

bool MMO_BaseServer::Render()
{
	return 0;
}

bool MMO_BaseServer::Terminalize()
{
	{
		closesocket(SS.Socket); //close accept thread
	
		FOR(i, MAXIMUM_CLIENT_COUNT) //induct client disconnection
		{
			shutdown(BCAV[i]->CS.Socket, SD_BOTH);
		}
	
		FOR(i, MAXIMUM_CLIENT_COUNT) //confirm disconnection
		{
			if (BCAV[i]->CS.CurrentPhase != IOCP_CLIENT_PHASE_ACCEPT)
			{
				--i;
	
				Sleep(0);
			}
		}
		
		BCAV.clear();
	}

	{
		SUB_THREAD_ON = 0;

		PostQueuedCompletionStatus(SEND_CP, NULL, NULL, nullptr); //close send thread

		PostQueuedCompletionStatus(DISCONNECT_CP, NULL, NULL, nullptr); //close disconnect thread

		FOR(i, SUB_THREAD_V.size() - IOCP_SERVER_SUB_THREAD_COUNT) //close work thread
		{
			PostQueuedCompletionStatus(WORK_CP, NULL, NULL, nullptr);
		}
	}

	{
		HANDLE_V subThreadHandleV;

		subThreadHandleV.resize(SUB_THREAD_V.size());

		FOR(i, subThreadHandleV.size())
		{
			subThreadHandleV[i] = SUB_THREAD_V[i].Handle;
		}

		WaitForMultipleObjects(SUB_THREAD_V.size(), &subThreadHandleV[0], 1, INFINITE); //confirm thread closed

		//CloseHandle(ACCEPT_CP);
		CloseHandle(DISCONNECT_CP);

		//CloseHandle(RECEIVE_CP);
		//CloseHandle(UPDATE_CP);
		CloseHandle(SEND_CP);

		//CloseHandle(AUTHORIZE_CP);
		//CloseHandle(GAME_CP);

		CloseHandle(WORK_CP);

		FOR(i, SUB_THREAD_V.size())
		{
			CloseHandle(SUB_THREAD_V[i].Handle);
		}


		SUB_THREAD_V.clear();
	}

	{
		TIMER.Termimalize();
		//CPU_PROFILER.Terminalize();
		//PDH_PROFILER.Terminalize();

		SPQ_TLP.Terminalize();
	}

	{
		WSACleanup();
	}

	{
		printf(
			"CURRENT_SPQ_COUNT : %u" CRALF
			"ALLOCATED_SPQ_COUNT : %u" CRALF
			"DEALLOCATED_SPQ_COUNT : %u" CRALF
			CRALF
			"CURRENT_SPQ_CHUNK_COUNT : %u" CRALF
			"MAXIMUM_SPQ_CHUNK_COUNT : %u" CRALF
			,
			CURRENT_SPQ_COUNT,
			ALLOCATED_SPQ_COUNT,
			DEALLOCATED_SPQ_COUNT,

			CURRENT_SPQ_CHUNK_COUNT,
			MAXIMUM_SPQ_CHUNK_COUNT
			);
	}

	return 0;
}