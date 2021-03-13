#include "pch.h"
#include "MMO.h"
#include "MMO_InnerClient.h"

void MMO_InnerClient::OnClientJoin(SOCKADDR_IN clientAddress, UINT64 sessionKey)
{
	InterlockedIncrement(&CurrentClientCount);
	++ConnectedClientCount;

	{
		MMO_Work sendingWork;
		WORD clientRequestIndex;
		INT serverIndex;

		sendingWork.SessionKey = sessionKey;
		InterlockedIncrement(&CURRENT_SPQ_COUNT);
		InterlockedIncrement(&ALLOCATED_SPQ_COUNT);
		sendingWork.PacketSPQ_Address = PACKET_SPQ_TLP.Allocate();

		clientRequestIndex = MONITOR_SERVER_REQUEST_LOGIN;
		serverIndex = MMO_SERVER_INDEX;

		*sendingWork.PacketSPQ_Address << clientRequestIndex;
		*sendingWork.PacketSPQ_Address << serverIndex;

		if (sendingWork.PacketSPQ_Address->PackPacket()) Unicast(sendingWork);

		if (!sendingWork.PacketSPQ_Address->DecreaseReferenceCount())
		{
			InterlockedDecrement(&CURRENT_SPQ_COUNT);
			InterlockedIncrement(&DEALLOCATED_SPQ_COUNT);
			PACKET_SPQ_TLP.Deallocate(sendingWork.PacketSPQ_Address);
		}
	}

	ICP.SessionKey = sessionKey;
	ICP.Index = MMO_SERVER_INDEX;

	ICP.PreviousHeartbeatTime = time(nullptr);

	InitializeSRWLock(&ICP.Lock);
}

void MMO_InnerClient::OnClientLeave(UINT64 sessionKey)
{
	InterlockedDecrement(&CurrentClientCount);
	++DisconnectedClientCount;

	return;
}

bool MMO_InnerClient::OnConnectionRequest(SOCKADDR_IN clientAddress, UINT port)
{
	return 0;
}

void MMO_InnerClient::OnRecv(UINT64 sessionKey, UINT receivedSize)
{
	while (1)
	{
		IOCP_PacketHeader receivedPacketHeader;
		SerialPacketQueue<IOCP_PacketHeader> receivedPacketSPQ;
		MMO_Work receivedWork;

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

			//ProcessWork(receivedWork);
		}
		else break;
	}

	InterlockedIncrement(&ReceiveCount);
	InterlockedExchange(&ReceivedSize, ReceivedSize + receivedSize);
}

void MMO_InnerClient::OnSend(UINT64 sessionKey, UINT sendedSize)
{
	InterlockedIncrement(&SendCount);
	InterlockedExchange(&SendedSize, SendedSize + sendedSize);
}

void MMO_InnerClient::OnWorkerThreadBegin()
{
	return;
}

void MMO_InnerClient::OnWorkerThreadEnd()
{
	return;
}

void MMO_InnerClient::OnError(UINT errorNumber, LPCWSTR errorText)
{
	wprintf(L"[Error %u] %s" CRALF, errorNumber, errorText);

	return;
}