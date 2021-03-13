#pragma once

enum CHAT_LOCAL_CONSTANT
{
	CHAT_SERVER_PORT = 6000,
	CHAT_CLIENT_SET_COUNT = 16,
	CHAT_CLIENT_COUNT = 64,
	TOTAL_CHAT_CLIENT_COUNT = CHAT_CLIENT_SET_COUNT * CHAT_CLIENT_COUNT,
	CHAT_ROOM_SET_COUNT = 16,
	CHAT_ROOM_COUNT= 64,
	TOTAL_CHAT_ROOM_COUNT = CHAT_ROOM_SET_COUNT * CHAT_ROOM_COUNT,

	CHAT_PACKET_HEADER_IDENTIFICATION_CODE = 0x89,
	CHAT_NICKNAME_LENGTH = 16,
	CHAT_NICKNAME_SIZE = 32,
	CHAT_USER_NAME_LENGTH = 16,
	CHAT_USER_NAME_SIZE = 32,
	CHAT_ROOM_NAME_LENGTH = 32,
	CHAT_ROOM_NAME_SIZE = 64,
	CHAT_MESSAGE_LENGTH = 128,
	CHAT_MESSAGE_SIZE = 256,

	CHAT_LOCAL_CONSTANT_COUNT
};

enum CHAT_REQUEST_INDEX
{
	CHAT_REQUEST_LOGIN = 1,
	CHAT_REQUEST_ROOM_LIST = 3,
	CHAT_REQUEST_ROOM_CREATE = 5,
	CHAT_REQUEST_ROOM_ENTER = 7,
	CHAT_REQUEST_ROOM_CHAT = 9,
	CHAT_REQUEST_ROOM_LEAVE = 11,

	CHAT_REQUEST_COUNT
};

enum CHAT_RESPONSE_INDEX
{
	CHAT_RESPONSE_LOGIN = 2, //REQUEST_LOGIN + 1,
	CHAT_RESPONSE_ROOM_LIST = 4,
	CHAT_RESPONSE_ROOM_CREATE = 6,
	CHAT_RESPONSE_ROOM_ENTER = 8,
	CHAT_RESPONSE_ROOM_CHAT = 10,
	CHAT_RESPONSE_ROOM_LEAVE = 12,

	CHAT_RESPONSE_ROOM_DELETE, 
	CHAT_RESPONSE_USER_ENTER,

	CHAT_RESPONSE_COUNT
};

enum CHAT_RESULT_INDEX
{
	CHAT_RESULT_SUCCESS = 1, //LOGIN, ROOM_CREATE, ROOM_ENTER
	CHAT_RESULT_FAILURE = 2, //LOGIN(duplicated nickname), ROOM_CREATE(duplicated room name), ROOM_ENTER(wrong room number)
	//CHAT_RESULT_NAME_OR_NUMBER_FAILURE = 2,
	CHAT_RESULT_EXCESS = 3,
	//CHAT_RESULT_USER_COUNT_EXCESS = 3,
	CHAT_RESELT_ET_CETERA,

	CHAT_RESULT_COUNT
};



#pragma pack(push, 1) //#pragma pack(2) //#pragma pack(push, 2) 

struct ChatPacketHeader //PacketHead
{
	BYTE IdentificationCode;
	BYTE Checksum256; 

	WORD PacketType; //MessageType
	WORD PacketBodySize; //PayloadSize
};

struct ChatPacketBody
{

};

struct ChatPacketTrailer //PacketTail
{

};

struct ChatPacket
{
	ChatPacketHeader Header;
	ChatPacketBody Body;
	ChatPacketTrailer Trailer;
};

#pragma pack(pop) //#pragma pack(4)



struct ChatServerSession : public ServerSession
{
	ByteQueue ReceiveBQ;
	ByteQueue SendBQ;

	SerialByteQueue CommandSBQ;
};

struct ChatClientSession : public ClientSession
{
	ByteQueue ReceiveBQ;
	ByteQueue SendBQ;

	//WCHAR Nickname[CHAT_NICKNAME_LENGTH];
	WCHAR UserName[CHAT_NICKNAME_LENGTH];
	unsigned int UserNumber;
	unsigned int RoomNumber;

	//ChatRoom* Room;
};



//typedef ChatClientSession ChatClientSessionA;
//typedef ChatClientSession* ChatClientSessionAI;

typedef list<ChatClientSession*> ChatClientSessionAddressL;
typedef list<ChatClientSession*>::iterator ChatClientSessionAddressLI;

typedef vector<ChatClientSession*> ChatClientSessionAddressV;
typedef vector<ChatClientSession*>::iterator ChatClientSessionAddressVI;

//typedef map<SOCKET, ChatClientSession*> ChatClientSessionAddressM;
//typedef map<SOCKET, ChatClientSession*>::iterator ChatClientSessionAddressMI;

typedef unordered_map<SOCKET, ChatClientSession> ChatClientSessionM;
typedef unordered_map<SOCKET, ChatClientSession>::iterator ChatClientSessionMI;



struct ChatRoom
{
	WORD RoomNameSize;
	WCHAR RoomName[CHAT_ROOM_NAME_LENGTH];
	unsigned int RoomNumber;

	ChatClientSessionAddressL UserL;
};



typedef unordered_map<unsigned int, ChatRoom> ChatRoomM;
typedef unordered_map<unsigned int, ChatRoom>::iterator ChatRoomMI;



extern ChatServerSession CHAT_SERVER_SESSION;

//extern ChatClientSessionA CHAT_CLIENT_SESSION_A[TOTAL_CHAT_CLIENT_COUNT]; //extern UserSession USER_SESSION_A[USER_GROUP_COUNT][USER_COUNT];
extern ChatClientSessionAddressL CHAT_CLIENT_SESSION_L; //CHAT_CLIENT_SESSION_ADDRESS_L;
extern ChatClientSessionAddressV CHAT_CLIENT_SESSION_V; //CHAT_CLIENT_SESSION_ADDRESS_V;
extern ChatClientSessionM CHAT_CLIENT_SESSION_M;

extern ChatRoomM CHAT_ROOM_M;

extern TIMEVAL CHAT_SERVER_TIME_VALUE;

extern unsigned int LAST_CHAT_CLIENT_INDEX;
extern unsigned int CURRENT_CHAT_CLIENT_COUNT;
extern unsigned int CHAT_USER_NUMBER_A[TOTAL_CHAT_CLIENT_COUNT];

extern unsigned int LAST_CHAT_ROOM_INDEX;
extern unsigned int CURRENT_CHAT_ROOM_COUNT;
extern unsigned int CHAT_ROOM_NUMBER_A[TOTAL_CHAT_ROOM_COUNT];



int ChatServer(int arguemntCount, char* argumentVector[]);


bool InitializeChatServer();
bool UpdateChatServer();
bool RenderChatServer();
bool TerminalizeChatServer();

int AcceptChatClient();
int DisconnectChatClient();

int ReceiveChatClientPacket();
int SendChatServerPacket();

int ProcessReceivedChatClientPacket();
int ProcessSendingChatServerPacket(CHAT_RESPONSE_INDEX chatResponseIndex, ChatClientSessionMI chatClientSessionMI, SerialByteQueue* receivedPacketSBQ = nullptr);

bool CheckReceivedChatClientPacket(ChatPacketHeader* receivedPacketHeader, SerialByteQueue* receivedPacketSBQ);
bool CheckSendingChatServerPacket(ChatPacketHeader* sendingPacketHeader, SerialByteQueue* sendingPacketSBQ);