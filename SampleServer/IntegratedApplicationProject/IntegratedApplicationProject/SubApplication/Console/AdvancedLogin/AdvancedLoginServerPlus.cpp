#include "stdafx.h"
#include "AdvancedLoginServer.h"

bool InitializeAdvancedLoginServer()
{
	if (LOGIN_WAN_SERVER.Initialize()) return 1;

	return 0;
}

bool UpdateAdvancedLoginServer()
{
	if (LOGIN_WAN_SERVER.Update()) return 1;

	return 0;
}

bool RenderAdvancedLoginServer()
{
	if (LOGIN_WAN_SERVER.Render()) return 1;

	return 0;
}

bool TerminalizeAdvancedLoginServer()
{
	if (LOGIN_WAN_SERVER.Terminalize()) return 1;

	return 0;
}