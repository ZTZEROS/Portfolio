#include "stdafx.h"
#include "AdvancedChat.h"
#include "ChatLAN_Client.h"

void ChatLAN_Client::OnClientJoin(SOCKADDR_IN clientAddress, UINT64 sessionKey)
{
	InterlockedIncrement(&CurrentClientCount);
	++ConnectedClientCount;

	ACCS.SessionKey = sessionKey;
	ACCS.PreviousHeartbeatTime = timeGetTime();
	InitializeSRWLock(&ACCS.Lock);

	return;
}

void ChatLAN_Client::OnClientLeave(UINT64 sessionKey)
{
	InterlockedDecrement(&CurrentClientCount);
	++DisconnectedClientCount;

	ACCS.SessionKey = 0xffffffffffffffff;
	ACCS.PreviousHeartbeatTime = 0;

	return;
}

bool ChatLAN_Client::OnConnectionRequest(SOCKADDR_IN clientAddress, UINT port)
{
	return 0;
}

void ChatLAN_Client::OnRecv(UINT64 sessionKey, UINT receivedSize)
{
	while (1)
	{
		IOCP_PacketHeader receivedPacketHeader;
		SerialPacketQueue<IOCP_PacketHeader> receivedPacketSPQ;
		AdvancedChatWork work;

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
	}

	InterlockedIncrement(&ReceiveCount);
	InterlockedExchange(&ReceivedSize, ReceivedSize + receivedSize);

	return;
}

void ChatLAN_Client::OnSend(UINT64 sessionKey, UINT sendedSize)
{
	InterlockedIncrement(&SendCount);
	InterlockedExchange(&SendedSize, SendedSize + sendedSize);

	return;
}

void ChatLAN_Client::OnWorkerThreadBegin()
{
	return;
}

void ChatLAN_Client::OnWorkerThreadEnd()
{
	return;
}

void ChatLAN_Client::OnError(UINT errorIndex, LPCWSTR errorText)
{
	wprintf(L"[Error %u] %s" CRALF, errorIndex, errorText);

	return;
}