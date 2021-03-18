#pragma once

enum JURASSIC_WAR_LOCAL_CONSTANT
{
	FREE_WIZARD_NET_SERVER_PORT = 9624,
	WIZARD_NET_SERVER_PORT = 9913,
	JW2_PEER_TO_PEER_PORT = 9922,

	JURASSIC_WAR_CLIENT_SET_COUNT = 2,
	JURASSIC_WAR_CLIENT_COUNT = 8,
	TOTAL_JURASSIC_WAR_CLIENT_COUNT = JURASSIC_WAR_CLIENT_SET_COUNT * JURASSIC_WAR_CLIENT_COUNT,

	JURASSIC_WAR_LOCAL_CONSTANT_COUNT
};

enum JURASSIC_WAR_SUB_THREAD_INDEX
{
	JURASSIC_WAR_SUB_THREAD_ACCEPT,
	//JURASSIC_WAR_SUB_THREAD_RECEIVE,
	//JURASSIC_WAR_SUB_THREAD_SEND,
	//JURASSIC_WAR_SUB_THREAD_PROCESS_RECEIVED_PACKET,
	//JURASSIC_WAR_SUB_THREAD_PROCESS_SENDING_PACKET,
	JURASSIC_WAR_SUB_THREAD_DISCONNECT,
	JURASSIC_WAR_SUB_THREAD_WORK1,
	JURASSIC_WAR_SUB_THREAD_WORK2,
	JURASSIC_WAR_SUB_THREAD_WORK3,
	JURASSIC_WAR_SUB_THREAD_WORK4,

	JURASSIC_WAR_SUB_THREAD_COUNT
};

enum JURASSIC_WAR_CLIENT_PHASE_INDEX
{
	JURASSIC_WAR_CLIENT_PHASE_ACCEPT,
	JURASSIC_WAR_CLIENT_PHASE_RECEIVE,
	JURASSIC_WAR_CLIENT_PHASE_UPDATE,
	JURASSIC_WAR_CLIENT_PHASE_SEND,
	JURASSIC_WAR_CLIENT_PHASE_DISCONNECT,

	JURASSIC_WAR_CLIENT_PHASE_COUNT
};

enum IOCP_CHAT_SERVER_IOCP_INDEX
{

};

struct JurassicWarServer : public CommonSession
{
	OVERLAPPED OverlappedReceiving;
	OVERLAPPED OverlappedSending;

	ByteQueue ReceiveBQ;
	ByteQueue SendBQ;

	SerialByteQueue CommandSBQ;
};

struct JurassicWarClient : public CommonSession
{
	OVERLAPPED OverlappedReceiving;
	OVERLAPPED OverlappedSending;

	ByteQueue ReceiveBQ;
	ByteQueue SendBQ;

	SerialByteQueue ReceiveSBQ;
	SerialByteQueue SendSBQ;

	JURASSIC_WAR_CLIENT_PHASE_INDEX CurrentStepIndex;

	UINT ReceiveCount;
	UINT SendCount;

	SRWLOCK JurassicWarClientLock;
};

struct JurassicWarSubThread : public CommonSubThread
{

};

typedef vector<JurassicWarClient> JurassicWarClientV;
typedef vector<JurassicWarClient>::iterator JurassicWarClientVI;

typedef vector<JurassicWarSubThread> JurassicWarSubThreadV;
typedef vector<JurassicWarSubThread>::iterator JurassicWarSubThreadVI;