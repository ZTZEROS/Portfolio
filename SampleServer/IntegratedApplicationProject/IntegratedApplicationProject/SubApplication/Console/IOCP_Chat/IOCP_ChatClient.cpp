#include "stdafx.h"
#include "IOCP_ChatClient.h"

IOCP_ChatClient IOCP_CHAT_CLIENT;
char BUFFER[BUFFER_SIZE];

int ExecuteIOCP_ChatClient(int argumentCount, char* argumentVector[])
{
	//Initialize:
	{
		InitializeIOCP_ChatClient();
	}

	//Update:
	//Render:
	{
		while (1)
		{
			if (UpdateIOCP_ChatClient()) break;
			if (RenderIOCP_ChatClient()) break;
		}
	}

	//Terminalize:
	{
		TerminalizeIOCP_ChatClient();
	}

	return 0;
}
