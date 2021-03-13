#include "stdafx.h"
#include "Protocol.h"
#include "ChatServer.h"

ChatServerSession CHAT_SERVER_SESSION;

//ChatClientSessionA CHAT_CLIENT_SESSION_A[TOTAL_CHAT_CLIENT_COUNT]; //UserSessionA USER_SESSION_A[USER_GROUP_COUNT][USER_COUNT];
ChatClientSessionAddressL CHAT_CLIENT_SESSION_L;
ChatClientSessionAddressV CHAT_CLIENT_SESSION_V;
ChatClientSessionM CHAT_CLIENT_SESSION_M;

ChatRoomM CHAT_ROOM_M;

TIMEVAL CHAT_SERVER_TIME_VALUE;

unsigned int LAST_CHAT_CLIENT_INDEX;
unsigned int CURRENT_CHAT_CLIENT_COUNT;
unsigned int CHAT_USER_NUMBER_A[TOTAL_CHAT_CLIENT_COUNT];

unsigned int LAST_CHAT_ROOM_INDEX;
unsigned int CURRENT_CHAT_ROOM_COUNT;
unsigned int CHAT_ROOM_NUMBER_A[TOTAL_CHAT_ROOM_COUNT];

int ChatServer(int arguemntCount, char* argumentVector[])
{
	//Initialize:
	{
		InitializeChatServer();
	}

	//Update:
	//Render:
	{
		while (1)
		{
			if (UpdateChatServer()) break;
			if (RenderChatServer()) break;
		}
	}

	//Terminalize:
	{
		TerminalizeChatServer();
	}

	return 0;
}