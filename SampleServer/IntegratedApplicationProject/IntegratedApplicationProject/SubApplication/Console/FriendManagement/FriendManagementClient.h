#pragma once

enum FM_LOCAL_CONSTANT //FRIEND_MANAGEMENT_LOCAL_CONSTANT
{
	FM_SERVER_PORT = 25000,

	FM_CLIENT_SET_COUNT = 32,
	FM_CLIENT_COUNT = 64,
	TOTAL_FM_CLIENT_COUNT = FM_CLIENT_SET_COUNT * FM_CLIENT_COUNT,

	FM_PACKET_HEADER_IDENTIFICATION_CODE = 0x89,

	FM_MEMBER_NAME_LENGTH = 32, //FM_NICKNAME_LENGTH = 24,
	FM_MEMBER_NAME_SIZE = FM_MEMBER_NAME_LENGTH * sizeof(WCHAR), //FM_NICKNAME_SIZE = 48,
	FM_MESSAGE_LENGTH = 128,
	FM_MESSAGE_SIZE = 256,

	FM_LOCAL_CONSTANT_COUNT
};

enum FM_REQUEST_INDEX //FRIEND_MANAGEMENT_REQUEST_INDEX
{
	FM_REQUEST_JOIN = 1,
	FM_REQUEST_LOGIN = 3,

	FM_REQUEST_MEMBER_LIST = 10,
	FM_REQUEST_FRIEND_LIST = 12,
	FM_REQUEST_FRIEND_OFFEREE_LIST = 14,
	FM_REQUEST_FRIEND_OFFERER_LIST = 16,

	FM_REQUEST_FRIEND_BREACH = 20,
	FM_REQUEST_FRIEND_OFFER = 22,
	FM_REQUEST_FRIEND_OFFER_CANCEL = 24,
	FM_REQUEST_FRIEND_DENY = 26,
	FM_REQUEST_FRIEND_AGREE = 28,

	FM_REQUEST_STRESS_ECHO = 100,

	FM_REQUEST_COUNT
};

enum FM_RESPONSE_INDEX //FRIEND_MANAGEMENT_RESPONSE_INDEX
{
	FM_RESPONSE_JOIN = 2,
	FM_RESPONSE_LOGIN = 4, //REQUEST_LOGIN + 1,

	FM_RESPONSE_MEMBER_LIST = 11,
	FM_RESPONSE_FRIEND_LIST = 13,
	FM_RESPONSE_FRIEND_OFFEREE_LIST = 15,
	FM_RESPONSE_FRIEND_OFFERER_LIST = 17,

	FM_RESPONSE_FRIEND_BREACH = 21,
	FM_RESPONSE_FRIEND_OFFER = 23,
	FM_RESPONSE_FRIEND_OFFER_CANCEL = 25,
	FM_RESPONSE_FRIEND_DENY = 27,
	FM_RESPONSE_FRIEND_AGREE = 29,

	FM_RESPONSE_STRESS_ECHO = 101,

	FM_RESPONSE_COUNT
};

enum FM_RESULT_INDEX //FRIEND_MANAGEMENT_RESULT_INDEX
{
	FM_RESULT_SUCCESS = 1,
	FM_RESULT_NOT_EXIST = 2,
	FM_RESULT_FAILURE = 3,
	FM_RESELT_ET_CETERA,

	FM_RESULT_COUNT
};



#pragma pack(push, 1) //#pragma pack(2) //#pragma pack(push, 2) 

struct FM_PacketHeader //FriendManagementPacketHeader //PacketHead
{
	BYTE IdentificationCode;

	WORD PacketType; //MessageType
	WORD PacketBodySize; //PayloadSize
};

struct FM_PacketBody //FriendManagement //PacketBody
{

};

struct FM_PacketTrailer //FriendManagementPacketTrailer //PacketTail
{

};

struct FM_Packet //FriendManagementPacket
{
	FM_PacketHeader Header;
	FM_PacketBody Body;
	FM_PacketTrailer Trailer;
};

#pragma pack(pop) //#pragma pack(4)



struct FM_ServerSession : public ServerSession //FriendManagementServerSession
{
	ByteQueue ReceiveBQ;
	ByteQueue SendBQ;

	SerialByteQueue CommandSBQ;
};

struct FM_ClientSession : public ClientSession //FriendManagementClientSession
{
	ByteQueue ReceiveBQ;
	ByteQueue SendBQ;

	UINT64 MemberNumber;
};



typedef FM_ClientSession FM_ClientSessionA;
typedef FM_ClientSession* FM_ClientSessionAI;

typedef list<FM_ClientSession*> FM_ClientSessionAddressL;
typedef list<FM_ClientSession*>::iterator FM_ClientSessionAddressLI;

typedef list<UINT64> FM_MemberNumberL;
typedef list<UINT64>::iterator FM_MemberNumberLI;

typedef vector<FM_ClientSession*> FM_ClientSessionAddressV;
typedef vector<FM_ClientSession*>::iterator FM_ClientSessionAddressVI;

typedef map<SOCKET, FM_ClientSession*> FM_ClientSessionAddressM;
typedef map<SOCKET, FM_ClientSession*>::iterator FM_ClientSessionAddressMI;

typedef multimap<UINT64, UINT64> FM_MemberNumberM;
typedef multimap<UINT64, UINT64>::iterator FM_MemberNumberMI;

typedef unordered_map<SOCKET, FM_ClientSession> FM_ClientSessionM;
typedef unordered_map<SOCKET, FM_ClientSession>::iterator FM_ClientSessionMI;

typedef unordered_map<SOCKET, FM_ServerSession> FM_ServerSessionM;
typedef unordered_map<SOCKET, FM_ServerSession>::iterator FM_ServerSessionMI;

typedef unordered_map<UINT64, WCHAR[FM_MEMBER_NAME_LENGTH]> FM_MemberNameM;
typedef unordered_map<UINT64, WCHAR[FM_MEMBER_NAME_LENGTH]>::iterator FM_MemberNameMI;

typedef unordered_map<UINT64, FM_MemberNumberL> FM_MemberNumberLM;
typedef unordered_map<UINT64, FM_MemberNumberL>::iterator FM_MemberNumberLMI;



extern FM_ServerSessionM FM_SERVER_SESSION_M;

extern TIMEVAL FM_CLIENT_TIME_VALUE;

extern unsigned int LAST_FM_SERVER_SESSION_INDEX;
extern unsigned int CURRENT_FM_SERVER_SESSION_COUNT;

int FM_Client(int arguemntCount, char* argumentVector[]);


bool InitializeFM_Client();
bool UpdateFM_Client();
bool RenderFM_Client();
bool TerminalizeFM_Client();

int ConnectFM_Client();
//int DisconnectFM_Client();

int ReceiveFM_ServerPacket();
int SendFM_ClientPacket();

int ProcessReceivedFM_ServerPacket();
int ProcessSendingFM_ClientPacket(FM_REQUEST_INDEX fmRequestIndex, FM_ServerSessionMI fmServerSessionMI, SerialByteQueue& receivedPacketSBQ);

bool CheckReceivedFM_ServerPacket(FM_PacketHeader& receivedPacketHeader, SerialByteQueue& receivedPacketSBQ);
bool CheckSendingFM_ClientPacket(FM_PacketHeader& sendingPacketHeader, SerialByteQueue& sendingPacketSBQ);