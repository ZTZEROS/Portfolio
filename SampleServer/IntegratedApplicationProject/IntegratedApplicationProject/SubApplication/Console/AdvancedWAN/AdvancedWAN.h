#pragma once

enum ADVANCED_WAN_LOCAL_CONSTANT
{
	ADVANCED_WAN_SESSION_KEY_BIT_COUNT = 64, //sizeof(UINT64) * 8
	ADVANCED_WAN_CLIENT_INDEX_BIT_COUNT = 32, //sizeof(UINT) * 8
	ADVANCED_WAN_CLIENT_INDEX_SHIFTED_BIT_COUNT = ADVANCED_WAN_SESSION_KEY_BIT_COUNT - ADVANCED_WAN_CLIENT_INDEX_BIT_COUNT,

	ADVANCED_WAN_ECHO_SERVER_PORT = 6000,
	ADVANCED_WAN_CHAT_SERVER_PORT = 12001,

	ADVANCED_WAN_SERVER_WSABUF_COUNT = 256,

	ADVANCED_WAN_CLIENT_SET_COUNT = 256,
	ADVANCED_WAN_CLIENT_COUNT = 64,
	TOTAL_ADVANCED_WAN_CLIENT_COUNT = ADVANCED_WAN_CLIENT_SET_COUNT * ADVANCED_WAN_CLIENT_COUNT,

	ADVANCED_WAN_CHAT_REAL_NAME_LENGTH = 20,
	ADVANCED_WAN_CHAT_NICKNAME_LENGTH = 20,
	ADVANCED_WAN_CHAT_PLAYER_KEY_LENGTH = 64,

	ADVANCED_WAN_CHAT_SECTOR_WIDTH = 50,
	ADVANCED_WAN_CHAT_SECTOR_HEIGHT = 50,

	ADVANCED_WAN_CHAT_MAXIMUM_MESSAGE_LENGTH = 1024
};

enum ADVANCED_WAN_SUB_THREAD_INDEX
{
	ADVANCED_WAN_SUB_THREAD_ACCEPT,
	//ADVANCED_WAN_SUB_THREAD_RECEIVE,
	//ADVANCED_WAN_SUB_THREAD_SEND,
	//ADVANCED_WAN_SUB_THREAD_PROCESS_RECEIVED_PACKET,
	//ADVANCED_WAN_SUB_THREAD_PROCESS_SENDING_PACKET,
	ADVANCED_WAN_SUB_THREAD_DISCONNECT,
	ADVANCED_WAN_SUB_THREAD_WORK1,
	ADVANCED_WAN_SUB_THREAD_WORK2,
	ADVANCED_WAN_SUB_THREAD_WORK3,
	ADVANCED_WAN_SUB_THREAD_WORK4,
	//ADVANCED_WAN_SUB_THREAD_WORK5,
	//ADVANCED_WAN_SUB_THREAD_WORK6,

	ADVANCED_WAN_SUB_THREAD_COUNT
};

enum ADVANCED_WAN_CLIENT_PHASE_INDEX //ADVANCED_WAN_CLIENT_STEP_INDEX
{
	ADVANCED_WAN_CLIENT_PHASE_ACCEPT,
	ADVANCED_WAN_CLIENT_PHASE_RECEIVE,
	ADVANCED_WAN_CLIENT_PHASE_UPDATE,
	ADVANCED_WAN_CLIENT_PHASE_SEND,
	ADVANCED_WAN_CLIENT_PHASE_DISCONNECT,

	ADVANCED_WAN_CLIENT_PHASE_COUNT
};



enum ADVANCED_WAN_CHAT_CLIENT_REQUEST_INDEX
{
	ADVANCED_WAN_CHAT_CLIENT_REQUEST_LOGIN = 1,
	ADVANCED_WAN_CHAT_CLIENT_REQUEST_SECTOR_MOVE = 3,
	ADVANCED_WAN_CHAT_CLIENT_REQUEST_MESSAGE = 5,
	ADVANCED_WAN_CHAT_CLIENT_REQUEST_HEARTBEAT = 7,
	//ADVANCED_WAN_CHAT_CLIENT_REQUEST_ALIVE_SIGNAL = 7,
	ADVANCED_WAN_CHAT_CLIENT_REQUEST_LOGOUT = 9,

	ADVANCED_WAN_CHAT_CLIENT_REQUEST_COUNT = 4
};

enum ADVANCED_WAN_CHAT_SERVER_RESPONSE_INDEX
{
	ADVANCED_WAN_CHAT_SERVER_RESPONSE_LOGIN = 2,
	ADVANCED_WAN_CHAT_SERVER_RESPONSE_SECTOR_MOVE = 4,
	ADVANCED_WAN_CHAT_SERVER_RESPONSE_MESSAGE = 6,
	ADVANCED_WAN_CHAT_SERVER_RESPONSE_HEARTBEAT = 8,
	//ADVANCED_WAN_CHAT_SERVER_RESPONSE_ALIVE_SIGNAL = 8,
	ADVANCED_WAN_CHAT_SERVER_RESPONSE_LOGOUT = 10,
	
	ADVANCED_WAN_CHAT_SERVER_RESPONSE_COUNT = 3
};

