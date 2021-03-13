#pragma once

struct SocketInformation
{
	OVERLAPPED Overlapped;
	SOCKET Socket;
	SOCKADDR_IN Address;
	char ReceiveBuffer[BUFFER_SIZE];
	char SendBuffer[BUFFER_SIZE];
	DWORD ReadCount;
	DWORD WriteCount;
	WSABUF WSA_Buffer[2];
};

int ExecuteIOCP_Experiment(int argumentCount, char* argumentVector[]);

unsigned int WINAPI WorkIOCP_Experiment(LPVOID argument);