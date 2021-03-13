#include "stdafx.h"
#include "AdvancedWAN_Server.h"

bool InitializeAdvancedWAN_Server()
{
	if (CHAT_WAN_SERVER.Initialize()) return 1;

	return 0;
}

bool UpdateAdvancedWAN_Server()
{
	if (CHAT_WAN_SERVER.Update()) return 1;

	return 0;
}

bool RenderAdvancedWAN_Server()
{
	if (CHAT_WAN_SERVER.Render()) return 1;

	return 0;
}

bool TerminalizeAdvancedWAN_Server()
{
	if (CHAT_WAN_SERVER.Terminalize()) return 1;

	return 0;
}