#pragma once

class MMO_DerivedClient : public MMO_BaseClient
{
private:
	UINT64 SessionKey; //SessionID

	UINT64 AccountKey; //AccountNo
	//WCHAR RealName[MMO_CLIENT_REAL_NAME_LENGTH];
	//WCHAR Nickname[MMO_CLIENT_NICKNAME_LENGTH];
	CHAR PlayerKey[MMO_CLIENT_PLAYER_KEY_LENGTH];

	UINT SectorIndexX;
	UINT SectorIndexY;

	UINT MessageSize;
	WCHAR Message[MMO_CLIENT_PLAYER_MESSAGE_LENGTH];

	DWORD PreviousHeartbeatTime;

	UINT Version;
	LONGLONG CurrentTick;

	SRWLOCK Lock;

public:
	MMO_DerivedClient();
	virtual ~MMO_DerivedClient();

	bool Initialize();
	bool Update();
	bool Render();
	bool Terminalize();



	virtual unsigned int OnAT_ClientJoin();
	virtual unsigned int OnAT_ClientReceive();
	virtual unsigned int OnAT_ClientLeave();

	virtual unsigned int OnGT_ClientJoin();
	virtual unsigned int OnGT_ClientReceive();
	virtual unsigned int OnGT_ClientLeave();
	virtual unsigned int OnGT_ClientDisconnect();
};

typedef vector<MMO_DerivedClient> MMO_DerivedClientV;
typedef vector<MMO_DerivedClient>::iterator MMO_DerivedClientVI;

typedef vector<MMO_DerivedClient*> MMO_DerivedClientAddressV;
typedef vector<MMO_DerivedClient*>::iterator MMO_DerivedClientAddressVI;