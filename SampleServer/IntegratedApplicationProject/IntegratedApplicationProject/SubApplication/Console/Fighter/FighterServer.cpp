#include "stdafx.h"
#include "PacketDefine.h"
#include "FighterServer.h"

FighterServerSession FIGHTER_SERVER_SESSION;

FighterClientSessionV FIGHTER_CLIENT_SESSION_V;
//FighterClientSessionA FIGHTER_CLIENT_SESSION_A[TOTAL_FIGHTER_CLIENT_COUNT];

FighterPlayerUM FIGHTER_PLAYER_UM;
FighterPlayerV FIGHTER_PLAYER_V;
//FighterPlayer FIGHTER_PLAYER_A[TOTAL_FIGHTER_CLIENT_COUNT];

FighterSector FIGHTER_SECTOR_A[FIGHTER_MAP_HEIGHT / FIGHTER_SECTOR_HEIGHT][FIGHTER_MAP_WIDTH / FIGHTER_SECTOR_WIDTH];

TIMEVAL FIGHTER_SERVER_TIME_VALUE;

unsigned int FIGHTER_IDENTIFICATION_NUMBER;

unsigned int LOOP_COUNT;
unsigned int UPDATE_COUNT;
unsigned int RECEIVE_COUNT;
unsigned int SEND_COUNT;

unsigned int CREATE_OTHER_CHARACTER_COUNT;
unsigned int DELETE_OTHER_CHARACTER_COUNT;
unsigned int MOVE_START_COUNT;
unsigned int MOVE_STOP_COUNT;
unsigned int ATTACK1_COUNT;
unsigned int ATTACK2_COUNT;
unsigned int ATTACK3_COUNT;
unsigned int CALCULATE_FIGHTER_SECTOR_COUNT;
unsigned int DEADRECKONING_COUNT;

int FighterServer(int argumentCount, char* argumentVector[])
{
	//Initialize:
	{
		InitializeFighterServer();
	}

	//Update:
	//Render:
	{
		while (1)
		{
			if (UpdateFighterServer()) break;
			if (RenderFighterServer()) break;
		}
	}

	//Terminalize:
	{
		TerminalizeFighterServer();
	}

	return 0;
}