#pragma once
#include "IOCP_Chat.h"

extern IOCP_ChatClient IOCP_CHAT_CLIENT;
extern char BUFFER[BUFFER_SIZE];

int ExecuteIOCP_ChatClient(int argumentCount, char* argumentVector[]);

bool InitializeIOCP_ChatClient();
bool UpdateIOCP_ChatClient();
bool RenderIOCP_ChatClient();
bool TerminalizeIOCP_ChatClient();