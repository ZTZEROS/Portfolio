#include "stdafx.h"
#include "Protocol.h"
#include "ChatServer.h"

bool InitializeChatServer()
{
	WSADATA wsaDatum;
	u_long nonblockOn;
	//LINGER lingeringTime;
	//BOOL nagleOn;
	//BOOL reuseOn;

	ZeroMemory(&CHAT_SERVER_SESSION, sizeof(CHAT_SERVER_SESSION));
	//ZeroMemory(CHAT_CLIENT_SESSION_A, sizeof(CHAT_CLIENT_SESSION_A));
	CHAT_CLIENT_SESSION_M.reserve(TOTAL_CHAT_CLIENT_COUNT);

	CHECK_ERROR(WSAStartup(0x0202, &wsaDatum));

	CHAT_SERVER_SESSION.Socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	CHECK_INVALID_SOCKET(CHAT_SERVER_SESSION.Socket);

	nonblockOn = 1;
	CHECK_SOCKET_ERROR(ioctlsocket(CHAT_SERVER_SESSION.Socket, FIONBIO, &nonblockOn));

	//lingeringTime.l_onoff = 1;
	//lingeringTime.l_linger = 0;
	//setsockopt(CHAT_SERVER_SESSION.Socket, SOL_SOCKET, SO_LINGER, (char*)&lingeringTime, sizeof(lingeringTime));

	//nagleOn = 1;
	//setsockopt(CHAT_SERVER_SESSION.Socket, IPPROTO_TCP, TCP_NODELAY, (char*)&nagleOn, sizeof(nagleOn));

	//reuseOn = 1;
	//setsockopt(CHAT_SERVER_SESSION.Socket, SOL_SOCKET, SO_REUSEADDR, (char*)&reuseOn, sizeof(reuseOn));

	CHAT_SERVER_SESSION.Address.sin_family = AF_INET;
	CHAT_SERVER_SESSION.Address.sin_addr.s_addr = htonl(INADDR_ANY); //WSAHtonl();
	//InetPton(AF_INET, INADDR_ANY, &serverAddress.sin_addr); //inet_addr(INADDR_ANY); //InetNtop();
	CHAT_SERVER_SESSION.Address.sin_port = htons(CHAT_SERVER_PORT);
	CHECK_SOCKET_ERROR(bind(CHAT_SERVER_SESSION.Socket, (SOCKADDR*)&CHAT_SERVER_SESSION.Address, sizeof(CHAT_SERVER_SESSION.Address)));
	CHECK_SOCKET_ERROR(listen(CHAT_SERVER_SESSION.Socket, SOMAXCONN));

	CHAT_SERVER_TIME_VALUE.tv_sec = 0;
	CHAT_SERVER_TIME_VALUE.tv_usec = 0;

	LAST_CHAT_CLIENT_INDEX = ~0; //0xffffffff; // -1;
	CURRENT_CHAT_CLIENT_COUNT = 0;

	FOR(i, TOTAL_CHAT_CLIENT_COUNT)
	{
		CHAT_USER_NUMBER_A[i] = i + 1;
	}

	LAST_CHAT_ROOM_INDEX = ~0;
	CURRENT_CHAT_ROOM_COUNT = 0;

	FOR(i, TOTAL_CHAT_CLIENT_COUNT)
	{
		CHAT_ROOM_NUMBER_A[i] = i + 1;
	}

	return 0;
}

bool UpdateChatServer()
{
	//AcceptPhase:
	{
		AcceptChatClient();
	}

	//ReceivePhase:
	{
		ReceiveChatClientPacket();
		ProcessReceivedChatClientPacket();
	}

	//SendPhase:
	{
		//ProcessSendingChatServerPacket(); //called by ProcessReceivedChatServerPacket();
		SendChatServerPacket();
	}

	return 0;
}

bool RenderChatServer()
{
	return 0;
}

bool TerminalizeChatServer()
{
	closesocket(CHAT_SERVER_SESSION.Socket);
	WSACleanup();

	return 0;
}

