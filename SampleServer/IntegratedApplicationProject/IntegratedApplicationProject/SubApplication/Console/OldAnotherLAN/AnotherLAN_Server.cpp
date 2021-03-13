#include "stdafx.h"
#include "AnotherLAN_Server.h"

DerivedLAN_Server DERIVED_LAN_SERVER;

int ExecuteAnotherLAN_Server(int argumentCount, char* argumentVector[])
{
	//Initialize:
	{
		InitializeAnotherLAN_Server();
	}

	//Update:
	//Render:
	{
		while (1)
		{
			if (UpdateAnotherLAN_Server()) break;
			if (RenderAnotherLAN_Server()) break;
		}
	}

	//Terminalize:
	{
		TerminalizeAnotherLAN_Server();
	}

	return 0;
}