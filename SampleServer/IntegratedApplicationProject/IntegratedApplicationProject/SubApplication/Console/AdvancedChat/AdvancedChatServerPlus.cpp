#include "stdafx.h"
#include "AdvancedChatServer.h"

bool InitializeAdvancedChatServer()
{
	if (CHAT_WAN_SERVER.Initialize()) return 1;

	return 0;
}

bool UpdateAdvancedChatServer()
{
	if (CHAT_WAN_SERVER.Update()) return 1;

	return 0;
}

bool RenderAdvancedChatServer()
{
	if (CHAT_WAN_SERVER.Render()) return 1;

	return 0;
}

bool TerminalizeAdvancedChatServer()
{
	if (CHAT_WAN_SERVER.Terminalize()) return 1;

	return 0;
}