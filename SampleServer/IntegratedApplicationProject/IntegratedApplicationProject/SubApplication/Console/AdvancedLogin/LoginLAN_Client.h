#pragma once

class LoginLAN_Client : public IOCP_Client
{
	friend class LoginWAN_Server;
	friend class LoginLAN_Server;

private:
	static AdvancedLoginClientServer ALCS;

	LockFreeQueue<AdvancedLoginWork> WorkLFQ;

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
	LoginLAN_Client();
	virtual ~LoginLAN_Client();

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



	//AdvancedLoginWork ProcessWork(AdvancedLoginWork receivedWork);
	//AdvancedLoginWork ProcessRequest(AdvancedChatClientPlayer* clientPlayerAddress, AdvancedLoginWork receivedWork);
	//AdvancedLoginWork ProcessResponse(AdvancedChatClientPlayer* clientPlayerAddress, AdvancedLoginWork sendingWork);
	AdvancedLoginWork ProcessWork(AdvancedLoginClientPlayer* clientPlayerAddress, AdvancedLoginWork receivedWork);

	unsigned int UnpackPacketSPQ(SerialPacketQueue<IOCP_PacketHeader>* packetSPQ_Address); //UnpackReceivedPacket
	unsigned int PackPacketSPQ(SerialPacketQueue<IOCP_PacketHeader>* packetSPQ_Address); //PackSendingPacket

	//unsigned int Broadcast(AdvancedChatWork sendingWork, UINT sectorIndexX, UINT sectorIndexY);
	//unsigned int Multicast(AdvancedChatWork sendingWork, UINT sectorIndexX, UINT sectorIndexY);
	unsigned int Unicast(AdvancedLoginWork sendingWork);
	unsigned int Singlecast(AdvancedLoginWork sendingWork); //SelfCast(AdvancedWAN_Work sendingWork)
};
