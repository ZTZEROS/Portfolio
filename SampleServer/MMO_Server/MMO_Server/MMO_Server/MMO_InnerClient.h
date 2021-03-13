#pragma once

class MMO_InnerClient : public IOCP_Client
{
private:
	MMO_InnerClientPlayer ICP;

	LockFreeQueue<MMO_Work> WorkLFQ;

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

public:
	MMO_InnerClient();
	virtual ~MMO_InnerClient();

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

	virtual void OnError(UINT errorNumber, LPCWSTR errorText);



	MMO_Work ProcessWork();

	unsigned int Broadcast(MMO_Work sendingWork);
	unsigned int Multicast(MMO_Work sendingWork);
	unsigned int Unicast(MMO_Work sendingWork);
	unsigned int Singlecast(MMO_Work sendingWork);
};