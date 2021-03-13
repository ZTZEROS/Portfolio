#include "stdafx.h"
#include "AdvancedWAN_Server.h"

ChatWAN_Server CHAT_WAN_SERVER;

int ExecuteAdvancedWAN_Server(int argumentCount, char* argumentVector[])
{
	//Initialize:
	{
		InitializeAdvancedWAN_Server();
	}

	//Update:
	//Render:
	{
		while (1)
		{
			if (UpdateAdvancedWAN_Server()) break;
			if (RenderAdvancedWAN_Server()) break;
		}
	}

	//Terminalize:
	{
		TerminalizeAdvancedWAN_Server();
	}

	return 0;
}