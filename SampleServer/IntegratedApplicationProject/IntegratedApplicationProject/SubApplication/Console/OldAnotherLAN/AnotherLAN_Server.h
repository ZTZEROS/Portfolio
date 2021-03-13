#pragma once
#include "AnotherLAN.h"
#include "BaseLAN_Server.h"
#include "DerivedLAN_Server.h"

extern DerivedLAN_Server DERIVED_LAN_SERVER;

int ExecuteAnotherLAN_Server(int argumentCount, char* argumentVector[]);

bool InitializeAnotherLAN_Server();
bool UpdateAnotherLAN_Server();
bool RenderAnotherLAN_Server();
bool TerminalizeAnotherLAN_Server();