enum ADVANCED_WAN_LOGIN_CLIENT_REQUEST_INDEX
{
	ADVANCED_WAN_LOGIN_CLIENT_REQUEST_LOGIN = 101,

	ADVANCED_WAN_LOGIN_CLIENT_REQUEST_COUNT = 1
};

enum ADVANCED_WAN_LOGIN_SERVER_RESPONSE_INDEX
{
	ADVANCED_WAN_LOGIN_SERVER_RESPONSE_LOGIN = 102,

	ADVANCED_WAN_LOGIN_CLIENT_RESPONSE_COUNT = 1
};

enum ADVANCED_WAN_GAME_CLIENT_REQUEST_INDEX
{

};

enum ADVANCED_WAN_GAME_SERVER_RESPONSE_INDEX
{

};




#pragma pack(push, 1)

struct AdvancedWAN_PacketHeader
{
	BYTE IdentificationCode;
	WORD BodySize; //PacketBodySize;
	BYTE CryptoCode; //XorCode; //XorCryptoCode;
	BYTE Checksum;
};

struct AdvancedWAN_PacketBody
{

};

struct AdvancedWAN_PacketTrailer
{

};

struct AdvancedWAN_Packet
{
	AdvancedWAN_PacketHeader Header;
	AdvancedWAN_PacketBody Body;
	AdvancedWAN_PacketTrailer Trailer;
};

#pragma pack(pop)



struct AdvancedWAN_ServerSession : public CommonSession
{
	OVERLAPPED OverlappedReceive;
	OVERLAPPED OverlappedSend;

	ByteQueue ReceiveBQ;
	ByteQueue SendBQ;

	SerialByteQueue CommandSBQ;

	SRWLOCK LanServerLock;
};

struct AdvancedWAN_ClientSession : public CommonSession
{
	OVERLAPPED OverlappedReceive;
	OVERLAPPED OverlappedSend;

	Queue<SerialPacketQueue<AdvancedWAN_PacketHeader>*> ReceiveQ;
	Queue<SerialPacketQueue<AdvancedWAN_PacketHeader>*> SendQ;

	ByteQueue ReceiveBQ;
	ByteQueue SendBQ;

	volatile UINT IO_Count;
	//volatile UINT ReceiveCount;
	//volatile UINT SendCount;

	SRWLOCK Lock;

	ADVANCED_WAN_CLIENT_PHASE_INDEX CurrentPhaseIndex;

	UINT AccumulatedUseCount;
};

struct AdvancedWAN_SubThread : public CommonSubThread
{
	
};

struct AdvancedWAN_Work
{
	UINT64 SessionKey;
	SerialPacketQueue<AdvancedWAN_PacketHeader>* PacketSPQ_Address;
};

struct AdvancedWAN_ClientPlayer
{
	UINT64 SessionKey; //SessionID

	UINT64 AccountKey; //AccountNo
	WCHAR RealName[ADVANCED_WAN_CHAT_REAL_NAME_LENGTH]; //ID
	WCHAR Nickname[ADVANCED_WAN_CHAT_NICKNAME_LENGTH];
	CHAR PlayerKey[ADVANCED_WAN_CHAT_PLAYER_KEY_LENGTH]; //SessionKey64

	UINT SectorIndexX; //SectorX
	UINT SectorIndexY; //SectorY

	UINT MessageSize; //MessageLength; //SpeechLength //ChatLength //TalkLength //ConversationLength
	WCHAR Message[ADVANCED_WAN_CHAT_MAXIMUM_MESSAGE_LENGTH];

	DWORD PreviousHeartbeatTime;

	SRWLOCK Lock;
};

struct AdvancedWAN_Sector
{
	list<AdvancedWAN_ClientPlayer*> CPAL;
	//AdvancedWAN_ClientPlayerAddressL CPAL;
	SRWLOCK Lock;
};

typedef vector<AdvancedWAN_ClientSession> AdvancedWAN_ClientSessionV;
typedef vector<AdvancedWAN_ClientSession>::iterator AdvancedWAN_ClientSessionVI;

typedef vector<AdvancedWAN_SubThread> AdvancedWAN_SubThreadV;
typedef vector<AdvancedWAN_SubThread>::iterator AdvancedWAN_SubThreadVI;

typedef list<AdvancedWAN_ClientPlayer> AdvancedWAN_ClientPlayerL;
typedef list<AdvancedWAN_ClientPlayer>::iterator AdvancedWAN_ClientPlayerLI;

typedef list<AdvancedWAN_ClientPlayer*> AdvancedWAN_ClientPlayerAddressL;
typedef list<AdvancedWAN_ClientPlayer*>::iterator AdvancedWAN_ClientPlayerAddressLI;

typedef unordered_map<UINT64, AdvancedWAN_ClientPlayer> AdvancedWAN_ClientPlayerUM;
typedef unordered_map<UINT64, AdvancedWAN_ClientPlayer>::iterator AdvancedWAN_ClientPlayerUMI;

typedef unordered_map<UINT64, AdvancedWAN_ClientPlayer*> AdvancedWAN_ClientPlayerAddressUM;
typedef unordered_map<UINT64, AdvancedWAN_ClientPlayer*>::iterator AdvancedWAN_ClientPlayerAddressUMI;
