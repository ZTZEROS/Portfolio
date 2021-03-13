#include "stdafx.h"
#include "AdvancedLogin.h"
#include "LoginLAN_Client.h"

void LoginLAN_Client::OnClientJoin(SOCKADDR_IN clientAddress, UINT64 sessionKey)
{
	InterlockedIncrement(&CurrentClientCount);
	++ConnectedClientCount;

	ALCS.SessionKey = sessionKey;
	ALCS.PreviousHeartbeatTime = timeGetTime();
	InitializeSRWLock(&ALCS.Lock);

	return;
}

void LoginLAN_Client::OnClientLeave(UINT64 sessionKey)
{
	InterlockedDecrement(&CurrentClientCount);
	++DisconnectedClientCount;

	ALCS.SessionKey = 0xffffffffffffffff;
	ALCS.PreviousHeartbeatTime = 0;

	return;
}

bool LoginLAN_Client::OnConnectionRequest(SOCKADDR_IN clientAddress, UINT port)
{
	return 0;
}

void LoginLAN_Client::OnRecv(UINT64 sessionKey, UINT receivedSize)
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

void LoginLAN_Client::OnSend(UINT64 sessionKey, UINT sendedSize)
{
	InterlockedIncrement(&SendCount);
	InterlockedExchange(&SendedSize, SendedSize + sendedSize);

	return;
}

void LoginLAN_Client::OnWorkerThreadBegin()
{
	return;
}

void LoginLAN_Client::OnWorkerThreadEnd()
{
	return;
}

void LoginLAN_Client::OnError(UINT errorIndex, LPCWSTR errorText)
{
	wprintf(L"[Error %u] %s" CRALF, errorIndex, errorText);

	return;
}