#include "stdafx.h"
#include "AdvancedWAN.h"
#include "BaseWAN_Server.h"
#include "ChatWAN_Server.h"

WORD ChatWAN_Server::ProcessRequest(AdvancedWAN_ClientPlayer* clientPlayerAddress, AdvancedWAN_Work receivedWork)
{
	WORD clientRequestIndex;
	WORD serverResponseIndex;

	if(UnpackPacketSPQ(receivedWork.PacketSPQ_Address))	*receivedWork.PacketSPQ_Address >> clientRequestIndex;
	else clientRequestIndex = 0xffff;

	//request
	switch (clientRequestIndex)
	{
	case ADVANCED_WAN_CHAT_CLIENT_REQUEST_LOGIN:
		{
			INT64 accountKey;
			WCHAR realName[ADVANCED_WAN_CHAT_REAL_NAME_LENGTH];
			WCHAR nickname[ADVANCED_WAN_CHAT_NICKNAME_LENGTH];
			CHAR playerKey[ADVANCED_WAN_CHAT_PLAYER_KEY_LENGTH];

			*receivedWork.PacketSPQ_Address >> accountKey;

			FOR(i, sizeof(realName) / sizeof(WCHAR))
			{
				*receivedWork.PacketSPQ_Address >> realName[i];
			}

			FOR(i, sizeof(nickname) / sizeof(WCHAR))
			{
				*receivedWork.PacketSPQ_Address >> nickname[i];
			}

			FOR(i, sizeof(playerKey) / sizeof(CHAR))
			{
				*receivedWork.PacketSPQ_Address >> playerKey[i];
			}

			//memcpy(receivedWork.PacketSPQ_Address->GetFrontAddress(), player.RealName, sizeof(player.RealName));
			//receivedWork.PacketSPQ_Address->DecreaseCurrentSize(sizeof(player.RealName));

			//memcpy(receivedWork.PacketSPQ_Address->GetFrontAddress(), player.Nickname, sizeof(player.Nickname));
			//receivedWork.PacketSPQ_Address->DecreaseCurrentSize(sizeof(player.Nickname));

			//memcpy(receivedWork.PacketSPQ_Address->GetFrontAddress(), player.PlayerKey, sizeof(player.PlayerKey));
			//receivedWork.PacketSPQ_Address->DecreaseCurrentSize(sizeof(player.PlayerKey));
			
			clientPlayerAddress->AccountKey = accountKey;

			FOR(i, sizeof(clientPlayerAddress->RealName) / sizeof(WCHAR))
			{
				clientPlayerAddress->RealName[i] = realName[i];
			}

			FOR(i, sizeof(clientPlayerAddress->Nickname) / sizeof(WCHAR))
			{
				clientPlayerAddress->Nickname[i] = nickname[i];
			}

			FOR(i, sizeof(clientPlayerAddress->PlayerKey) / sizeof(CHAR))
			{
				clientPlayerAddress->PlayerKey[i] = playerKey[i];
			}

			clientPlayerAddress->SectorIndexX = 0xff;
			clientPlayerAddress->SectorIndexY = 0xff;



			serverResponseIndex = ADVANCED_WAN_CHAT_SERVER_RESPONSE_LOGIN;
		}
		break;
	case ADVANCED_WAN_CHAT_CLIENT_REQUEST_SECTOR_MOVE:
		{
			INT64 accountKey;
			WORD sectorX;
			WORD sectorY;

			if (clientPlayerAddress->SectorIndexX < ADVANCED_WAN_CHAT_SECTOR_WIDTH && clientPlayerAddress->SectorIndexY < ADVANCED_WAN_CHAT_SECTOR_HEIGHT)
			{
				AdvancedWAN_ClientPlayerAddressL* cpalAddress;

				cpalAddress = &AWAN_SECTOR[clientPlayerAddress->SectorIndexY][clientPlayerAddress->SectorIndexX].CPAL;

				for (AdvancedWAN_ClientPlayerAddressLI playerAddressLI = cpalAddress->begin(); playerAddressLI != cpalAddress->end(); ++playerAddressLI)
				{
					if ((*playerAddressLI) == clientPlayerAddress)
					{
						cpalAddress->erase(playerAddressLI);

						break;
					}
				}
			}

			*receivedWork.PacketSPQ_Address >> accountKey;
			*receivedWork.PacketSPQ_Address >> sectorX;
			*receivedWork.PacketSPQ_Address >> sectorY;
			
			//clientPlayerAddress->AccountKey = accountKey;
			clientPlayerAddress->SectorIndexX = sectorX;
			clientPlayerAddress->SectorIndexY = sectorY;



			serverResponseIndex = ADVANCED_WAN_CHAT_SERVER_RESPONSE_SECTOR_MOVE;
		}
		break;
	case ADVANCED_WAN_CHAT_CLIENT_REQUEST_MESSAGE:
		{
			INT64 accountKey;
			WORD messageSize;
			WCHAR message[ADVANCED_WAN_CHAT_MAXIMUM_MESSAGE_LENGTH];

			*receivedWork.PacketSPQ_Address >> accountKey;
			*receivedWork.PacketSPQ_Address >> messageSize;

			if (messageSize == receivedWork.PacketSPQ_Address->GetBodySize())
			{
				FOR(i, messageSize / sizeof(WCHAR))
				{
					*receivedWork.PacketSPQ_Address >> message[i];
				}

				//memcpy(receivedWork.PacketSPQ_Address->GetFrontAddress(), player.Message, sizeof(player.Message));
				//receivedWork.PacketSPQ_Address->DecreaseCurrentSize(sizeof(player.Message));

				clientPlayerAddress->AccountKey = accountKey;
				clientPlayerAddress->MessageSize = messageSize;

				FOR(i, clientPlayerAddress->MessageSize / sizeof(WCHAR))
				{
					clientPlayerAddress->Message[i] = message[i];
				}



				serverResponseIndex = ADVANCED_WAN_CHAT_SERVER_RESPONSE_MESSAGE;
			}
			else serverResponseIndex = 0xffff;
		}
		break;
	case ADVANCED_WAN_CHAT_CLIENT_REQUEST_HEARTBEAT:
		{
			DWORD currentHeartbeatTime;

			currentHeartbeatTime = timeGetTime();

			clientPlayerAddress->PreviousHeartbeatTime = currentHeartbeatTime;
		}
		break;
	case ADVANCED_WAN_CHAT_CLIENT_REQUEST_LOGOUT:
		{
			//slow part
			AcquireSRWLockExclusive(&AWAN_CPAL_LOCK);
			for (AdvancedWAN_ClientPlayerAddressLI playerAddressLI = AWAN_CPAL.begin(); playerAddressLI != AWAN_CPAL.end(); ++playerAddressLI)
			{
				if ((*playerAddressLI) == clientPlayerAddress)
				{
					AWAN_CPAL.erase(playerAddressLI);

					break;
				}
			}
			ReleaseSRWLockExclusive(&AWAN_CPAL_LOCK);



			serverResponseIndex = ADVANCED_WAN_CHAT_SERVER_RESPONSE_LOGOUT;
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
		InterlockedIncrement(&SPQ_DEALLOCATION_COUNT);
		//delete receivedWork.PacketSPQ_Address;
		PACKET_SPQ_TLP.Deallocate(receivedWork.PacketSPQ_Address);
	}

	return serverResponseIndex;
}

/*
WORD ChatWAN_Server::ProcessResponse(AdvancedWAN_ClientPlayer* clientPlayerAddress, AdvancedWAN_Work& sendingWork)
{
	WORD serverResponseIndex;

	sendingWork.SessionKey = clientPlayerAddress->SessionKey;
	sendingWork.PacketSPQ_Address = PACKET_SPQ_TLP.Allocate();

	//response
	switch (serverResponseIndex)
	{
	case ADVANCED_WAN_CHAT_SERVER_RESPONSE_LOGIN:
		{
			BYTE status;
			INT64 accountKey;

			*sendingWork.PacketSPQ_Address << serverResponseIndex;

			*sendingWork.PacketSPQ_Address << (BYTE)0x01;
			*sendingWork.PacketSPQ_Address << clientPlayerAddress->AccountKey; //0xffffffffffffffff;
		}
		break;
	case ADVANCED_WAN_CHAT_SERVER_RESPONSE_SECTOR_MOVE:
		{
			INT64 accountKey;
			WORD sectorX;
			WORD sectorY;

			*sendingWork.PacketSPQ_Address << serverResponseIndex;

			*sendingWork.PacketSPQ_Address << clientPlayerAddress->AccountKey;
			*sendingWork.PacketSPQ_Address << clientPlayerAddress->SectorX;
			*sendingWork.PacketSPQ_Address << clientPlayerAddress->SectorY;

			if (clientPlayerAddress->SectorX < ADVANCED_WAN_SECTOR_WIDTH && clientPlayerAddress->SectorY < ADVANCED_WAN_SECTOR_HEIGHT)
			{
				AWAN_SECTOR[clientPlayerAddress->SectorY][clientPlayerAddress->SectorX].CPAL.push_back(clientPlayerAddress);
			}
		}
		break;
	case ADVANCED_WAN_CHAT_SERVER_RESPONSE_MESSAGE:
		{
			INT64 accountKey;
			WCHAR realName[20];
			WCHAR nickname[20];

			WORD messageLength;
			WCHAR message[ADVANCED_WAN_CHAT_MAXIMUM_MESSAGE_LENGTH];

			*sendingWork.PacketSPQ_Address << serverResponseIndex;

			*sendingWork.PacketSPQ_Address << clientPlayerAddress->AccountKey;

			FOR(i, sizeof(clientPlayerAddress->RealName) / sizeof(WCHAR))
			{
				*sendingWork.PacketSPQ_Address << clientPlayerAddress->RealName[i];
			}

			//memcpy(sendingWork.PacketSPQ_Address->GetFrontAddress(), clientPlayerAddress->RealName, sizeof(clientPlayerAddress->RealName));
			//sendingWork.PacketSPQ_Address->IncreaseCurrentSize(sizeof(clientPlayerAddress->RealName));

			FOR(i, sizeof(clientPlayerAddress->Nickname) / sizeof(WCHAR))
			{
				*sendingWork.PacketSPQ_Address << clientPlayerAddress->Nickname[i];
			}

			//memcpy(sendingWork.PacketSPQ_Address->GetFrontAddress(), player.Nickname, sizeof(player.Nickname));
			//sendingWork.PacketSPQ_Address->IncreaseCurrentSize(sizeof(player.Nickname));

			*sendingWork.PacketSPQ_Address << clientPlayerAddress->MessageSize;

			FOR(i, clientPlayerAddress->MessageSize / sizeof(WCHAR))
			{
				*sendingWork.PacketSPQ_Address << clientPlayerAddress->Message[i];
			}

			//memcpy(sendingWork.PacketSPQ_Address->GetFrontAddress(),  player.Message, sizeof(player.Message));
			//sendingWork.PacketSPQ_Address->IncreaseCurrentSize(sizeof(player.Message));
		}
		break;
	default:
		break;
	}

	PackSendingPacketSPQ(sendingWork.PacketSPQ_Address);

	return 0;
}
*/

/*
AdvancedWAN_Work ChatWAN_Server::ProcessRequest(AdvancedWAN_ClientPlayer* clientPlayerAddress, WORD clientRequestIndex)
{
AdvancedWAN_Work receivedWork;

return receivedWork;
}
*/

AdvancedWAN_Work ChatWAN_Server::ProcessResponse(AdvancedWAN_ClientPlayer* clientPlayerAddress, WORD serverResponseIndex)
{
	AdvancedWAN_Work sendingWork;

	sendingWork.SessionKey = clientPlayerAddress->SessionKey;
	InterlockedIncrement(&SPQ_ALLOCATION_COUNT);
	//sendingWork.PacketSPQ_Address = new SerialPacketQueue<AdvancedWAN_PacketHeader>;
	sendingWork.PacketSPQ_Address = PACKET_SPQ_TLP.Allocate();

	//response
	switch (serverResponseIndex)
	{
	case ADVANCED_WAN_CHAT_SERVER_RESPONSE_LOGIN:
		{
			WORD type;
			BYTE status;
			INT64 accountKey;

			type = serverResponseIndex;
			status = 0x01;
			accountKey = clientPlayerAddress->AccountKey;

			*sendingWork.PacketSPQ_Address << type;
			*sendingWork.PacketSPQ_Address << status;
			*sendingWork.PacketSPQ_Address << accountKey;
		}
		break;
	case ADVANCED_WAN_CHAT_SERVER_RESPONSE_SECTOR_MOVE:
		{
			WORD type;
			INT64 accountKey;
			WORD sectorX;
			WORD sectorY;

			type = serverResponseIndex;
			accountKey = clientPlayerAddress->AccountKey;
			sectorX = clientPlayerAddress->SectorIndexX;
			sectorY = clientPlayerAddress->SectorIndexY;

			*sendingWork.PacketSPQ_Address << type;
			*sendingWork.PacketSPQ_Address << accountKey;
			*sendingWork.PacketSPQ_Address << sectorX;
			*sendingWork.PacketSPQ_Address << sectorY;

			if (clientPlayerAddress->SectorIndexX < ADVANCED_WAN_CHAT_SECTOR_WIDTH && clientPlayerAddress->SectorIndexY < ADVANCED_WAN_CHAT_SECTOR_HEIGHT)
			{
				AdvancedWAN_ClientPlayerAddressL* cpalAddress;

				cpalAddress = &AWAN_SECTOR[clientPlayerAddress->SectorIndexY][clientPlayerAddress->SectorIndexX].CPAL;

				cpalAddress->push_back(clientPlayerAddress);
			}
		}
		break;
	case ADVANCED_WAN_CHAT_SERVER_RESPONSE_MESSAGE:
		{
			WORD type;
			INT64 accountKey;
			WCHAR realName[ADVANCED_WAN_CHAT_REAL_NAME_LENGTH];
			WCHAR nickname[ADVANCED_WAN_CHAT_NICKNAME_LENGTH];
			WORD messageSize;
			WCHAR message[ADVANCED_WAN_CHAT_MAXIMUM_MESSAGE_LENGTH];

			type = serverResponseIndex;
			accountKey = clientPlayerAddress->AccountKey;

			FOR(i, sizeof(realName) / sizeof(WCHAR))
			{
				realName[i] = clientPlayerAddress->RealName[i];
			}

			FOR(i, sizeof(nickname) / sizeof(WCHAR))
			{
				nickname[i] = clientPlayerAddress->Nickname[i];
			}

			messageSize = clientPlayerAddress->MessageSize;

			FOR(i, messageSize / sizeof(WCHAR))
			{
				message[i] = clientPlayerAddress->Message[i];
			}

			//memcpy(sendingWork.PacketSPQ_Address->GetFrontAddress(), player.RealName, sizeof(player.RealName));
			//sendingWork.PacketSPQ_Address->DecreaseCurrentSize(sizeof(player.RealName));

			//memcpy(sendingWork.PacketSPQ_Address->GetFrontAddress(), player.Nickname, sizeof(player.Nickname));
			//sendingWork.PacketSPQ_Address->DecreaseCurrentSize(sizeof(player.Nickname));

			//memcpy(sendingWork.PacketSPQ_Address->GetFrontAddress(),  player.Message, sizeof(player.Message));
			//sendingWork.PacketSPQ_Address->DecreaseCurrentSize(sizeof(player.Message));

			*sendingWork.PacketSPQ_Address << type;
			*sendingWork.PacketSPQ_Address << accountKey;

			FOR(i, sizeof(realName) / sizeof(WCHAR))
			{
				*sendingWork.PacketSPQ_Address << realName[i];
			}

			FOR(i, sizeof(nickname) / sizeof(WCHAR))
			{
				*sendingWork.PacketSPQ_Address << nickname[i];
			}

			*sendingWork.PacketSPQ_Address << messageSize;

			FOR(i, messageSize / sizeof(WCHAR))
			{
				*sendingWork.PacketSPQ_Address << message[i];
			}
		}
		break;
	case ADVANCED_WAN_CHAT_SERVER_RESPONSE_HEARTBEAT:
		{

		}
		break;
	case ADVANCED_WAN_CHAT_SERVER_RESPONSE_LOGOUT:
		{
			if (clientPlayerAddress->SectorIndexX < ADVANCED_WAN_CHAT_SECTOR_WIDTH && clientPlayerAddress->SectorIndexY < ADVANCED_WAN_CHAT_SECTOR_HEIGHT)
			{
				AdvancedWAN_ClientPlayerAddressL* cpalAddress;

				cpalAddress = &AWAN_SECTOR[clientPlayerAddress->SectorIndexY][clientPlayerAddress->SectorIndexX].CPAL;

				for (AdvancedWAN_ClientPlayerAddressLI playerAddressLI = cpalAddress->begin(); playerAddressLI != cpalAddress->end(); ++playerAddressLI)
				{
					if ((*playerAddressLI) == clientPlayerAddress)
					{
						cpalAddress->erase(playerAddressLI);

						break;
					}
				}
			}

			delete clientPlayerAddress;
		}
		break;
	default:
		{

		}
		break;
	}

	PackPacketSPQ(sendingWork.PacketSPQ_Address);

	return sendingWork;
}

unsigned int ChatWAN_Server::UnpackPacketSPQ(SerialPacketQueue<AdvancedWAN_PacketHeader>* receivedPacketSPQ_Address) //UnpackReceivedPacket //UnpackReceivedPacketSPQ
{
	AdvancedWAN_PacketHeader header;
	CHAR* frontAddress;
	BYTE checksum;

	header = receivedPacketSPQ_Address->GetHeader();
	frontAddress = receivedPacketSPQ_Address->GetFrontAddress();
	checksum = 0;

	if (header.IdentificationCode == 0x77)
	{
		if (header.BodySize == receivedPacketSPQ_Address->GetCurrentSize())
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

			if (header.Checksum == checksum)
			{
				return header.BodySize;
			}
		}
	}

	return 0;
}

