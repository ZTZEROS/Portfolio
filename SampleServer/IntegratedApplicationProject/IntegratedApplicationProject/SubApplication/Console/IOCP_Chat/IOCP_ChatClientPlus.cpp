#include "stdafx.h"
#include "IOCP_ChatClient.h"

bool InitializeIOCP_ChatClient()
{
	WSADATA wsaDatum;
	SOCKADDR_IN serverAddress;
	LPCWSTR ipAddress;

	{
		CHECK_ERROR(WSAStartup(0x0202, &wsaDatum));

		IOCP_CHAT_CLIENT.Socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
		CHECK_INVALID_SOCKET(IOCP_CHAT_CLIENT.Socket);

		//ipAddress = L"127.0.0.1";

		ZeroMemory(&serverAddress, sizeof(serverAddress));
		serverAddress.sin_family = AF_INET;
		InetPton(AF_INET, L"127.0.0.1", &serverAddress.sin_addr);
		serverAddress.sin_port = htons(IOCP_CHAT_SERVER_PORT);
	}

	TIMER.Initialize();

	srand(time(nullptr));

	{
		CHECK_SOCKET_ERROR(connect(IOCP_CHAT_CLIENT.Socket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)))
	}

	return 0;
}

bool UpdateIOCP_ChatClient()
{
	char receivingBuffer[BUFFER_SIZE];
	char sendingBuffer[BUFFER_SIZE];
	int receivingSize;
	int sendingSize;
	int receivedSize;
	int sendedSize;

	ZeroMemory(receivingBuffer, sizeof(receivingBuffer));
	ZeroMemory(sendingBuffer, sizeof(sendingBuffer));

	scanf_s("%s", sendingBuffer);
	sendingSize = strlen(sendingBuffer);

	sendedSize = send(IOCP_CHAT_CLIENT.Socket, sendingBuffer, sendingSize, 0);
	if (sendedSize < 1) return 1;
	else
	{
		receivingSize = sendedSize;

		receivedSize = recv(IOCP_CHAT_CLIENT.Socket, receivingBuffer, receivingSize, 0);
		if (receivedSize < 1) return 1;
		else
		{
			if (sendedSize == receivedSize)
			{
				//receivingBuffer[receivedSize] = '\0';
				//memcpy(BUFFER, receivingBuffer, receivedSize + 1);

				memcpy(BUFFER, receivingBuffer, receivedSize);
				BUFFER[receivedSize] = '\0';
			}
		}
	}

	return 0;
}

bool RenderIOCP_ChatClient()
{
	printf("%s" CRALF, BUFFER);

	return 0;
}

bool TerminalizeIOCP_ChatClient()
{
	closesocket(IOCP_CHAT_CLIENT.Socket);

	return 0;
}