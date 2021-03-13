#pragma once

class ChatWAN_Server : public BaseWAN_Server
{
private:
	static AdvancedWAN_ClientPlayerAddressL AWAN_CPAL;
	static SRWLOCK AWAN_CPAL_LOCK;

	static AdvancedWAN_Sector AWAN_SECTOR[ADVANCED_WAN_CHAT_SECTOR_HEIGHT][ADVANCED_WAN_CHAT_SECTOR_WIDTH];

	LockFreeQueue<AdvancedWAN_Work> WorkLFQ;
	Queue<AdvancedWAN_ClientPlayer*> ConnectedCPAQ;
	Queue<AdvancedWAN_ClientPlayer*> DisconnectedCPAQ;

	UINT CurrentClientCount; 
	UINT ConnectionCount; //AccumulatedConnectionCount;
	UINT DisconnectionCount; //AccumulatedDisconnectionCount;

	UINT WorkLFQ_CurrentCount;
	UINT WorkLFQ_EnqueueCount;
	UINT WorkLFQ_DequeueCount;

	UINT ReceiveCount;
	UINT ReceivedSize;
	UINT SendCount;
	UINT SendedSize;

	UINT CheckPoint[12];

public:
	ChatWAN_Server();
	virtual ~ChatWAN_Server();

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



	WORD ProcessRequest(AdvancedWAN_ClientPlayer* clientPlayerAddress, AdvancedWAN_Work receivedWork);
	//WORD ProcessResponse(AdvancedWAN_ClientPlayer* clientPlayerAddress, AdvancedWAN_Work sendingWork);

	//AdvancedWAN_Work ProcessRequest(AdvancedWAN_ClientPlayer* clientPlayerAddress, WORD clientRequestIndex);
	AdvancedWAN_Work ProcessResponse(AdvancedWAN_ClientPlayer* clientPlayerAddress, WORD serverResponseIndex);

	unsigned int UnpackPacketSPQ(SerialPacketQueue<AdvancedWAN_PacketHeader>* packetSPQ_Address); //UnpackReceivedPacket
	unsigned int PackPacketSPQ(SerialPacketQueue<AdvancedWAN_PacketHeader>* packetSPQ_Address); //PackSendingPacket

	unsigned int Broadcast(AdvancedWAN_Work sendingWork, UINT sectorIndexX, UINT sectorIndexY);
	unsigned int Multicast(AdvancedWAN_Work sendingWork, UINT sectorIndexX, UINT sectorIndexY);
	unsigned int Unicast(AdvancedWAN_Work sendingWork);
	unsigned int Singlecast(AdvancedWAN_Work sendingWork); //SelfCast(AdvancedWAN_Work sendingWork)
};