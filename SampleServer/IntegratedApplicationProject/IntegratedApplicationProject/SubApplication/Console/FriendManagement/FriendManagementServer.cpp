#include "stdafx.h"
#include "Protocol.h"
#include "FriendManagementServer.h"

FM_ServerSession FM_SERVER_SESSION;

FM_ClientSessionM FM_CLIENT_SESSION_M;

FM_MemberNameM FM_MEMBER_NAME_M;
FM_MemberNumberLM FM_OFFEREE_MEMBER_NUMBER_LM; //<OFFERER_MemberNumber, OFFEREE_L>
FM_MemberNumberLM FM_OFFERER_MEMBER_NUMBER_LM; //<OFFEREE_MemberNumber, OFFERER_M>
FM_MemberNumberLM FM_FRIEND_MEMBER_NUMBER_LM;

TIMEVAL FM_SERVER_TIME_VALUE;

unsigned int LAST_FM_CLIENT_INDEX;
unsigned int CURRENT_FM_CLIENT_COUNT;

unsigned int LAST_FM_MEMBER_INDEX;
unsigned int CURRENT_FM_MEMBER_COUNT;
unsigned int FM_MEMBER_NUMBER_A[TOTAL_FM_CLIENT_COUNT];

int FM_Server(int arguemntCount, char* argumentVector[])
{
	//Initialize:
	{
		InitializeFM_Server();
	}

	//Update:
	//Render:
	{
		while (1)
		{
			if (UpdateFM_Server()) break;
			if (RenderFM_Server()) break;
		}
	}

	//Terminalize:
	{
		TerminalizeFM_Server();
	}

	return 0;
}