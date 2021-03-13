#pragma once

class MonitorInnerServer : public IOCP_InnerServer
{
	friend class MonitorOuterServer;

private:
	Queue<UINT64> ConnectorSessionKeyQ; //ConnectedMonitorClientSessionKeyQ
	Queue<UINT64> DisconnectorSessionKeyQ; //DisconnectedMonitorClientSessionKeyQ
	LockFreeQueue<InnerWork> WorkLFQ;

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

protected:
	InnerClientAddressL ICAL;
	SRWLOCK ICAL_LOCK;

	ThreadLocalPool<InnerClient> IC_TLP;
	ThreadLocalPool<InnerWork> IW_TLP;

	//ThreadLocalDatabaseHandler TLDH;

public:
	MonitorInnerServer();
	virtual ~MonitorInnerServer();

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



	InnerWork ProcessWork(InnerClient* InnerClientAddress, InnerWork receivedWork);

	unsigned int UnpackPacketSPQ(SerialPacketQueue<IOCP_PacketHeader>* packetSPQ_Address); //UnpackReceivedPacket //engine part
	unsigned int PackPacketSPQ(SerialPacketQueue<IOCP_PacketHeader>* packetSPQ_Address); //PackSendingPacket //engine part

	unsigned int Broadcast(InnerWork sendingWork);
	unsigned int Multicast(InnerWork sendingWork);
	unsigned int Unicast(InnerWork sendingWork);
	unsigned int Singlecast(InnerWork sendingWork); //SelfCast(AdvancedWAN_Work sendingWork)
};