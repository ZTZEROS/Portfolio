#pragma once
#include "..\Network.h"

enum IOCP_LOCAL_CONSTANT
{
	IOCP_SESSION_KEY_BIT_COUNT = 64, //sizeof(UINT64) * 8
	IOCP_CLIENT_INDEX_BIT_COUNT = 32, //sizeof(UINT) * 8
	IOCP_CLIENT_INDEX_SHIFTED_BIT_COUNT = IOCP_SESSION_KEY_BIT_COUNT - IOCP_CLIENT_INDEX_BIT_COUNT,

	IOCP_SERVER_WSA_BUFFER_COUNT = 256,
	IOCP_CLIENT_WSA_BUFFER_COUNT = 256,

	IOCP_CLIENT_SET_COUNT = 256,
	IOCP_CLIENT_COUNT = 64,
	TOTAL_IOCP_CLIENT_COUNT = IOCP_CLIENT_SET_COUNT * IOCP_CLIENT_COUNT
};

enum IOCP_SERVER_SUB_THREAD_INDEX
{
	IOCP_SERVER_SUB_THREAD_ACCEPT,
	//IOCP_SERVER_SUB_THREAD_CONNECT,
	//IOCP_SERVER_SUB_THREAD_RECEIVE,
	//IOCP_SERVER_SUB_THREAD_SEND,
	//IOCP_SERVER_SUB_THREAD_PROCESS_RECEIVED_PACKET,
	//IOCP_SERVER_SUB_THREAD_PROCESS_SENDING_PACKET,
	IOCP_SERVER_SUB_THREAD_DISCONNECT,
	//IOCP_SERVER_SUB_THREAD_WORK1,
	//IOCP_SERVER_SUB_THREAD_WORK2,
	//IOCP_SERVER_SUB_THREAD_WORK3,
	//IOCP_SERVER_SUB_THREAD_WORK4,
	//IOCP_SERVER_SUB_THREAD_WORK5,
	//IOCP_SERVER_SUB_THREAD_WORK6,

	IOCP_SERVER_SUB_THREAD_COUNT
};

enum IOCP_INNER_SERVER_SUB_THREAD_INDEX
{
	IOCP_INNER_SERVER_SUB_THREAD_ACCEPT,
	IOCP_INNER_SERVER_SUB_THREAD_CONNECT,
	//IOCP_INNER_SERVER_SUB_THREAD_RECEIVE,
	//IOCP_INNER_SERVER_SUB_THREAD_SEND,
	//IOCP_INNER_SERVER_SUB_THREAD_PROCESS_RECEIVED_PACKET,
	//IOCP_INNER_SERVER_SUB_THREAD_PROCESS_SENDING_PACKET,
	IOCP_INNER_SERVER_SUB_THREAD_DISCONNECT,
	//IOCP_INNER_SERVER_SUB_THREAD_WORK1,
	//IOCP_INNER_SERVER_SUB_THREAD_WORK2,
	//IOCP_INNER_SERVER_SUB_THREAD_WORK3,
	//IOCP_INNER_SERVER_SUB_THREAD_WORK4,
	//IOCP_INNER_SERVER_SUB_THREAD_WORK5,
	//IOCP_INNER_SERVER_SUB_THREAD_WORK6,

	IOCP_INNER_SERVER_SUB_THREAD_COUNT
};

enum IOCP_OUTER_SERVER_SUB_THREAD_INDEX
{
	IOCP_OUTER_SERVER_SUB_THREAD_ACCEPT,
	//IOCP_OUTER_SERVER_SUB_THREAD_CONNECT,
	//IOCP_OUTER_SERVER_SUB_THREAD_RECEIVE,
	//IOCP_OUTER_SERVER_SUB_THREAD_SEND,
	//IOCP_OUTER_SERVER_SUB_THREAD_PROCESS_RECEIVED_PACKET,
	//IOCP_OUTER_SERVER_SUB_THREAD_PROCESS_SENDING_PACKET,
	IOCP_OUTER_SERVER_SUB_THREAD_DISCONNECT,
	//IOCP_OUTER_SERVER_SUB_THREAD_WORK1,
	//IOCP_OUTER_SERVER_SUB_THREAD_WORK2,
	//IOCP_OUTER_SERVER_SUB_THREAD_WORK3,
	//IOCP_OUTER_SERVER_SUB_THREAD_WORK4,
	//IOCP_OUTER_SERVER_SUB_THREAD_WORK5,
	//IOCP_OUTER_SERVER_SUB_THREAD_WORK6,

