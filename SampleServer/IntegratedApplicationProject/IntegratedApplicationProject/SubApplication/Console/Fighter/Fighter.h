#pragma once

enum FIGHTER_LOCAL_CONSTANT
{
	FIGHTER_SERVER_PORT = 20000,

	FIGHTER_CLIENT_SET_COUNT = 64,
	FIGHTER_CLIENT_COUNT = 64,
	TOTAL_FIGHTER_CLIENT_COUNT = FIGHTER_CLIENT_SET_COUNT * FIGHTER_CLIENT_COUNT,

	FIGHTER_FD_SET_COUNT = FIGHTER_CLIENT_SET_COUNT,
	FIGHTER_FD_COUNT = FIGHTER_CLIENT_COUNT,
	TOTAL_FIGHTER_FD_COUNT = FIGHTER_FD_SET_COUNT * FIGHTER_FD_COUNT,

	FIGHTER_PACKET_HEADER_IDENTIFICATION_CODE = 0x89,
	FIGHTER_PACKET_TRAILER_IDENTIFICATION_CODE = 0x79,

	FIGHTER_MAP_RANGE_TOP = 0,
	FIGHTER_MAP_RANGE_LEFT = 0,
	FIGHTER_MAP_RANGE_RIGHT = 6400,
	FIGHTER_MAP_RANGE_BOTTOM = 6400,
	FIGHTER_MAP_WIDTH = FIGHTER_MAP_RANGE_RIGHT - FIGHTER_MAP_RANGE_LEFT,
	FIGHTER_MAP_HEIGHT = FIGHTER_MAP_RANGE_BOTTOM - FIGHTER_MAP_RANGE_TOP,

	FIGHTER_SECTOR_WIDTH = 256,
	FIGHTER_SECTOR_HEIGHT = 128,
	FIGHTER_SECTOR_INDEX_TOP = 0,
	FIGHTER_SECTOR_INDEX_LEFT = 0,
	FIGHTER_SECTOR_INDEX_RIGHT = FIGHTER_MAP_WIDTH / FIGHTER_SECTOR_WIDTH - 1,
	FIGHTER_SECTOR_INDEX_BOTTOM = FIGHTER_MAP_HEIGHT / FIGHTER_SECTOR_HEIGHT - 1,

	FIGHTER_SECTOR_LEFT_UP = -4,
	FIGHTER_SECTOR_LEFT = -3,
	FIGHTER_SECTOR_LEFT_DOWN = -2,
	FIGHTER_SECTOR_UP = -1,
	FIGHTER_SECTOR_DOWN = 1,
	FIGHTER_SECTOR_RIGHT_UP = 2,
	FIGHTER_SECTOR_RIGHT = 3,
	FIGHTER_SECTOR_RIGHT_DOWN = 4,

	FIGHTER_MOVE_DIRECTION_LEFT = 0,
	FIGHTER_MOVE_DIRECTION_LEFT_UP = 1,
	FIGHTER_MOVE_DIRECTION_UP = 2,
	FIGHTER_MOVE_DIRECTION_RIGHT_UP = 3,
	FIGHTER_MOVE_DIRECTION_RIGHT = 4,
	FIGHTER_MOVE_DIRECTION_RIGHT_DOWN = 5,
	FIGHTER_MOVE_DIRECTION_DOWN = 6,
	FIGHTER_MOVE_DIRECTION_LEFT_DOWN = 7,

	FIGHTER_MOVE_HORIZONTAL_VELOCITY = 3,
	FIGHTER_MOVE_VERTICAL_VELOCITY = 2,

	FIGHTER_PLAYER_LIFE = 100,

	FIGHTER_LOCAL_CONSTANT_COUNT
};

enum FIGHTER_REQUEST_INDEX //FromClientToServer
{
	FIGHTER_REQUEST_MOVE_START = 10,
	FIGHTER_REQUEST_MOVE_STOP = 12,
	FIGHTER_REQUEST_ATTACK1 = 20,
	FIGHTER_REQUEST_ATTACK2 = 22,
	FIGHTER_REQUEST_ATTACK3 = 24,
	FIGHTER_REQUEST_ECHO = 252,

	FIGHTER_REQUEST_COUNT = 6
};