unsigned int ChatWAN_Server::PackPacketSPQ(SerialPacketQueue<AdvancedWAN_PacketHeader>* sendingPacketSPQ_Address) //PackSendingPacket //PackSendingPacketSPQ
{
	AdvancedWAN_PacketHeader header;
	CHAR* frontAddress;
	BYTE checksum;

	header.IdentificationCode = 0x77;
	header.BodySize = sendingPacketSPQ_Address->GetCurrentSize();
	header.CryptoCode = 0x00;
	header.Checksum = 0;

	frontAddress = sendingPacketSPQ_Address->GetFrontAddress();

	for (unsigned int i = 0; header.BodySize; ++i)
	{
		header.Checksum += frontAddress[i];
	}

	header.Checksum = header.Checksum ^ header.CryptoCode;

	for (unsigned int i = 0; i < header.BodySize; ++i)
	{
		frontAddress[i] = frontAddress[i] ^ header.CryptoCode;
	}

	sendingPacketSPQ_Address->SetHeader(header);

	return header.BodySize;
}

unsigned int ChatWAN_Server::Broadcast(AdvancedWAN_Work sendingWork, UINT sectorIndexX, UINT sectorIndexY)
{
	AdvancedWAN_ClientPlayerAddressL* cpalAddress;

	//LeftTop
	if (sectorIndexX - 1 < ADVANCED_WAN_CHAT_SECTOR_WIDTH && sectorIndexY - 1 < ADVANCED_WAN_CHAT_SECTOR_HEIGHT)
	{
		//Multicast(sendingWork, sectorX - 1, sectorY - 1);

		cpalAddress = &AWAN_SECTOR[sectorIndexY - 1][sectorIndexX - 1].CPAL;

		for (AdvancedWAN_ClientPlayerAddressLI playerAddressLI = cpalAddress->begin(); playerAddressLI != cpalAddress->end(); ++playerAddressLI)
		{
			if(SendPacket((*playerAddressLI)->SessionKey, sendingWork.PacketSPQ_Address)) PostSend((*playerAddressLI)->SessionKey);
			
			//SendPacket((*playerAddressLI)->SessionKey, sendingWork.PacketSPQ_Address);
			//
			//PostSend((*playerAddressLI)->SessionKey);
		}
	}

	//Top
	if (sectorIndexX < ADVANCED_WAN_CHAT_SECTOR_WIDTH && sectorIndexY - 1 < ADVANCED_WAN_CHAT_SECTOR_HEIGHT)
	{
		//Multicast(sendingWork, sectorX, sectorY - 1);

		cpalAddress = &AWAN_SECTOR[sectorIndexY - 1][sectorIndexX].CPAL;

		for (AdvancedWAN_ClientPlayerAddressLI playerAddressLI = cpalAddress->begin(); playerAddressLI != cpalAddress->end(); ++playerAddressLI)
		{
			if (SendPacket((*playerAddressLI)->SessionKey, sendingWork.PacketSPQ_Address)) PostSend((*playerAddressLI)->SessionKey);

			//SendPacket((*playerAddressLI)->SessionKey, sendingWork.PacketSPQ_Address);
			//
			//PostSend((*playerAddressLI)->SessionKey);
		}
	}

	//RightTop
	if (sectorIndexX + 1 < ADVANCED_WAN_CHAT_SECTOR_WIDTH && sectorIndexY - 1 < ADVANCED_WAN_CHAT_SECTOR_HEIGHT)
	{
		//Multicast(sendingWork, sectorX + 1, sectorY - 1);

		cpalAddress = &AWAN_SECTOR[sectorIndexY - 1][sectorIndexX + 1].CPAL;

		for (AdvancedWAN_ClientPlayerAddressLI playerAddressLI = cpalAddress->begin(); playerAddressLI != cpalAddress->end(); ++playerAddressLI)
		{
			if (SendPacket((*playerAddressLI)->SessionKey, sendingWork.PacketSPQ_Address)) PostSend((*playerAddressLI)->SessionKey);

			//SendPacket((*playerAddressLI)->SessionKey, sendingWork.PacketSPQ_Address);
			//
			//PostSend((*playerAddressLI)->SessionKey);
		}
	}

	//Left
	if (sectorIndexX - 1 < ADVANCED_WAN_CHAT_SECTOR_WIDTH && sectorIndexY < ADVANCED_WAN_CHAT_SECTOR_HEIGHT)
	{
		//Multicast(sendingWork, sectorX - 1, sectorY);

		cpalAddress = &AWAN_SECTOR[sectorIndexY][sectorIndexX - 1].CPAL;

		for (AdvancedWAN_ClientPlayerAddressLI playerAddressLI = cpalAddress->begin(); playerAddressLI != cpalAddress->end(); ++playerAddressLI)
		{
			if (SendPacket((*playerAddressLI)->SessionKey, sendingWork.PacketSPQ_Address)) PostSend((*playerAddressLI)->SessionKey);

			//SendPacket((*playerAddressLI)->SessionKey, sendingWork.PacketSPQ_Address);
			//
			//PostSend((*playerAddressLI)->SessionKey);
		}
	}

	//Center
	if (sectorIndexX < ADVANCED_WAN_CHAT_SECTOR_WIDTH && sectorIndexY < ADVANCED_WAN_CHAT_SECTOR_HEIGHT) //(1)
	{
		//Multicast(sendingWork, sectorX, sectorY);

		cpalAddress = &AWAN_SECTOR[sectorIndexY][sectorIndexX].CPAL;

		for (AdvancedWAN_ClientPlayerAddressLI playerAddressLI = cpalAddress->begin(); playerAddressLI != cpalAddress->end(); ++playerAddressLI)
		{
			if (SendPacket((*playerAddressLI)->SessionKey, sendingWork.PacketSPQ_Address)) PostSend((*playerAddressLI)->SessionKey);

			//SendPacket((*playerAddressLI)->SessionKey, sendingWork.PacketSPQ_Address);
			//
			//PostSend((*playerAddressLI)->SessionKey);
		}
	}

	//Right
	if (sectorIndexX + 1 < ADVANCED_WAN_CHAT_SECTOR_WIDTH && sectorIndexY < ADVANCED_WAN_CHAT_SECTOR_HEIGHT)
	{
		//Multicast(sendingWork, sectorX + 1, sectorY);

		cpalAddress = &AWAN_SECTOR[sectorIndexY][sectorIndexX + 1].CPAL;

		for (AdvancedWAN_ClientPlayerAddressLI playerAddressLI = cpalAddress->begin(); playerAddressLI != cpalAddress->end(); ++playerAddressLI)
		{
			if (SendPacket((*playerAddressLI)->SessionKey, sendingWork.PacketSPQ_Address)) PostSend((*playerAddressLI)->SessionKey);

			//SendPacket((*playerAddressLI)->SessionKey, sendingWork.PacketSPQ_Address);
			//
			//PostSend((*playerAddressLI)->SessionKey);
		}
	}

	//LeftBottom
	if (sectorIndexX - 1 < ADVANCED_WAN_CHAT_SECTOR_WIDTH && sectorIndexY + 1 < ADVANCED_WAN_CHAT_SECTOR_HEIGHT)
	{
		//Multicast(sendingWork, sectorX - 1, sectorY + 1);

		cpalAddress = &AWAN_SECTOR[sectorIndexY + 1][sectorIndexX - 1].CPAL;

		for (AdvancedWAN_ClientPlayerAddressLI playerAddressLI = cpalAddress->begin(); playerAddressLI != cpalAddress->end(); ++playerAddressLI)
		{
			if (SendPacket((*playerAddressLI)->SessionKey, sendingWork.PacketSPQ_Address)) PostSend((*playerAddressLI)->SessionKey);

			//SendPacket((*playerAddressLI)->SessionKey, sendingWork.PacketSPQ_Address);
			//
			//PostSend((*playerAddressLI)->SessionKey);
		}
	}

	//Bottom
	if (sectorIndexX < ADVANCED_WAN_CHAT_SECTOR_WIDTH && sectorIndexY + 1 < ADVANCED_WAN_CHAT_SECTOR_HEIGHT)
	{
		//Multicast(sendingWork, sectorX, sectorY + 1);

		cpalAddress = &AWAN_SECTOR[sectorIndexY + 1][sectorIndexX].CPAL;

		for (AdvancedWAN_ClientPlayerAddressLI playerAddressLI = cpalAddress->begin(); playerAddressLI != cpalAddress->end(); ++playerAddressLI)
		{
			if (SendPacket((*playerAddressLI)->SessionKey, sendingWork.PacketSPQ_Address)) PostSend((*playerAddressLI)->SessionKey);

			//SendPacket((*playerAddressLI)->SessionKey, sendingWork.PacketSPQ_Address);
			//
			//PostSend((*playerAddressLI)->SessionKey);
		}
	}

	//RightBottom
	if (sectorIndexX + 1 < ADVANCED_WAN_CHAT_SECTOR_WIDTH && sectorIndexY + 1 < ADVANCED_WAN_CHAT_SECTOR_HEIGHT)
	{
		//Multicast(sendingWork, sectorX + 1, sectorY + 1);

		cpalAddress = &AWAN_SECTOR[sectorIndexY + 1][sectorIndexX + 1].CPAL;

		for (AdvancedWAN_ClientPlayerAddressLI playerAddressLI = cpalAddress->begin(); playerAddressLI != cpalAddress->end(); ++playerAddressLI)
		{
			if (SendPacket((*playerAddressLI)->SessionKey, sendingWork.PacketSPQ_Address)) PostSend((*playerAddressLI)->SessionKey);

			//SendPacket((*playerAddressLI)->SessionKey, sendingWork.PacketSPQ_Address);
			//
			//PostSend((*playerAddressLI)->SessionKey);
		}
	}

	if (!sendingWork.PacketSPQ_Address->DecreaseReferenceCount())
	{
		InterlockedIncrement(&SPQ_DEALLOCATION_COUNT);
		PACKET_SPQ_TLP.Deallocate(sendingWork.PacketSPQ_Address);
	}

	return 0;
}

