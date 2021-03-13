#pragma once

class ChatLAN_Server : public IOCP_Server
{
	friend class ChatWAN_Server;
	friend class ChatLAN_Client;

private:
	static AdvancedChatClientPlayerAddressL ACCPAL;
	static SRWLOCK ACCPAL_LOCK;

	static ThreadLocalPool<AdvancedChatClientPlayer> ACCP_TLP;
	static ThreadLocalPool<AdvancedChatWork> ACW_TLP;

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
	ChatLAN_Server();
	virtual ~ChatLAN_Server();

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



	AdvancedChatWork ProcessWork(AdvancedChatWork receivedWork);
	//AdvancedChatWork ProcessRequest(AdvancedChatClientPlayer* clientPlayerAddress, AdvancedChatWork receivedWork);
	//AdvancedChatWork ProcessResponse(AdvancedChatClientPlayer* clientPlayerAddress, AdvancedChatWork sendingWork);
	//AdvancedChatWork ProcessWork(AdvancedChatClientPlayer* clientPlayerAddress, AdvancedChatWork receivedWork);

	unsigned int UnpackPacketSPQ(SerialPacketQueue<IOCP_PacketHeader>* packetSPQ_Address); //UnpackReceivedPacket
	unsigned int PackPacketSPQ(SerialPacketQueue<IOCP_PacketHeader>* packetSPQ_Address); //PackSendingPacket

	//unsigned int Broadcast(AdvancedChatWork sendingWork, UINT sectorIndexX, UINT sectorIndexY);
	//unsigned int Multicast(AdvancedChatWork sendingWork, UINT sectorIndexX, UINT sectorIndexY);
	unsigned int Unicast(AdvancedChatWork sendingWork);
	unsigned int Singlecast(AdvancedChatWork sendingWork); //SelfCast(AdvancedWAN_Work sendingWork)
};