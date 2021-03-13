#include "stdafx.h"
#include "AnotherLAN_Server.h"

bool InitializeAnotherLAN_Server()
{
	if(DERIVED_LAN_SERVER.Initialize()) return 1;

	return 0;
}

bool UpdateAnotherLAN_Server()
{
	if(DERIVED_LAN_SERVER.Update()) return 1;

	return 0;
}

bool RenderAnotherLAN_Server()
{
	if(DERIVED_LAN_SERVER.Render()) return 1;

	return 0;
}

bool TerminalizeAnotherLAN_Server()
{
	if(DERIVED_LAN_SERVER.Terminalize()) return 1;

	return 0;
}