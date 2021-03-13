#include "stdafx.h"
#include "AdvancedLogin.h"
#include "LoginWAN_Server.h"

AdvancedLoginWork LoginWAN_Server::ProcessWork(AdvancedLoginWork receivedWork)
{
	AdvancedLoginWork sendingWork;
	WORD clientRequestIndex;
	WORD serverResponseIndex;

	AdvancedLoginClientPlayer* clientPlayerAddress;

	//sendingWork.SessionKey = clientPlayerAddress->SessionKey;
	InterlockedIncrement(&CURRENT_SPQ_COUNT);
	InterlockedIncrement(&ALLOCATED_SPQ_COUNT);
	sendingWork.PacketSPQ_Address = PACKET_SPQ_TLP.Allocate();

	clientRequestIndex = 0xffff;
	serverResponseIndex = 0xffff;

	clientPlayerAddress = nullptr;

	if (UnpackPacketSPQ(receivedWork.PacketSPQ_Address)) *receivedWork.PacketSPQ_Address >> clientRequestIndex;
	else clientRequestIndex = 0xffff;

	switch (clientRequestIndex)
	{
	case ADVANCED_CHAT_SERVER_RESPONSE_LOGIN_CONFIRM:
		{
			{
				INT64 accountKey;
				INT64 sessionKey;

				*receivedWork.PacketSPQ_Address >> accountKey;
				*receivedWork.PacketSPQ_Address >> sessionKey;

				for (AdvancedLoginClientPlayerAddressLI cpaLI = ALCPAL.begin(); cpaLI != ALCPAL.end(); ++cpaLI)
				{
					if ((*cpaLI)->SessionKey == sessionKey)
					{
						clientPlayerAddress = (*cpaLI);

						break;
					}
				}

				if (!clientPlayerAddress) break;
				else sendingWork.SessionKey = clientPlayerAddress->SessionKey;

				if (clientPlayerAddress->AccountKey == accountKey && clientPlayerAddress->SessionKey == sessionKey) //meaningless check
				{
					wstring queryText;
					MYSQL_ROW mysqlRow;

					queryText = L"SELECT * FROM account WHERE accountno = ";
					queryText += to_wstring(accountKey);

					TLDH.Query((WCHAR*)queryText.c_str());
					mysqlRow = TLDH.FetchRow();
					TLDH.FreeResult();

					clientPlayerAddress->Status = ADVANCED_LOGIN_SERVER_LOGIN_STATUS_SUCCESS;

					FOR(i, sizeof(clientPlayerAddress->RealName) / sizeof(WCHAR))
					{
						clientPlayerAddress->RealName[i] = mysqlRow[1][i];
					}
				
					FOR(i, sizeof(clientPlayerAddress->Nickname) / sizeof(WCHAR))
					{
						clientPlayerAddress->Nickname[i] = mysqlRow[3][i];
					}
				}

				serverResponseIndex = ADVANCED_LOGIN_SERVER_RESPONSE_LOGIN;
			}
			++CheckPoint[0];
			{
				INT64 accountKey;
				BYTE status;
				WCHAR realName[ADVANCED_LOGIN_REAL_NAME_LENGTH];
				WCHAR nickname[ADVANCED_LOGIN_NICKNAME_LENGTH];

				WCHAR gameServerIP_Address[16];
				USHORT gameServerPort;
				WCHAR chatServerIP_Address[16];
				USHORT chatServerPort;

				accountKey = clientPlayerAddress->AccountKey;
				status = clientPlayerAddress->Status;

				FOR(i, sizeof(clientPlayerAddress->RealName) / sizeof(WCHAR))
				{
					realName[i] = clientPlayerAddress->RealName[i];
				}

				FOR(i, sizeof(clientPlayerAddress->Nickname) / sizeof(WCHAR))
				{
					nickname[i] = clientPlayerAddress->Nickname[i];
				}

				FOR(i, sizeof(gameServerIP_Address) / sizeof(WCHAR))
				{
					gameServerIP_Address[i] = GameServerIP_Address[i];
				}

				gameServerPort = GameServerPort;

				FOR(i, sizeof(chatServerIP_Address) / sizeof(WCHAR))
				{
					chatServerIP_Address[i] = ChatServerIP_Address[i];
				}

				chatServerPort = ChatServerPort;



				*sendingWork.PacketSPQ_Address << serverResponseIndex;
				*sendingWork.PacketSPQ_Address << accountKey;
				*sendingWork.PacketSPQ_Address << status;

				FOR(i, sizeof(realName) / sizeof(WCHAR))
				{
					*sendingWork.PacketSPQ_Address << realName[i];
				}

				FOR(i, sizeof(nickname) / sizeof(WCHAR))
				{
					*sendingWork.PacketSPQ_Address << nickname[i];
				}

				FOR(i, sizeof(gameServerIP_Address) / sizeof(WCHAR))
				{
					*sendingWork.PacketSPQ_Address << gameServerIP_Address[i];
				}

				*sendingWork.PacketSPQ_Address << gameServerPort;

				FOR(i, sizeof(chatServerIP_Address) / sizeof(WCHAR))
				{
					*sendingWork.PacketSPQ_Address << chatServerIP_Address[i];
				}

				*sendingWork.PacketSPQ_Address << chatServerPort;

				if (PackPacketSPQ(sendingWork.PacketSPQ_Address)) Unicast(sendingWork);
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

AdvancedLoginWork LoginWAN_Server::ProcessRequest(AdvancedLoginClientPlayer* clientPlayerAddress, AdvancedLoginWork receivedWork)
{
	AdvancedLoginWork work;

	return work;
}

AdvancedLoginWork LoginWAN_Server::ProcessResponse(AdvancedLoginClientPlayer* clientPlayerAddress, AdvancedLoginWork sendingWork)
{
	AdvancedLoginWork work;

	return work;
}

unsigned int LoginWAN_Server::UnpackPacketSPQ(SerialPacketQueue<IOCP_PacketHeader>* packetSPQ_Address)
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

unsigned int LoginWAN_Server::PackPacketSPQ(SerialPacketQueue<IOCP_PacketHeader>* packetSPQ_Address)
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

unsigned int LoginWAN_Server::Unicast(AdvancedLoginWork sendingWork)
{
	if (SendPacket(sendingWork.SessionKey, sendingWork.PacketSPQ_Address)) PostSend(sendingWork.SessionKey);

	return 0;
}

unsigned int LoginWAN_Server::Singlecast(AdvancedLoginWork sendingWork)
{
	if (SendPacket(sendingWork.SessionKey, sendingWork.PacketSPQ_Address)) PostSend(sendingWork.SessionKey);

	return 0;
}
