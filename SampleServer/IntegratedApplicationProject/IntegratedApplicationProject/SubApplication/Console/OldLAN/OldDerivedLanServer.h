#pragma once

class DerivedLanServer : public BaseLanServer
{
private:
	LanClientPlayerM LCPM;

	SRWLOCK LCPM_LOCK;

	UINT CurrentClientCount;
	UINT ConnectionCount;
	UINT DisconnectionCount;
	UINT ReceiveCount; //PerSecond
	UINT SendCount; //PerSecond

public:
	DerivedLanServer();
	virtual ~DerivedLanServer();

	bool Initialize();
	bool Update();
	bool Render();
	bool Terminalize();

	virtual void OnClientJoin(SOCKADDR_IN clientAddress, unsigned int sessionKey);
	virtual void OnClientLeave(unsigned int sessionKey);
	virtual bool OnConnectionRequest(SOCKADDR_IN clientAddress, unsigned int port);

	virtual void OnRecv(unsigned int sessionKey, SerialByteQueue& sendingPacketSBQ);
	virtual void OnSend(unsigned int sessionKey, unsigned int sendedSize);
	virtual void OnWorkerThreadBegin();
	virtual void OnWorkerThreadEnd();

	virtual void OnError(unsigned int errorIndex, wchar_t* errorText);
};

