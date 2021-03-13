#pragma once

class BaseLAN_Server
{
private:
	static AdvancedLAN_ServerSession ALAN_SS; //AdvancedLanServerSession;
	static AdvancedLAN_ClientSessionV ALAN_CSV; //AdvancedLanClientSessionV;
	static AdvancedLAN_SubThreadV ALAN_SUB_THREAD_V; //AdvancedLanSubThreadV;
	
	static LockFreeStack<UINT> ACCEPTABLE_ALAN_CS_INDEX_LFS; //AcceptableAdvancedLanClientSessionIndexLFS;

	static BOOL ALAN_S_SHUTDOWN;
	static UINT CURRENT_SESSION_KEY;

	static HANDLE ALAN_S_COMMON_CP; //AdvancedLanServerCommonCompletionPort;
	//static HANDLE ALAN_S_ACCEPT_CP;
	//static HANDLE ALAN_S_RECEIVE_CP;
	//static HANDLE ALAN_S_UPDATE_CP;
	//static HANDLE ALAN_S_SEND_CP;
	static HANDLE ALAN_S_DISCONNECT_CP;

	static SRWLOCK ALAN_CSAM_LOCK;

	static BaseLAN_Server* THIS; //InstanceSelfAddress;

public:
	static UINT SPQ_ALLOCATION_COUNT;
	static UINT SPQ_DEALLOCATION_COUNT;

public:
	BaseLAN_Server();
	virtual ~BaseLAN_Server();

	bool Initialize();
	//virtual bool Update() = 0;
	//virtual bool Render() = 0;
	bool Terminalize();

	unsigned int ReceivePacket(UINT64 sessionKey, SerialPacketQueue<AdvancedLAN_PacketHeader>& receivedPacketSPQ);
	unsigned int ReceivePacket(UINT64 sessionKey, SerialPacketQueue<AdvancedLAN_PacketHeader>* receivedPacketSPQ_Address);
	unsigned int SendPacket(UINT64 sessionKey, SerialPacketQueue<AdvancedLAN_PacketHeader>& sendingPacketSPQ);
	unsigned int SendPacket(UINT64 sessionKey, SerialPacketQueue<AdvancedLAN_PacketHeader>* sendingPacketSPQ_Address);



	bool Start(LPCWSTR ipAddress = L"127.0.0.1", unsigned int port = ADVANCED_LAN_SERVER_PORT, unsigned int workThreadCount = ADVANCED_LAN_SUB_THREAD_COUNT, bool nagleOff = 1, unsigned int maximumLanClientCount = TOTAL_ADVANCED_LAN_CLIENT_COUNT);
	void Stop();
	bool Disconnect(UINT64 sessionKey);



	static unsigned int WINAPI AcceptAdvancedLAN_Client(LPVOID instanceAddress);
	static unsigned int WINAPI DisconnectAdvancedLAN_Client(LPVOID instanceAddress);
	static unsigned int WINAPI WorkAdvancedLAN_Server(LPVOID instanceAddress);



	virtual void OnClientJoin(SOCKADDR_IN remoteClientAddress, UINT64 sessionKey) = 0;
	virtual void OnClientLeave(UINT64 sessionKey) = 0;
	virtual bool OnConnectionRequest(SOCKADDR_IN clientAddress, UINT port) = 0;

	virtual void OnRecv(UINT64 sessionKey, UINT receivedSize) = 0;
	virtual void OnSend(UINT64 sessionKey, UINT sendedSize) = 0;
	virtual void OnWorkerThreadBegin() = 0;
	virtual void OnWorkerThreadEnd() = 0;

	virtual void OnError(UINT errorIndex, wchar_t* errorText) = 0;
};