	IOCP_OUTER_SERVER_SUB_THREAD_COUNT
};

enum IOCP_CLIENT_SUB_THREAD_INDEX
{
	IOCP_CLIENT_SUB_THREAD_CONNECT,
	//IOCP_CLIENT_SUB_THREAD_RECEIVE,
	//IOCP_CLIENT_SUB_THREAD_SEND,
	//IOCP_CLIENT_SUB_THREAD_PROCESS_RECEIVED_PACKET,
	//IOCP_CLIENT_SUB_THREAD_PROCESS_SENDING_PACKET,
	IOCP_CLIENT_SUB_THREAD_DISCONNECT,
	//IOCP_CLIENT_SUB_THREAD_WORK1,
	//IOCP_CLIENT_SUB_THREAD_WORK2,
	//IOCP_CLIENT_SUB_THREAD_WORK3,
	//IOCP_CLIENT_SUB_THREAD_WORK4,
	//IOCP_CLIENT_SUB_THREAD_WORK5,
	//IOCP_CLIENT_SUB_THREAD_WORK6,

	IOCP_CLIENT_SUB_THREAD_COUNT
};

enum IOCP_CLIENT_PHASE_INDEX
{
	IOCP_CLIENT_PHASE_ACCEPT,
	IOCP_CLIENT_PHASE_CONNECT,
	IOCP_CLIENT_PHASE_RECEIVE,
	IOCP_CLIENT_PHASE_UPDATE,
	IOCP_CLIENT_PHASE_SEND,
	IOCP_CLIENT_PHASE_DISCONNECT,

	IOCP_CLIENT_PHASE_COUNT
};



#pragma pack(push, 1)

struct IOCP_PacketHeader
{
	BYTE IdentificationCode;
	WORD BodySize;
	BYTE CryptoCode;
	BYTE Checksum;
};

struct IOCP_PacketBody
{

};

struct IOCP_PacketTrailer
{

};

struct IOCP_Packet
{
	IOCP_PacketHeader Header;
	IOCP_PacketBody Body;
	IOCP_PacketTrailer Trailer;
};

#pragma pack(pop)



struct IOCP_ServerSession : public CommonSession
{
	OVERLAPPED OverlappedReceive;
	OVERLAPPED OverlappedSend;

	ByteQueue ReceiveBQ;
	ByteQueue SendBQ;

	SerialByteQueue CommandSBQ;

	SRWLOCK Lock;
};

struct IOCP_ClientSession : public CommonSession
{
	OVERLAPPED OverlappedReceive;
	OVERLAPPED OverlappedSend;

	Queue<SerialPacketQueue<IOCP_PacketHeader>*> ReceiveQ; //ReceivePacketSPQ_AddressQ;
	Queue<SerialPacketQueue<IOCP_PacketHeader>*> SendQ; //SendPacketSPQ_AddressQ;

	//Queue<SerialPacketQueue<IOCP_PacketHeader>> ReceiveQ; //ReceivePacketSPQ_Q;
	//Queue<SerialPacketQueue<IOCP_PacketHeader>> SendQ; //SendPacketSPQ_Q;

	ByteQueue ReceiveBQ;
	ByteQueue SendBQ;

	volatile UINT IO_Count;
	//volatile UINT ReceiveCount;
	//volatile UINT SendCount;

	IOCP_CLIENT_PHASE_INDEX CurrentPhase;

	SRWLOCK Lock;

	//UINT AccumulatedUseCount;
};

struct IOCP_SubThread : public CommonSubThread
{

};

typedef vector<IOCP_ClientSession> IOCP_ClientSessionV;
typedef vector<IOCP_ClientSession>::iterator IOCP_ClientSessionVI;

typedef vector<IOCP_SubThread> IOCP_SubThreadV;
typedef vector<IOCP_SubThread>::iterator IOCP_SubThreadVI;

typedef vector<HANDLE> HANDLE_V;
typedef vector<HANDLE>::iterator HANDLE_VI;