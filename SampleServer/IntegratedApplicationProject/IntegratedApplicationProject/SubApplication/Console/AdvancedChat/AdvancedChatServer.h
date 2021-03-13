#pragma once
#include "AdvancedChat.h"
#include "ChatWAN_Server.h"

extern ChatWAN_Server CHAT_WAN_SERVER;

int ExecuteAdvancedChatServer(int argumentCount, char* argumentVector[]);

bool InitializeAdvancedChatServer();
bool UpdateAdvancedChatServer();
bool RenderAdvancedChatServer();
bool TerminalizeAdvancedChatServer();