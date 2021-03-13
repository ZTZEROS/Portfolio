#include "stdafx.h"
#include "Lan.h"
#include "BaseLanServer.h"
#include "DerivedLanServer.h"

DerivedLanServer::DerivedLanServer() { ConnectionCount = 0; DisconnectionCount = 0; ReceiveCount = 0; SendCount = 0; }
DerivedLanServer::~DerivedLanServer() {}

bool DerivedLanServer::Initialize()
{
	BaseLanServer::Initialize();

	InitializeSRWLock(&LCPM_LOCK);

	return 0;
}

bool DerivedLanServer::Update()
{
	if (TIMER.CheckUpdate())
	{
		if (GetAsyncKeyState('Q')) return 1;
	}

	{
		LanClientPlayerM lanClientPlayerM;

		AcquireSRWLockShared(&LCPM_LOCK);
		lanClientPlayerM = LCPM;
		ReleaseSRWLockShared(&LCPM_LOCK);

		for (LanClientPlayerMI lanClientPlayerMI = lanClientPlayerM.begin(); lanClientPlayerMI != lanClientPlayerM.end(); ++lanClientPlayerMI)
		{
			WORD receivedPacketHeader;
			WORD sendingPacketHeader;
			SerialByteQueue receivedPacketSBQ(sizeof(receivedPacketHeader), BUFFER_SIZE);
			SerialByteQueue sendingPacketSBQ(sizeof(sendingPacketHeader), BUFFER_SIZE);

			LanClientPlayer* lanClientPlayerAddress;

			lanClientPlayerAddress = &lanClientPlayerMI->second;

			if (ReceivePacket(lanClientPlayerAddress->SessionKey, receivedPacketSBQ));
			else
			{
				//sendingPacketHeader = receivedPacketHeader;
				sendingPacketSBQ = receivedPacketSBQ;
				//sendingPacketSBQ.SetHeader(sendingPacketHeader);

				if (SendPacket(lanClientPlayerAddress->SessionKey, sendingPacketSBQ));
				//센드큐가 가득차는 상황(유저가 안받는 상황)에는 유저를 강제로 끊는다
			}
		}
		
	}

	return 0;
}

bool DerivedLanServer::Render()
{
	if (TIMER.ShowCounter())
	{
		printf(CRALF);
		printf("CurrentClientCount : %d" CRALF, CurrentClientCount);
		printf("ConnectionCount : %d" CRALF, ConnectionCount);
		printf("DisconnectionCount : %d" CRALF, DisconnectionCount);
		printf("ReceiveCount : %d" CRALF, ReceiveCount);
		printf("SendCount : %d" CRALF, SendCount);
		printf(CRALF);

		ReceiveCount = 0;
		SendCount = 0;
	}

	return 0;
}

bool DerivedLanServer::Terminalize()
{
	BaseLanServer::Terminalize();

	return 0;
}

void DerivedLanServer::OnClientJoin(SOCKADDR_IN clientAddress, unsigned int sessionKey)
{
	WORD sendingPacketHeader;
	SerialByteQueue sendingPacketSBQ(sizeof(sendingPacketHeader), BUFFER_SIZE);

	LanClientPlayer lanClientPlayer;
	INT64 loginConfirmValue;


	
	lanClientPlayer.SessionKey = sessionKey;
	InitializeSRWLock(&lanClientPlayer.LanClientLock);



	AcquireSRWLockExclusive(&LCPM_LOCK);
	LCPM[lanClientPlayer.SessionKey] = lanClientPlayer;
	ReleaseSRWLockExclusive(&LCPM_LOCK);



	loginConfirmValue = 0x7fffffffffffffff;

	sendingPacketSBQ << loginConfirmValue;

	sendingPacketHeader = sizeof(loginConfirmValue);

	sendingPacketSBQ.SetHeader(sendingPacketHeader);

	SendPacket(lanClientPlayer.SessionKey, sendingPacketSBQ);



	//++CurrentClientCount;
	//CurrentClientCount = LCPUM.size();
	CurrentClientCount = LCPM.size();
	
	++ConnectionCount;

	return;
}

void DerivedLanServer::OnClientLeave(unsigned int sessionKey)
{
	AcquireSRWLockExclusive(&LCPM_LOCK);
	LCPM.erase(sessionKey);
	ReleaseSRWLockExclusive(&LCPM_LOCK);

	//--CurrentClientCount;
	//CurrentClientCount = LCPUM.size();
	CurrentClientCount = LCPM.size();

	++DisconnectionCount;

	return;
}

bool DerivedLanServer::OnConnectionRequest(SOCKADDR_IN clientAddress, unsigned int port)
{
	if(0)
	{
		return 0;
	}

	//printf("Connection allowed." CRALF);

	return 1;
}

void DerivedLanServer::OnRecv(unsigned int sessionKey, SerialByteQueue& sendingPacketSBQ)
{
	++ReceiveCount;

	return;
}

void DerivedLanServer::OnSend(unsigned int sessionKey, unsigned int sendedSize)
{
	++SendCount;

	return;
}

void DerivedLanServer::OnWorkerThreadBegin()
{
	return;
}

void DerivedLanServer::OnWorkerThreadEnd()
{
	return;
}

void DerivedLanServer::OnError(unsigned int errorIndex, wchar_t* errorText)
{
	printf("[Error] %d, %s", errorIndex, errorText);

	return;
}
