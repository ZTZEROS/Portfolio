#include "stdafx.h"
#include "LanServer.h"

DerivedLanServer DERIVED_LAN_SERVER;


int ExecuteLanServer(int argumentCount, char* argumentVector[])
{
	//Initialize:
	{
		InitializeLanServer();
	}

	//Update:
	//Render:
	{
		while (1)
		{
			if (UpdateLanServer()) break;
			if (RenderLanServer()) break;
		}
	}

	//Terminalize:
	{
		TerminalizeLanServer();
	}

	return 0;
}