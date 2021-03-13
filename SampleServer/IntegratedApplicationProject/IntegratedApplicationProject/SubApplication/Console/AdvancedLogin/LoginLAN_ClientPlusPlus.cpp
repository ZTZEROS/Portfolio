#include "stdafx.h"
#include "AdvancedLogin.h"
#include "LoginLAN_Client.h"

AdvancedLoginWork LoginLAN_Client::ProcessWork(AdvancedLoginClientPlayer* clientPlayerAddress, AdvancedLoginWork receivedWork)
{
	AdvancedLoginWork sendingWork;
	WORD clientRequestIndex;
	WORD serverResponseIndex;

	sendingWork.SessionKey = clientPlayerAddress->SessionKey;
	InterlockedIncrement(&CURRENT_SPQ_COUNT);
	InterlockedIncrement(&ALLOCATED_SPQ_COUNT);
	sendingWork.PacketSPQ_Address = PACKET_SPQ_TLP.Allocate();

	clientRequestIndex = 0xffff;
	serverResponseIndex = 0xffff;

	if (UnpackPacketSPQ(receivedWork.PacketSPQ_Address)) *receivedWork.PacketSPQ_Address >> clientRequestIndex;
	else clientRequestIndex = 0xffff;

	switch (clientRequestIndex)
	{
	case ADVANCED_LOGIN_CLIENT_REQUEST_LOGIN:
		{
			{
				INT64 accountKey;
				CHAR playerKey[ADVANCED_LOGIN_PLAYER_KEY_LENGTH];

				*receivedWork.PacketSPQ_Address >> accountKey;

				FOR(i, sizeof(playerKey) / sizeof(CHAR))
				{
					*receivedWork.PacketSPQ_Address >> playerKey[i];
				}



				clientPlayerAddress->AccountKey = accountKey;

				FOR(i, sizeof(clientPlayerAddress->PlayerKey) / sizeof(CHAR))
				{
					clientPlayerAddress->PlayerKey[i] = playerKey[i];
				}

				clientPlayerAddress->Status = ADVANCED_LOGIN_SERVER_LOGIN_STATUS_SUCCESS;

				serverResponseIndex = ADVANCED_LOGIN_SERVER_REQUEST_LOGIN_CONFIRM;
			}

			{
				//load player information from database
			}
			++CheckPoint[0];
			{
				INT64 accountKey;
				CHAR playerKey[ADVANCED_LOGIN_PLAYER_KEY_LENGTH];
				INT64 sessionKey;

				accountKey = clientPlayerAddress->AccountKey;

				FOR(i, sizeof(playerKey) / sizeof(CHAR))
				{
					playerKey[i] = clientPlayerAddress->PlayerKey[i];
				}

				sessionKey = clientPlayerAddress->SessionKey;


				*sendingWork.PacketSPQ_Address << serverResponseIndex;
				*sendingWork.PacketSPQ_Address << accountKey;

				FOR(i, sizeof(playerKey) / sizeof(CHAR))
				{
					*sendingWork.PacketSPQ_Address << playerKey[i];
				}

				*sendingWork.PacketSPQ_Address << sessionKey;
			}

			if (PackPacketSPQ(sendingWork.PacketSPQ_Address)) Unicast(sendingWork);
		}
		break;
	default:
		{
			serverResponseIndex = 0xffff;
		}
		break;
	}

	if (!receivedWork.PacketSPQ_Address->DecreaseReferenceCount())
	{
		InterlockedDecrement(&CURRENT_SPQ_COUNT);
		InterlockedIncrement(&DEALLOCATED_SPQ_COUNT);
		PACKET_SPQ_TLP.Deallocate(receivedWork.PacketSPQ_Address);
	}

	if (!sendingWork.PacketSPQ_Address->DecreaseReferenceCount())
	{
		InterlockedDecrement(&CURRENT_SPQ_COUNT);
		InterlockedIncrement(&DEALLOCATED_SPQ_COUNT);
		PACKET_SPQ_TLP.Deallocate(sendingWork.PacketSPQ_Address);
	}

	return sendingWork;
}

unsigned int LoginLAN_Client::UnpackPacketSPQ(SerialPacketQueue<IOCP_PacketHeader>* packetSPQ_Address)
{
	IOCP_PacketHeader header;
	CHAR* frontAddress;
	BYTE checksum;

	UINT result;

	header = packetSPQ_Address->GetHeader();
	frontAddress = packetSPQ_Address->GetFrontAddress();
	checksum = 0;

	if (header.IdentificationCode == 0x77)
	{
		if (header.BodySize == packetSPQ_Address->GetCurrentSize())
		{
			header.Checksum = header.Checksum ^ header.CryptoCode;

			for (unsigned int i = 0; i < header.BodySize; ++i)
			{
				frontAddress[i] = frontAddress[i] ^ header.CryptoCode;
			}

			for (unsigned int i = 0; i < header.BodySize; ++i)
			{
				checksum += frontAddress[i];
			}

			if (header.Checksum == checksum) result = header.BodySize;
			else result = 0;
		}
		else result = 0;
	}
	else result = 0;

	return result;
}

unsigned int LoginLAN_Client::PackPacketSPQ(SerialPacketQueue<IOCP_PacketHeader>* packetSPQ_Address)
{
	IOCP_PacketHeader header;
	CHAR* frontAddress;
	BYTE checksum;

	UINT result;

	header.IdentificationCode = 0x77;
	header.BodySize = packetSPQ_Address->GetBodySize();
	header.CryptoCode = 0x27;
	header.Checksum = 0;

	frontAddress = packetSPQ_Address->GetFrontAddress();

	for (unsigned int i = 0; i < header.BodySize; ++i)
	{
		header.Checksum += frontAddress[i];
	}

	header.Checksum = header.Checksum ^ header.CryptoCode;

	for (unsigned int i = 0; i < header.BodySize; ++i)
	{
		frontAddress[i] = frontAddress[i] ^ header.CryptoCode;
	}

	packetSPQ_Address->SetHeader(header);

	result = packetSPQ_Address->GetTotalSize(); //sizeof(header) + header.BodySize;

	return result;
}

unsigned int LoginLAN_Client::Unicast(AdvancedLoginWork sendingWork)
{
	if (SendPacket(ALCS.SessionKey, sendingWork.PacketSPQ_Address)) PostSend(ALCS.SessionKey);

	return 0;
}

unsigned int LoginLAN_Client::Singlecast(AdvancedLoginWork sendingWork)
{
	if (SendPacket(ALCS.SessionKey, sendingWork.PacketSPQ_Address)) PostSend(ALCS.SessionKey);

	return 0;
}