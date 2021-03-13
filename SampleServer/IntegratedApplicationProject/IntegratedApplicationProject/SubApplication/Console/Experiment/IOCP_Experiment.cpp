#include "stdafx.h"
#include "IOCP_Experiment.h"

HANDLE IOCP_EXPERIMENT_SUB_THREAD[SUB_THREAD_COUNT];
HANDLE IOCP_EXPERIMENT_IOCP;

int ExecuteIOCP_Experiment(int argumentCount, char* argumentVector[])
{
	WSADATA wsaDatum;
	SocketInformation server;
	SocketInformation* clientAddress;
	int readCount;
	int addressLength;
	DWORD flag;

	ZeroMemory(&server, sizeof(server));

	if (WSAStartup(0x0202, &wsaDatum)) return 1;

	server.Socket = socket(PF_INET, SOCK_STREAM, 0);
	if (server.Socket == INVALID_SOCKET) return 1;

	server.Address.sin_family = AF_INET;
	server.Address.sin_addr.s_addr = htonl(INADDR_ANY);
	server.Address.sin_port = htons(SERVER_PORT);

	if (bind(server.Socket, (SOCKADDR*)&server.Address, sizeof(server.Address)) == SOCKET_ERROR) return 1;

	if (listen(server.Socket, SOMAXCONN) == SOCKET_ERROR) return 1;

	IOCP_EXPERIMENT_IOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 0);

	for (unsigned int i = 0; i < SUB_THREAD_COUNT; ++i)
	{
		IOCP_EXPERIMENT_SUB_THREAD[i] = (HANDLE)_beginthreadex(nullptr, 0, WorkIOCP_Experiment, nullptr, 0, nullptr);
	}

	addressLength = sizeof(server.Address);

	while (1)
	{
		clientAddress = (SocketInformation*)malloc(sizeof(SocketInformation));

		clientAddress->Socket = accept(server.Socket, (SOCKADDR*)&server.Address, &addressLength);
		if (clientAddress->Socket == INVALID_SOCKET)
		{
			free(clientAddress);

			return 1;
		}
		else
		{
			clientAddress->ReadCount = 0;
			clientAddress->WriteCount = 0;
			clientAddress->WSA_Buffer[0].buf = clientAddress->ReceiveBuffer;
			clientAddress->WSA_Buffer[0].len = sizeof(clientAddress->ReceiveBuffer);
			clientAddress->WSA_Buffer[1].buf = clientAddress->SendBuffer;
			clientAddress->WSA_Buffer[1].len = sizeof(clientAddress->SendBuffer);

			CreateIoCompletionPort((HANDLE)clientAddress->Socket, IOCP_EXPERIMENT_IOCP, (ULONG_PTR)clientAddress, 0);

			WSARecv(clientAddress->Socket, &clientAddress->WSA_Buffer[0], 1, &clientAddress->ReadCount, &flag, &clientAddress->Overlapped, nullptr);
		}
	}

	return 0;
}

unsigned int WINAPI WorkIOCP_Experiment(LPVOID argument)
{
	DWORD readCount;
	ULONG_PTR completionKey;
	DWORD flag;
	SocketInformation* clientAddress;

	while (1)
	{
		GetQueuedCompletionStatus(IOCP_EXPERIMENT_IOCP, &readCount, &completionKey, (LPOVERLAPPED*)&clientAddress, INFINITE);

		if (!readCount)
		{
			closesocket(clientAddress->Socket);
			free(clientAddress);

			continue;
		}
		else
		{
			memcpy(clientAddress->SendBuffer, clientAddress->ReceiveBuffer, sizeof(clientAddress->ReceiveBuffer));
			WSASend(clientAddress->Socket, &clientAddress->WSA_Buffer[1], 1, &readCount, 0, nullptr, nullptr);
		}

		flag = 0;
		clientAddress->ReadCount = 0;
		clientAddress->WriteCount = 0;

		WSARecv(clientAddress->Socket, &clientAddress->WSA_Buffer[0], 1, &readCount, &flag, &clientAddress->Overlapped, nullptr);
	}

	return 0;
}