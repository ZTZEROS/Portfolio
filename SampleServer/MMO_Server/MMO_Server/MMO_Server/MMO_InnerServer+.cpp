#include "pch.h"
#include "MMO.h"
#include "MMO_InnerServer.h"

void MMO_InnerServer::OnClientJoin(SOCKADDR_IN clientAddress, UINT64 sessionKey)
{

}

void MMO_InnerServer::OnClientLeave(UINT64 sessionKey)
{

}

bool MMO_InnerServer::OnConnectionRequest(SOCKADDR_IN clientAddress, UINT port)
{
	return 0;
}

void MMO_InnerServer::OnRecv(UINT64 sessionKey, UINT receivedSize)
{

}

void MMO_InnerServer::OnSend(UINT64 sessionKey, UINT sendedSize)
{

}

void MMO_InnerServer::OnWorkerThreadBegin()
{

}

void MMO_InnerServer::OnWorkerThreadEnd()
{

}

void MMO_InnerServer::OnError(UINT errorNumber, LPCWSTR errorText)
{

}