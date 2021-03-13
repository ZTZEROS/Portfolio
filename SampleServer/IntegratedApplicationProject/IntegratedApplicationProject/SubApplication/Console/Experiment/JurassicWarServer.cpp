#include "stdafx.h"
#include "JurassicWarServer.h"

JurassicWarServer JURASSIC_WAR_SERVER;
JurassicWarClientV JURASSIC_WAR_CLIENT_V;

JurassicWarSubThreadV JURASSIC_WAR_SUB_THREAD_V;
HANDLE JURASSIC_WAR_SERVER_IOCP;
HANDLE JURASSIC_WAR_SERVER_RECEIVE_CP;
HANDLE JURASSIC_WAR_SERVER_UPDATE_CP;
HANDLE JURASSIC_WAR_SERVER_SEND_CP;
HANDLE JURASSIC_WAR_SERVER_DISCONNECT_CP;

BOOL JURASSIC_WAR_SERVER_SHUTDOWN;

int ExecuteJurassicWarServer(int argumentCount, char* argumentVector[])
{
	//Initialize:
	{
		InitializeJurassicWarServer();
	}

	//Update:
	//Render:
	{
		while (1)
		{
			if (UpdateJurassicWarServer()) break;
			if (RenderJurassicWarServer()) break;
		}
	}

	//Terminalize:
	{
		TerminalizeJurassicWarServer();
	}

	return 0;
}

