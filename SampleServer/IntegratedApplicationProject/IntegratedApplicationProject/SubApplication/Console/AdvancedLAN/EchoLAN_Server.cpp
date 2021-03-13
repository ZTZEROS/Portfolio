#include "stdafx.h"
#include "AdvancedLAN.h"
#include "BaseLAN_Server.h"
#include "EchoLAN_Server.h"

AdvancedLAN_ClientPlayerL EchoLAN_Server::ALAN_CPL;
SRWLOCK EchoLAN_Server::ALAN_CPL_LOCK;

EchoLAN_Server::EchoLAN_Server()
{
	ConnectionCount = 0;
	DisconnectionCount = 0;
	ReceiveCount = 0;
	SendCount = 0;
}

EchoLAN_Server::~EchoLAN_Server() {}

bool EchoLAN_Server::Initialize()
{
	BaseLAN_Server::Initialize();

	InitializeSRWLock(&ALAN_CPL_LOCK);

	return 0;
}

bool EchoLAN_Server::Update()
{
	if (TIMER.CheckUpdate())
	{
		//CurerntClientCount = ALAN_CPL.size();

		if (GetAsyncKeyState('Q')) return 1;
	}

	//{
	//	AdvancedLAN_ClientPlayerL playerL;
	//	UINT result;
	//
	//	AcquireSRWLockShared(&ALAN_CPL_LOCK);
	//	playerL = ALAN_CPL;
	//	ReleaseSRWLockShared(&ALAN_CPL_LOCK);
	//
	//	for (AdvancedLAN_ClientPlayerLI playerLI = playerL.begin(); playerLI != playerL.end(); ++playerLI)
	//	{
	//		//copy SBQ
	//		//{
	//		//
	//		//}
	//
	//		//copy SPQ_Address
	//		//one packet per loop
	//		{
	//			AdvancedLAN_PacketHeader receivedPacketHeader;
	//			AdvancedLAN_PacketHeader sendingPacketHeader;
	//			SerialPacketQueue<AdvancedLAN_PacketHeader> receivedPacketSPQ;
	//			SerialPacketQueue<AdvancedLAN_PacketHeader>* sendingPacketSPQ_Address;
	//
	//			result = ReceivePacket(playerLI->SessionKey, receivedPacketSPQ);
	//			if (result)
	//			{
	//				receivedPacketHeader = receivedPacketSPQ.GetHeader();
	//				sendingPacketHeader = receivedPacketHeader;
	//
	//				InterlockedIncrement(&SPQ_ALLOCATION_COUNT);
	//				sendingPacketSPQ_Address = new SerialPacketQueue<AdvancedLAN_PacketHeader>;
	//
	//				*sendingPacketSPQ_Address = receivedPacketSPQ;
	//				sendingPacketSPQ_Address->SetHeader(sendingPacketHeader);
	//			}
	//			else continue;
	//
	//			result = SendPacket(playerLI->SessionKey, sendingPacketSPQ_Address);
	//			if (result) OnSend(playerLI->SessionKey, sendingPacketSPQ_Address->GetTotalSize());
	//			else
	//			{
	//				//센드큐가 가득차는 상황(==유저가 안받는 상황)에는 유저를 강제로 끊는다
	//
	//				continue;
	//			}
	//
	//			PostSend(playerLI->SessionKey);
	//		}
	//
	//		//copy SPQ_Address
	//		//all packet per loop
	//		//while (1)
	//		//{
	//		//	AdvancedLAN_PacketHeader receivedPacketHeader;
	//		//	AdvancedLAN_PacketHeader sendingPacketHeader;
	//		//	SerialPacketQueue<AdvancedLAN_PacketHeader> receivedPacketSPQ;
	//		//	SerialPacketQueue<AdvancedLAN_PacketHeader>* sendingPacketSPQ_Address;
	//		//
	//		//	result = ReceivePacket(playerLI->SessionKey, receivedPacketSPQ);
	//		//	if (result)
	//		//	{
	//		//		receivedPacketHeader = receivedPacketSPQ.GetHeader();
	//		//		sendingPacketHeader = receivedPacketHeader;
	//		//
	//		//		InterlockedIncrement(&SPQ_ALLOCATION_COUNT);
	//		//		sendingPacketSPQ_Address = new SerialPacketQueue<AdvancedLAN_PacketHeader>;
	//		//
	//		//		*sendingPacketSPQ_Address = receivedPacketSPQ;
	//		//		sendingPacketSPQ_Address->SetHeader(sendingPacketHeader);
	//		//	}
	//		//	else break;
	//		//
	//		//	result = SendPacket(playerLI->SessionKey, sendingPacketSPQ_Address);
	//		//	if (result);
	//		//	else
	//		//	{
	//		//		Disconnect(sessionKey); //센드큐가 가득차는 상황(==유저가 안받는 상황)에는 유저를 강제로 끊는다
	//		//
	//		//		break;
	//		//	}
	//		//}
	//		//
	//		//PostSend(playerLI->SessionKey);
	//	}
	//
	//}

	return 0;
}

bool EchoLAN_Server::Render()
{
	if (TIMER.ShowCounter())
	{
		printf("CurrentClientCount : %u" CRALF, CurrentClientCount);
		printf("ConnectionCount : %u" CRALF, ConnectionCount);
		printf("DisconnectionCount : %u" CRALF, DisconnectionCount);
		printf("ReceiveCount : %u" CRALF, ReceiveCount);
		printf("SendCount : %u" CRALF, SendCount);
		printf("SPQ_ALLOCATION_COUNT : %u" CRALF, SPQ_ALLOCATION_COUNT);
		printf("SPQ_DEALLOCATION_COUNT : %u" CRALF, SPQ_DEALLOCATION_COUNT);
		printf(CRALF);

		ReceiveCount = 0;
		SendCount = 0;
	}

	return 0;
}

