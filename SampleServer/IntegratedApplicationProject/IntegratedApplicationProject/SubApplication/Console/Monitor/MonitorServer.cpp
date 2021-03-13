#include "stdafx.h"
#include "MonitorServer.h"

MonitorOuterServer MONITOR_OUTER_SERVER;

int ExecuteMonitorServer(int argumentCount, char* argumentVector[])
{
	//Initialize:
	{
		InitializeMonitorServer();
	}

	//Update:
	//Render:
	{
		while (1)
		{
			if (UpdateMonitorServer()) break;
			if (RenderMonitorServer()) break;
		}
	}

	//Terminalize:
	{
		TerminalizeMonitorServer();
	}

	return 0;
}
