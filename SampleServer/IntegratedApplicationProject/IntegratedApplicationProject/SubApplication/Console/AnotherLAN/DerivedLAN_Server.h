#pragma once

class DerivedLAN_Server : public BaseLAN_Server
{
private:
	AnotherLAN_ClientPlayerL ALAN_CPL;

	SRWLOCK ALAN_CPL_LOCK;

	UINT CurrentClientCount;
	UINT MaximumClientCount;
	UINT ConnectionCount; //AccumulatedConnectionCount
	UINT DisconnectionCount; //AccumulatedDisconnectionCount;
	UINT ReceiveCount; //ReceiveCountPerSecond
	UINT SendCount; //SendCountPerSecond

public:
	DerivedLAN_Server();
	virtual ~DerivedLAN_Server();

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

	virtual void OnError(UINT errorIndex, LPWSTR errorText);

	int GetCurrentClientCount() { return CurrentClientCount; }
};

