#include "stdafx.h"
#include "IOCP_ChatServer.h"

IOCP_ChatServer IOCP_CHAT_SERVER;
IOCP_ChatClientV IOCP_CHAT_CLIENT_V;

IOCP_ChatSubThreadV IOCP_CHAT_SUB_THREAD_V;
HANDLE IOCP_CHAT_SERVER_IOCP;
HANDLE IOCP_CHAT_SERVER_RECEIVE_CP;
HANDLE IOCP_CHAT_SERVER_UPDATE_CP;
HANDLE IOCP_CHAT_SERVER_SEND_CP;
HANDLE IOCP_CHAT_SERVER_DISCONNECT_CP;

BOOL IOCP_CHAT_SERVER_SHUTDOWN;

int ExecuteIOCP_ChatServer(int argumentCount, char* argumentVector[])
{
	//Initialize:
	{
		InitializeIOCP_ChatServer();
	}

	//Update:
	//Render:
	{
		while (1)
		{
			if (UpdateIOCP_ChatServer()) break;
			if (RenderIOCP_ChatServer()) break;
		}
	}

	//Terminalize:
	{
		TerminalizeIOCP_ChatServer();
	}

	return 0;
}
