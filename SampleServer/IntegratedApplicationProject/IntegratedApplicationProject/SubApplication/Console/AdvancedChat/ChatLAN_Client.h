#pragma once

class ChatLAN_Client : public IOCP_Client
{
	friend class ChatWAN_Server;
	friend class ChatLAN_Server;

private:
	static AdvancedChatClientServer ACCS;

	LockFreeQueue<AdvancedChatWork> WorkLFQ;

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
	ChatLAN_Client();
	virtual ~ChatLAN_Client();

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
};