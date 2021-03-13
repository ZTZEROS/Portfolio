#pragma once

enum MY_SQL_CLIENT_LOCAL_CONSTANT
{
	MY_SQL_SERVER_PORT = 3306,

	MY_SQL_CLIENT_LOCAL_CONSTANT_COUNT
};

enum MY_SQL_CLIENT_SUB_THREAD_INDEX
{
	//MY_SQL_CLIENT_SUB_THREAD_CONNECT,
	MY_SQL_CLIENT_SUB_THREAD_RECEIVE,
	MY_SQL_CLIENT_SUB_THREAD_SEND,
	//MY_SQL_CLIENT_SUB_THREAD_PROCESS_RECEIVED_PACKET,
	//MY_SQL_CLIENT_SUB_THREAD_PROCESS_SENDING_PACKET,
	//MY_SQL_CLIENT_SUB_THREAD_DISCONNECT,
	MY_SQL_CLIENT_SUB_THREAD_WORK1,
	MY_SQL_CLIENT_SUB_THREAD_WORK2,
	MY_SQL_CLIENT_SUB_THREAD_WORK3,
	MY_SQL_CLIENT_SUB_THREAD_WORK4,

	MY_SQL_CLIENT_SUB_THREAD_COUNT
};

enum MY_SQL_CLIENT_PHASE_INDEX
{
	MY_SQL_CLIENT_PHASE_CONNECT,
	MY_SQL_CLIENT_PHASE_RECEIVE,
	MY_SQL_CLIENT_PHASE_UPDATE,
	MY_SQL_CLIENT_PHASE_SEND,
	MY_SQL_CLIENT_PHASE_DISCONNECT,

	MY_SQL_CLIENT_PHASE_COUNT
};

enum MY_SQL_CLIENT_IOCP_INDEX
{

};

enum MY_SQL_CLIENT_QUERY_INDEX
{
	MY_SQL_CLIENT_QUERY_SIGN_IN, //USER_INFORMATION //user_information
	MY_SQL_CLIENT_QUERY_STAGE_CLEAR, //USER_STAGE //user_stage
	MY_SQL_CLIENT_QUERY_LEVEL_UP, //USER_LEVEL //user_level

	MY_SQL_CLIENT_QUERY_COUNT
};



#pragma pack(push, 1)

struct MySQL_ClientQueryHeader
{
	MY_SQL_CLIENT_QUERY_INDEX Index;
	WORD Size;
};

struct MySQL_ClientQueryBody
{

};

struct MySQL_ClientQueryTrailer //MysQL_PacketTail
{

};

struct MySQL_ClientQuery
{
	MySQL_ClientQueryHeader Header;
	MySQL_ClientQueryBody Body;
	MySQL_ClientQueryTrailer Trailer;
};

#pragma pack(pop)

struct MySQL_Server : public CommonSession
{
	OVERLAPPED OverlappedReceiving;
	OVERLAPPED OverlappedSending;

	ByteQueue ReceiveBQ;
	ByteQueue SendBQ;

	SerialByteQueue CommandSBQ;

	const char* MySQL_ServerIP_Address;
	const char* MySQL_UserName;
	const char* MySQL_Password;
	const char* MySQL_DatabaseName;
};

struct MySQL_Client : public CommonSession
{
	OVERLAPPED OverlappedReceiving;
	OVERLAPPED OverlappedSending;

	ByteQueue ReceiveBQ;
	ByteQueue SendBQ;

	SerialByteQueue ReceiveSBQ;
	SerialByteQueue SendSBQ;

	MY_SQL_CLIENT_PHASE_INDEX CurrentStepIndex;

	UINT ReceiveCount;
	UINT SendCount;

	SRWLOCK MySQL_ClientLock;



	MYSQL MySQL;
	MYSQL* MySQL_Address;
	MYSQL_RES MySQL_Result;
	MYSQL_RES* MySQL_ResultAddress;
	MYSQL_ROW MySQL_Row;

	ByteQueue QueryBQ;

	SerialByteQueue QuerySBQ;

	volatile BOOL QueryEnqueuing;

	UINT TransactionPerSecond;
};

struct MySQL_SubThread : public CommonSubThread
{

};

typedef vector<MySQL_Client> MySQL_ClientV;
typedef vector<MySQL_Client>::iterator MySQL_ClientVI;

typedef vector<MySQL_SubThread> MySQL_SubThreadV;
typedef vector<MySQL_SubThread>::iterator MySQL_SubThreadVI;