int AcceptChatClient()
{
	FD_SET readSet;

	FD_ZERO(&readSet);

	FD_SET(CHAT_SERVER_SESSION.Socket, &readSet);

	IF_SOCKET_ERROR(select(0, &readSet, nullptr, nullptr, &CHAT_SERVER_TIME_VALUE))
	ELSE
	{
		if (FD_ISSET(CHAT_SERVER_SESSION.Socket, &readSet))
		{
			ChatClientSession chatClientSession;
			int addressLength;

			ZeroMemory(&chatClientSession, sizeof(chatClientSession));
			addressLength = sizeof(chatClientSession.Address);

			chatClientSession.Socket = accept(CHAT_SERVER_SESSION.Socket, (SOCKADDR*)&chatClientSession.Address, &addressLength);
			IF_WSAEWOULDBLOCK(chatClientSession.Socket)
			ELSE
			{
				CHAT_CLIENT_SESSION_M[chatClientSession.Socket] = chatClientSession;
				CHAT_CLIENT_SESSION_M[chatClientSession.Socket].ReceiveBQ.Resize(BUFFER_SIZE);
				CHAT_CLIENT_SESSION_M[chatClientSession.Socket].SendBQ.Resize(BUFFER_SIZE);
				CHAT_CLIENT_SESSION_L.push_back(&CHAT_CLIENT_SESSION_M[chatClientSession.Socket]);
				//CHAT_CLIENT_SESSION_M.insert(make_pair(chatClientSocket, &CHAT_CLIENT_SESSION_A[LAST_CHAT_CLIENT_INDEX]));
				//CHAT_CLIENT_SESSION_M.insert(pair<unsigned int, ChatClientSession*>(chatClientSocket, &CHAT_CLIENT_SESSION_A[LAST_CHAT_CLIENT_INDEX]));
				//CHAT_CLIENT_SESSION_M.insert(ChatClientSessionM::value_type(LAST_CHAT_CLIENT_INDEX, &CHAT_CLIENT_SESSION_A[LAST_CHAT_CLIENT_INDEX]));
			}
		}
	}

	return 0;
}

int DisconnectChatClient()
{
	//CHAT_USER_NUMBER_A[x] = CHAT_USER_NUMBER_A[LAST_CHAT_CLINET_INDEX];
	//CHAT_USER_NUMBER_A[LAST_CHAT_CLIENT_INDEX] = x;

	--LAST_CHAT_CLIENT_INDEX;
	CURRENT_CHAT_CLIENT_COUNT = LAST_CHAT_CLIENT_INDEX + 1;

	return 0;
}

