#include "stdafx.h"
#include "AnotherLan.h"
#include "BaseLAN_Server.h"
#include "DerivedLAN_Server.h"

DerivedLAN_Server::DerivedLAN_Server()
{ 
	ConnectionCount = 0;
	MaximumClientCount = 0;
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

	{
		AnotherLAN_ClientPlayerL anotherLAN_ClientPlayerL;
	
		AcquireSRWLockShared(&ALAN_CPL_LOCK);
		anotherLAN_ClientPlayerL = ALAN_CPL;
		ReleaseSRWLockShared(&ALAN_CPL_LOCK);
	
		for (AnotherLAN_ClientPlayerLI playerLI = anotherLAN_ClientPlayerL.begin(); playerLI != anotherLAN_ClientPlayerL.end(); ++playerLI)
		{
			//copy SBQ
			//{
			//	WORD receivedPacketHeader;
			//	WORD sendingPacketHeader;
			//
			//	SerialByteQueue receivedPacketSBQ(sizeof(receivedPacketHeader), BUFFER_SIZE);
			//	//SerialByteQueue sendingPacketSBQ(sizeof(sendingPacketHeader), BUFFER_SIZE);
			//	SerialByteQueue sendingPacketSBQ;
			//
			//
			//	if (ReceivePacket(playerLI->SessionKey, receivedPacketSBQ));
			//	else
			//	{
			//		//sendingPacketHeader = receivedPacketHeader;
			//		sendingPacketSBQ = receivedPacketSBQ; //copy deeper. sendingPacketSBQ.QueueAddress == receivedPacketSBQ.Queue
			//		//sendingPacketSBQ.SetHeader(sendingPacketHeader);
			//	}
			//
			//	if (SendPacket(playerLI->SessionKey, sendingPacketSBQ));
			//	else
			//	{
			//		//센드큐가 가득차는 상황(유저가 안받는 상황)에는 유저를 강제로 끊는다
			//	}
			//}

			//copy SBQ_Address
			{
				AnotherLAN_PacketHeader receivedPacketHeader;
				AnotherLAN_PacketHeader sendingPacketHeader;
				SerialPacketQueue<AnotherLAN_PacketHeader> receivedPacketSPQ;
				SerialPacketQueue<AnotherLAN_PacketHeader>* sendingPacketSPQ_Address;

				ZeroMemory(&receivedPacketHeader, sizeof(receivedPacketHeader));
				ZeroMemory(&sendingPacketHeader, sizeof(sendingPacketHeader));
				sendingPacketSPQ_Address = nullptr;

				if (ReceivePacket(playerLI->SessionKey, receivedPacketSPQ));
				else
				{
					receivedPacketHeader = receivedPacketSPQ.GetHeader();
					sendingPacketHeader = receivedPacketHeader;
					
					InterlockedIncrement(&SBQ_ALLOCATION_COUNT);
					sendingPacketSPQ_Address = new SerialPacketQueue<AnotherLAN_PacketHeader>;

					*sendingPacketSPQ_Address = receivedPacketSPQ;
					sendingPacketSPQ_Address->SetHeader(sendingPacketHeader);
				}
				
				//if (sendingPacketSPQ_Address)
				{
					if (SendPacket(playerLI->SessionKey, sendingPacketSPQ_Address));
					else
					{
						//센드큐가 가득차는 상황(유저가 안받는 상황)에는 유저를 강제로 끊는다
					}
				}
			}
		}
	}

	return 0;
}

