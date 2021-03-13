#pragma once
#include "AdvancedLAN.h"
#include "BaseLAN_Server.h"
#include "DerivedLAN_Server.h"

extern DerivedLAN_Server DERIVED_LAN_SERVER;

int ExecuteAdvancedLAN_Server(int argumentCount, char* argumentVector[]);

bool InitializeAdvancedLAN_Server();
bool UpdateAdvancedLAN_Server();
bool RenderAdvancedLAN_Server();
bool TerminalizeAdvancedLAN_Server();