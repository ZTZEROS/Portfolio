#pragma once

class BaseLanServer
{
private:
	static LanServerSession LSS; //LanServerSession;
	static LanClientSessionV LCSV; //LanClientSessionV;
	static LanClientSessionAddressM LCSAM;
	static LanSubThreadV LAN_SUB_THREAD_V;

	static BOOL LS_SHUTDOWN;
	static UINT CURRENT_SESSION_KEY;

	static HANDLE LSCP; //LanServerCompletionPort;
	//static HANDLE LS_RECEIVE_CP;
	//static HANDLE LS_UPDATE_CP;
	//static HANDLE LS_SEND_CP;
	//static HANDLE LS_DISCONNECT_CP;

	static SRWLOCK LCSAM_LOCK;

	static BaseLanServer* THIS;

public:
	BaseLanServer();
	virtual ~BaseLanServer();

	bool Initialize();

	//virtual bool Update() = 0;
	//virtual bool Render() = 0;

	bool Terminalize();



	bool Start(LPCWSTR ipAddress = L"127.0.0.1", unsigned int port = LAN_SERVER_PORT, unsigned int workThreadCount = LAN_SUB_THREAD_COUNT, bool nagleOff = 1, unsigned int maximumLanClientCount = TOTAL_LAN_CLIENT_COUNT);
	void Stop();

	bool Disconnect(unsigned int sessionKey);
	bool ReceivePacket(unsigned int sessionKey, SerialByteQueue& sendingPacketSBQ);
	bool SendPacket(unsigned int sessionKey, SerialByteQueue& sendingPacketSBQ);

	static unsigned int WINAPI AcceptLanClient(LPVOID argument);
	static unsigned int WINAPI WorkLanServer(LPVOID argument);

	virtual void OnClientJoin(SOCKADDR_IN clientAddress, unsigned int sessionKey) = 0;
	virtual void OnClientLeave(unsigned int sessionKey) = 0;
	virtual bool OnConnectionRequest(SOCKADDR_IN clientAddress, unsigned int port) = 0;

	virtual void OnRecv(unsigned int sessionKey, SerialByteQueue& sendingPacketSBQ) = 0;
	virtual void OnSend(unsigned int sessionKey, unsigned int sendedSize) = 0;
	virtual void OnWorkerThreadBegin() = 0;
	virtual void OnWorkerThreadEnd() = 0;

	virtual void OnError(unsigned int errorIndex, wchar_t* errorText) = 0;

	int GetClientCount() { return LCSAM.size(); }
};