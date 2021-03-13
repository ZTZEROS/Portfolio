#pragma once

class MMO_DerivedServer : public MMO_BaseServer
{
private:
	MMO_DerivedClientV DCV;

	//ThreadLocalPool<MMO_DerivedClient> MMODC_TLP;
	//ThreadLocalPool<MMO_Work> MMOW_TLP;

	//ThreadLocalDatabaseHandler TLDH;

	Queue<UINT64> ConnectorSessionKeyQ; //ConnectedClientPlayerSessionKeyQ; //LockFreeQueue<UINT64> ConnectorSessionKeyQ;
	Queue<UINT64> DisconnectorSessionKeyQ; //DisconnectedClientPlayerSessionKeyQ; //LockFreeQueue<UINT64> DisconnectorSessionkeyQ;
	LockFreeQueue<MMO_Work> WorkLFQ;

	UINT CurrentClientCount;
	UINT ConnectedClientCount; //AccumulatedConnectionCount;
	UINT DisconnectedClientCount; //AccumulatedDisconnectionCount;

	//UINT CurrentWorkCount;
	//UINT EnqueuedWorkCount;
	//UINT DequeuedWorkCount;

	UINT CurrentReceiveCount;
	UINT ReceiveCount;
	UINT ReceivedSize;
	UINT CurrentSendCount;
	UINT SendCount;
	UINT SendedSize;

	UINT CurrentGameThreadUpdateCount;
	UINT CurrentAuthorizeThreadUpdateCount;

	UINT CheckPoint[12];

	//MMO_InnerServer InnerServer;
	MMO_InnerClient InnerClient;

public:
	MMO_DerivedServer();
	virtual ~MMO_DerivedServer();

	bool Initialize();
	bool Update();
	bool Render();
	bool Terminalize();

	virtual unsigned int OnAuthorizeThreadUpdate();
	virtual unsigned int OnGameThreadUpdate();

	virtual unsigned int OnConnect();
	virtual unsigned int OnReceive(UINT receivedSize);
	virtual unsigned int OnSend(UINT sendedSize);
	virtual unsigned int OnDisconnect();
};