unsigned int ChatWAN_Server::Multicast(AdvancedWAN_Work sendingWork, UINT sectorIndexX, UINT sectorIndexY)
{
	AdvancedWAN_ClientPlayerAddressL* cpalAddress;

	cpalAddress = &AWAN_SECTOR[sectorIndexY][sectorIndexX].CPAL;

	for (AdvancedWAN_ClientPlayerAddressLI playerAddressLI = cpalAddress->begin(); playerAddressLI != cpalAddress->end(); ++playerAddressLI)
	{
		if (SendPacket((*playerAddressLI)->SessionKey, sendingWork.PacketSPQ_Address)) PostSend((*playerAddressLI)->SessionKey);

		//SendPacket((*playerAddressLI)->SessionKey, sendingWork.PacketSPQ_Address);
		//
		//PostSend((*playerAddressLI)->SessionKey);
	}

	if (!sendingWork.PacketSPQ_Address->DecreaseReferenceCount())
	{
		InterlockedIncrement(&SPQ_DEALLOCATION_COUNT);
		//delete sendingWork.PacketSPQ_Address;
		PACKET_SPQ_TLP.Deallocate(sendingWork.PacketSPQ_Address);
	}

	return 0;
}

unsigned int ChatWAN_Server::Unicast(AdvancedWAN_Work sendingWork)
{
	if (SendPacket(sendingWork.SessionKey, sendingWork.PacketSPQ_Address)) PostSend(sendingWork.SessionKey);

	//SendPacket(sendingWork.SessionKey, sendingWork.PacketSPQ_Address);
	//
	//PostSend(sendingWork.SessionKey);

	if (!sendingWork.PacketSPQ_Address->DecreaseReferenceCount())
	{
		InterlockedIncrement(&SPQ_DEALLOCATION_COUNT);
		//delete sendingWork.PacketSPQ_Address;
		PACKET_SPQ_TLP.Deallocate(sendingWork.PacketSPQ_Address);
	}

	return 0;
}

unsigned int ChatWAN_Server::Singlecast(AdvancedWAN_Work sendingWork)
{
	if (SendPacket(sendingWork.SessionKey, sendingWork.PacketSPQ_Address)) PostSend(sendingWork.SessionKey);

	//SendPacket(sendingWork.SessionKey, sendingWork.PacketSPQ_Address);
	//
	//PostSend(sendingWork.SessionKey);

	if (!sendingWork.PacketSPQ_Address->DecreaseReferenceCount())
	{
		InterlockedIncrement(&SPQ_DEALLOCATION_COUNT);
		//delete sendingWork.PacketSPQ_Address;
		PACKET_SPQ_TLP.Deallocate(sendingWork.PacketSPQ_Address);
	}

	return 0;
}