#include "stdafx.h"
#include "AdvancedLAN_Server.h"

bool InitializeAdvancedLAN_Server()
{
	if(ECHO_LAN_SERVER.Initialize()) return 1;

	return 0;
}

bool UpdateAdvancedLAN_Server()
{
	if(ECHO_LAN_SERVER.Update()) return 1;

	return 0;
}

bool RenderAdvancedLAN_Server()
{
	if(ECHO_LAN_SERVER.Render()) return 1;

	return 0;
}

bool TerminalizeAdvancedLAN_Server()
{
	if(ECHO_LAN_SERVER.Terminalize()) return 1;

	return 0;
}