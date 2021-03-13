#pragma once

#include "HTTP.h"

extern HTTP_Server HTTP_SERVER;
extern HTTP_Server HTTP_CLIENT;

int ExecuteHTTP_Client(int argumentCount, char* argumentVector[]);

bool InitializeHTTP_Client();
bool UpdateHTTP_Client();
bool RenderHTTP_Client();
bool TerminalizeHTTP_Client();

int SendHTTP_ClientPacket();
int ReceiveHTTP_ServerPacket()