enum FIGHTER_RESPONSE_INDEX //FromServerToClient
{
	FIGHTER_RESPONSE_CREATE_MY_CHARACTER = 0,
	FIGHTER_RESPONSE_CREATE_OTHER_CHARACTER = 1,
	FIGHTER_RESPONSE_DELETE_OTHER_CHARACTER = 2,
	FIGHTER_RESPONSE_MOVE_START = 11, //FIGHTER_REQUEST_MOVE_START + 1,
	FIGHTER_RESPONSE_MOVE_STOP = 13, //FIGHTER_REQUEST_MOVE_STOP + 1,
	FIGHTER_RESPONSE_ATTACK1 = 21,
	FIGHTER_RESPONSE_ATTACK2 = 23,
	FIGHTER_RESPONSE_ATTACK3 = 25,
	FIGHTER_RESPONSE_DAMAGE = 30,
	FIGHTER_RESPONSE_SYNCHRONIZE = 251,
	FIGHTER_RESPONSE_ECHO = 253,

	FIGHTER_RESPONSE_COUNT
};

enum FIGHTER_PLAYER_ACTION_INDEX
{
	FIGHTER_PLAYER_ACTION_MOVE_START,
	FIGHTER_PLAYER_ACTION_MOVE_STOP,
	FIGHTER_PLAYER_ACTION_ATTACK1,
	FIGHTER_PLAYER_ACTION_ATTACK2,
	FIGHTER_PLAYER_ACTION_ATTACK3,

	FIGHTER_PLAYER_ACTION_COUNT
};



#pragma pack(push, 1) //#pragma pack(2) //#pragma pack(push, 2) 

struct FighterPacketHeader //FriendManagementPacketHeader //PacketHead
{
	BYTE IdentificationCode;
	BYTE PacketBodySize; //PayloadSize
	BYTE PacketType; //MessageType
	BYTE PaddingByte;
};

struct FighterPacketBody //FriendManagement //PacketBody
{

};

struct FighterPacketTrailer //FriendManagementPacketTrailer //PacketTail
{

};

struct FighterPacket //FriendManagementPacket
{
	FighterPacketHeader Header;
	FighterPacketBody Body;
	FighterPacketTrailer Trailer;
};

#pragma pack(pop) //#pragma pack(4)

struct FighterPlayer;

struct FighterServerSession : public ServerSession //FriendManagementServerSession
{
	ByteQueue ReceiveBQ;
	ByteQueue SendBQ;

	SerialByteQueue CommandSBQ;
};

struct FighterClientSession : public ClientSession //FriendManagementClientSession
{
	ByteQueue ReceiveBQ;
	ByteQueue SendBQ;

	DWORD IdentificationNumber;
	FighterPlayer* FighterPlayerAddress;
};

struct FighterPlayer
{
	DWORD IdentificationNumber;
	FIGHTER_PLAYER_ACTION_INDEX ActionIndex;
	LARGE_INTEGER InitialPerformanceCounter;
	BYTE Direction; //CurrentDirection;
	BYTE LeftRightDirection;

	WORD PositionX; //CurrentPositionX;
	WORD PositionY; //CurrentPositionY;
	WORD InitialPositionX;
	WORD InitialPositionY;

	WORD SectorIndexX; //CurrentSectorIndexX;
	WORD SectorIndexY; // CurrentSectorIndexY;
	WORD PreviousSectorIndexX;
	WORD PreviousSectorIndexY;

	BYTE Life;

	FighterClientSession* ClientSessionAddress;
};



typedef FighterClientSession FighterClientSessionA;
typedef FighterClientSession* FighterClientSessionAI;

typedef FighterPlayer FighterPlayerA;
typedef FighterPlayer* FighterPlayerAI;

typedef list<FighterClientSession*> FighterClientSessionAddressL;
typedef list<FighterClientSession*>::iterator FighterClientSessionAddressLI;

typedef list<FighterPlayer*> FighterPlayerAddressL;
typedef list<FighterPlayer*>::iterator FighterPlayerAddressLI;

typedef vector<FighterClientSession> FighterClientSessionV;
typedef vector<FighterClientSession>::iterator FighterClientSessionVI;

typedef vector<FighterPlayer> FighterPlayerV;
typedef vector<FighterPlayer>::iterator IghterPlayerVI;

typedef unordered_map<SOCKET, FighterClientSession> FighterClientSessionUM;
typedef unordered_map<SOCKET, FighterClientSession>::iterator FighterClientSessionUMI;

typedef unordered_map<DWORD, FighterPlayer> FighterPlayerUM;
typedef unordered_map<DWORD, FighterPlayer>::iterator FighterPlayerUMI;



struct FighterSector
{
	FighterPlayerAddressL FighterPlayerL;
};