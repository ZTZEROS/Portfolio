#pragma once

enum LAN_LOCAL_CONSTANT
{
	LAN_SERVER_PORT = 6000,

	LAN_CLIENT_SET_COUNT = 2,
	LAN_CLIENT_COUNT = 64,
	TOTAL_LAN_CLIENT_COUNT = LAN_CLIENT_SET_COUNT * LAN_CLIENT_COUNT,
};

enum LAN_SUB_THREAD_INDEX
{
	LAN_SUB_THREAD_ACCEPT,
	//LAN_SUB_THREAD_RECEIVE,
	//LAN_SUB_THREAD_SEND,
	//LAN_SUB_THREAD_PROCESS_RECEIVED_PACKET,
	//LAN_SUB_THREAD_PROCESS_SENDING_PACKET,
	//LAN_SUB_THREAD_DISCONNECT,
	LAN_SUB_THREAD_WORK1,
	LAN_SUB_THREAD_WORK2,
	LAN_SUB_THREAD_WORK3,
	LAN_SUB_THREAD_WORK4,

	LAN_SUB_THREAD_COUNT
};

enum LAN_CLIENT_PHASE_INDEX
{
	LAN_CLIENT_PHASE_ACCEPT,
	LAN_CLIENT_PHASE_RECEIVE,
	LAN_CLIENT_PHASE_UPDATE,
	LAN_CLIENT_PHASE_SEND,
	LAN_CLIENT_PHASE_DISCONNECT,

	LAN_CLIENT_PHASE_COUNT
};

struct LanServerSession : public CommonSession
{
	UINT SessionKey;

	OVERLAPPED OverlappedReceive;
	OVERLAPPED OverlappedSend;

	ByteQueue ReceiveBQ;
	ByteQueue SendBQ;

	SerialByteQueue CommandSBQ;

	SRWLOCK LanServerLock;
};

struct LanClientSession : public CommonSession
{
	UINT SessionKey;

	OVERLAPPED OverlappedReceive;
	OVERLAPPED OverlappedSend;

	ByteQueue ReceiveBQ;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
	ByteQueue SendBQ;

	SerialByteQueue ReceiveSBQ;
	SerialByteQueue SendSBQ;

	LAN_CLIENT_PHASE_INDEX CurrentPhaseIndex;

	volatile UINT IO_Count;
	//volatile UINT ReceiveCount;
	//volatile UINT SendCount;

	SRWLOCK LanClientLock;
};

struct LanSubThread : public CommonSubThread
{

};

struct LanClientPlayer
{
	UINT SessionKey;

	SRWLOCK LanClientLock;
};

typedef list<LanClientSession> LanClientSessionL;
typedef list<LanClientSession>::iterator LanClientSessionLI;

typedef list<LanClientSession*> LanClientSessionAddressL;
typedef list<LanClientSession*>::iterator LanClientSessionAddressLI;

typedef vector<LanClientSession> LanClientSessionV;
typedef vector<LanClientSession>::iterator LanClientSessionVI;

typedef map<UINT, LanClientSession> LanClientSessionM;
typedef map<UINT, LanClientSession>::iterator LanClientSessionMI;

typedef map<UINT, LanClientSession*> LanClientSessionAddressM;
typedef map<UINT, LanClientSession*>::iterator LanClientSessionAddressMI;

typedef unordered_map<UINT, LanClientSession> LanClientSessionUM;
typedef unordered_map<UINT, LanClientSession>::iterator LanClientSessionUMI;

typedef unordered_map<UINT, LanClientSession*> LanClientSessionAddressUM;
typedef unordered_map<UINT, LanClientSession*>::iterator LanClientSessionAddressUMI;

typedef vector<LanSubThread> LanSubThreadV;
typedef vector<LanSubThread>::iterator LanSubThreadVI;

typedef map<UINT, LanClientPlayer> LanClientPlayerM;
typedef map<UINT, LanClientPlayer>::iterator LanClientPlayerMI;

typedef unordered_map<UINT, LanClientPlayer> LanClientPlayerUM;
typedef unordered_map<UINT, LanClientPlayer>::iterator LanClientPlayerUMI;