#include "stdafx.h"
#include "AdvancedChatServer.h"

ChatWAN_Server CHAT_WAN_SERVER;

int ExecuteAdvancedChatServer(int argumentCount, char* argumentVector[])
{
	//Initialize:
	{
		InitializeAdvancedChatServer();
	}

	//Update:
	//Render:
	{
		while (1)
		{
			if (UpdateAdvancedChatServer()) break;
			if (RenderAdvancedChatServer()) break;
		}
	}

	//Terminalize:
	{
		TerminalizeAdvancedChatServer();
	}

	return 0;
}
