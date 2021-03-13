#include "stdafx.h"
#include "AdvancedChat.h"
#include "ChatWAN_Server.h"

AdvancedChatWork ChatWAN_Server::ProcessRequest(AdvancedChatClientPlayer* clientPlayerAddress, AdvancedChatWork receivedWork)
{
	AdvancedChatWork work;

	return work;
}

AdvancedChatWork ChatWAN_Server::ProcessResponse(AdvancedChatClientPlayer* clientPlayerAddress, AdvancedChatWork sendingWork)
{
	AdvancedChatWork work;

	return work;
}

AdvancedChatWork ChatWAN_Server::ProcessWork(AdvancedChatClientPlayer* clientPlayerAddress, AdvancedChatWork receivedWork)
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

	if (UnpackPacketSPQ(receivedWork.PacketSPQ_Address)) *receivedWork.PacketSPQ_Address >> clientRequestIndex;
	else clientRequestIndex = 0xffff;

	switch (clientRequestIndex)
	{
	case ADVANCED_CHAT_CLIENT_REQUEST_LOGIN:
		{
			{
				INT64 accountKey;
				WCHAR realName[ADVANCED_CHAT_REAL_NAME_LENGTH];
				WCHAR nickname[ADVANCED_CHAT_NICKNAME_LENGTH];
				CHAR playerKey[ADVANCED_CHAT_PLAYER_KEY_LENGTH];

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

				//check playerkey
				for (AdvancedChatClientPlayerAddressLI cpaLI = LAN_Server.ACCPAL.begin(); cpaLI != LAN_Server.ACCPAL.end(); ++cpaLI)
				{
					if ((*cpaLI)->AccountKey == accountKey)
					{
						FOR(i, sizeof((*cpaLI)->PlayerKey) / sizeof(CHAR))
						{
							if ((*cpaLI)->PlayerKey[i] == playerKey[i]);
							else
							{
								Disconnect((*cpaLI)->SessionKey);

								//PostDisconnect((*cpaLI)->SessionKey);

								break;
							}
						}

						LAN_Server.ACCPAL.erase(cpaLI);

						break;
					}
				}

				CheckPoint[11] = LAN_Server.ACCPAL.size();
				//dangerous part

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

				clientPlayerAddress->SectorIndexX = 0xffffffff;
				clientPlayerAddress->SectorIndexY = 0xffffffff;

				serverResponseIndex = ADVANCED_CHAT_SERVER_RESPONSE_LOGIN;
			}
			++CheckPoint[0];
			{
				BYTE status;
				INT64 accountKey;

				status = ADVANCED_CHAT_SERVER_LOGIN_STATUS_SUCCESS;
				accountKey = clientPlayerAddress->AccountKey;

				*sendingWork.PacketSPQ_Address << serverResponseIndex;
				*sendingWork.PacketSPQ_Address << status;
				*sendingWork.PacketSPQ_Address << accountKey;
			}

			if(PackPacketSPQ(sendingWork.PacketSPQ_Address)) Unicast(sendingWork);
		}
		break;
	case ADVANCED_CHAT_CLIENT_REQUEST_SECTOR_MOVE:
		{
			{
				INT64 accountKey;
				WORD sectorIndexX;
				WORD sectorIndexY;
				
				if (clientPlayerAddress->SectorIndexX < ADVANCED_CHAT_SECTOR_WIDTH && clientPlayerAddress->SectorIndexY < ADVANCED_CHAT_SECTOR_HEIGHT)
				{
					AdvancedChatClientPlayerAddressL* cpaL_Address;

					cpaL_Address = &AC_SECTOR_A[clientPlayerAddress->SectorIndexY][clientPlayerAddress->SectorIndexX].CPAL;

					for (AdvancedChatClientPlayerAddressLI cpaLI = cpaL_Address->begin(); cpaLI != cpaL_Address->end(); ++cpaLI)
					{
						if ((*cpaLI) == clientPlayerAddress)
						{
							cpaL_Address->erase(cpaLI);

							break;
						}
					}
				}

				*receivedWork.PacketSPQ_Address >> accountKey;
				*receivedWork.PacketSPQ_Address >> sectorIndexX;
				*receivedWork.PacketSPQ_Address >> sectorIndexY;

				//clientPlayerAddress->AccountKey = accountKey;
				clientPlayerAddress->SectorIndexX = sectorIndexX;
				clientPlayerAddress->SectorIndexY = sectorIndexY;

				serverResponseIndex = ADVANCED_CHAT_SERVER_RESPONSE_SECTOR_MOVE;
			}
			++CheckPoint[1];
			{
				INT64 accountKey;
				WORD sectorIndexX;
				WORD sectorIndexY;

				accountKey = clientPlayerAddress->AccountKey;
				sectorIndexX = clientPlayerAddress->SectorIndexX;
				sectorIndexY = clientPlayerAddress->SectorIndexY;

				*sendingWork.PacketSPQ_Address << serverResponseIndex;
				*sendingWork.PacketSPQ_Address << accountKey;
				*sendingWork.PacketSPQ_Address << sectorIndexX;
				*sendingWork.PacketSPQ_Address << sectorIndexY;

				if (clientPlayerAddress->SectorIndexX < ADVANCED_CHAT_SECTOR_WIDTH && clientPlayerAddress->SectorIndexY < ADVANCED_CHAT_SECTOR_HEIGHT)
				{
					AdvancedChatClientPlayerAddressL* cpaL_Address;

					cpaL_Address = &AC_SECTOR_A[clientPlayerAddress->SectorIndexY][clientPlayerAddress->SectorIndexX].CPAL;

					cpaL_Address->push_back(clientPlayerAddress);
				}
			}

			if (PackPacketSPQ(sendingWork.PacketSPQ_Address)) Unicast(sendingWork);
		}
		break;
	case ADVANCED_CHAT_CLIENT_REQUEST_MESSAGE:
		{
			{
				INT64 accountKey;
				WORD messageSize;
				WCHAR message[ADVANCED_CHAT_MAXIMUM_MESSAGE_LENGTH];

				*receivedWork.PacketSPQ_Address >> accountKey;
				*receivedWork.PacketSPQ_Address >> messageSize;

				if (messageSize == receivedWork.PacketSPQ_Address->GetBodySize())
				{
					FOR(i, messageSize / sizeof(WCHAR))
					{
						*receivedWork.PacketSPQ_Address >> message[i];
					}
					


					clientPlayerAddress->AccountKey = accountKey;
					clientPlayerAddress->MessageSize = messageSize;

					FOR(i, clientPlayerAddress->MessageSize / sizeof(WCHAR))
					{
						clientPlayerAddress->Message[i] = message[i];
					}

					serverResponseIndex = ADVANCED_CHAT_SERVER_RESPONSE_MESSAGE;
				}
				else serverResponseIndex = 0xffff;
			}
			++CheckPoint[2];
			{
				INT64 accountKey;
				WCHAR realName[ADVANCED_CHAT_REAL_NAME_LENGTH];
				WCHAR nickname[ADVANCED_CHAT_NICKNAME_LENGTH];
				WORD messageSize;
				WCHAR message[ADVANCED_CHAT_MAXIMUM_MESSAGE_LENGTH];

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

				*sendingWork.PacketSPQ_Address << serverResponseIndex;
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

			if (PackPacketSPQ(sendingWork.PacketSPQ_Address)) Unicast(sendingWork); //Broadcast(sendingWork, clientPlayerAddress->SectorIndexX, clientPlayerAddress->SectorIndexY);
		}
		break;
	case ADVANCED_CHAT_CLIENT_REQUEST_HEARTBEAT:
		{
			{
				serverResponseIndex = ADVANCED_CHAT_SERVER_RESPONSE_HEARTBEAT;
			}

			{

			}
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

unsigned int ChatWAN_Server::UnpackPacketSPQ(SerialPacketQueue<IOCP_PacketHeader>* packetSPQ_Address)
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

unsigned int ChatWAN_Server::PackPacketSPQ(SerialPacketQueue<IOCP_PacketHeader>* packetSPQ_Address)
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

unsigned int ChatWAN_Server::Broadcast(AdvancedChatWork sendingWork, UINT sectorIndexX, UINT sectorIndexY)
{
	AdvancedChatClientPlayerAddressL* cpaL_Address;

	//LeftTop
	if (sectorIndexX - 1 < ADVANCED_CHAT_SECTOR_WIDTH && sectorIndexY - 1 < ADVANCED_CHAT_SECTOR_HEIGHT)
	{
		//Multicast(sendingWork, sectorX - 1, sectorY - 1);

		cpaL_Address = &AC_SECTOR_A[sectorIndexY - 1][sectorIndexX - 1].CPAL;

		for (AdvancedChatClientPlayerAddressLI cpaLI = cpaL_Address->begin(); cpaLI != cpaL_Address->end(); ++cpaLI)
		{
			if (SendPacket((*cpaLI)->SessionKey, sendingWork.PacketSPQ_Address)) PostSend((*cpaLI)->SessionKey);
		}
	}

	//Top
	if (sectorIndexX < ADVANCED_CHAT_SECTOR_WIDTH && sectorIndexY - 1 < ADVANCED_CHAT_SECTOR_HEIGHT)
	{
		//Multicast(sendingWork, sectorX, sectorY - 1);

		cpaL_Address = &AC_SECTOR_A[sectorIndexY - 1][sectorIndexX].CPAL;

		for (AdvancedChatClientPlayerAddressLI cpaLI = cpaL_Address->begin(); cpaLI != cpaL_Address->end(); ++cpaLI)
		{
			if (SendPacket((*cpaLI)->SessionKey, sendingWork.PacketSPQ_Address)) PostSend((*cpaLI)->SessionKey);
		}
	}

	//RightTop
	if (sectorIndexX + 1 < ADVANCED_CHAT_SECTOR_WIDTH && sectorIndexY - 1 < ADVANCED_CHAT_SECTOR_HEIGHT)
	{
		//Multicast(sendingWork, sectorX + 1, sectorY - 1);

		cpaL_Address = &AC_SECTOR_A[sectorIndexY - 1][sectorIndexX + 1].CPAL;

		for (AdvancedChatClientPlayerAddressLI cpaLI = cpaL_Address->begin(); cpaLI != cpaL_Address->end(); ++cpaLI)
		{
			if (SendPacket((*cpaLI)->SessionKey, sendingWork.PacketSPQ_Address)) PostSend((*cpaLI)->SessionKey);
		}
	}

	//Left
	if (sectorIndexX - 1 < ADVANCED_CHAT_SECTOR_WIDTH && sectorIndexY < ADVANCED_CHAT_SECTOR_HEIGHT)
	{
		//Multicast(sendingWork, sectorX - 1, sectorY);

		cpaL_Address = &AC_SECTOR_A[sectorIndexY][sectorIndexX - 1].CPAL;

		for (AdvancedChatClientPlayerAddressLI cpaLI = cpaL_Address->begin(); cpaLI != cpaL_Address->end(); ++cpaLI)
		{
			if (SendPacket((*cpaLI)->SessionKey, sendingWork.PacketSPQ_Address)) PostSend((*cpaLI)->SessionKey);
		}
	}

	//Center
	if (sectorIndexX < ADVANCED_CHAT_SECTOR_WIDTH && sectorIndexY < ADVANCED_CHAT_SECTOR_HEIGHT) //(1)
	{
		//Multicast(sendingWork, sectorX, sectorY);

		cpaL_Address = &AC_SECTOR_A[sectorIndexY][sectorIndexX].CPAL;

		for (AdvancedChatClientPlayerAddressLI cpaLI = cpaL_Address->begin(); cpaLI != cpaL_Address->end(); ++cpaLI)
		{
			if (SendPacket((*cpaLI)->SessionKey, sendingWork.PacketSPQ_Address)) PostSend((*cpaLI)->SessionKey);
		}
	}

	//Right
	if (sectorIndexX + 1 < ADVANCED_CHAT_SECTOR_WIDTH && sectorIndexY < ADVANCED_CHAT_SECTOR_HEIGHT)
	{
		//Multicast(sendingWork, sectorX + 1, sectorY);

		cpaL_Address = &AC_SECTOR_A[sectorIndexY][sectorIndexX + 1].CPAL;

		for (AdvancedChatClientPlayerAddressLI cpaLI = cpaL_Address->begin(); cpaLI != cpaL_Address->end(); ++cpaLI)
		{
			if (SendPacket((*cpaLI)->SessionKey, sendingWork.PacketSPQ_Address)) PostSend((*cpaLI)->SessionKey);
		}
	}

	//LeftBottom
	if (sectorIndexX - 1 < ADVANCED_CHAT_SECTOR_WIDTH && sectorIndexY + 1 < ADVANCED_CHAT_SECTOR_HEIGHT)
	{
		//Multicast(sendingWork, sectorX - 1, sectorY + 1);

		cpaL_Address = &AC_SECTOR_A[sectorIndexY + 1][sectorIndexX - 1].CPAL;

		for (AdvancedChatClientPlayerAddressLI cpaLI = cpaL_Address->begin(); cpaLI != cpaL_Address->end(); ++cpaLI)
		{
			if (SendPacket((*cpaLI)->SessionKey, sendingWork.PacketSPQ_Address)) PostSend((*cpaLI)->SessionKey);
		}
	}

	//Bottom
	if (sectorIndexX < ADVANCED_CHAT_SECTOR_WIDTH && sectorIndexY + 1 < ADVANCED_CHAT_SECTOR_HEIGHT)
	{
		//Multicast(sendingWork, sectorX, sectorY + 1);

		cpaL_Address = &AC_SECTOR_A[sectorIndexY + 1][sectorIndexX].CPAL;

		for (AdvancedChatClientPlayerAddressLI cpaLI = cpaL_Address->begin(); cpaLI != cpaL_Address->end(); ++cpaLI)
		{
			if (SendPacket((*cpaLI)->SessionKey, sendingWork.PacketSPQ_Address)) PostSend((*cpaLI)->SessionKey);
		}
	}

	//RightBottom
	if (sectorIndexX + 1 < ADVANCED_CHAT_SECTOR_WIDTH && sectorIndexY + 1 < ADVANCED_CHAT_SECTOR_HEIGHT)
	{
		//Multicast(sendingWork, sectorX + 1, sectorY + 1);

		cpaL_Address = &AC_SECTOR_A[sectorIndexY + 1][sectorIndexX + 1].CPAL;

		for (AdvancedChatClientPlayerAddressLI cpaLI = cpaL_Address->begin(); cpaLI != cpaL_Address->end(); ++cpaLI)
		{
			if (SendPacket((*cpaLI)->SessionKey, sendingWork.PacketSPQ_Address)) PostSend((*cpaLI)->SessionKey);
		}
	}

	return 0;
}

unsigned int ChatWAN_Server::Multicast(AdvancedChatWork sendingWork, UINT sectorIndexX, UINT sectorIndexY)
{
	AdvancedChatClientPlayerAddressL* cpaL_Address;

	cpaL_Address = &AC_SECTOR_A[sectorIndexY + 1][sectorIndexX + 1].CPAL;

	for (AdvancedChatClientPlayerAddressLI cpaLI = cpaL_Address->begin(); cpaLI != cpaL_Address->end(); ++cpaLI)
	{
		if (SendPacket((*cpaLI)->SessionKey, sendingWork.PacketSPQ_Address)) PostSend((*cpaLI)->SessionKey);
	}

	return 0;
}

unsigned int ChatWAN_Server::Unicast(AdvancedChatWork sendingWork)
{
	if (SendPacket(sendingWork.SessionKey, sendingWork.PacketSPQ_Address)) PostSend(sendingWork.SessionKey);

	return 0;
}

unsigned int ChatWAN_Server::Singlecast(AdvancedChatWork sendingWork)
{
	if (SendPacket(sendingWork.SessionKey, sendingWork.PacketSPQ_Address)) PostSend(sendingWork.SessionKey);

	return 0;
}