#include "stdafx.h"
#include "PacketDefine.h"
#include "FighterClient.h"

FighterServerSession FIGHTER_SERVER_SESSION;

FighterClientSessionV FIGHTER_CLIENT_SESSION_V;

FighterPlayerUM FIGHTER_PLAYER_UM;
FighterPlayerV FIGHTER_PLAYER_V;

FighterSector FIGHTER_SECTOR_A[3][3];

TIMEVAL FIGHTER_CLIENT_TIME_VALUE;



int FighterClient(int argumentCount, char* argumentVector[])
{
	//Initialize:
	{
		InitializeFighterClient();
	}

	//Update:
	//Render:
	{
		while (1)
		{
			if (UpdateFighterClient()) break;
			if (RenderFighterClient()) break;
		}
	}

	//Terminalize:
	{
		TerminalizeFighterClient();
	}

	return 0;
}