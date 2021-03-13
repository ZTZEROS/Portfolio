#pragma once
#include "MySQL.h"

extern MySQL_Server MY_SQL_SERVER;
extern MySQL_Client MY_SQL_CLIENT;

extern MySQL_SubThreadV MY_SQL_CLIENT_SUB_THREAD_V;
extern HANDLE MY_SQL_CLIENT_IOCP;
extern HANDLE MY_SQL_CLIENT_RECEIVE_CP;
extern HANDLE MY_SQL_CLIENT_UPDATE_CP;
extern HANDLE MY_SQL_CLIENT_SEND_CP;

extern BOOL MY_SQL_CLIENT_SHUTDOWN;

int ExecuteMySQL_Client(int argumentCount, char* argumentVector[]);

bool InitializeMySQL_Client();
bool UpdateMySQL_Client();
bool RenderMySQL_Client();
bool TerminalizeMySQL_Client();

//unsigned int WINAPI ConnectToMySQL_Server(LPVOID argument);
//unsigned int WINAPI DisconnectFromMySQL_Server(LPVOID argument);

unsigned int WINAPI ReceiveMySQL_ServerPacket(LPVOID argument);
unsigned int WINAPI SendMySQL_ClientPacket(LPVOID argument);
//unsigned int WINAPI WriteDatum(LPVOID argument);

unsigned int WINAPI WorkMySQL_Client(LPVOID argument);
