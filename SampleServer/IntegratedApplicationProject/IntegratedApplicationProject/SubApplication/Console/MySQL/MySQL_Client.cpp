#include "stdafx.h"
#include "MySQL_Client.h"

MySQL_Server MY_SQL_SERVER;
MySQL_Client MY_SQL_CLIENT;

MySQL_SubThreadV MY_SQL_CLIENT_SUB_THREAD_V;
HANDLE MY_SQL_CLIENT_IOCP;
HANDLE MY_SQL_CLIENT_RECEIVE_CP;
HANDLE MY_SQL_CLIENT_UPDATE_CP;
HANDLE MY_SQL_CLIENT_SEND_CP;

BOOL MY_SQL_CLIENT_SHUTDOWN;

int ExecuteMySQL_Client(int argumentCount, char* argumentVector[])
{
	//Initialize:
	{
		InitializeMySQL_Client();
	}

	//Update:
	//Render:
	{
		while (1)
		{
			if (UpdateMySQL_Client()) break;
			if (RenderMySQL_Client()) break;
		}
	}

	//Terminalize:
	{
		TerminalizeMySQL_Client();
	}

	return 0;

	return 0;
}