#pragma once

class BaseWAN_Server
{
private:
	static AdvancedWAN_ServerSession AWAN_SS;
	static AdvancedWAN_ClientSessionV AWAN_CSV;
	static AdvancedWAN_SubThreadV AWAN_SUB_THREAD_V;

	static BOOL SUB_THREAD_ON;
	static UINT CURRENT_ACCEPTION_COUNT;

	static HANDLE AWAN_S_COMMON_CP;
	//static HANDLE AWAN_S_ACCEPT_CP;
	//static HANDLE AWAN_S_RECEIVE_CP;
	//static HANDLE AWAN_S_UPDATE_CP;
	//static HANDLE AWAN_S_SEND_CP;
	static HANDLE AWAN_S_DISCONNECT_CP;

	static BaseWAN_Server* THIS;

public:
	static ThreadLocalPool<SerialPacketQueue<AdvancedWAN_PacketHeader>> PACKET_SPQ_TLP;
	
	static UINT SPQ_ALLOCATION_COUNT;
	static UINT SPQ_DEALLOCATION_COUNT;

	static BOOL THREAD_CONTEXT_SWITCH_ON;
	static BOOL INDICATOR_ON;

public:
	BaseWAN_Server();
	virtual ~BaseWAN_Server();

	bool Initialize();
	//virtual bool Update() = 0;
	//virtual bool Render() = 0;
	bool Terminalize();



	static unsigned int WINAPI AcceptAdvancedWAN_Client(LPVOID selfInstanceAddress);
	static unsigned int WINAPI DisconnectAdvancedWAN_Client(LPVOID selfInstanceAddress);
	static unsigned int WINAPI WorkAdvancedWAN_Server(LPVOID selfInstanceAddress);



	unsigned int ReceivePacket(UINT64 sessionKey, SerialPacketQueue<AdvancedWAN_PacketHeader>* receivedPacketSPQ_Address);
	unsigned int SendPacket(UINT64 sessionKey, SerialPacketQueue<AdvancedWAN_PacketHeader>* sendingPacketSPQ_Address);

	unsigned int PostAccept(UINT64 sessionKey); //deprecated
	unsigned int PostReceive(UINT64 sessionKey); //deprecated
	unsigned int PostSend(UINT64 sessionKey);
	unsigned int PostDisconnect(UINT64 sessionKey);

	bool Start(LPCWSTR ipAddress = L"127.0.0.1", UINT port = ADVANCED_WAN_CHAT_SERVER_PORT, UINT workThreadCount = ADVANCED_WAN_SUB_THREAD_COUNT, BOOL nagleOff = 1, UINT maximumLAN_ClientCount = TOTAL_ADVANCED_WAN_CLIENT_COUNT);
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