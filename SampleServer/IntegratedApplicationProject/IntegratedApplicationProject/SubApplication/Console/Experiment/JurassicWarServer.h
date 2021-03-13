#pragma once
#include "JurassicWar.h"

extern JurassicWarServer JURASSIC_WAR_SERVER;
extern JurassicWarClientV JURASSIC_WAR_CLIENT_V;

extern JurassicWarSubThreadV JURASSIC_WAR_SUB_THREAD_V;
extern HANDLE JURASSIC_WAR_SERVER_IOCP;
extern HANDLE JURASSIC_WAR_SERVER_RECEIVE_CP;
extern HANDLE JURASSIC_WAR_SERVER_UPDATE_CP;
extern HANDLE JURASSIC_WAR_SERVER_SEND_CP;
extern HANDLE JURASSIC_WAR_SERVER_DISCONNECT_CP;

extern BOOL JURASSIC_WAR_SERVER_SHUTDOWN;

int ExecuteJurassicWarServer(int argumentCount, char* argumentVector[]);

bool InitializeJurassicWarServer();
bool UpdateJurassicWarServer();
bool RenderJurassicWarServer();
bool TerminalizeJurassicWarServer();

unsigned int WINAPI AcceptJurassicWarClient(LPVOID argument);
unsigned int WINAPI DisconnectJurassicWarClient(LPVOID argument);

unsigned int WINAPI ReceiveJurassicWarClientPacket(LPVOID argument);
unsigned int WINAPI SendJurassicWarServerPacket(LPVOID argument);

unsigned int WINAPI WorkJurassicWarServer(LPVOID argument);