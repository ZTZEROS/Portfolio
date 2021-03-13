#include "stdafx.h"
#include "HTTP_Client.h"

bool InitializeHTTP_Client()
{
	WSADATA wsaDatum;
	BOOL nagleOff;
	u_long nonblocked;
	LPCWSTR ipAddress;

	{
		CHECK_ERROR(WSAStartup(0x0202, &wsaDatum));

		HTTP_CLIENT.Socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
		CHECK_INVALID_SOCKET(HTTP_CLIENT.Socket);

		nagleOff = 1;
		CHECK_SOCKET_ERROR(setsockopt(HTTP_CLIENT.Socket, IPPROTO_TCP, TCP_NODELAY, (char*)&nagleOff, sizeof(nagleOff)));

		nonblocked = 1;
		CHECK_SOCKET_ERROR(ioctlsocket(HTTP_CLIENT.Socket, FIONBIO, &nonblocked));

		//ipAddress = L"127.0.0.1";

		ZeroMemory(&HTTP_SERVER.Address, sizeof(HTTP_SERVER.Address));
		HTTP_SERVER.Address.sin_family = AF_INET;
		InetPton(AF_INET, L"127.0.0.1", &HTTP_SERVER.Address.sin_addr);
		HTTP_SERVER.Address.sin_port = htons(HTTP_SERVER_PORT);
	}

	TIMER.Initialize();

	srand(time(nullptr));

	{
		
	}

	{
		while (1)
		{
			int result;

			result = connect(HTTP_CLIENT.Socket, (SOCKADDR*)&HTTP_SERVER.Address, sizeof(HTTP_SERVER.Address));
			if (result == SOCKET_ERROR)
			{
				FD_SET readSet;
				FD_SET writeSet;
				FD_SET exceptionSet;
				timeval timeValue;

				writeSet.fd_array[0] = HTTP_CLIENT.Socket;
				writeSet.fd_count = 1;
				exceptionSet.fd_array[0] = HTTP_CLIENT.Socket;
				exceptionSet.fd_count = 1;

				timeValue.tv_sec = 0;
				timeValue.tv_usec = 5000000;

				IF_SOCKET_ERROR(select(0, &readSet, &writeSet, &exceptionSet, &timeValue))
				ELSE
				{
					if (writeSet.fd_array[0] == HTTP_CLIENT.Socket) break;
					//else if (exceptionSet.fd_array[0] == HTTP_CLIENT.Socket) continue;
				}

			}
			else break;
		}

		nonblocked = 0;
		CHECK_SOCKET_ERROR(ioctlsocket(HTTP_CLIENT.Socket, FIONBIO, &nonblocked));
	}

	return 0;
}

bool UpdateHTTP_Client()
{
	SendHTTP_ClientPacket(HTTP_SERVER.Address, nullptr, nullptr);
	ReceiveHTTP_ServerPacket();

	return 0;
}

bool RenderHTTP_Client()
{
	return 0;
}

bool TerminalizeHTTP_Client()
{
	WSACleanup();

	return 0;
}

int SendHTTP_ClientPacket(SOCKADDR_IN serverAddress, char* filePath, char* content)
{
	//char sendingPacketHeader[BUFFER_SIZE]; //requestHeader
	//SerialByteQueue sendingPacketSBQ(sizeof(sendingPacketHeader), BUFFER_SIZE);
	char sendingPacket[BUFFER_SIZE]; 
	WCHAR ipAddress[16];
	unsigned int contentLength;

	ZeroMemory(sendingPacket, sizeof(sendingPacket));
	ZeroMemory(ipAddress, sizeof(ipAddress));

	InetNtop(AF_INET, &serverAddress.sin_addr, ipAddress, sizeof(ipAddress));
	contentLength = strlen(content);

	sprintf(sendingPacket, "POST http://%s/%s HTTP/1.1" CRALF, ipAddress, filePath);
	sprintf(sendingPacket, "%sUSER-Agent: XXXXXXX" CRALF, sendingPacket); //strcat_s(sendingPacketHeader, "User-Agent: XXXXXXX" CRALF);
	sprintf(sendingPacket, "%sHost: %s" CRALF, sendingPacket, ipAddress);
	sprintf(sendingPacket, "%sConnection: Close" CRALF, sendingPacket);
	sprintf(sendingPacket, "%sContent-Length: %d" CRALF CRALF, sendingPacket, contentLength);
	//("POST http://%s/%s HTTP/1.1" CRALF "User-Agent: XXXXXXX" CRALF "Host: %s" CRALF "Connection: Close" CRALF "Content-Length: %d" CRALF CRALF, ipAddress, filePath, ipAddress, contentLength);
	sprintf(sendingPacket, "%s%s", sendingPacket, content);

	IF_SOCKET_ERROR(send(HTTP_CLIENT.Socket, sendingPacket, strlen(sendingPacket), 0))

	return 0;
}

int ReceiveHTTP_ServerPacket(char* body)
{
	//char receivedPacketHeader[BUFFER_SIZE]; //responseHeader
	//SerialByteQueue receivedPacketSBQ(sizeof(receivedPacketHeader), BUFFER_SIZE);
	char receivedPacket[BUFFER_SIZE];
	char responseStatusCode[4]; //unsigned int responseStatusCode;
	char contentLength[8]; //unsigned int contentLength;
	char content[BUFFER_SIZE];
	char* needle;

	ZeroMemory(receivedPacket, sizeof(receivedPacket));
	ZeroMemory(responseStatusCode, sizeof(responseStatusCode));
	ZeroMemory(contentLength, sizeof(contentLength));

	IF_SOCKET_ERROR(recv(HTTP_CLIENT.Socket, receivedPacket, 256, 0))

	needle = strchr(receivedPacket, ' ');

	if(needle) memcpy(responseStatusCode, needle, 3);

	needle = strstr(receivedPacket, "Content-Length: ");
	
	if (needle)
	{
		for (unsigned int i = 0; *needle != ' '; ++i)
		{
			memcpy(&contentLength[i], needle, 1);
			++needle;
		}
	}
	
	needle = strstr(receivedPacket, CRALF CRALF);

	if (needle)
	{
		memcpy(content, needle, ?);
	}

	printf("responseStatustCode : %s", responseStatusCode);
	printf("contentLength : %s", contentLength);

	return 0;
}