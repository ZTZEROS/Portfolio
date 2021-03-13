#include "stdafx.h"
#include "AdvancedLoginServer.h"

LoginWAN_Server LOGIN_WAN_SERVER;

int ExecuteAdvancedLoginServer(int argumentCount, char* argumentVector[])
{
	//Initialize:
	{
		InitializeAdvancedLoginServer();
	}

	//Update:
	//Render:
	{
		while (1)
		{
			if (UpdateAdvancedLoginServer()) break;
			if (RenderAdvancedLoginServer()) break;
		}
	}

	//Terminalize:
	{
		TerminalizeAdvancedLoginServer();
	}

	return 0;
}