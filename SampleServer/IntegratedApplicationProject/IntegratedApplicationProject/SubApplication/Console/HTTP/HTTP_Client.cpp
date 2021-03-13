#include "stdafx.h"
#include "HTTP_Client.h"

HTTP_Server HTTP_SERVER;
HTTP_Server HTTP_CLIENT;

int ExecuteHTTP(int argumentCount, char* argumentVector[])
{
	//Initialize:
	{
		InitializeHTTP_Client();
	}

	//Update:
	//Render:
	{
		while (1)
		{
			if (UpdateHTTP_Client()) break;
			if (RenderHTTP_Client()) break;
		}
	}

	//Terminalize:
	{
		TerminalizeHTTP_Client();
	}

	system("pause > nul");

	return 0;
}