int ReceiveChatClientPacket()
{
	FD_SET readSet;
	FD_SET writeSet;
	unsigned int setClientCount;

	FD_ZERO(&readSet);
	setClientCount = 0;

	for(ChatClientSessionMI chatClientSessionMI = CHAT_CLIENT_SESSION_M.begin(); chatClientSessionMI != CHAT_CLIENT_SESSION_M.end();)
	{
		SOCKET chatClientSessionSocket;

		chatClientSessionSocket = chatClientSessionMI->second.Socket;
		
		FD_SET(chatClientSessionSocket, &readSet);
		
		++chatClientSessionMI;

		if (readSet.fd_count == CHAT_CLIENT_COUNT || 0 < readSet.fd_count && chatClientSessionMI == CHAT_CLIENT_SESSION_M.end())
		{
			IF_SOCKET_ERROR(select(0, &readSet, nullptr, nullptr, &CHAT_SERVER_TIME_VALUE))
			ELSE
			{
				FOR(i, readSet.fd_count)
				{
					ChatClientSession* chatClientSessionAddress;
					int receivedSize;

					chatClientSessionAddress = &CHAT_CLIENT_SESSION_M[readSet.fd_array[i]];
					
					if (chatClientSessionAddress->ReceiveBQ.GetNextIndex() < chatClientSessionAddress->ReceiveBQ.GetFrontIndex())
					{
						receivedSize = recv(chatClientSessionAddress->Socket, chatClientSessionAddress->ReceiveBQ.GetNextAddress(), chatClientSessionAddress->ReceiveBQ.GetLeftSize(), 0);
						chatClientSessionAddress->ReceiveBQ.IncreaseCurrentSize(receivedSize);
					}
					else
					{
						int rearSpace;

						rearSpace = chatClientSessionAddress->ReceiveBQ.GetMaximumSize() - chatClientSessionAddress->ReceiveBQ.GetNextIndex();

						receivedSize = recv(chatClientSessionAddress->Socket, chatClientSessionAddress->ReceiveBQ.GetNextAddress(), rearSpace, 0);
						chatClientSessionAddress->ReceiveBQ.IncreaseCurrentSize(receivedSize);

						if (rearSpace == receivedSize) --i;
					}

					if (receivedSize == SOCKET_ERROR || receivedSize == 0)
					{
						if (WSAGetLastError() == WSAEWOULDBLOCK);
						else
						{
							closesocket(chatClientSessionAddress->Socket);

							CHAT_USER_NUMBER_A[chatClientSessionAddress->UserNumber] = CHAT_USER_NUMBER_A[LAST_CHAT_CLIENT_INDEX];
							CHAT_USER_NUMBER_A[LAST_CHAT_CLIENT_INDEX] = chatClientSessionAddress->UserNumber;

							--LAST_CHAT_CLIENT_INDEX;
							CURRENT_CHAT_CLIENT_COUNT = LAST_CHAT_CLIENT_INDEX + 1;

							CHAT_CLIENT_SESSION_M.erase(chatClientSessionAddress->Socket);
						}
					}
				}
			}

			FD_ZERO(&readSet);
			setClientCount = 0;
		}
	}

	return 0;
}

int SendChatServerPacket()
{
	FD_SET readSet;
	FD_SET writeSet;

	FD_ZERO(&writeSet);

	for (ChatClientSessionMI chatClientSessionMI = CHAT_CLIENT_SESSION_M.begin(); chatClientSessionMI != CHAT_CLIENT_SESSION_M.end();)
	{
		SOCKET chatClientSessionSocket;

		chatClientSessionSocket = chatClientSessionMI->second.Socket;

		if (CHAT_CLIENT_SESSION_M[chatClientSessionSocket].SendBQ.GetCurrentSize()) FD_SET(chatClientSessionSocket, &writeSet);

		++chatClientSessionMI;

		if (writeSet.fd_count == FD_SETSIZE || 0 < writeSet.fd_count && chatClientSessionMI == CHAT_CLIENT_SESSION_M.end())
		{
			IF_SOCKET_ERROR(select(0, nullptr, &writeSet, nullptr, &CHAT_SERVER_TIME_VALUE))
			ELSE
			{
				FOR(i, writeSet.fd_count)
				{
					ChatClientSession* chatClientSessionAddress;
					int sendedSize;

					chatClientSessionAddress = &CHAT_CLIENT_SESSION_M[writeSet.fd_array[i]];

					if (chatClientSessionAddress->SendBQ.GetFrontIndex() < chatClientSessionAddress->SendBQ.GetNextIndex())
					{
						sendedSize = send(chatClientSessionAddress->Socket, chatClientSessionAddress->SendBQ.GetFrontAddress(), chatClientSessionAddress->SendBQ.GetCurrentSize(), 0);
						chatClientSessionAddress->SendBQ.DecreaseCurrentSize(sendedSize);
					}
					else
					{
						int frontSize;

						frontSize = chatClientSessionAddress->SendBQ.GetMaximumSize() - chatClientSessionAddress->SendBQ.GetFrontIndex();

						sendedSize = send(chatClientSessionAddress->Socket, chatClientSessionAddress->SendBQ.GetFrontAddress(), frontSize, 0);
						chatClientSessionAddress->SendBQ.DecreaseCurrentSize(sendedSize);

						if (chatClientSessionAddress->SendBQ.GetCurrentSize()) --i;
					}

					if (sendedSize == SOCKET_ERROR) //if(sendedSize == SOCKET_ERROR || sendedSize == 0)
					{
						if (WSAGetLastError() == WSAEWOULDBLOCK);
						else
						{
							closesocket(chatClientSessionAddress->Socket);

							CHAT_USER_NUMBER_A[chatClientSessionAddress->UserNumber] = CHAT_USER_NUMBER_A[LAST_CHAT_CLIENT_INDEX];
							CHAT_USER_NUMBER_A[LAST_CHAT_CLIENT_INDEX] = chatClientSessionAddress->UserNumber;

							--LAST_CHAT_CLIENT_INDEX;
							CURRENT_CHAT_CLIENT_COUNT = LAST_CHAT_CLIENT_INDEX + 1;

							CHAT_CLIENT_SESSION_M.erase(chatClientSessionAddress->Socket);
						}
					}
				}
			}

			FD_ZERO(&writeSet);
		}
	}

	return 0;
}

