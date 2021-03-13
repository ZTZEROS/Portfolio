#pragma once
#include "ChatLAN_Server.h"
#include "ChatLAN_Client.h"

class ChatWAN_Server : public IOCP_Server
{
	friend class ChatLAN_Server;
	friend class ChatLAN_Client;

private:
	static AdvancedChatClientPlayerAddressL ACCPAL;
	static SRWLOCK ACCPAL_LOCK;

	static AdvancedChatSector AC_SECTOR_A[ADVANCED_CHAT_SECTOR_HEIGHT][ADVANCED_CHAT_SECTOR_WIDTH];

	static ThreadLocalPool<AdvancedChatClientPlayer> ACCP_TLP;
	static ThreadLocalPool<AdvancedChatWork> ACW_TLP;

	static ThreadLocalDatabaseHandler TLDH;

	Queue<UINT64> ConnectorSessionKeyQ; //ConnectedClientPlayerSessionKeyQ; //LockFreeQueue<UINT64> ConnectorSessionKeyQ;
	Queue<UINT64> DisconnectorSessionKeyQ; //DisconnectedClientPlayerSessionKeyQ; //LockFreeQueue<UINT64> DisconnectorSessionkeyQ;
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

	ChatLAN_Server LAN_Server;
	//ChatLAN_Client LAN_Client;

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



	AdvancedChatWork ProcessRequest(AdvancedChatClientPlayer* clientPlayerAddress, AdvancedChatWork receivedWork);
	AdvancedChatWork ProcessResponse(AdvancedChatClientPlayer* clientPlayerAddress, AdvancedChatWork sendingWork);
	AdvancedChatWork ProcessWork(AdvancedChatClientPlayer* clientPlayerAddress, AdvancedChatWork receivedWork);

	unsigned int UnpackPacketSPQ(SerialPacketQueue<IOCP_PacketHeader>* packetSPQ_Address); //UnpackReceivedPacket //engine part
	unsigned int PackPacketSPQ(SerialPacketQueue<IOCP_PacketHeader>* packetSPQ_Address); //PackSendingPacket //engine part

	unsigned int Broadcast(AdvancedChatWork sendingWork, UINT sectorIndexX, UINT sectorIndexY);
	unsigned int Multicast(AdvancedChatWork sendingWork, UINT sectorIndexX, UINT sectorIndexY);
	unsigned int Unicast(AdvancedChatWork sendingWork);
	unsigned int Singlecast(AdvancedChatWork sendingWork); //SelfCast(AdvancedWAN_Work sendingWork)
};