#include "stdafx.h"
#include "AdvancedLogin.h"
#include "LoginLAN_Server.h"

void LoginLAN_Server::OnClientJoin(SOCKADDR_IN clientAddress, UINT64 sessionKey)
{
	InterlockedIncrement(&CurrentClientCount);
	++ConnectedClientCount;

	return;
}

void LoginLAN_Server::OnClientLeave(UINT64 sessionKey)
{
	InterlockedDecrement(&CurrentClientCount);
	++DisconnectedClientCount;

	return;
}

bool LoginLAN_Server::OnConnectionRequest(SOCKADDR_IN clientAddress, UINT port)
{
	return 0;
}

void LoginLAN_Server::OnRecv(UINT64 sessionKey, UINT receivedSize)
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

			//ProcessWork(receivedWork);
		}
	}

	InterlockedIncrement(&ReceiveCount);
	InterlockedExchange(&ReceivedSize, ReceivedSize + receivedSize);

	return;
}

void LoginLAN_Server::OnSend(UINT64 sessionKey, UINT sendedSize)
{
	InterlockedIncrement(&SendCount);
	InterlockedExchange(&SendedSize, SendedSize + sendedSize);

	return;
}

void LoginLAN_Server::OnWorkerThreadBegin()
{
	return;
}

void LoginLAN_Server::OnWorkerThreadEnd()
{
	return;
}

void LoginLAN_Server::OnError(UINT errorIndex, LPCWSTR errorText)
{
	wprintf(L"[Error %u] %s" CRALF, errorIndex, errorText);

	return;
}