#pragma once
#include "Lan.h"
#include "BaseLanServer.h"
#include "DerivedLanServer.h"

extern DerivedLanServer DERIVED_LAN_SERVER;

int ExecuteLanServer(int argumentCount, char* argumentVector[]);

bool InitializeLanServer();
bool UpdateLanServer();
bool RenderLanServer();
bool TerminalizeLanServer();