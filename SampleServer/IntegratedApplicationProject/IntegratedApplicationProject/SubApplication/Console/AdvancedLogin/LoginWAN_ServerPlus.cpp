#include "stdafx.h"
#include "AdvancedLogin.h"
#include "LoginWAN_Server.h"

void LoginWAN_Server::OnClientJoin(SOCKADDR_IN clientAddress, UINT64 sessionKey)
{
	ConnectorSessionKeyQ.Enqueue(sessionKey);

	InterlockedIncrement(&CurrentClientCount);
	++ConnectedClientCount;

	return;
}

void LoginWAN_Server::OnClientLeave(UINT64 sessionKey)
{
	DisconnectorSessionKeyQ.Enqueue(sessionKey);

	InterlockedDecrement(&CurrentClientCount);
	++DisconnectedClientCount;

	return;
}

bool LoginWAN_Server::OnConnectionRequest(SOCKADDR_IN clientAddress, UINT port)
{
	return 0;
}

void LoginWAN_Server::OnRecv(UINT64 sessionKey, UINT receivedSize)
{
	while (1)
	{
		IOCP_PacketHeader receivedPacketHeader;
		SerialPacketQueue<IOCP_PacketHeader> receivedPacketSPQ;
		AdvancedLoginWork work;

		UINT result;

		result = ReceivePacket(sessionKey, &receivedPacketSPQ);
		if (result)
		{
			work.SessionKey = sessionKey;

			//receivedPacketHeader = receivedPacketSPQ.GetHeader();

			InterlockedIncrement(&CURRENT_SPQ_COUNT);
			InterlockedIncrement(&ALLOCATED_SPQ_COUNT);
			work.PacketSPQ_Address = PACKET_SPQ_TLP.Allocate();
			*work.PacketSPQ_Address = receivedPacketSPQ;

			//work.PacketSPQ_Address->SetHeader(receivedPacketHeader);

			InterlockedIncrement(&CurrentWorkCount);
			InterlockedIncrement(&EnqueuedWorkCount);
			WorkLFQ.Enqueue(work);
		}
		else break;
	}

	InterlockedIncrement(&ReceiveCount);
	InterlockedExchange(&ReceivedSize, ReceivedSize + receivedSize);
	//InterlockedAdd();

	return;
}

void LoginWAN_Server::OnSend(UINT64 sessionKey, UINT sendedSize)
{
	Disconnect(sessionKey);

	InterlockedIncrement(&SendCount);
	InterlockedExchange(&SendedSize, SendedSize + sendedSize);

	return;
}

void LoginWAN_Server::OnWorkerThreadBegin()
{
	return;
}

void LoginWAN_Server::OnWorkerThreadEnd()
{
	return;
}

void LoginWAN_Server::OnError(UINT errorIndex, LPCWSTR errorText)
{
	wprintf(L"[Error %u] %s" CRALF, errorIndex, errorText);

	return;
}