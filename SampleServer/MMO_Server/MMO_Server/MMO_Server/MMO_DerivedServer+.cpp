#include "pch.h"
#include "MMO.h"
#include "MMO_BaseClient.h"
#include "MMO_DerivedClient.h"
#include "MMO_InnerClient.h"
#include "MMO_BaseServer.h"
#include "MMO_DerivedServer.h"

unsigned int MMO_DerivedServer::OnAuthorizeThreadUpdate()
{
	return 0;
}

unsigned int MMO_DerivedServer::OnGameThreadUpdate()
{
	return 0;
}

unsigned int  MMO_DerivedServer::OnConnect()
{
	InterlockedIncrement(&CurrentClientCount);
	++ConnectedClientCount;

	return 0;
}

unsigned int MMO_DerivedServer::OnReceive(UINT receivedSize)
{
	InterlockedIncrement(&CurrentReceiveCount);
	InterlockedIncrement(&ReceiveCount);
	InterlockedExchange(&ReceivedSize, ReceivedSize + receivedSize);

	return 0;
}

unsigned int MMO_DerivedServer::OnSend(UINT sendedSize)
{
	InterlockedIncrement(&CurrentSendCount);
	InterlockedIncrement(&SendCount);
	InterlockedExchange(&SendedSize, SendedSize + sendedSize);

	return 0;
}

unsigned int MMO_DerivedServer::OnDisconnect()
{
	InterlockedDecrement(&CurrentClientCount);
	++DisconnectedClientCount;

	return 0;
}