int ProcessReceivedChatClientPacket()
{
	for (ChatClientSessionMI chatClientSessionMI = CHAT_CLIENT_SESSION_M.begin(); chatClientSessionMI != CHAT_CLIENT_SESSION_M.end(); ++chatClientSessionMI)
	{
		if (0 < chatClientSessionMI->second.ReceiveBQ.GetCurrentSize())
		{
			ChatPacketHeader receivedPacketHeader;
			SerialByteQueue receivedPacketSBQ(sizeof(receivedPacketHeader), BUFFER_SIZE);

			chatClientSessionMI->second.ReceiveBQ.Peek((char*)&receivedPacketHeader, sizeof(receivedPacketHeader));
			chatClientSessionMI->second.ReceiveBQ.Peek(receivedPacketSBQ.GetQueueAddress(), sizeof(receivedPacketHeader) + receivedPacketHeader.PacketBodySize);
			//chatClientSessionMI->second.ReceiveBQ.Peek(receivedPacketSBQ.GetQueueAddress(), receivedPacketSBQ.GetMaximumSize());

			receivedPacketSBQ.IncreaseCurrentSize(receivedPacketHeader.PacketBodySize);

			if (CheckReceivedChatClientPacket(&receivedPacketHeader, &receivedPacketSBQ))
			{
				chatClientSessionMI->second.ReceiveBQ.DecreaseCurrentSize(sizeof(receivedPacketHeader) + receivedPacketHeader.PacketBodySize);

				switch (receivedPacketHeader.PacketType)
				{
				case CHAT_REQUEST_LOGIN:
					{
						if (receivedPacketHeader.PacketBodySize < CHAT_NICKNAME_LENGTH * sizeof(WCHAR) && !(receivedPacketHeader.PacketBodySize % 2))
						{
							ProcessSendingChatServerPacket(CHAT_RESPONSE_LOGIN, chatClientSessionMI, &receivedPacketSBQ);
						}
					}
					break;
				case CHAT_REQUEST_ROOM_LIST:
					{
						if (receivedPacketHeader.PacketBodySize == 0)
						{
							ProcessSendingChatServerPacket(CHAT_RESPONSE_ROOM_LIST, chatClientSessionMI, &receivedPacketSBQ);
						}
					}
					break;
				case CHAT_REQUEST_ROOM_CREATE:
					{
						if (receivedPacketHeader.PacketBodySize)
						{
							ProcessSendingChatServerPacket(CHAT_RESPONSE_ROOM_CREATE, chatClientSessionMI, &receivedPacketSBQ);
						}
					}
					break;
				case CHAT_REQUEST_ROOM_ENTER:
					{
						if (receivedPacketHeader.PacketBodySize == 4)
						{
							ProcessSendingChatServerPacket(CHAT_RESPONSE_ROOM_ENTER, chatClientSessionMI, &receivedPacketSBQ);
							ProcessSendingChatServerPacket(CHAT_RESPONSE_USER_ENTER, chatClientSessionMI, &receivedPacketSBQ);
						}
					}
					break;
				case CHAT_REQUEST_ROOM_CHAT:
					{
						if (receivedPacketHeader.PacketBodySize)
						{
							ProcessSendingChatServerPacket(CHAT_RESPONSE_ROOM_CHAT, chatClientSessionMI, &receivedPacketSBQ);
						}
					}
					break;
				case CHAT_REQUEST_ROOM_LEAVE:
					{
						if (receivedPacketHeader.PacketBodySize == 0)
						{
							ProcessSendingChatServerPacket(CHAT_RESPONSE_ROOM_LEAVE, chatClientSessionMI, &receivedPacketSBQ);

							if (!CHAT_ROOM_M[chatClientSessionMI->second.RoomNumber].UserL.size())
							{
								ProcessSendingChatServerPacket(CHAT_RESPONSE_ROOM_DELETE, chatClientSessionMI, &receivedPacketSBQ);
							}

							chatClientSessionMI->second.RoomNumber = 0;
						}
					}
					break;
				default:
					break;
				}
			}
		}
	}

	return 0;
}

