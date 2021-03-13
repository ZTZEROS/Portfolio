#include "stdafx.h"
#include "AdvancedChat.h"
#include "ChatLAN_Server.h"

AdvancedChatWork ChatLAN_Server::ProcessWork(AdvancedChatWork receivedWork)
{
	AdvancedChatWork sendingWork;
	WORD clientRequestIndex;
	WORD serverResponseIndex;

	sendingWork.SessionKey = receivedWork.SessionKey;
	InterlockedIncrement(&CURRENT_SPQ_COUNT);
	InterlockedIncrement(&ALLOCATED_SPQ_COUNT);
	sendingWork.PacketSPQ_Address = PACKET_SPQ_TLP.Allocate();

	clientRequestIndex = 0xffff;
	serverResponseIndex = 0xffff;

	if(UnpackPacketSPQ(receivedWork.PacketSPQ_Address)) *receivedWork.PacketSPQ_Address >> clientRequestIndex;
	else clientRequestIndex = 0xffff;

	switch (clientRequestIndex)
	{
	case ADVANCED_LOGIN_SERVER_REQUEST_LINK:
		{
			{
				BYTE serverType;
				WCHAR serverName[32];

				*receivedWork.PacketSPQ_Address >> serverType;

				FOR(i, sizeof(serverName) / sizeof(WCHAR))
				{
					*receivedWork.PacketSPQ_Address >> serverName[i];
				}

				serverResponseIndex = ADVANCED_CHAT_SERVER_RESPONSE_LINK;
			}
			++CheckPoint[0];
			{
				*sendingWork.PacketSPQ_Address << serverResponseIndex;
			}

			if (PackPacketSPQ(sendingWork.PacketSPQ_Address)) Unicast(sendingWork);
		}
		break;
	case ADVANCED_LOGIN_SERVER_REQUEST_LOGIN_CONFIRM:
		{
			AdvancedChatClientPlayer* clientPlayerAddress;

			{
				INT64 accountKey;
				CHAR playerKey[ADVANCED_CHAT_PLAYER_KEY_LENGTH];
				UINT64 sessionKey;

				*receivedWork.PacketSPQ_Address >> accountKey;

				FOR(i, sizeof(playerKey) / sizeof(CHAR))
				{
					*receivedWork.PacketSPQ_Address >> playerKey[i];
				}

				*receivedWork.PacketSPQ_Address >> sessionKey;


				
				clientPlayerAddress = ACCP_TLP.Allocate(); //consider faster method

				ZeroMemory(clientPlayerAddress, sizeof(*clientPlayerAddress));

				clientPlayerAddress->AccountKey = accountKey;

				FOR(i, sizeof(clientPlayerAddress->PlayerKey) / sizeof(CHAR))
				{
					clientPlayerAddress->PlayerKey[i] = playerKey[i];
				}

				clientPlayerAddress->SessionKey = sessionKey;

				clientPlayerAddress->PreviousHeartbeatTime = timeGetTime();

				ACCPAL.push_back(clientPlayerAddress);

				CheckPoint[11] = ACCPAL.size();

				serverResponseIndex = ADVANCED_CHAT_SERVER_RESPONSE_LOGIN_CONFIRM;
			}
			++CheckPoint[1];
			{
				INT64 accountKey;
				UINT64 sessionKey;

				accountKey = clientPlayerAddress->AccountKey;
				sessionKey = clientPlayerAddress->SessionKey;

				*sendingWork.PacketSPQ_Address << serverResponseIndex;
				*sendingWork.PacketSPQ_Address << accountKey;
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

unsigned int ChatLAN_Server::UnpackPacketSPQ(SerialPacketQueue<IOCP_PacketHeader>* packetSPQ_Address)
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

unsigned int ChatLAN_Server::PackPacketSPQ(SerialPacketQueue<IOCP_PacketHeader>* packetSPQ_Address)
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

unsigned int ChatLAN_Server::Unicast(AdvancedChatWork sendingWork)
{
	if (SendPacket(sendingWork.SessionKey, sendingWork.PacketSPQ_Address)) PostSend(sendingWork.SessionKey);

	return 0;
}

unsigned int ChatLAN_Server::Singlecast(AdvancedChatWork sendingWork)
{
	if (SendPacket(sendingWork.SessionKey, sendingWork.PacketSPQ_Address)) PostSend(sendingWork.SessionKey);

	return 0;
}