bool EchoLAN_Server::Terminalize()
{
	BaseLAN_Server::Terminalize();

	return 0;
}

void EchoLAN_Server::OnClientJoin(SOCKADDR_IN clientAddress, UINT64 sessionKey)
{
	AdvancedLAN_PacketHeader sendingPacketHeader;
	SerialPacketQueue<AdvancedLAN_PacketHeader>* sendingPacketSPQ_Address;

	AdvancedLAN_ClientPlayer joinedClientPlayer;
	INT64 loginConfirmValue;

	ZeroMemory(&sendingPacketHeader, sizeof(sendingPacketHeader));

	joinedClientPlayer.SessionKey = sessionKey;
	InitializeSRWLock(&joinedClientPlayer.Lock);

	//AcquireSRWLockExclusive(&ALAN_CPL_LOCK);
	//ALAN_CPL.push_back(joinedClientPlayer);
	//ReleaseSRWLockExclusive(&ALAN_CPL_LOCK);



	loginConfirmValue = 0x7fffffffffffffff;

	sendingPacketHeader.BodySize = sizeof(loginConfirmValue);

	InterlockedIncrement(&SPQ_ALLOCATION_COUNT);
	sendingPacketSPQ_Address = new SerialPacketQueue<AdvancedLAN_PacketHeader>;
	*sendingPacketSPQ_Address << loginConfirmValue;
	sendingPacketSPQ_Address->SetHeader(sendingPacketHeader);



	SendPacket(joinedClientPlayer.SessionKey, sendingPacketSPQ_Address);

	InterlockedIncrement(&CurrentClientCount);
	//CurrentClientCount = ALAN_CPL.size();
	++ConnectionCount;
}

void EchoLAN_Server::OnClientLeave(UINT64 sessionKey)
{
	//AcquireSRWLockExclusive(&ALAN_CPL_LOCK);
	//for (AdvancedLAN_ClientPlayerLI playerLI = ALAN_CPL.begin(); playerLI != ALAN_CPL.end(); ++playerLI)
	//{
	//	if (playerLI->SessionKey == sessionKey)
	//	{
	//		ALAN_CPL.erase(playerLI);
	//
	//		break;
	//	}
	//}
	//ReleaseSRWLockExclusive(&ALAN_CPL_LOCK);

	InterlockedDecrement(&CurrentClientCount);
	//CurrentClientCount = ALAN_CPL.size();
	++DisconnectionCount;
}

bool EchoLAN_Server::OnConnectionRequest(SOCKADDR_IN clientAddress, UINT port)
{
	if (0) return 1;

	return 0;
}

void EchoLAN_Server::OnRecv(UINT64 sessionKey, UINT receivedSize)
{
	//AdvancedLAN_PacketHeader receivedPacketHeader;
	//AdvancedLAN_PacketHeader sendingPacketHeader;
	//SerialPacketQueue<AdvancedLAN_PacketHeader> receivedPacketSPQ;
	//SerialPacketQueue<AdvancedLAN_PacketHeader>* sendingPacketSPQ_Address;
	
	UINT result;

	while (1)
	{
		AdvancedLAN_PacketHeader receivedPacketHeader;
		AdvancedLAN_PacketHeader sendingPacketHeader;
		SerialPacketQueue<AdvancedLAN_PacketHeader> receivedPacketSPQ;
		SerialPacketQueue<AdvancedLAN_PacketHeader>* sendingPacketSPQ_Address;

		//UINT result;

		result = ReceivePacket(sessionKey, &receivedPacketSPQ);
		if (result)
		{
			receivedPacketHeader = receivedPacketSPQ.GetHeader();
			sendingPacketHeader = receivedPacketHeader;
	
			InterlockedIncrement(&SPQ_ALLOCATION_COUNT);
			sendingPacketSPQ_Address = new SerialPacketQueue<AdvancedLAN_PacketHeader>;
			*sendingPacketSPQ_Address = receivedPacketSPQ;
	
			sendingPacketSPQ_Address->SetHeader(sendingPacketHeader);
		}
		else break;
	
		result = SendPacket(sessionKey, sendingPacketSPQ_Address);
		if (result); //receivedPacketSPQ.DecreaseCurrentSize(result); //~receivedPacketSPQ();
		else
		{
			PostDisconnect(sessionKey);	//센드큐가 가득차는 상황(==유저가 안받는 상황)에는 유저를 강제로 끊는다
	
			break;
		}
	}
	
	if(result) PostSend(sessionKey);

	//if (!sendingPacketSPQ_Address->DecreaseReferenceCount())
	//{
	//	InterlockedIncrement(&SPQ_DEALLOCATION_COUNT);
	//	delete sendingPacketSPQ_Address;
	//}

	InterlockedIncrement(&ReceiveCount);

	return;
}

void EchoLAN_Server::OnSend(UINT64 sessionKey, UINT sendedSize)
{
	InterlockedIncrement(&SendCount);

	return;
}

void EchoLAN_Server::OnWorkerThreadBegin()
{
	return;
}

void EchoLAN_Server::OnWorkerThreadEnd()
{
	return;
}

void EchoLAN_Server::OnError(UINT errorIndex, LPCWSTR errorText)
{
	wprintf(L"[Error %u] %s", errorIndex, errorText);

	return;
}
