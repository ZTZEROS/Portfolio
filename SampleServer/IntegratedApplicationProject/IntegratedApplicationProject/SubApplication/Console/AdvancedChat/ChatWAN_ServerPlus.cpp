#include "stdafx.h"
#include "AdvancedChat.h"
#include "ChatWAN_Server.h"

void ChatWAN_Server::OnClientJoin(SOCKADDR_IN clientAddress, UINT64 sessionKey)
{
	ConnectorSessionKeyQ.Enqueue(sessionKey);

	InterlockedIncrement(&CurrentClientCount);
	++ConnectedClientCount;

	return;
}

void ChatWAN_Server::OnClientLeave(UINT64 sessionKey)
{
	DisconnectorSessionKeyQ.Enqueue(sessionKey);

	InterlockedDecrement(&CurrentClientCount);
	++DisconnectedClientCount;

	return;
}

bool ChatWAN_Server::OnConnectionRequest(SOCKADDR_IN clientAddress, UINT port)
{
	return 0;
}

void ChatWAN_Server::OnRecv(UINT64 sessionKey, UINT receivedSize)
{
	while (1)
	{
		IOCP_PacketHeader receivedPacketHeader;
		SerialPacketQueue<IOCP_PacketHeader> receivedPacketSPQ;
		AdvancedChatWork receivedWork;

		UINT result;

		result = ReceivePacket(sessionKey, &receivedPacketSPQ);
		if (result)
		{
			receivedWork.SessionKey = sessionKey;

			//receivedPacketHeader = receivedPacketSPQ.GetHeader();

			InterlockedIncrement(&CURRENT_SPQ_COUNT);
			InterlockedIncrement(&ALLOCATED_SPQ_COUNT);
			receivedWork.PacketSPQ_Address = PACKET_SPQ_TLP.Allocate();
			*receivedWork.PacketSPQ_Address = receivedPacketSPQ;

			//work.PacketSPQ_Address->SetHeader(receivedPacketHeader);

			InterlockedIncrement(&CurrentWorkCount);
			InterlockedIncrement(&EnqueuedWorkCount);
			WorkLFQ.Enqueue(receivedWork);
		}
		else break;
	}

	InterlockedIncrement(&ReceiveCount);
	InterlockedExchange(&ReceivedSize, ReceivedSize + receivedSize);

	return;
}

void ChatWAN_Server::OnSend(UINT64 sessionKey, UINT sendedSize)
{
	InterlockedIncrement(&SendCount);
	InterlockedExchange(&SendedSize, SendedSize + sendedSize);

	return;
}

void ChatWAN_Server::OnWorkerThreadBegin()
{
	return;
}

void ChatWAN_Server::OnWorkerThreadEnd()
{
	return;
}

void ChatWAN_Server::OnError(UINT errorIndex, LPCWSTR errorText)
{
	wprintf(L"[Error %u] %s" CRALF, errorIndex, errorText);

	return;
}