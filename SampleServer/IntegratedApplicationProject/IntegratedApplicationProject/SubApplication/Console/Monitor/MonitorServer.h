#pragma once
#include "Monitor.h"
#include "MonitorOuterServer.h"

extern MonitorOuterServer MONITOR_OUTER_SERVER;

int ExecuteMonitorServer(int argumentCount, char* argumentVector[]);

bool InitializeMonitorServer();
bool UpdateMonitorServer();
bool RenderMonitorServer();
bool TerminalizeMonitorServer();