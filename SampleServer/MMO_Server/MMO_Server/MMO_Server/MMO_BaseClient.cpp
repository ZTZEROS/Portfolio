#include "pch.h"
#include "MMO.h"
#include "MMO_BaseClient.h"
#include "MMO_BaseServer.h"

MMO_BaseClient::MMO_BaseClient()
{
	CS.Socket = INVALID_SOCKET;

	ZeroMemory(&CS.Address, sizeof(CS.Address));

	CS.Key = 0;
	CS.Index = 0;
	CS.Connected = 0;
	CS.Sendable = 0;
	CS.ReceiveCount = 0;
	CS.SendCount = 0;

	ZeroMemory(&CS.OverlappedReceive, sizeof(CS.OverlappedReceive));
	ZeroMemory(&CS.OverlappedSend, sizeof(CS.OverlappedSend));

	CS.IO_Count = 0;

	CS.CurrentPhase = MMO_CLIENT_PHASE_ACCEPT;

	InitializeSRWLock(&CS.Lock);
}

MMO_BaseClient::~MMO_BaseClient()
{

}

bool MMO_BaseClient::Initialize()
{
	CS.Socket = INVALID_SOCKET;

	ZeroMemory(&CS.Address, sizeof(CS.Address));

	CS.Key = 0;
	CS.Index = 0;
	CS.Connected = 0;
	CS.Sendable = 0;
	CS.ReceiveCount = 0;
	CS.SendCount = 0;

	ZeroMemory(&CS.OverlappedReceive, sizeof(CS.OverlappedReceive));
	ZeroMemory(&CS.OverlappedSend, sizeof(CS.OverlappedSend));

	CS.IO_Count = 0;

	CS.CurrentPhase = MMO_CLIENT_PHASE_ACCEPT;

	InitializeSRWLock(&CS.Lock);

	return 0;
}

bool MMO_BaseClient::Update()
{
	return 0;
}

bool MMO_BaseClient::Render()
{
	return 0;
}

bool MMO_BaseClient::Terminalize()
{
	return 0;
}