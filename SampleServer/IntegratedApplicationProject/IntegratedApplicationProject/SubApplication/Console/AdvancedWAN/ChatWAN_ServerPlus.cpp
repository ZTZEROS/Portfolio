#include "stdafx.h"
#include "AdvancedWAN.h"
#include "BaseWAN_Server.h"
#include "ChatWAN_Server.h"

void ChatWAN_Server::OnClientJoin(SOCKADDR_IN clientAddress, UINT64 sessionKey)
{
	SerialPacketQueue<AdvancedWAN_PacketHeader>* sendingPacketSPQ_Address;
	AdvancedWAN_ClientPlayer* joinedClientPlayerAddress;

	joinedClientPlayerAddress = new AdvancedWAN_ClientPlayer;

	ZeroMemory(joinedClientPlayerAddress, sizeof(*joinedClientPlayerAddress));

	joinedClientPlayerAddress->SessionKey = sessionKey;
	InitializeSRWLock(&joinedClientPlayerAddress->Lock);

	//slow part
	AcquireSRWLockExclusive(&AWAN_CPAL_LOCK);
	AWAN_CPAL.push_back(joinedClientPlayerAddress);
	ReleaseSRWLockExclusive(&AWAN_CPAL_LOCK);

	InterlockedIncrement(&CurrentClientCount);
	++ConnectionCount;

	return;
}

void ChatWAN_Server::OnClientLeave(UINT64 sessionKey)
{
	AdvancedWAN_Work work;
	WORD type;

	type = ADVANCED_WAN_CHAT_CLIENT_REQUEST_LOGOUT;

	work.SessionKey = sessionKey;

	InterlockedIncrement(&SPQ_ALLOCATION_COUNT);
	//work.PacketSPQ_Address = new SerialPacketQueue<AdvancedWAN_PacketHeader>;
	work.PacketSPQ_Address = PACKET_SPQ_TLP.Allocate();

	*work.PacketSPQ_Address << type;

	PackPacketSPQ(work.PacketSPQ_Address);

	InterlockedIncrement(&WorkLFQ_CurrentCount);
	InterlockedIncrement(&WorkLFQ_EnqueueCount);
	WorkLFQ.Enqueue(work);

	InterlockedDecrement(&CurrentClientCount);
	++DisconnectionCount;

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
		AdvancedWAN_PacketHeader receivedPacketHeader;
		SerialPacketQueue<AdvancedWAN_PacketHeader> receivedPacketSPQ;
		AdvancedWAN_Work work;

		UINT result;

		result = ReceivePacket(sessionKey, &receivedPacketSPQ);
		if (result)
		{
			work.SessionKey = sessionKey;

			//receivedPacketHeader = receivedPacketSPQ.GetHeader();

			InterlockedIncrement(&SPQ_ALLOCATION_COUNT);
			//work.PacketSPQ_Address = new SerialPacketQueue<AdvancedWAN_PacketHeader>;
			work.PacketSPQ_Address = PACKET_SPQ_TLP.Allocate();
			*work.PacketSPQ_Address = receivedPacketSPQ;

			//work.PacketSPQ_Address->SetHeader(receivedPacketHeader);

			InterlockedIncrement(&WorkLFQ_CurrentCount);
			InterlockedIncrement(&WorkLFQ_EnqueueCount);
			WorkLFQ.Enqueue(work);
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
	return;
}