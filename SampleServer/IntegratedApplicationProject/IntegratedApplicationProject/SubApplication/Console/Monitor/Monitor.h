#pragma once

enum MONITOR_LOCAL_CONSTANT
{
	
};

enum MONITOR_CLIENT_REQUEST_INDEX
{
	MONITOR_CLIENT_REQUEST_LOGIN = 25001,

	MONITOR_CLIENT_REQUEST_COUNT
};

enum MONITOR_SERVER_REQUEST_INDEX
{
	MONITOR_SERVER_REQUEST_LOGIN = 20001,
	MONITOR_SERVER_REQUEST_UPDATE = 20002,

	MONITOR_SERVER_REQUEST_COUNT
};

enum MONITOR_SERVER_RESPONSE_INDEX
{
	MONITOR_SERVER_RESPONSE_CLIENT_LOGIN = 25002,
	MONITOR_SERVER_RESPONSE_CLIENT_UPDATE = 25003,

	MONITOR_SERVER_RESPONSE_COUNT
};

enum MONITOR_UPDATE_INDEX
{
	MONITOR_UPDATE_TOTAL_SERVER_CPU_USAGE = 1,
	MONITOR_UPDATE_TOTAL_SERVER_AVAILABLE_MEMORY,
	MONITOR_UPDATE_TOTAL_SERVER_ETHERNET_RECEIVED_SIZE, //NETWORK_RECEIVED_SIZE
	MONITOR_UPDATE_TOTAL_SERVER_ETHERNET_SENDED_SIZE, //RECEIVED_SIZE
	MONITOR_UPDATE_TOTAL_SERVER_NONPAGED_MEMORY,

	MONITOR_UPDATE_MATCH_SERVER_ON,
	MONITOR_UPDATE_MATCH_SERVER_TOTAL_CPU_USAGE,
	MONITOR_UPDATE_MATCH_SERVER_MEMORY_USER_COMMIT,
	MONITOR_UPDATE_MATCH_SERVER_PACKET_POOL_USAGE,
	MONITOR_UPDATE_MATCH_SERVER_SESSION_COUNT,
	MONITOR_UPDATE_MATCH_SERVER_PLAYER_COUNT,
	MONITOR_UPDATE_MATCH_SERVER_CURRET_MATCH_SUCCESS_COUNT,

	MONITOR_UPDATE_MASTER_SERVER_ON,
	MONITOR_UPDATE_MASTER_SERVER_PROCESS_CPU_USAGE,
	MONITOR_UPDATE_MASTER_SERVER_TOTAL_CPU_USAGE,
	MONITOR_UPDATE_MASTER_SERVER_MEMORY_USER_COMMIT,
	MONITOR_UPDATE_MASTER_SERVER_MATCH_SERVER_CONNECTION_COUNT,
	MONITOR_UPDATE_MASTER_SERVER_MATCH_SERVER_LOGIN_COUNT,
	MONITOR_UPDATE_MASTER_SERVER_WAITING_PLAYER_COUNT,
	MONITOR_UPDATE_MASTER_SERVER_BATTLE_SERVER_CONNECTION_COUNT,
	MONITOR_UPDATE_MASTER_SERVER_BATTLE_SERVER_LOGIN_COUNT,
	MONITOR_UPDATE_MASTER_SERVER_BATTLE_SERVER_STANDBY_ROOM_COUNT,


	MONITOR_UPDATE_BATTLE_SERVER_ON,
	MONITOR_UPDATE_BATTLE_SERVER_TOTAL_CPU_USAGE,
	MONITOR_UPDATE_BATTLE_SERVER_MEMORY_USER_COMMIT,
	MONITOR_UPDATE_BATTLE_SERVER_PACKET_POOL_USAGE,
	MONITOR_UPDATE_BATTLE_SERVER_CURRENT_AUTHORIZE_THREAD_LOOP_COUNT,
	MONITOR_UPDATE_BATTLE_SERVER_CURRENT_GAME_THREAD_LOOP_COUNT,
	MONITOR_UPDATE_BATTLE_SERVER_TOTAL_SESSION_COUNT,
	MONITOR_UPDATE_BATTLE_SERVER_AUTHORIZING_SESSION_COUNT,
	MONITOR_UPDATE_BATTLE_SERVER_GAMING_SESSION_COUNT,
	MONITOR_UPDATE_BATTLE_SERVER_PLAYER_COUNT,
	MONITOR_UPDATE_BATTLE_SERVER_WAITING_ROOM_COUNT,
	MONITOR_UPDATE_BATTLE_SERVER_PLAYING_ROOM_COUNT,

	MONITOR_UPDATE_CHAT_SERVER_ON,
	MONITOR_UPDATE_CHAT_TOTAL_CPU_USAGE,
	MONITOR_UPDATE_CHAT_MEMORY_USER_COMMIT,
	MONITOR_UPDATE_CHAT_PACKET_POOL_USAGE,
	MONITOR_UPDATE_CHAT_SESSION,
	MONITOR_UPDATE_CHAT_PLAYER_COUNT,
	MONITOR_UPDATE_CHAT_ROOM_COUNT
};

struct InnerClient
{
	UINT64 SessionKey;
	UINT Index;

	BYTE datumType;
	INT datumValue;
	INT timeStamp;

	DWORD PreviousHeartbeatTime;

	SRWLOCK Lock;
};

struct MonitorClient
{
	UINT64 SessionKey;

	CHAR MonitorClientKey[32];

	DWORD PreviousHeartbeatTime;

	SRWLOCK Lock;
};

struct InnerWork
{
	UINT64 SessionKey;
	SerialPacketQueue<IOCP_PacketHeader>* PacketSPQ_Address;
};

struct MonitorWork
{
	UINT64 SessionKey;
	SerialPacketQueue<IOCP_PacketHeader>* PacketSPQ_Address;
};

typedef list<InnerClient> InnerClientL;
typedef list<InnerClient>::iterator InnerClientLI;

typedef list<InnerClient*> InnerClientAddressL;
typedef list<InnerClient*>::iterator InnerClientAddressLI;

typedef list<MonitorClient> MonitorClientL;
typedef list<MonitorClient>::iterator MonitorClientLI;

typedef list<MonitorClient*> MonitorClientAddressL;
typedef list<MonitorClient*>::iterator MonitorClientAddressLI;