#include "stdafx.h"
#include "MonitorServer.h"

bool InitializeMonitorServer()
{
	if (MONITOR_OUTER_SERVER.Initialize()) return 1;

	return 0;
}

bool UpdateMonitorServer()
{
	if (MONITOR_OUTER_SERVER.Update()) return 1;

	return 0;
}

bool RenderMonitorServer()
{
	if (MONITOR_OUTER_SERVER.Render()) return 1;

	return 0;
}

bool TerminalizeMonitorServer()
{
	if (MONITOR_OUTER_SERVER.Terminalize()) return 1;

	return 0;
}