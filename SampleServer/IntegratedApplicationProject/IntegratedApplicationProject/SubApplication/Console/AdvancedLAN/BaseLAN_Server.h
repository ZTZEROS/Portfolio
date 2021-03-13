#pragma once

class BaseLAN_Server
{
private:
	static AdvancedLAN_ServerSession ALAN_SS; //AdvancedLAN_ServerSession;
	static AdvancedLAN_ClientSessionV ALAN_CSV; //AdvancedLAN_ClientSessionV;
	static AdvancedLAN_SubThreadV ALAN_SUB_THREAD_V; //AdvancedLAN_SubThreadV;

	static BOOL ALAN_S_SHUTDOWN;
	static UINT CURRENT_ACCEPTION_COUNT;

	static HANDLE ALAN_S_COMMON_CP;
	//static HANDLE ALAN_S_ACCEPT_CP;
	//static HANDLE ALAN_S_RECEIVE_CP;
	//static HANDLE ALAN_S_UPDATE_CP;
	//static HANDLE ALAN_S_SEND_CP;
	static HANDLE ALAN_S_DISCONNECT_CP;

	static BaseLAN_Server* THIS;

public:
	static UINT SPQ_ALLOCATION_COUNT;
	static UINT SPQ_DEALLOCATION_COUNT;

	//static UINT RCV_DISCONNECTION_COUNT;
	//static UINT SND_DISCONNECTION_COUNT;

	//static UINT RCV_GHOST_SESSION_COUNT;
	//static UINT SND_GHOST_SESSION_COUNT;

	//static UINT DISCONNECTION_LEAK_COUNT;
	//static UINT RCV_LEAK_COUNT;
	//static UINT SND_LEAK_COUNT;

	//static UINT SLOW_DISCONNECTION_COUNT;

	//static UINT PACKET_ERROR;

public:
	BaseLAN_Server();
	virtual ~BaseLAN_Server();

	bool Initialize();
	//virtual bool Update() = 0;
	//virtual bool Render() = 0;
	bool Terminalize();



	static unsigned int WINAPI AcceptAdvancedLAN_Client(LPVOID selfInstanceAddress);
	static unsigned int WINAPI DisconnectAdvancedLAN_Client(LPVOID selfInstanceAddress);
	static unsigned int WINAPI WorkAdvancedLAN_Server(LPVOID selfInstanceAddress);



	unsigned int ReceivePacket(UINT64 sessionKey, SerialPacketQueue<AdvancedLAN_PacketHeader>* receivedPacketSPQ_Address);
	unsigned int SendPacket(UINT64 sessionKey, SerialPacketQueue<AdvancedLAN_PacketHeader>* sendingPacketSPQ_Address);

	unsigned int PostAccept(UINT64 sessionKey); //deprecated
	unsigned int PostReceive(UINT64 sessionKey); //deprecated
	unsigned int PostSend(UINT64 sessionKey);
	unsigned int PostDisconnect(UINT64 sessionKey);

	bool Start(LPCWSTR ipAddress = L"127.0.0.1", UINT port = ADVANCED_LAN_SERVER_PORT, UINT workThreadCount = ADVANCED_LAN_SUB_THREAD_COUNT, BOOL nagleOff = 1, UINT maximumLAN_ClientCount = TOTAL_ADVANCED_LAN_CLIENT_COUNT);
	void Stop();
	bool Disconnect(UINT64 sessionKey);



	virtual void OnClientJoin(SOCKADDR_IN remoteClientAddress, UINT64 sessionKey) = 0;
	virtual void OnClientLeave(UINT64 sessionKey) = 0;
	virtual bool OnConnectionRequest(SOCKADDR_IN clientAddress, UINT port) = 0;

	virtual void OnRecv(UINT64 sessionKey, UINT receivedSize) = 0;
	virtual void OnSend(UINT64 sessionkey, UINT sendedSize) = 0;
	virtual void OnWorkerThreadBegin() = 0;
	virtual void OnWorkerThreadEnd() = 0;

	virtual void OnError(UINT errorIndex, LPCWSTR errorText) = 0;
};