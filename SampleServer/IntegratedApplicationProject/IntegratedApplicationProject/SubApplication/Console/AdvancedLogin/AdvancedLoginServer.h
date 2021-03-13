#pragma once
#include "AdvancedLogin.h"
#include "LoginWAN_Server.h"

extern LoginWAN_Server LOGIN_WAN_SERVER;

int ExecuteAdvancedLoginServer(int argumentCount, char* argumentVector[]);

bool InitializeAdvancedLoginServer();
bool UpdateAdvancedLoginServer();
bool RenderAdvancedLoginServer();
bool TerminalizeAdvancedLoginServer();