int ProcessSendingChatServerPacket(CHAT_RESPONSE_INDEX chatResponseIndex, ChatClientSessionMI chatClientSessionMI, SerialByteQueue* receivedPacketSBQ)
{
	ChatPacketHeader sendingPacketHeader;
	SerialByteQueue sendingPacketSBQ(sizeof(sendingPacketHeader), BUFFER_SIZE);
	
	ZeroMemory(&sendingPacketHeader, sizeof(sendingPacketHeader));

	sendingPacketHeader.PacketType = chatResponseIndex;

	switch (sendingPacketHeader.PacketType)
	{
	case CHAT_RESPONSE_LOGIN:
		{
			//if(CHAT_CLIENT_SESSION_M[currentChatClientSocket].Nickname) //check same nickname

			FOR(i, CHAT_USER_NAME_LENGTH - 1) //15
			{
				(*receivedPacketSBQ) >> chatClientSessionMI->second.UserName[i];

				chatClientSessionMI->second.UserName[i + 1] = '\0\0'; //foolish

				if (chatClientSessionMI->second.UserName[i] == '\0\0') break;
			}

			++LAST_CHAT_CLIENT_INDEX;
			CURRENT_CHAT_CLIENT_COUNT = LAST_CHAT_CLIENT_INDEX + 1;

			chatClientSessionMI->second.UserNumber = CHAT_USER_NUMBER_A[LAST_CHAT_CLIENT_INDEX];


			sendingPacketSBQ << (BYTE)CHAT_RESULT_SUCCESS;
			sendingPacketSBQ << chatClientSessionMI->second.UserNumber;

			if (CheckSendingChatServerPacket(&sendingPacketHeader, &sendingPacketSBQ))
			{
				chatClientSessionMI->second.SendBQ.Enqueue(sendingPacketSBQ.GetQueueAddress(), sendingPacketSBQ.GetTotalSize());
				//chatClientSessionMI->second.SendBQ.Enqueue(sendingPacketSBQ.GetQueueAddress(), sendingPacketSBQ.GetHeaderSize());
				//chatClientSessionMI->second.SendBQ.Enqueue(sendingPacketSBQ.GetFrontAddress(), sendingPacketSBQ.GetCurrentSize());
			}
		}
		break;
	case CHAT_RESPONSE_ROOM_LIST:
		{
			sendingPacketSBQ << (WORD)CHAT_ROOM_M.size();

			for (ChatRoomMI chatRoomMI = CHAT_ROOM_M.begin(); chatRoomMI != CHAT_ROOM_M.end(); ++chatRoomMI)
			{
				sendingPacketSBQ << chatRoomMI->second.RoomNumber;
				sendingPacketSBQ << (WORD)(wcslen(chatRoomMI->second.RoomName) * sizeof(WCHAR));
				
				FOR(i, CHAT_ROOM_NAME_LENGTH)
				{
					sendingPacketSBQ << chatRoomMI->second.RoomName[i];

					if (chatRoomMI->second.RoomName[i] == '\0\0') break;
				}
				
				sendingPacketSBQ << (BYTE)chatRoomMI->second.UserL.size();

				for (ChatClientSessionAddressLI userLI = chatRoomMI->second.UserL.begin(); userLI != chatRoomMI->second.UserL.end(); ++userLI)
				{
					FOR(i, CHAT_USER_NAME_LENGTH - 1) //15
					{
						sendingPacketSBQ << (*userLI)->UserName[i];

						//if ((*userLI)->UserName[i] == '\0\0') break;
					}
				}
			}

			if(CheckSendingChatServerPacket(&sendingPacketHeader, &sendingPacketSBQ)) chatClientSessionMI->second.SendBQ.Enqueue(sendingPacketSBQ.GetQueueAddress(), sendingPacketSBQ.GetTotalSize());
		}
		break;
	case CHAT_RESPONSE_ROOM_CREATE:
		{
			if (CURRENT_CHAT_ROOM_COUNT < TOTAL_CHAT_ROOM_COUNT)
			{
				ChatRoom chatRoom;
				WORD chatRoomNameSize;

				chatRoomNameSize = 0;

				++LAST_CHAT_ROOM_INDEX;
				CURRENT_CHAT_ROOM_COUNT = LAST_CHAT_ROOM_INDEX + 1;

				chatRoom.RoomNumber = CHAT_ROOM_NUMBER_A[LAST_CHAT_ROOM_INDEX];
				(*receivedPacketSBQ) >> chatRoomNameSize;

				FOR(i, chatRoomNameSize / sizeof(WCHAR))
				{
					(*receivedPacketSBQ) >> chatRoom.RoomName[i];

					chatRoom.RoomName[i + 1] = '\0\0'; //foolish

					if (chatRoom.RoomName[i] == '\0\0') break;
				}

				CHAT_ROOM_M[chatRoom.RoomNumber] = chatRoom;
				


				sendingPacketSBQ << (BYTE)CHAT_RESULT_SUCCESS;
				sendingPacketSBQ << chatRoom.RoomNumber;
				sendingPacketSBQ << chatRoomNameSize;

				FOR(i, chatRoomNameSize / sizeof(WCHAR))
				{
					sendingPacketSBQ << chatRoom.RoomName[i];

					if (chatRoom.RoomName[i] == '\0\0') break; //warning
				}

				if (CheckSendingChatServerPacket(&sendingPacketHeader, &sendingPacketSBQ))
				{
					for (ChatClientSessionMI userMI = CHAT_CLIENT_SESSION_M.begin(); userMI != CHAT_CLIENT_SESSION_M.end(); ++userMI)
					{
						userMI->second.SendBQ.Enqueue(sendingPacketSBQ.GetQueueAddress(), sendingPacketSBQ.GetTotalSize());
					}
				}
			}
			else
			{
				sendingPacketSBQ << (BYTE)CHAT_RESULT_FAILURE; //CHAT_RESULT_EXCESS;

				if(CheckSendingChatServerPacket(&sendingPacketHeader, &sendingPacketSBQ)) chatClientSessionMI->second.SendBQ.Enqueue(sendingPacketSBQ.GetQueueAddress(), sendingPacketSBQ.GetTotalSize());
			}
		}
		break;
	case CHAT_RESPONSE_ROOM_ENTER:
		{
			ChatRoomMI chatRoomMI;
			ChatRoom chatRoom;
			unsigned int roomNumber;
			
			(*receivedPacketSBQ) >> roomNumber;

			chatRoomMI = CHAT_ROOM_M.find(roomNumber);
			if (chatRoomMI != CHAT_ROOM_M.end())
			{
				chatClientSessionMI->second.RoomNumber = roomNumber;

				sendingPacketSBQ << (BYTE)CHAT_RESULT_SUCCESS;
				sendingPacketSBQ << roomNumber;
				sendingPacketSBQ << (WORD)(wcslen(chatRoom.RoomName) * sizeof(WCHAR));

				FOR(i, CHAT_ROOM_NAME_LENGTH)
				{
					sendingPacketSBQ << chatRoom.RoomName[i];

					if (chatRoom.RoomName[i + 1] == '\0\0') break; //warning
				}

				CHAT_ROOM_M[roomNumber].UserL.push_back(&(chatClientSessionMI->second));

				sendingPacketSBQ << (BYTE)CHAT_ROOM_M[roomNumber].UserL.size();

				for (ChatClientSessionAddressLI userLI = CHAT_ROOM_M[roomNumber].UserL.begin(); userLI != CHAT_ROOM_M[roomNumber].UserL.end(); ++userLI)
				{
					FOR(i, CHAT_USER_NAME_LENGTH - 1)
					{
						sendingPacketSBQ << (*userLI)->UserName[i];

						//if ((*userLI)->UserName[i] == '\0\0') break;
					}

					sendingPacketSBQ << (*userLI)->UserNumber;
				}

				if (CheckSendingChatServerPacket(&sendingPacketHeader, &sendingPacketSBQ))
				{
					chatClientSessionMI->second.SendBQ.Enqueue(sendingPacketSBQ.GetQueueAddress(), sendingPacketSBQ.GetTotalSize());
				}
			}
			else
			{
				sendingPacketSBQ << (BYTE)CHAT_RESULT_FAILURE;

				if (CheckSendingChatServerPacket(&sendingPacketHeader, &sendingPacketSBQ)) chatClientSessionMI->second.SendBQ.Enqueue(sendingPacketSBQ.GetQueueAddress(), sendingPacketSBQ.GetTotalSize());
			}
		}
		break;
	case CHAT_RESPONSE_ROOM_CHAT:
		{
			WORD chatMessageSize;
			WCHAR chatMessage[CHAT_MESSAGE_LENGTH];
			unsigned int roomNumber;
			
			chatMessageSize = 0;

			roomNumber = chatClientSessionMI->second.RoomNumber;

			(*receivedPacketSBQ) >> chatMessageSize;

			FOR(i, chatMessageSize / sizeof(WCHAR))
			{
				(*receivedPacketSBQ) >> chatMessage[i];

				chatMessage[i + 1] = '\0\0'; //foolish

				if (chatMessage[i] == '\0\0') break;
			}

			sendingPacketSBQ << chatClientSessionMI->second.UserNumber;
			sendingPacketSBQ << chatMessageSize;

			FOR(i, chatMessageSize / sizeof(WCHAR))
			{
				sendingPacketSBQ << chatMessage[i];

				if (chatMessage[i] == '\0\0') break;
			}

			if (CheckSendingChatServerPacket(&sendingPacketHeader, &sendingPacketSBQ))
			{
				for (ChatClientSessionAddressLI userLI = CHAT_ROOM_M[roomNumber].UserL.begin(); userLI != CHAT_ROOM_M[roomNumber].UserL.end(); ++userLI)
				{
					if (chatClientSessionMI->second.Socket != (*userLI)->Socket) (*userLI)->SendBQ.Enqueue(sendingPacketSBQ.GetQueueAddress(), sendingPacketSBQ.GetTotalSize());
				}
			}
		}
		break;
	case CHAT_RESPONSE_ROOM_LEAVE:
		{
			unsigned int userNumber;
			unsigned int roomNumber;

			userNumber = chatClientSessionMI->second.UserNumber;
			roomNumber = chatClientSessionMI->second.RoomNumber;
			
			sendingPacketSBQ << userNumber;

			if (CheckSendingChatServerPacket(&sendingPacketHeader, &sendingPacketSBQ))
			{
				for (ChatClientSessionAddressLI userLI = CHAT_ROOM_M[roomNumber].UserL.begin(); userLI != CHAT_ROOM_M[roomNumber].UserL.end(); ++userLI)
				{
					(*userLI)->SendBQ.Enqueue(sendingPacketSBQ.GetQueueAddress(), sendingPacketSBQ.GetTotalSize());
				}
			}

			for (ChatClientSessionAddressLI userLI = CHAT_ROOM_M[roomNumber].UserL.begin(); userLI != CHAT_ROOM_M[roomNumber].UserL.end(); ++userLI)
			{
				if ((*userLI)->UserNumber == userNumber)
				{
					CHAT_ROOM_M[roomNumber].UserL.erase(userLI);

					break;
				}
			}
		}
		break;
	case CHAT_RESPONSE_ROOM_DELETE:
		{
			unsigned int roomNumber;

			roomNumber = chatClientSessionMI->second.RoomNumber;

			sendingPacketSBQ << roomNumber;

			if (CheckSendingChatServerPacket(&sendingPacketHeader, &sendingPacketSBQ))
			{
				for (ChatClientSessionMI userMI = CHAT_CLIENT_SESSION_M.begin(); userMI != CHAT_CLIENT_SESSION_M.end(); ++userMI)
				{
					userMI->second.SendBQ.Enqueue(sendingPacketSBQ.GetQueueAddress(), sendingPacketSBQ.GetTotalSize());
				}
			}

			CHAT_ROOM_M.erase(roomNumber);

			CHAT_ROOM_NUMBER_A[roomNumber] = CHAT_ROOM_NUMBER_A[LAST_CHAT_ROOM_INDEX];
			CHAT_ROOM_NUMBER_A[LAST_CHAT_ROOM_INDEX] = roomNumber;

			--LAST_CHAT_ROOM_INDEX;
			CURRENT_CHAT_ROOM_COUNT = LAST_CHAT_ROOM_INDEX + 1;
		}
		break;
	case CHAT_RESPONSE_USER_ENTER:
		{
			unsigned int userNumber;
			unsigned int roomNumber;

			userNumber = chatClientSessionMI->second.UserNumber;
			roomNumber = chatClientSessionMI->second.RoomNumber;

			FOR(i, CHAT_USER_NAME_LENGTH - 1)
			{
				sendingPacketSBQ << chatClientSessionMI->second.UserName[i];

				//if (chatClientSessionMI->second.UserName[i] == '\0\0') break;
			}

			sendingPacketSBQ << userNumber;

			if (CheckSendingChatServerPacket(&sendingPacketHeader, &sendingPacketSBQ))
			{
				for (ChatClientSessionAddressLI userLI = CHAT_ROOM_M[roomNumber].UserL.begin(); userLI != CHAT_ROOM_M[roomNumber].UserL.end(); ++userLI)
				{
					if((*userLI)->UserNumber != userNumber) (*userLI)->SendBQ.Enqueue(sendingPacketSBQ.GetQueueAddress(), sendingPacketSBQ.GetTotalSize());
				}
			}
		}
		break;
	default:
		break;
	}

	return 0;
}

