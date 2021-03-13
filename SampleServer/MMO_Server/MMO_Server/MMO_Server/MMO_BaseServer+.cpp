#include "pch.h"
#include "MMO.h"
#include "MMO_BaseClient.h"
#include "MMO_DerivedClient.h"
#include "MMO_BaseServer.h"

unsigned int WINAPI MMO_BaseServer::AcceptThread(LPVOID selfInstanceAddress)
{
	MMO_BaseServer* thisInstanceAddress;

	thisInstanceAddress = (MMO_BaseServer*)selfInstanceAddress;
	thisInstanceAddress->AcceptThread();

	return 0;
}

unsigned int WINAPI MMO_BaseServer::DisconnectThread(LPVOID selfInstanceAddress)
{
	MMO_BaseServer* thisInstanceAddress;

	thisInstanceAddress = (MMO_BaseServer*)selfInstanceAddress;
	thisInstanceAddress->DisconnectThread();

	return 0;
}

unsigned int WINAPI MMO_BaseServer::SendThread(LPVOID selfInstanceAddress)
{
	MMO_BaseServer* thisInstanceAddress;

	thisInstanceAddress = (MMO_BaseServer*)selfInstanceAddress;
	thisInstanceAddress->SendThread();

	return 0;
}

unsigned int WINAPI MMO_BaseServer::AuthorizeThread(LPVOID selfInstanceAddress)
{
	MMO_BaseServer* thisInstanceAddress;

	thisInstanceAddress = (MMO_BaseServer*)selfInstanceAddress;
	thisInstanceAddress->AuthorizeThread();

	return 0;
}

unsigned int WINAPI MMO_BaseServer::GameThread(LPVOID selfInstanceAddress)
{
	MMO_BaseServer* thisInstanceAddress;

	thisInstanceAddress = (MMO_BaseServer*)selfInstanceAddress;
	thisInstanceAddress->GameThread();

	return 0;
}

unsigned int WINAPI MMO_BaseServer::WorkThread(LPVOID selfInstanceAddress)
{
	MMO_BaseServer* thisInstanceAddress;

	thisInstanceAddress = (MMO_BaseServer*)selfInstanceAddress;
	thisInstanceAddress->WorkThread();

	return 0;
}