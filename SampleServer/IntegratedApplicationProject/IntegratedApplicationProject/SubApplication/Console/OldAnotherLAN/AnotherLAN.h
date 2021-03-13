#pragma once

enum ANOTHER_LAN_LOCAL_CONSTANT
{
	ANOTHER_LAN_SESSION_KEY_BIT_COUNT = 64, //sizeof(UINT64) * 8
	ANOTHER_LAN_CLIENT_INDEX_BIT_COUNT = 32, //sizeof(UINT) * 8
	ANOTHER_LAN_CLIENT_INDEX_SHIFTED_BIT_COUNT = ANOTHER_LAN_SESSION_KEY_BIT_COUNT - ANOTHER_LAN_CLIENT_INDEX_BIT_COUNT,

	ANOTHER_LAN_SERVER_PORT = 6000,

	ANOTHER_LAN_SERVER_WSABUF_COUNT = 64,

	ANOTHER_LAN_CLIENT_SET_COUNT = 2,
	ANOTHER_LAN_CLIENT_COUNT = 64,
	TOTAL_ANOTHER_LAN_CLIENT_COUNT = ANOTHER_LAN_CLIENT_SET_COUNT * ANOTHER_LAN_CLIENT_COUNT,
};

enum ANOTHER_LAN_SUB_THREAD_INDEX
{
	ANOTHER_LAN_SUB_THREAD_ACCEPT,
	//ANOTHER_LAN_SUB_THREAD_RECEIVE,
	//ANOTHER_LAN_SUB_THREAD_SEND,
	//ANOTHER_LAN_SUB_THREAD_PROCESS_RECEIVED_PACKET,
	//ANOTHER_LAN_SUB_THREAD_PROCESS_SENDING_PACKET,
	ANOTHER_LAN_SUB_THREAD_DISCONNECT,
	ANOTHER_LAN_SUB_THREAD_WORK1,
	ANOTHER_LAN_SUB_THREAD_WORK2,
	ANOTHER_LAN_SUB_THREAD_WORK3,
	ANOTHER_LAN_SUB_THREAD_WORK4,

	ANOTHER_LAN_SUB_THREAD_COUNT
};

enum ANOTHER_LAN_CLIENT_PHASE_INDEX
{
	ANOTHER_LAN_CLIENT_PHASE_ACCEPT,
	ANOTHER_LAN_CLIENT_PHASE_RECEIVE,
	ANOTHER_LAN_CLIENT_PHASE_UPDATE,
	ANOTHER_LAN_CLIENT_PHASE_SEND,
	ANOTHER_LAN_CLIENT_PHASE_DISCONNECT,

	ANOTHER_LAN_CLIENT_PHASE_COUNT
};



#pragma pack(push, 1)

struct AnotherLAN_PacketHeader
{
	//BYTE IdentificationCode;
	WORD BodySize; //PacketBodySize;
	//BYTE CryptoCode; //XorCode; //XorCryptoCode;
	//BYTE Checksum;
};

struct AnotherLAN_PacketBody
{

};

struct AnotherLAN_PacketTrailer
{

};

struct AnotherLAN_Packet
{
	AnotherLAN_PacketHeader Header;
	AnotherLAN_PacketBody Body;
	AnotherLAN_PacketTrailer Trailer;
};

#pragma pack(pop)



struct AnotherLAN_ServerSession : public CommonSession
{
	OVERLAPPED OverlappedReceive;
	OVERLAPPED OverlappedSend;

	ByteQueue ReceiveBQ;
	ByteQueue SendBQ;

	SerialByteQueue CommandSBQ;

	SRWLOCK LanServerLock;
};

struct AnotherLAN_ClientSession : public CommonSession
{
	OVERLAPPED OverlappedReceive;
	OVERLAPPED OverlappedSend;

	Queue<SerialPacketQueue<AnotherLAN_PacketHeader>*> ReceiveQ;
	Queue<SerialPacketQueue<AnotherLAN_PacketHeader>*> SendQ;

	ByteQueue ReceiveBQ;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
	ByteQueue SendBQ;

	ANOTHER_LAN_CLIENT_PHASE_INDEX CurrentPhaseIndex;

	volatile UINT IO_Count;
	//volatile UINT ReceiveCount;
	//volatile UINT SendCount;

	SRWLOCK LAN_ClientLock;
	
	UINT AccumulatedUseCount;
};

struct AnotherLAN_SubThread : public CommonSubThread
{

};

struct AnotherLAN_ClientPlayer
{
	UINT64 SessionKey;

	SRWLOCK LAN_ClientLock;
};

typedef vector<AnotherLAN_ClientSession> AnotherLAN_ClientSessionV;
typedef vector<AnotherLAN_ClientSession>::iterator AnotherLAN_ClientSessionVI;

typedef vector<AnotherLAN_SubThread> AnotherLAN_SubThreadV;
typedef vector<AnotherLAN_SubThread>::iterator AnotherLAN_SubThreadVI;

typedef list<AnotherLAN_ClientPlayer> AnotherLAN_ClientPlayerL;
typedef list<AnotherLAN_ClientPlayer>::iterator AnotherLAN_ClientPlayerLI;