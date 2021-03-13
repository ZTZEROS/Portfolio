#include "stdafx.h"
#include "Monitor.h"
#include "MonitorOuterServer.h"

void MonitorOuterServer::OnClientJoin(SOCKADDR_IN clientAddress, UINT64 sessionKey)
{
	ConnectorSessionKeyQ.Enqueue(sessionKey);

	InterlockedIncrement(&CurrentClientCount);
	++ConnectedClientCount;

	return;
}

void MonitorOuterServer::OnClientLeave(UINT64 sessionKey)
{
	DisconnectorSessionKeyQ.Enqueue(sessionKey);

	InterlockedDecrement(&CurrentClientCount);
	++DisconnectedClientCount;
}

bool MonitorOuterServer::OnConnectionRequest(SOCKADDR_IN clientAddress, UINT port)
{
	return 0;
}

void MonitorOuterServer::OnRecv(UINT64 sessionKey, UINT receivedSize)
{
	while (1)
	{
		IOCP_PacketHeader receivedPacketHeader;
		SerialPacketQueue<IOCP_PacketHeader> receivedPacketSPQ;
		MonitorWork receivedWork;

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

			//receivedWork.PacketSPQ_Address->SetHeader(receivedPacketHeader);

			InterlockedIncrement(&CurrentWorkCount);
			InterlockedIncrement(&EnqueuedWorkCount);
			WorkLFQ.Enqueue(receivedWork);
		}
		else break;
	}

	InterlockedIncrement(&ReceiveCount);
	InterlockedExchange(&ReceivedSize, ReceivedSize + receivedSize); //danger InterlockedAdd()

	return;
}

void MonitorOuterServer::OnSend(UINT64 sessionKey, UINT sendedSize)
{
	InterlockedIncrement(&SendCount);
	InterlockedExchange(&SendedSize, SendedSize + sendedSize);
}

void MonitorOuterServer::OnWorkerThreadBegin()
{
	return;
}

void MonitorOuterServer::OnWorkerThreadEnd()
{
	return;
}

void MonitorOuterServer::OnError(UINT errorIndex, LPCWSTR errorText)
{
	wprintf(L"[Error %u] %s" CRALF, errorIndex, errorText);

	return;
}