bool CheckReceivedChatClientPacket(ChatPacketHeader* receivedPacketHeader, SerialByteQueue* receivedPacketSBQ)
{
	BYTE checksum256;

	checksum256 = 0;

	if (receivedPacketHeader->IdentificationCode == CHAT_PACKET_HEADER_IDENTIFICATION_CODE)
	{
		if (receivedPacketHeader->PacketBodySize == receivedPacketSBQ->GetCurrentSize())
		{
			checksum256 += ((BYTE*)&receivedPacketHeader->PacketType)[0];
			checksum256 += ((BYTE*)&receivedPacketHeader->PacketType)[1];

			FOR(i, receivedPacketHeader->PacketBodySize)
			{
				checksum256 += ((BYTE*)(receivedPacketSBQ->GetFrontAddress()))[i];
			}

			if (receivedPacketHeader->Checksum256 == checksum256) return 1;
		}
	}

	return 0;
}

bool CheckSendingChatServerPacket(ChatPacketHeader* sendingPacketHeader, SerialByteQueue* sendingPacketSBQ)
{
	sendingPacketHeader->IdentificationCode = CHAT_PACKET_HEADER_IDENTIFICATION_CODE;

	{
		sendingPacketHeader->PacketBodySize = sendingPacketSBQ->GetCurrentSize();

		{
			sendingPacketHeader->Checksum256 += ((BYTE*)&sendingPacketHeader->PacketType)[0];
			sendingPacketHeader->Checksum256 += ((BYTE*)&sendingPacketHeader->PacketType)[1];

			FOR(i, sendingPacketHeader->PacketBodySize)
			{
				sendingPacketHeader->Checksum256 += ((BYTE*)(sendingPacketSBQ->GetFrontAddress()))[i];
			}

			sendingPacketSBQ->SetHeader(*sendingPacketHeader);

			return 1;
		}
	}

	return 0;
}