#include "stdafx.h"
#include "AdvancedLAN.h"
#include "BaseLAN_Server.h"
#include "DerivedLAN_Server.h"

DerivedLAN_Server::DerivedLAN_Server()
{
	ConnectionCount = 0;
	DisconnectionCount = 0;
	ReceiveCount = 0;
	SendCount = 0;
}

DerivedLAN_Server::~DerivedLAN_Server() {}

bool DerivedLAN_Server::Initialize()
{
	BaseLAN_Server::Initialize();

	InitializeSRWLock(&ALAN_CPL_LOCK);

	return 0;
}

bool DerivedLAN_Server::Update()
{
	if (TIMER.CheckUpdate())
	{
		if (GetAsyncKeyState('Q')) return 1;
	}

	return 0;
}

bool DerivedLAN_Server::Render()
{
	if (TIMER.ShowCounter())
	{
		printf(CRALF);
		printf("CurrentClientCount : %d" CRALF, CurrentClientCount);
		printf("ConnectionCount : %d" CRALF, ConnectionCount);
		printf("DisconnectionCount : %d" CRALF, DisconnectionCount);
		printf("ReceiveCount : %d" CRALF, ReceiveCount);
		printf("SendCount : %d" CRALF, SendCount);
		printf("SPQ_ALLOCATION_COUNT : %d" CRALF, SPQ_ALLOCATION_COUNT);
		printf("SPQ_DEALLOCATION_COUNT : %d" CRALF, SPQ_DEALLOCATION_COUNT);
		printf(CRALF);
	
		ReceiveCount = 0;
		SendCount = 0;
	}

	return 0;
}

bool DerivedLAN_Server::Terminalize()
{
	BaseLAN_Server::Terminalize();

	return 0;
}

void DerivedLAN_Server::OnClientJoin(SOCKADDR_IN clientAddress, UINT64 sessionKey)
{
	AdvancedLAN_PacketHeader sendingPacketHeader;
	SerialPacketQueue<AdvancedLAN_PacketHeader>* sendingPacketSPQ_Address;

	AdvancedLAN_ClientPlayer advancedLAN_ClientPlayer;
	INT64 loginConfirmValue;

	ZeroMemory(&sendingPacketHeader, sizeof(sendingPacketHeader));

	advancedLAN_ClientPlayer.SessionKey = sessionKey;
	InitializeSRWLock(&advancedLAN_ClientPlayer.LAN_ClientLock);



	AcquireSRWLockExclusive(&ALAN_CPL_LOCK);
	ALAN_CPL.push_back(advancedLAN_ClientPlayer);
	ReleaseSRWLockExclusive(&ALAN_CPL_LOCK);



	loginConfirmValue = 0x7fffffffffffffff;

	sendingPacketHeader.BodySize = sizeof(loginConfirmValue);

	++SPQ_ALLOCATION_COUNT;
	sendingPacketSPQ_Address = new SerialPacketQueue<AdvancedLAN_PacketHeader>;
	*sendingPacketSPQ_Address << loginConfirmValue;
	sendingPacketSPQ_Address->SetHeader(sendingPacketHeader);


	SendPacket(advancedLAN_ClientPlayer.SessionKey, sendingPacketSPQ_Address);



	//++CurrentClientCount;
	CurrentClientCount = ALAN_CPL.size();

	++ConnectionCount;

	return;
}

void DerivedLAN_Server::OnClientLeave(UINT64 sessionKey)
{
	AcquireSRWLockExclusive(&ALAN_CPL_LOCK);
	for (AdvancedLAN_ClientPlayerLI playerLI = ALAN_CPL.begin(); playerLI != ALAN_CPL.end(); ++playerLI)
	{
		if (playerLI->SessionKey == sessionKey)
		{
			ALAN_CPL.erase(playerLI);

			//printf("Session %d is erased aleady." CRALF, sessionKey);

			break;
		}
	}
	ReleaseSRWLockExclusive(&ALAN_CPL_LOCK);

	//--CurrentClientCount;
	CurrentClientCount = ALAN_CPL.size();

	++DisconnectionCount;

	return;
}

bool DerivedLAN_Server::OnConnectionRequest(SOCKADDR_IN clientAddress, UINT port)
{
	if (0)
	{
		return 0;
	}

	//printf("%d::%d Connection allowed." CRALF, clientAddress.sin_addr, port);

	return 1;
}

void DerivedLAN_Server::OnRecv(UINT64 sessionKey, UINT receivedSize)
{
	UINT result;

	while(1)
	{
		AdvancedLAN_PacketHeader receivedPacketHeader;
		AdvancedLAN_PacketHeader sendingPacketHeader;
		SerialPacketQueue<AdvancedLAN_PacketHeader> receivedPacketSPQ;
		SerialPacketQueue<AdvancedLAN_PacketHeader>* sendingPacketSPQ_Address;
	
		result = ReceivePacket(sessionKey, receivedPacketSPQ);
		if (result)
		{
			receivedPacketHeader = receivedPacketSPQ.GetHeader();
			sendingPacketHeader = receivedPacketHeader;
			
			++SPQ_ALLOCATION_COUNT;
			sendingPacketSPQ_Address = new SerialPacketQueue<AdvancedLAN_PacketHeader>;
			*sendingPacketSPQ_Address = receivedPacketSPQ;
			
			sendingPacketSPQ_Address->SetHeader(sendingPacketHeader);

			result = SendPacket(sessionKey, sendingPacketSPQ_Address); //이미 디스커넥트된 세션이라면 센드큐를 다시 한번 비워줘야 한다
			if (result) OnSend(sessionKey, sendingPacketSPQ_Address->GetTotalSize());
			else
			{
				//센드큐가 가득차는 상황(유저가 안받는 상황)에는 유저를 강제로 끊는다

				break;
			}
		}
		else break;
	}

	++ReceiveCount;

	return;
}

void DerivedLAN_Server::OnSend(UINT64 sessionKey, UINT sendedSize)
{
	++SendCount;

	return;
}

void DerivedLAN_Server::OnWorkerThreadBegin()
{
	return;
}

void DerivedLAN_Server::OnWorkerThreadEnd()
{
	return;
}

void DerivedLAN_Server::OnError(UINT errorIndex, LPWSTR errorText)
{
	printf("[Error] %u, %s", errorIndex, errorText);

	return;
}
