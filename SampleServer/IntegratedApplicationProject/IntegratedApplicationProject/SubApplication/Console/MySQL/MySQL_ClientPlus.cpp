#include "stdafx.h"
#include "MySQL_Client.h"

bool InitializeMySQL_Client()
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
	MySQL_SubThread subThread;

	LPCWSTR serverIP_Address;

	const char* mysqlServerIP_Address;
	const char* mysqlUserName;
	const char* mysqlPassword;
	const char* mysqlDatabaseName;

	//ZeroMemory(&MY_SQL_SERVER, sizeof(MY_SQL_SERVER));
	//ZeroMemory(&MY_SQL_CLIENT, sizeof(MY_SQL_CLIENT));
	MY_SQL_CLIENT_SUB_THREAD_V.resize(MY_SQL_CLIENT_SUB_THREAD_COUNT);

	{
		CHECK_ERROR(WSAStartup(0x0202, &wsaDatum));

		MY_SQL_CLIENT.Socket = WSASocket(PF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED);
		CHECK_INVALID_SOCKET(MY_SQL_CLIENT.Socket);

		//lingeringTime.l_onoff = 1;
		//lingeringTime.l_linger = 0;
		//CHECK_SOCKET_ERROR(setsockopt(MY_SQL_CLIENT.Socket, SOL_SOCKET, SO_LINGER, (char*)&lingeringTime, sizeof(lingeringTime)));

		//reusable = 1;
		//CHECK_SOCKET_ERROR(setsockopt(MY_SQL_CLIENT.Socket, SOL_SOCKET, SO_REUSEADDR, (char*)&reusable, sizeof(reusable)));

		//keepAliveOn = 1;
		//CHECK_SOCKET_ERROR(setsockopt(MY_SQL_CLIENT.Socket, SOL_SOCKET, SO_KEEPALIVE, (char*)&keepAliveOn, sizeof(keepAliveOn)));

		nagleOff = 1;
		CHECK_SOCKET_ERROR(setsockopt(MY_SQL_CLIENT.Socket, IPPROTO_TCP, TCP_NODELAY, (char*)&nagleOff, sizeof(nagleOff)));

		//socketReceiveBufferSize = 0;
		//CHECK_SOCKET_ERROR(setsockopt(MY_SQL_CLIENT.Socket, SOL_SOCKET, SO_RCVBUF, (char*)&socketReceiveBufferSize, sizeof(socketReceiveBufferSize)));

		socketSendBufferSize = 0;
		CHECK_SOCKET_ERROR(setsockopt(MY_SQL_CLIENT.Socket, SOL_SOCKET, SO_SNDBUF, (char*)&socketSendBufferSize, sizeof(socketSendBufferSize)));

		//nonblocked = 1;
		//CHECK_SOCKET_ERROR(ioctlsocket(MY_SQL_CLIENT.Socket, FIONBIO, &nonblocked));



		serverIP_Address = L"127.0.0.1";

		MY_SQL_SERVER.Address.sin_family = AF_INET;
		//MY_SQL_SERVER.Address.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
		InetPton(AF_INET, serverIP_Address, &MY_SQL_SERVER.Address.sin_addr); //inet_addr(INADDR_ANY); //InetNtop();
		//WSAAddressToString();
		//WSAStringToAddress();
		MY_SQL_SERVER.Address.sin_port = htons(MY_SQL_SERVER_PORT);



		mysqlServerIP_Address = "127.0.0.1";
		mysqlUserName = "guest";
		//mysqlUserName = "phpAdministrator";
		mysqlPassword = nullptr;
		//mysqlPassword = "2705";
		mysqlDatabaseName = "homework";

		MY_SQL_SERVER.MySQL_ServerIP_Address = mysqlServerIP_Address;
		MY_SQL_SERVER.MySQL_UserName = mysqlUserName;
		MY_SQL_SERVER.MySQL_Password = mysqlPassword;
		MY_SQL_SERVER.MySQL_DatabaseName = mysqlDatabaseName;
	}

	timeBeginPeriod(1);

	TIMER.Initialize();

	//srand(time(nullptr));
	srand(2705);

	GetSystemInfo(&systemInformation);

	MY_SQL_CLIENT_SHUTDOWN = 0;

	{
		//CHECK_SOCKET_ERROR(connect(MY_SQL_CLIENT.Socket, (SOCKADDR*)&MY_SQL_SERVER.Address, sizeof(MY_SQL_SERVER.Address)))

		mysql_init(&MY_SQL_CLIENT.MySQL);

		//MY_SQL_SERVER.MySQL_Address = mysql_real_connect(&MY_SQL_SERVER.MySQL, mysqlServerIP_Address, mysqlUserName, mysqlPassword, mysqlDatabaseName, MY_SQL_SERVER_PORT, nullptr, NULL))
		if (!mysql_real_connect(&MY_SQL_CLIENT.MySQL, mysqlServerIP_Address, mysqlUserName, mysqlPassword, mysqlDatabaseName, MY_SQL_SERVER_PORT, nullptr, NULL))
		{
			printf("Mysql connection error : %s", mysql_error(&MY_SQL_CLIENT.MySQL));

			return 1;
		}

		mysql_set_character_set(&MY_SQL_CLIENT.MySQL, "utf8");

		mysql_query(&MY_SQL_CLIENT.MySQL, "DELETE FROM user_information");
		mysql_query(&MY_SQL_CLIENT.MySQL, "DELETE FROM user_stage");
		mysql_query(&MY_SQL_CLIENT.MySQL, "DELETE FROM user_level");

		MY_SQL_CLIENT.CurrentStepIndex = MY_SQL_CLIENT_PHASE_CONNECT;
		MY_SQL_CLIENT.QueryEnqueuing = 0;
		MY_SQL_CLIENT.TransactionPerSecond = 0;
	}

	{
		MY_SQL_CLIENT_IOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, MY_SQL_CLIENT_SUB_THREAD_WORK4 - MY_SQL_CLIENT_SUB_THREAD_WORK1 + 1);
		MY_SQL_CLIENT_RECEIVE_CP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 1);
		MY_SQL_CLIENT_UPDATE_CP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 1);
		MY_SQL_CLIENT_SEND_CP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 1);

		//subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, ConnectToMySQL_Server, nullptr, 0, (unsigned int*)&subThread.IdentificationNumber);
		//MY_SQL_SUB_THREAD_V[MY_SQL_CLIENT_SUB_THREAD_CONNECT] = subThread;

		subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, ReceiveMySQL_ServerPacket, nullptr, 0, (unsigned int*)&subThread.IdentificationNumber);
		MY_SQL_CLIENT_SUB_THREAD_V[MY_SQL_CLIENT_SUB_THREAD_RECEIVE] = subThread;

		//subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, UpdateMySQL_Client, nullptr, 0, (unsigned int*)&subThread.IdentificationNumber);
		//MY_SQL_SUB_THREAD_V[IOCP_CHAT_SUB_THREAD_UPDATE] = subThread;

		subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, SendMySQL_ClientPacket, nullptr, 0, (unsigned int*)&subThread.IdentificationNumber);
		MY_SQL_CLIENT_SUB_THREAD_V[MY_SQL_CLIENT_SUB_THREAD_SEND] = subThread;

		//subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, DisconnectFromMySQL_Server, nullptr, 0, (unsigned int*)&subThread.IdentificationNumber);
		//MY_SQL_SUB_THREAD_V[MY_SQL_CLIENT_SUB_THREAD_DISCONNECT] = subThread;

		subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, WorkMySQL_Client, nullptr, 0, (unsigned int*)&subThread.IdentificationNumber);
		MY_SQL_CLIENT_SUB_THREAD_V[MY_SQL_CLIENT_SUB_THREAD_WORK1] = subThread;

		subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, WorkMySQL_Client, nullptr, 0, (unsigned int*)&subThread.IdentificationNumber);
		MY_SQL_CLIENT_SUB_THREAD_V[MY_SQL_CLIENT_SUB_THREAD_WORK2] = subThread;

		subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, WorkMySQL_Client, nullptr, 0, (unsigned int*)&subThread.IdentificationNumber);
		MY_SQL_CLIENT_SUB_THREAD_V[MY_SQL_CLIENT_SUB_THREAD_WORK3] = subThread;

		subThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, WorkMySQL_Client, nullptr, 0, (unsigned int*)&subThread.IdentificationNumber);
		MY_SQL_CLIENT_SUB_THREAD_V[MY_SQL_CLIENT_SUB_THREAD_WORK4] = subThread;
	}

	return 0;
}

