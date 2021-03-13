#include "stdafx.h"
#include "Monitor.h"
#include "MonitorInnerServer.h"

void MonitorInnerServer::OnClientJoin(SOCKADDR_IN clientAddress, UINT64 sessionKey)
{
	InterlockedIncrement(&CurrentClientCount);
	++ConnectedClientCount;

	return;
}

void MonitorInnerServer::OnClientLeave(UINT64 sessionKey)
{
	InterlockedDecrement(&CurrentClientCount);
	++DisconnectedClientCount;

	return;
}

bool MonitorInnerServer::OnConnectionRequest(SOCKADDR_IN clientAddress, UINT port)
{
	return 0;
}

void MonitorInnerServer::OnRecv(UINT64 sessionKey, UINT receivedSize)
{
	while (1)
	{
		IOCP_PacketHeader receivedPacketHeader;
		SerialPacketQueue<IOCP_PacketHeader> receivedPacketSPQ;
		InnerWork receivedWork;

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

void MonitorInnerServer::OnSend(UINT64 sessionKey, UINT sendedSize)
{
	InterlockedIncrement(&SendCount);
	InterlockedExchange(&SendedSize, SendedSize + sendedSize);

	return;
}

void MonitorInnerServer::OnWorkerThreadBegin()
{
	return;
}

void MonitorInnerServer::OnWorkerThreadEnd()
{
	return;
}

void MonitorInnerServer::OnError(UINT errorIndex, LPCWSTR errorText)
{
	wprintf(L"[Error %u] %s" CRALF, errorIndex, errorText);

	return;
}