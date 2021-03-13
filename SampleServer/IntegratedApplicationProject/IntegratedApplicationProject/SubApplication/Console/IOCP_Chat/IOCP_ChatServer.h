#pragma once
#include "IOCP_Chat.h"

extern IOCP_ChatServer IOCP_CHAT_SERVER;
extern IOCP_ChatClientV IOCP_CHAT_CLIENT_V;

extern IOCP_ChatSubThreadV IOCP_CHAT_SUB_THREAD_V;
extern HANDLE IOCP_CHAT_SERVER_IOCP;
extern HANDLE IOCP_CHAT_SERVER_RECEIVE_CP;
extern HANDLE IOCP_CHAT_SERVER_UPDATE_CP;
extern HANDLE IOCP_CHAT_SERVER_SEND_CP;
extern HANDLE IOCP_CHAT_SERVER_DISCONNECT_CP;

extern BOOL IOCP_CHAT_SERVER_SHUTDOWN;

int ExecuteIOCP_ChatServer(int argumentCount, char* argumentVector[]);

bool InitializeIOCP_ChatServer();
bool UpdateIOCP_ChatServer();
bool RenderIOCP_ChatServer();
bool TerminalizeIOCP_ChatServer();

unsigned int WINAPI AcceptIOCP_ChatClient(LPVOID argument);
unsigned int WINAPI DisconnectIOCP_ChatClient(LPVOID argument);

unsigned int WINAPI ReceiveIOCP_ChatClientPacket(LPVOID argument);
unsigned int WINAPI SendIOCP_ChatServerPacket(LPVOID argument);

unsigned int WINAPI ProcessReceivedIOCP_ChatClientPacket(LPVOID argument);
unsigned int WINAPI ProcessSendingIOCP_ChatServerPacket(LPVOID argument);

unsigned int WINAPI WorkIOCP_ChatServer(LPVOID argument);