#pragma once
#include "LoginLAN_Server.h"
#include "LoginLAN_Client.h"

class LoginWAN_Server : public IOCP_Server
{
	friend class LoginLAN_Server;
	friend class ChatLAN_Client;

private:
	static AdvancedLoginClientPlayerAddressL ALCPAL;
	static SRWLOCK ALCPAL_LOCK;

	static ThreadLocalPool<AdvancedLoginClientPlayer> ALCP_TLP;
	static ThreadLocalPool<AdvancedLoginWork> ALW_TLP;

	static ThreadLocalDatabaseHandler TLDH;

	Queue<UINT64> ConnectorSessionKeyQ; //ConnectedClientPlayerSessionKeyQ;
	Queue<UINT64> DisconnectorSessionKeyQ; //DisconnectedClientPlayerSessionKeyQ;
	LockFreeQueue<AdvancedLoginWork> WorkLFQ;

	wstring GameServerIP_Address;
	UINT GameServerPort;
	wstring ChatServerIP_Address;
	UINT ChatServerPort;

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

	//LoginLAN_Server LAN_Server;
	LoginLAN_Client LAN_Client;

public:
	LoginWAN_Server();
	virtual ~LoginWAN_Server();

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


	AdvancedLoginWork ProcessWork(AdvancedLoginWork receivedWork);
	AdvancedLoginWork ProcessRequest(AdvancedLoginClientPlayer* clientPlayerAddress, AdvancedLoginWork receivedWork);
	AdvancedLoginWork ProcessResponse(AdvancedLoginClientPlayer* clientPlayerAddress, AdvancedLoginWork sendingWork);
	//AdvancedLoginWork ProcessWork(AdvancedLoginClientPlayer* clientPlayerAddress, AdvancedLoginWork receivedWork);

	unsigned int UnpackPacketSPQ(SerialPacketQueue<IOCP_PacketHeader>* packetSPQ_Address); //UnpackReceivedPacket
	unsigned int PackPacketSPQ(SerialPacketQueue<IOCP_PacketHeader>* packetSPQ_Address); //PackSendingPacket

	//unsigned int Broadcast(AdvancedLoginWork sendingWork, UINT sectorIndexX, UINT sectorIndexY);
	//unsigned int Multicast(AdvancedLoginWork sendingWork, UINT sectorIndexX, UINT sectorIndexY);
	unsigned int Unicast(AdvancedLoginWork sendingWork);
	unsigned int Singlecast(AdvancedLoginWork sendingWork); //SelfCast(AdvancedWAN_Work sendingWork)
};
