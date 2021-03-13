#include "stdafx.h"
#include "Protocol.h"
#include "FriendManagementClient.h"

FM_ServerSessionM FM_SERVER_SESSION_M;

TIMEVAL FM_CLIENT_TIME_VALUE;

unsigned int LAST_FM_SERVER_SESSION_INDEX; //LAST_FM_CLIENT_INDEX;
unsigned int CURRENT_FM_SERVER_SESSION_COUNT; //CURRENT_FM_CLIENT_COUNT;

int FM_Client(int arguemntCount, char* argumentVector[])
{
	//Initialize:
	{
		InitializeFM_Client();
	}

	//Update:
	//Render:
	{
		while (1)
		{
			if (UpdateFM_Client()) break;
			if (RenderFM_Client()) break;
		}
	}

	//Terminalize:
	{
		TerminalizeFM_Client();
	}

	return 0;
}