#include "stdafx.h"
#include "MySQL_Client.h"

unsigned int WINAPI ReceiveMySQL_ServerPacket(LPVOID argument)
{
	while (!MY_SQL_CLIENT_SHUTDOWN)
	{
		Sleep(1000);
	}

	return 0;
}

unsigned int WINAPI SendMySQL_ClientPacket(LPVOID argument)
{
	LARGE_INTEGER performanceCounter;

	QueryPerformanceCounter(&performanceCounter);

	srand(performanceCounter.QuadPart * (GetThreadId(GetCurrentThread()) + 1));

	while (!MY_SQL_CLIENT_SHUTDOWN || MY_SQL_CLIENT.QueryBQ.GetCurrentSize())
	{
		MySQL_ClientQueryHeader receivedQueryHeader;
		SerialByteQueue receivedQuerySBQ;

		if (sizeof(receivedQueryHeader) == MY_SQL_CLIENT.QueryBQ.Peek((char*)&receivedQueryHeader, sizeof(receivedQueryHeader)))
		{
			if (sizeof(receivedQueryHeader) + receivedQueryHeader.Size == MY_SQL_CLIENT.QueryBQ.Peek(receivedQuerySBQ.GetQueueAddress(), sizeof(receivedQueryHeader) + receivedQueryHeader.Size))
			{
				string query;

				receivedQuerySBQ.SetHeader(receivedQueryHeader);

				receivedQuerySBQ.IncreaseCurrentSize(receivedQueryHeader.Size);

				switch (receivedQueryHeader.Index)
				{
				case MY_SQL_CLIENT_QUERY_SIGN_IN:
					{
						__int64 user_key;
						char nickname[21];
						char password[21];

						user_key = (rand() * rand()) % 0xffffffff;

						for (unsigned int i = 0; i < sizeof(nickname)-1; ++i)
						{
							receivedQuerySBQ >> nickname[i];
						}

						nickname[20] = '\0';

						for (unsigned int j = 0; j < sizeof(password)-1; ++j)
						{
							receivedQuerySBQ >> password[j];
						}

						password[20] = '\0';

						query = (string)"INSERT INTO user_information VALUES(" + to_string(user_key) + ", \"" + nickname + "\", \"" + password + "\")";
					}
					break;
				case MY_SQL_CLIENT_QUERY_STAGE_CLEAR:
					{
						__int64 user_key;
						int stage;

						receivedQuerySBQ >> user_key;
						receivedQuerySBQ >> stage;

						query = (string)"INSERT INTO user_stage VALUES(" + to_string(user_key) + ", " + to_string(stage) + ")";
					}
					break;
				case MY_SQL_CLIENT_QUERY_LEVEL_UP:
					{
						__int64 user_key;
						int level;
						__int64 experience;

						receivedQuerySBQ >> user_key;
						receivedQuerySBQ >> level;
						receivedQuerySBQ >> experience;

						query = (string)"INSERT INTO user_level VALUES(" + to_string(user_key) + ", " + to_string(level) + ", " + to_string(experience) + ")";
					}
					break;
				default:
					break;
				}

				switch (mysql_query(&MY_SQL_CLIENT.MySQL, query.c_str()))
				{
				case 0:
					{
						MY_SQL_CLIENT.QueryBQ.DecreaseCurrentSize(sizeof(receivedQueryHeader) + receivedQueryHeader.Size);

						//printf(query.c_str());
						//printf(CRALF);
					}
					break;
				case CR_SOCKET_CREATE_ERROR:
				case CR_CONNECTION_ERROR:
				case CR_CONN_HOST_ERROR:
				case CR_SERVER_GONE_ERROR:
				case CR_SERVER_HANDSHAKE_ERR:
				case CR_SERVER_LOST:
				case CR_INVALID_CONN_HANDLE:
					{
						printf("Mysql query error : %s, %d", mysql_error(&MY_SQL_CLIENT.MySQL), mysql_errno(&MY_SQL_CLIENT.MySQL));
						printf(CRALF);

						mysql_init(&MY_SQL_CLIENT.MySQL);

						//MY_SQL_SERVER.MySQL_Address = mysql_real_connect(&MY_SQL_SERVER.MySQL, mysqlServerIP_Address, mysqlUserName, mysqlPassword, mysqlDatabaseName, MY_SQL_SERVER_PORT, nullptr, NULL))
						if (!mysql_real_connect(&MY_SQL_CLIENT.MySQL, MY_SQL_SERVER.MySQL_ServerIP_Address, MY_SQL_SERVER.MySQL_UserName, MY_SQL_SERVER.MySQL_Password, MY_SQL_SERVER.MySQL_DatabaseName, MY_SQL_SERVER_PORT, nullptr, NULL))
						{
							printf("Mysql connection error : %s", mysql_error(&MY_SQL_CLIENT.MySQL));
							printf(CRALF);
						}

						mysql_set_character_set(&MY_SQL_CLIENT.MySQL, "utf8");

						MY_SQL_CLIENT.CurrentStepIndex = MY_SQL_CLIENT_PHASE_CONNECT;
					}
				default:
					{
						printf("Mysql query error : %s, %d", mysql_error(&MY_SQL_CLIENT.MySQL), mysql_errno(&MY_SQL_CLIENT.MySQL));
						printf(CRALF);

						MY_SQL_CLIENT.QueryBQ.DecreaseCurrentSize(sizeof(receivedQueryHeader) + receivedQueryHeader.Size);

						printf(query.c_str());
						printf(CRALF);
					}
					break;
				}
				
				//if (mysql_query(&MY_SQL_CLIENT.MySQL, "START TRANSACTION")
				//	&& mysql_query(&MY_SQL_CLIENT.MySQL, query.c_str())
				//	&& mysql_query(&MY_SQL_CLIENT.MySQL, "COMMIT"))
				//{
				//	mysql_query(&MY_SQL_CLIENT.MySQL, "ROLLBACK");
				//
				//	printf("Mysql query error : %s", mysql_error(&MY_SQL_CLIENT.MySQL));
				//
				//	return 1;
				//}

				//MY_SQL_CLIENT.MySQL_ResultAddress = mysql_store_result(&MY_SQL_CLIENT.MySQL);
				//
				//if (MY_SQL_CLIENT.MySQL_ResultAddress)
				//{
				//	MY_SQL_CLIENT.MySQL_Result = *MY_SQL_CLIENT.MySQL_ResultAddress;
				//	//mysqlResult = mysql_use_result(mysqlAddress);
				//
				//	MY_SQL_CLIENT.MySQL_Row = mysql_fetch_row(&MY_SQL_CLIENT.MySQL_Result);
				//
				//	while (MY_SQL_CLIENT.MySQL_Row)
				//	{
				//		for (unsigned int i = 0; i < *MY_SQL_CLIENT.MySQL_Result.lengths; ++i)
				//		{
				//			printf(MY_SQL_CLIENT.MySQL_Row[i]);
				//			printf(CRALF);
				//		}
				//
				//		MY_SQL_CLIENT.MySQL_Row = mysql_fetch_row(&MY_SQL_CLIENT.MySQL_Result);
				//	}
				//
				//	mysql_free_result(MY_SQL_CLIENT.MySQL_ResultAddress);
				//}

				++MY_SQL_CLIENT.TransactionPerSecond;
			}
		}
	}

	return 0;
}