bool DerivedLAN_Server::Render()
{
	if (TIMER.ShowCounter())
	{
		printf(CRALF);
		printf("CurrentClientCount : %u" CRALF, CurrentClientCount);
		printf("MaximumClientCount : %u" CRALF, MaximumClientCount);
		printf("ConnectionCount : %u" CRALF, ConnectionCount);
		printf("DisconnectionCount : %u" CRALF, DisconnectionCount);

		printf("GQCS_DISCONNECTION_COUNT : %u" CRALF, GQCS_DISCONNECTION_COUNT);
		printf("RCV_DISCONNECTION_COUNT : %u" CRALF, RCV_DISCONNECTION_COUNT);
		printf("SND_DISCONNECTION_COUNT : %u" CRALF, SND_DISCONNECTION_COUNT);
		
		printf("ACCEPT_LEAK_COUNT : %u" CRALF, ACCEPT_LEAK_COUNT);
		printf("DISCONNECT_LEAK_COUNT : %u" CRALF, DISCONNECT_LEAK_COUNT);
		printf("RCV_LEAK? : %u" CRALF, RCV_LEAK);
		printf("SND_LEAK? : %u" CRALF, SND_LEAK);
		//printf("RCV_GHOST_SESSION_COUNT : %u" CRALF, RCV_GHOST_SESSION_COUNT);
		//printf("SND_GHOST_SESSION_COUNT : %u" CRALF, SND_GHOST_SESSION_COUNT);
		printf("PACKET_ERROR? : %u" CRALF, PACKET_ERROR);
		printf("SLOW_DISCONNECTION_COUNT : %u" CRALF, SLOW_DISCONNECTION_COUNT);

		printf("ReceiveCount : %u" CRALF, ReceiveCount);
		printf("SendCount : %u" CRALF, SendCount);
		printf("SBQ_ALLOCATION_COUNT : %d" CRALF, SBQ_ALLOCATION_COUNT);
		printf("SBQ_DEALLOCATION_COUNT : %d" CRALF, SBQ_DEALLOCATION_COUNT);
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
	AnotherLAN_PacketHeader sendingPacketHeader;
	SerialPacketQueue<AnotherLAN_PacketHeader> sendingPacketSPQ;
	SerialPacketQueue<AnotherLAN_PacketHeader>* sendingPacketSPQ_Address;

	AnotherLAN_ClientPlayer advancedLanClientPlayer;
	INT64 loginConfirmValue;

	ZeroMemory(&sendingPacketHeader, sizeof(sendingPacketHeader));
	sendingPacketSPQ.SetHeader(sendingPacketHeader);

	
	advancedLanClientPlayer.SessionKey = sessionKey;
	InitializeSRWLock(&advancedLanClientPlayer.LAN_ClientLock);



	AcquireSRWLockExclusive(&ALAN_CPL_LOCK);
	ALAN_CPL.push_back(advancedLanClientPlayer);
	ReleaseSRWLockExclusive(&ALAN_CPL_LOCK);



	loginConfirmValue = 0x7fffffffffffffff;

	sendingPacketHeader.BodySize = sizeof(loginConfirmValue);

	InterlockedIncrement(&SBQ_ALLOCATION_COUNT);
	sendingPacketSPQ_Address = new SerialPacketQueue<AnotherLAN_PacketHeader>;
	*sendingPacketSPQ_Address << loginConfirmValue;
	sendingPacketSPQ_Address->SetHeader(sendingPacketHeader);


	//SendPacket(advancedLanClientPlayer.SessionKey, sendingPacketSBQ);
	SendPacket(advancedLanClientPlayer.SessionKey, sendingPacketSPQ_Address);



	//++CurrentClientCount;
	CurrentClientCount = ALAN_CPL.size();
	if (MaximumClientCount < CurrentClientCount) MaximumClientCount = CurrentClientCount;
	
	++ConnectionCount;

	return;
}

void DerivedLAN_Server::OnClientLeave(UINT64 sessionKey)
{
	AcquireSRWLockExclusive(&ALAN_CPL_LOCK);
	for(AnotherLAN_ClientPlayerLI playerLI = ALAN_CPL.begin(); playerLI != ALAN_CPL.end(); ++playerLI)
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
	if(0)
	{
		return 0;
	}

	//printf("%d::%d Connection allowed." CRALF, clientAddress.sin_addr, port);

	return 1;
}

void DerivedLAN_Server::OnRecv(UINT64 sessionKey, UINT receivedSize)
{
	//{
	//	WORD receivedPacketHeader;
	//	WORD sendingPacketHeader;
	//	SerialByteQueue receivedPacketSBQ(sizeof(receivedPacketHeader), BUFFER_SIZE);
	//	SerialByteQueue sendingPacketSBQ(sizeof(sendingPacketHeader), BUFFER_SIZE);
	//
	//	if (ReceivePacket(sessionKey, receivedPacketSBQ))
	//	{
	//		//receivedPacketHeader = receivedPacketSBQ.GetHeader(receivedPacketHeader);
	//		//sendingPacketHeader = receivedPacketHeader;
	//		sendingPacketSBQ = receivedPacketSBQ;
	//		//sendingPacketSBQ.SetHeader(sendingPacketHeader);
	//	}
	//
	//	if (SendPacket(sessionKey, sendingPacketSBQ));
	//	{
	//		//센드큐가 가득차는 상황(유저가 안받는 상황)에는 유저를 강제로 끊는다
	//	}
	//}

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
	printf("ThreadBegin" CRALF);

	return;
}

void DerivedLAN_Server::OnWorkerThreadEnd()
{
	printf("ThreadEnd" CRALF);

	return;
}

void DerivedLAN_Server::OnError(UINT errorIndex, LPWSTR errorText)
{
	printf("[Error] %d, %s", errorIndex, errorText);

	return;
}