bool UpdateMySQL_Client()
{
	if (GetAsyncKeyState('Q')) return 1;

	Sleep(1000);

	return 0;
}

bool RenderMySQL_Client()
{
	//if (TIMER.CheckRender())
	//if (TIMER.ShowCounter())
	if (TIMER.CheckUpdate())
	{
		printf("CurrentSize of QueryBQ : %d", MY_SQL_CLIENT.QueryBQ.GetCurrentSize());
		printf(CRALF);

		printf("Transaction per second : %d", MY_SQL_CLIENT.TransactionPerSecond);
		printf(CRALF);

		MY_SQL_CLIENT.TransactionPerSecond = 0;
	}

	return 0;
}

bool TerminalizeMySQL_Client()
{
	HANDLE subThreadHandle[MY_SQL_CLIENT_SUB_THREAD_COUNT];

	mysql_close(&MY_SQL_CLIENT.MySQL);

	MY_SQL_CLIENT_SHUTDOWN = 1;

	FOR(i, MY_SQL_CLIENT_SUB_THREAD_COUNT)
	{
		subThreadHandle[i] = MY_SQL_CLIENT_SUB_THREAD_V[i].Handle;
	}

	WaitForMultipleObjects(MY_SQL_CLIENT_SUB_THREAD_COUNT, subThreadHandle, 1, INFINITE);

	FOR(i, MY_SQL_CLIENT_SUB_THREAD_COUNT)
	{
		CloseHandle(subThreadHandle[i]);
	}

	CloseHandle(MY_SQL_CLIENT_SEND_CP);
	CloseHandle(MY_SQL_CLIENT_UPDATE_CP);
	CloseHandle(MY_SQL_CLIENT_RECEIVE_CP);
	CloseHandle(MY_SQL_CLIENT_IOCP);

	//여기서 쿼리 큐를 확인해서 데이터베이스 쓰기 스레드를 종료시키는 것이 좋다

	return 0;
}