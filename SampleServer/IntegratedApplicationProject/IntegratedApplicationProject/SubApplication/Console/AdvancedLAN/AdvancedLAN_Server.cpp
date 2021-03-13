#include "stdafx.h"
#include "AdvancedLAN_Server.h"

EchoLAN_Server ECHO_LAN_SERVER;

int ExecuteAdvancedLAN_Server(int argumentCount, char* argumentVector[])
{
	//Initialize:
	{
		InitializeAdvancedLAN_Server();
	}

	//Update:
	//Render:
	{
		while (1)
		{
			if (UpdateAdvancedLAN_Server()) break;
			if (RenderAdvancedLAN_Server()) break;
		}
	}

	//Terminalize:
	{
		TerminalizeAdvancedLAN_Server();
	}

	return 0;
}