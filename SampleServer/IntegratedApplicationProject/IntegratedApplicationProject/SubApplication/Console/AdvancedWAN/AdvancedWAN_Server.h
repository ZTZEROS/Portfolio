#pragma once
#include "AdvancedWAN.h"
#include "BaseWAN_Server.h"
#include "ChatWAN_Server.h"

extern ChatWAN_Server CHAT_WAN_SERVER;

int ExecuteAdvancedWAN_Server(int argumentCount, char* argumentVector[]);

bool InitializeAdvancedWAN_Server();
bool UpdateAdvancedWAN_Server();
bool RenderAdvancedWAN_Server();
bool TerminalizeAdvancedWAN_Server();