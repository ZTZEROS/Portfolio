#pragma once

class BaseLAN_Server
{
private:
	static AnotherLAN_ServerSession ALAN_SS; //AnotherLAN_ServerSession;
	static AnotherLAN_ClientSessionV ALAN_CSV; //AnotehrLAN_ClientSessionV;
	static AnotherLAN_SubThreadV ALAN_SUB_THREAD_V; //AnotherLAN_SubThreadV;

	static BOOL ALAN_S_SHUTDOWN;
	static UINT CURRENT_SESSION_KEY;

	static HANDLE ALAN_S_COMMON_CP; //AdvancedLanServerCommonCompletionPort;
	//static HANDLE ALAN_S_ACCEPT_CP;
	//static HANDLE ALAN_S_RECEIVE_CP;
	//static HANDLE ALAN_S_UPDATE_CP;
	//static HANDLE ALAN_S_SEND_CP;
	static HANDLE ALAN_S_DISCONNECT_CP;

	static SRWLOCK ALAN_CSAM_LOCK;

	static BaseLAN_Server* THIS;

public:
	static UINT SBQ_ALLOCATION_COUNT;
	static UINT SBQ_DEALLOCATION_COUNT;

	static UINT GQCS_DISCONNECTION_COUNT;
	static UINT RCV_DISCONNECTION_COUNT;
	static UINT SND_DISCONNECTION_COUNT;

	static UINT ACCEPT_LEAK_COUNT;
	static UINT DISCONNECT_LEAK_COUNT;
	static UINT RCV_LEAK;
	static UINT SND_LEAK;
	static UINT RCV_GHOST_SESSION_COUNT;
	static UINT SND_GHOST_SESSION_COUNT;
	static UINT PACKET_ERROR;
	static UINT SLOW_DISCONNECTION_COUNT;

public:
	BaseLAN_Server();
	virtual ~BaseLAN_Server();

	bool Initialize();
	//virtual bool Update() = 0;
	//virtual bool Render() = 0;
	bool Terminalize();



	static unsigned int WINAPI AcceptAnotherLAN_Client(LPVOID instanceAddress);
	static unsigned int WINAPI DisconnectAnotherLAN_Client(LPVOID instanceAddress);
	static unsigned int WINAPI WorkAnotherLAN_Server(LPVOID instanceAddress);

	//static unsigned int WINAPI ReceiveAdvancedLAN_Server(LPVOID selfInstanceAddress);
	//static unsigned int WINAPI SendAdvancedLAN_Server(LPVOID selfInstanceAddress);
	//static unsigned int WINAPI CompleteAdvancedLAN_Server(LPVOID selfInstanceAddress);



	bool ReceivePacket(UINT64 sessionKey, SerialPacketQueue<AnotherLAN_PacketHeader>& receivedPacketSPQ);
	bool ReceivePacket(UINT64 sessionKey, SerialPacketQueue<AnotherLAN_PacketHeader>* receivedPacketSPQ_Address);
	bool SendPacket(UINT64 sessionKey, SerialPacketQueue<AnotherLAN_PacketHeader>& sendingPacketSPQ);
	bool SendPacket(UINT64 sessionKey, SerialPacketQueue<AnotherLAN_PacketHeader>* sendingPacketSPQ_Address);

	bool Start(LPCWSTR ipAddress = L"127.0.0.1", unsigned int port = ANOTHER_LAN_SERVER_PORT, unsigned int workThreadCount = ANOTHER_LAN_SUB_THREAD_COUNT, bool nagleOff = 1, unsigned int maximumLanClientCount = TOTAL_ANOTHER_LAN_CLIENT_COUNT);
	void Stop();
	bool Disconnect(UINT64 sessionKey);

	

	virtual void OnClientJoin(SOCKADDR_IN remoteClientAddress, UINT64 sessionKey) = 0;
	virtual void OnClientLeave(UINT64 sessionKey) = 0;
	virtual bool OnConnectionRequest(SOCKADDR_IN clientAddress, UINT port) = 0;

	virtual void OnRecv(UINT64 sessionKey, UINT receivedSize) = 0;
	virtual void OnSend(UINT64 sessionKey, UINT sendedSize) = 0;
	virtual void OnWorkerThreadBegin() = 0;
	virtual void OnWorkerThreadEnd() = 0;

	virtual void OnError(UINT errorIndex, wchar_t* errorText) = 0;
};