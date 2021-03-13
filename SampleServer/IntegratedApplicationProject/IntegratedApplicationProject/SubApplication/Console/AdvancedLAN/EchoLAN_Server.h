#pragma once

class EchoLAN_Server : public BaseLAN_Server
{
private:
	static AdvancedLAN_ClientPlayerL ALAN_CPL;
	static SRWLOCK ALAN_CPL_LOCK;

	UINT CurrentClientCount;
	UINT ConnectionCount; //AccumulatedConnectionCount;
	UINT DisconnectionCount; //AccumulatedDisconnectionCount;
	UINT ReceiveCount; //ReceiveCountPerSecond;
	UINT SendCount; //SendCountPerSecond;

public:
	EchoLAN_Server();
	virtual ~EchoLAN_Server();

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

	int GetCurrentClientCount() { return CurrentClientCount; }
};
