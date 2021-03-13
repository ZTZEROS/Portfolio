#pragma once

class MMO_BaseClient;

class MMO_BaseServer
{
	friend class MMO_BaseClient;

private:
	MMO_ServerSession SS;
	
	IOCP_SubThreadV SUB_THREAD_V;

	Queue<SerialPacketQueue<IOCP_PacketHeader>*> ACCEPTED_CLIENT_Q;
	LockFreeQueue<SerialPacketQueue<IOCP_PacketHeader>*> ACCEPTED_CLIENT_LFQ;

	LockFreeStack<UINT> ACCEPTABLE_CS_INDEX_LFS; //AcceptableClientSessionIndexLFS;

	BOOL SUB_THREAD_ON;
	UINT CURRENT_ACCEPTION_COUNT;

	//HANDLE ACCEPT_CP;
	HANDLE DISCONNECT_CP;

	//HANDLE RECEIVE_CP;
	//HANDLE UPDATE_CP;
	HANDLE SEND_CP;

	//HANDLE AUTHORIZE_CP;
	//HANDLE GAME_CP;
	
	HANDLE WORK_CP;

protected:
	//MMO_ClientSessionV CSV;
	MMO_BaseClientAddressV BCAV;
	MMO_BaseClientAddressL BCAL;

	ThreadLocalPool<MMO_Work> MMOW_TLP;
	ThreadLocalPool<SerialPacketQueue<IOCP_PacketHeader>> SPQ_TLP;

	UINT CURRENT_SPQ_COUNT;
	UINT ALLOCATED_SPQ_COUNT;
	UINT DEALLOCATED_SPQ_COUNT;
	
	UINT CURRENT_SPQ_CHUNK_COUNT;
	UINT MAXIMUM_SPQ_CHUNK_COUNT;

	BOOL THREAD_CONTEXT_SWITCH_ON;
	BOOL INDICATOR_ON;

	wstring IP_ADDRESS;
	UINT PORT;
	UINT WORK_THREAD_COUNT;
	UINT MAXIMUM_CLIENT_COUNT;

public:
	MMO_BaseServer();
	virtual ~MMO_BaseServer();

	bool Initialize();
	bool Update();
	bool Render();
	bool Terminalize();



	static unsigned int WINAPI AcceptThread(LPVOID selfInstanceAddress);
	static unsigned int WINAPI DisconnectThread(LPVOID selfInstanceAddress);
	
	static unsigned int WINAPI SendThread(LPVOID selfInstanceAddress);
	
	static unsigned int WINAPI AuthorizeThread(LPVOID selfInstanceAddress);
	static unsigned int WINAPI GameThread(LPVOID selfInstanceAddress);

	static unsigned int WINAPI WorkThread(LPVOID selfInstanceAddress);




	unsigned int AcceptThread();
	unsigned int DisconnectThread();

	unsigned int SendThread();

	unsigned int AuthorizeThread();
	unsigned int GameThread();

	unsigned int WorkThread();



	bool Disconnect(UINT64 sessionKey);

	unsigned int PostSend(UINT64 sessionKey);
	unsigned int PostDisconnect(UINT64 sessionKey);



	SerialPacketQueue<IOCP_PacketHeader>* SPQ_TLP_Allocate();
	unsigned int SPQ_TLP_Deallocate(SerialPacketQueue<IOCP_PacketHeader>* serialPacketQueueAddress);



	virtual unsigned int OnAuthorizeThreadUpdate() = 0;
	virtual unsigned int OnGameThreadUpdate() = 0;

	virtual unsigned int OnConnect() = 0;
	virtual unsigned int OnReceive(UINT receivedSize) = 0;
	virtual unsigned int OnSend(UINT sendedSize) = 0;
	virtual unsigned int OnDisconnect() = 0;
};