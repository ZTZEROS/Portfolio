#pragma once
#include "MonitorInnerServer.h"

class MonitorOuterServer : public IOCP_OuterServer
{
	friend class MonitorInnerServer;

private:
	Queue<UINT64> ConnectorSessionKeyQ; //ConnectedMonitorClientSessionKeyQ
	Queue<UINT64> DisconnectorSessionKeyQ; //DisconnectedMonitorClientSessionKeyQ
	LockFreeQueue<MonitorWork> WorkLFQ;

	UINT CurrentClientCount;
	UINT ConnectedClientCount; //AccumulatedConnectionCount;
	UINT DisconnectedClientCount; //AccumulatedDisconnectionCount;

	UINT CurrentWorkCount;
	UINT EnqueuedWorkCount;
	UINT DequeuedWorkCount;

	UINT ReceiveCount;
	UINT ReceivedSize;
	UINT SendCount;
	UINT SendedSize;

	UINT CheckPoint[12];

	MonitorInnerServer InnerServer;

	wstring LoginKey;

protected:
	MonitorClientAddressL MCAL;
	SRWLOCK MCAL_LOCK;

	ThreadLocalPool<MonitorClient> MC_TLP;
	ThreadLocalPool<MonitorWork> MW_TLP;

	ThreadLocalDatabaseHandler TLDH;

public:
	MonitorOuterServer();
	virtual ~MonitorOuterServer();

	bool Initialize();
	bool Update();
	bool Render();
	bool Terminalize();



	virtual void OnClientJoin(SOCKADDR_IN clientAddress, UINT64 sessionKey);
	virtual void OnClientLeave(UINT64 sessionKey);
	virtual bool OnConnectionRequest(SOCKADDR_IN clientAddress, UINT port);

	virtual void OnRecv(UINT64 sessionKey, UINT receivedSize);
	virtual void OnSend(UINT64 sessionKey, UINT sendedSize);
	virtual void OnWorkerThreadBegin();
	virtual void OnWorkerThreadEnd();

	virtual void OnError(UINT errorIndex, LPCWSTR errorText);



	MonitorWork ProcessWork(MonitorClient* monitorClientAddress, MonitorWork receivedWork);

	unsigned int UnpackPacketSPQ(SerialPacketQueue<IOCP_PacketHeader>* packetSPQ_Address); //UnpackReceivedPacket //engine part
	unsigned int PackPacketSPQ(SerialPacketQueue<IOCP_PacketHeader>* packetSPQ_Address); //PackSendingPacket //engine part

	unsigned int Broadcast(MonitorWork sendingWork);
	unsigned int Multicast(MonitorWork sendingWork);
	unsigned int Unicast(MonitorWork sendingWork);
	unsigned int Singlecast(MonitorWork sendingWork); //SelfCast(AdvancedWAN_Work sendingWork)
};