unsigned int WINAPI WorkMySQL_Client(LPVOID argument)
{
	LARGE_INTEGER performanceCounter;

	QueryPerformanceCounter(&performanceCounter);

	srand(performanceCounter.QuadPart * (GetThreadId(GetCurrentThread())+1));

	while (!MY_SQL_CLIENT_SHUTDOWN)
	{
		if (!InterlockedExchange((unsigned int*)&MY_SQL_CLIENT.QueryEnqueuing, 1))
		{
			MySQL_ClientQueryHeader sendingQueryHeader;
			SerialByteQueue sendingQuerySBQ;

			sendingQueryHeader.Index = (MY_SQL_CLIENT_QUERY_INDEX)(rand() % MY_SQL_CLIENT_QUERY_COUNT);

			sendingQuerySBQ.SetHeader(sendingQueryHeader);

			switch (sendingQueryHeader.Index)
			{
			case MY_SQL_CLIENT_QUERY_SIGN_IN:
				{
					char nickname[20];
					char password[20];

					for (unsigned int i = 0; i < sizeof(nickname); ++i)
					{
						nickname[i] = "0123456789AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz"[rand() % sizeof(nickname)];
						sendingQuerySBQ << nickname[i];
					}

					for (unsigned int j = 0; j < sizeof(password); ++j)
					{
						password[j] = "0123456789AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz"[rand() % sizeof(password)];
						sendingQuerySBQ << password[j];
					}
				}
				break;

			case MY_SQL_CLIENT_QUERY_STAGE_CLEAR:
				{
					__int64 user_key;
					int stage;

					user_key = (rand() * rand()) % 0xffffffff; //0xffffffffffffffff;
					stage = rand() % 0xff;

					sendingQuerySBQ << user_key;
					sendingQuerySBQ << stage;
				}
				break;

			case MY_SQL_CLIENT_QUERY_LEVEL_UP:
				{
					__int64 user_key;
					int level;
					__int64 experience;

					user_key = (rand() * rand()) % 0xffffffff;
					level = rand() % 0xff;
					experience = (rand() * rand()) % 0xffffffff;

					sendingQuerySBQ << user_key;
					sendingQuerySBQ << level;
					sendingQuerySBQ << experience;
				}
				break;
				//case MY_SQL_CLIENT_QUERY_LOG_OUT:
				//	{
				//		쿼리 마지막에 로그아웃 플래그를 바꾸는 쿼리를 넣어준다.
				//		로그아웃 체크를 db에 해 줘야 중복로그인을 막을 수 있다.
				//		다른 쿼리도 처리가 끝날 때까지 같은 요청을 반복할 수 없게끔 막는다
				//	}
				//	break;
			default:
				break;
			}

			sendingQueryHeader.Size = sendingQuerySBQ.GetBodySize();
			sendingQuerySBQ.SetHeader(sendingQueryHeader);

			MY_SQL_CLIENT.QueryBQ.Enqueue(sendingQuerySBQ.GetQueueAddress(), sendingQuerySBQ.GetTotalSize());

			MY_SQL_CLIENT.QueryEnqueuing = 0; //InterlockedExchange((unsigned int*)&MY_SQL_CLIENT.QueryEnqueuing, 0);
		}
		//else
		//{
			//enqueue to local queue
			//스레드마다 쿼리 버퍼 큐를 따로 둔다
			//큐에는 데이터가 아니라 메모리풀의 포인터를 넣는다
			//메모리풀에 동기화개체를 넣어서 락을 건다
		//}
		
		Sleep(10);
	}

	return 0;
}