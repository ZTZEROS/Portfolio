#pragma once

enum ADVANCED_LAN_LOCAL_CONSTANT
{
	ADVANCED_LAN_SESSION_KEY_BIT_COUNT = 64, //sizeof(UINT64) * 8
	ADVANCED_LAN_CLIENT_INDEX_BIT_COUNT = 32, //sizeof(UINT) * 8
	ADVANCED_LAN_CLIENT_INDEX_SHIFTED_BIT_COUNT = ADVANCED_LAN_SESSION_KEY_BIT_COUNT - ADVANCED_LAN_CLIENT_INDEX_BIT_COUNT,

	ADVANCED_LAN_SERVER_PORT = 6000,

	ADVANCED_LAN_SERVER_WSABUF_COUNT = 256,

	ADVANCED_LAN_CLIENT_SET_COUNT = 2,
	ADVANCED_LAN_CLIENT_COUNT = 64,
	TOTAL_ADVANCED_LAN_CLIENT_COUNT = ADVANCED_LAN_CLIENT_SET_COUNT * ADVANCED_LAN_CLIENT_COUNT,
};

enum ADVANCED_LAN_SUB_THREAD_INDEX
{
	ADVANCED_LAN_SUB_THREAD_ACCEPT,
	//ADVANCED_LAN_SUB_THREAD_RECEIVE,
	//ADVANCED_LAN_SUB_THREAD_SEND,
	//ADVANCED_LAN_SUB_THREAD_PROCESS_RECEIVED_PACKET,
	//ADVANCED_LAN_SUB_THREAD_PROCESS_SENDING_PACKET,
	ADVANCED_LAN_SUB_THREAD_DISCONNECT,
	ADVANCED_LAN_SUB_THREAD_WORK1,
	ADVANCED_LAN_SUB_THREAD_WORK2,
	ADVANCED_LAN_SUB_THREAD_WORK3,
	ADVANCED_LAN_SUB_THREAD_WORK4,
	//ADVANCED_LAN_SUB_THREAD_WORK5,
	//ADVANCED_LAN_SUB_THREAD_WORK6,

	ADVANCED_LAN_SUB_THREAD_COUNT
};

enum ADVANCED_LAN_CLIENT_PHASE_INDEX
{
	ADVANCED_LAN_CLIENT_PHASE_ACCEPT,
	ADVANCED_LAN_CLIENT_PHASE_RECEIVE,
	ADVANCED_LAN_CLIENT_PHASE_UPDATE,
	ADVANCED_LAN_CLIENT_PHASE_SEND,
	ADVANCED_LAN_CLIENT_PHASE_DISCONNECT,

	ADVANCED_LAN_CLIENT_PHASE_COUNT
};



#pragma pack(push, 1)

struct AdvancedLAN_PacketHeader
{
	//BYTE IdentificationCode;
	WORD BodySize; //PacketBodySize;
	//BYTE CryptoCode; //XorCode; //XorCryptoCode;
	//BYTE Checksum;
};

struct AdvancedLAN_PacketBody
{

};

struct AdvancedLAN_PacketTrailer
{

};

struct AdvancedLAN_Packet
{
	AdvancedLAN_PacketHeader Header;
	AdvancedLAN_PacketBody Body;
	AdvancedLAN_PacketTrailer Trailer;
};

#pragma pack(pop)



struct AdvancedLAN_ServerSession : public CommonSession
{
	OVERLAPPED OverlappedReceive;
	OVERLAPPED OverlappedSend;

	ByteQueue ReceiveBQ;
	ByteQueue SendBQ;

	SerialByteQueue CommandSBQ;

	SRWLOCK LanServerLock;
};

struct AdvancedLAN_ClientSession : public CommonSession
{
	OVERLAPPED OverlappedReceive;
	OVERLAPPED OverlappedSend;

	Queue<SerialPacketQueue<AdvancedLAN_PacketHeader>*> ReceiveQ;
	Queue<SerialPacketQueue<AdvancedLAN_PacketHeader>*> SendQ;

	ByteQueue ReceiveBQ;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
	ByteQueue SendBQ;

	volatile UINT IO_Count;
	//volatile UINT ReceiveCount;
	//volatile UINT SendCount;

	SRWLOCK Lock;
	
	ADVANCED_LAN_CLIENT_PHASE_INDEX CurrentPhaseIndex;

	UINT AccumulatedUseCount;
};

struct AdvancedLAN_SubThread : public CommonSubThread
{

};

struct AdvancedLAN_ClientPlayer
{
	UINT64 SessionKey;

	SRWLOCK Lock;
};

typedef vector<AdvancedLAN_ClientSession> AdvancedLAN_ClientSessionV;
typedef vector<AdvancedLAN_ClientSession>::iterator AdvancedLAN_ClientSessionVI;

typedef vector<AdvancedLAN_SubThread> AdvancedLAN_SubThreadV;
typedef vector<AdvancedLAN_SubThread>::iterator AdvancedLAN_SubThreadVI;

typedef list<AdvancedLAN_ClientPlayer> AdvancedLAN_ClientPlayerL;
typedef list<AdvancedLAN_ClientPlayer>::iterator AdvancedLAN_ClientPlayerLI;