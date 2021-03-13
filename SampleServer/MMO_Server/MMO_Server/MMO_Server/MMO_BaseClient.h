#pragma once

class MMO_BaseServer;

class MMO_BaseClient
{
	friend MMO_BaseServer;

protected:
	MMO_ClientSession CS;

	MMO_BaseServer* BS_Address;

public:
	MMO_BaseClient();
	virtual ~MMO_BaseClient();

	bool Initialize();
	bool Update();
	bool Render();
	bool Terminalize();



	unsigned int ReceivePacket(SerialPacketQueue<IOCP_PacketHeader>* receivedPacketSPQ_Address);
	unsigned int SendPacket(SerialPacketQueue<IOCP_PacketHeader>* sendingPacketSPQ_Address);

	SerialPacketQueue<IOCP_PacketHeader>* SPQ_TLP_Allocate();
	unsigned int SPQ_TLP_Deallocate(SerialPacketQueue<IOCP_PacketHeader>* serialPacketQueueAddress);



	virtual unsigned int OnAT_ClientJoin() = 0;
	virtual unsigned int OnAT_ClientReceive() = 0;
	virtual unsigned int OnAT_ClientLeave() = 0;

	virtual unsigned int OnGT_ClientJoin() = 0;
	virtual unsigned int OnGT_ClientReceive() = 0;
	virtual unsigned int OnGT_ClientLeave() = 0;
	virtual unsigned int OnGT_ClientDisconnect() = 0;



	void SetBS_Address(MMO_BaseServer* bsAddress) { BS_Address = bsAddress; }
};

typedef vector<MMO_BaseClient> MMO_BaseClientL;
typedef vector<MMO_BaseClient>::iterator MMO_BaseClientLI;

typedef vector<MMO_BaseClient> MMO_BaseClientV;
typedef vector<MMO_BaseClient>::iterator MMO_BaseClientVI;

typedef vector<MMO_BaseClient*> MMO_BaseClientAddressL;
typedef vector<MMO_BaseClient*>::iterator MMO_BaseClientAddressLI;

typedef vector<MMO_BaseClient*> MMO_BaseClientAddressV;
typedef vector<MMO_BaseClient*>::iterator MMO_BaseClientAddressVI;