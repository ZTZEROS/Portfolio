#include "stdafx.h"
#include "Protocol.h"
#include "FriendManagementServer.h"

bool InitializeFM_Server()
{
	WSADATA wsaDatum;
	u_long nonblockOn;
	//LINGER lingeringTime;
	BOOL nagleOn;
	//BOOL reuseOn;

	ZeroMemory(&FM_SERVER_SESSION, sizeof(FM_SERVER_SESSION));
	FM_CLIENT_SESSION_M.reserve(TOTAL_FM_CLIENT_COUNT);

	CHECK_ERROR(WSAStartup(0x0202, &wsaDatum));

	FM_SERVER_SESSION.Socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	CHECK_INVALID_SOCKET(FM_SERVER_SESSION.Socket);

	nonblockOn = 1;
	CHECK_SOCKET_ERROR(ioctlsocket(FM_SERVER_SESSION.Socket, FIONBIO, &nonblockOn));

	//lingeringTime.l_onoff = 1;
	//lingeringTime.l_linger = 0;
	//setsockopt(FM_SERVER_SESSION.Socket, SOL_SOCKET, SO_LINGER, (char*)&lingeringTime, sizeof(lingeringTime));

	nagleOn = 1;
	setsockopt(FM_SERVER_SESSION.Socket, IPPROTO_TCP, TCP_NODELAY, (char*)&nagleOn, sizeof(nagleOn));

	//reuseOn = 1;
	//setsockopt(FM_SERVER_SESSION.Socket, SOL_SOCKET, SO_REUSEADDR, (char*)&reuseOn, sizeof(reuseOn));

	FM_SERVER_SESSION.Address.sin_family = AF_INET;
	FM_SERVER_SESSION.Address.sin_addr.s_addr = htonl(INADDR_ANY); //WSAHtonl();
	//InetPton(AF_INET, INADDR_ANY, &serverAddress.sin_addr); //inet_addr(INADDR_ANY); //InetNtop();
	FM_SERVER_SESSION.Address.sin_port = htons(FM_SERVER_PORT);
	CHECK_SOCKET_ERROR(bind(FM_SERVER_SESSION.Socket, (SOCKADDR*)&FM_SERVER_SESSION.Address, sizeof(FM_SERVER_SESSION.Address)));
	CHECK_SOCKET_ERROR(listen(FM_SERVER_SESSION.Socket, SOMAXCONN));

	FM_SERVER_TIME_VALUE.tv_sec = 0;
	FM_SERVER_TIME_VALUE.tv_usec = 0;

	LAST_FM_CLIENT_INDEX = ~0; //0xffffffff; // -1;
	CURRENT_FM_CLIENT_COUNT = 0;

	LAST_FM_MEMBER_INDEX = ~0;
	CURRENT_FM_MEMBER_COUNT = 0;

	FOR(i, TOTAL_FM_CLIENT_COUNT)
	{
		FM_MEMBER_NUMBER_A[i] = i + 1;
	}

	FM_CLIENT_SESSION_M[FM_SERVER_SESSION.Socket].Socket = FM_SERVER_SESSION.Socket;
	FM_CLIENT_SESSION_M[FM_SERVER_SESSION.Socket].Address = FM_SERVER_SESSION.Address;
	FM_CLIENT_SESSION_M[FM_SERVER_SESSION.Socket].ReceiveBQ.Resize(BUFFER_SIZE);
	FM_CLIENT_SESSION_M[FM_SERVER_SESSION.Socket].SendBQ.Resize(BUFFER_SIZE);

	return 0;
}

bool UpdateFM_Server()
{
	//AcceptPhase:
	{
		//AcceptFM_Client();
	}

	//ReceivePhase:
	{
		ReceiveFM_ClientPacket(); //combined with AcceptFM_Client() and SendFM_ServerPacket();
		ProcessReceivedFM_ClientPacket();
	}

	//SendPhase:
	{
		//ProcessSendingChatServerPacket(); //called by ProcessReceivedChatServerPacket();
		//SendFM_ServerPacket();
	}

	if (GetAsyncKeyState('T') && GetAsyncKeyState('I') && GetAsyncKeyState('U') && GetAsyncKeyState('Q')
		|| GetAsyncKeyState('T') && GetAsyncKeyState('I') && GetAsyncKeyState('X') && GetAsyncKeyState('E')) return 1;

	return 0;
}

bool RenderFM_Server()
{
	//printf("CURRENT_FM_CLIENT_COUNT : %d" CRLF, CURRENT_FM_CLIENT_COUNT);

	return 0;
}

bool TerminalizeFM_Server()
{
	closesocket(FM_SERVER_SESSION.Socket);
	WSACleanup();

	return 0;
}

int AcceptFM_Client()
{
	FD_SET readSet;

	FD_ZERO(&readSet);

	FD_SET(FM_SERVER_SESSION.Socket, &readSet);

	IF_SOCKET_ERROR(select(0, &readSet, nullptr, nullptr, &FM_SERVER_TIME_VALUE))
	ELSE
	{
		if(FD_ISSET(FM_SERVER_SESSION.Socket, &readSet))
		{
			FM_ClientSession fmClientSession;
			int addressLength;

			ZeroMemory(&fmClientSession, sizeof(fmClientSession));
			addressLength = sizeof(fmClientSession.Address);
			
			while (fmClientSession.Socket != INVALID_SOCKET)
			{
				fmClientSession.Socket = accept(FM_SERVER_SESSION.Socket, (SOCKADDR*)&fmClientSession.Address, &addressLength);
				IF_INVALID_SOCKET(fmClientSession.Socket)
				ELSE
				{
					FM_CLIENT_SESSION_M[fmClientSession.Socket] = fmClientSession;
					FM_CLIENT_SESSION_M[fmClientSession.Socket].ReceiveBQ.Resize(BUFFER_SIZE);
					FM_CLIENT_SESSION_M[fmClientSession.Socket].SendBQ.Resize(BUFFER_SIZE);

					++LAST_FM_CLIENT_INDEX;
					CURRENT_FM_CLIENT_COUNT = LAST_FM_CLIENT_INDEX + 1;
				}
			}
		}
	}

	return 0;
}

int DisconnectFM_Client()
{
	//FM_USER_NUMBER_A[x] = FM_USER_NUMBER_A[LAST_FM_CLINET_INDEX];
	//FM_USER_NUMBER_A[LAST_FM_CLIENT_INDEX] = x;

	--LAST_FM_CLIENT_INDEX;
	CURRENT_FM_CLIENT_COUNT = LAST_FM_CLIENT_INDEX + 1;

	return 0;
}

int ReceiveFM_ClientPacket()
{
	FD_SET readSet;
	FD_SET writeSet;

	//FD_ZERO(&readSet);
	//FD_ZERO(&writeSet);

	readSet.fd_count = 0;
	writeSet.fd_count = 0;

	for (FM_ClientSessionMI fmClientSessionMI = FM_CLIENT_SESSION_M.begin(); fmClientSessionMI != FM_CLIENT_SESSION_M.end();)
	{
		SOCKET fmClientSessionSocket;

		fmClientSessionSocket = fmClientSessionMI->second.Socket;

		FD_SET(fmClientSessionSocket, &readSet);
		//readSet.fd_array[readSet.fd_count++] = fmClientSessionSocket;
		
		if (FM_CLIENT_SESSION_M[fmClientSessionSocket].SendBQ.GetCurrentSize()) FD_SET(fmClientSessionSocket, &writeSet);
		//if (FM_CLIENT_SESSION_M[fmClientSessionSocket].SendBQ.GetCurrentSize()) writeSet.fd_array[writeSet.fd_count++] = fmClientSessionSocket;

		++fmClientSessionMI;

		if (readSet.fd_count == FD_SETSIZE || 0 < readSet.fd_count && fmClientSessionMI == FM_CLIENT_SESSION_M.end())
		{
			if (select(0, &readSet, &writeSet, nullptr, &FM_SERVER_TIME_VALUE) != SOCKET_ERROR)
			{
				unsigned int i;

				FOR(j, writeSet.fd_count)
				{
					FM_ClientSession* fmClientSessionAddress;
					int sendedSize;
					
					fmClientSessionAddress = &FM_CLIENT_SESSION_M[writeSet.fd_array[j]];
					
					sendedSize = fmClientSessionAddress->SendBQ.Send(send, fmClientSessionAddress->Socket);

					//sendedSize = FM_CLIENT_SESSION_M[writeSet.fd_array[j]].SendBQ.Send(send, FM_CLIENT_SESSION_M[writeSet.fd_array[j]].Socket);

					/*
					if (fmClientSessionAddress->SendBQ.GetFrontIndex() < fmClientSessionAddress->SendBQ.GetNextIndex())
					{
						sendedSize = send(fmClientSessionAddress->Socket, fmClientSessionAddress->SendBQ.GetFrontAddress(), fmClientSessionAddress->SendBQ.GetCurrentSize(), 0);
						fmClientSessionAddress->SendBQ.DecreaseCurrentSize(sendedSize);
					}
					else
					{
						int frontSize;

						frontSize = fmClientSessionAddress->SendBQ.GetMaximumSize() - fmClientSessionAddress->SendBQ.GetFrontIndex();

						sendedSize = send(fmClientSessionAddress->Socket, fmClientSessionAddress->SendBQ.GetFrontAddress(), frontSize, 0);
						fmClientSessionAddress->SendBQ.DecreaseCurrentSize(sendedSize);

						if (fmClientSessionAddress->SendBQ.GetCurrentSize()) --j;
					}
					*/

					//if (sendedSize == SOCKET_ERROR) //if(sendedSize == SOCKET_ERROR || sendedSize == 0)
					//{
					//	if (WSAGetLastError() == WSAEWOULDBLOCK);
					//	else
					//	{
					//		closesocket(fmClientSessionAddress->Socket);
					//	
					//		FM_CLIENT_SESSION_M.erase(fmClientSessionAddress->Socket);
					//	
					//		--LAST_FM_CLIENT_INDEX;
					//		CURRENT_FM_CLIENT_COUNT = LAST_FM_CLIENT_INDEX + 1;
					//	}
					//}
				}
				
				if (!FD_ISSET(FM_SERVER_SESSION.Socket, &readSet)) i = 0;
				else
				{
					FM_ClientSession fmClientSession;
					int addressLength;

					ZeroMemory(&fmClientSession, sizeof(fmClientSession));
					addressLength = sizeof(fmClientSession.Address);

					while (fmClientSession.Socket != INVALID_SOCKET)
					{
						fmClientSession.Socket = accept(FM_SERVER_SESSION.Socket, (SOCKADDR*)&fmClientSession.Address, &addressLength);
						IF_INVALID_SOCKET(fmClientSession.Socket)
						ELSE
						{
							FM_CLIENT_SESSION_M[fmClientSession.Socket] = fmClientSession;
							FM_CLIENT_SESSION_M[fmClientSession.Socket].ReceiveBQ.Resize(BUFFER_SIZE);
							FM_CLIENT_SESSION_M[fmClientSession.Socket].SendBQ.Resize(BUFFER_SIZE);

							++LAST_FM_CLIENT_INDEX;
							CURRENT_FM_CLIENT_COUNT = LAST_FM_CLIENT_INDEX + 1;
						}
					}

					i = 1;
				}

				for(; i < readSet.fd_count; ++i)
				{
					FM_ClientSession* fmClientSessionAddress;
					int receivedSize;

					fmClientSessionAddress = &FM_CLIENT_SESSION_M[readSet.fd_array[i]];

					receivedSize = fmClientSessionAddress->ReceiveBQ.Receive(recv, fmClientSessionAddress->Socket);

					//receivedSize = FM_CLIENT_SESSION_M[readSet.fd_array[i]].ReceiveBQ.Receive(recv, FM_CLIENT_SESSION_M[readSet.fd_array[i]].Socket);

					/*
					if (fmClientSessionAddress->ReceiveBQ.GetNextIndex() < fmClientSessionAddress->ReceiveBQ.GetFrontIndex())
					{
						receivedSize = recv(fmClientSessionAddress->Socket, fmClientSessionAddress->ReceiveBQ.GetNextAddress(), fmClientSessionAddress->ReceiveBQ.GetLeftSize(), 0);
						fmClientSessionAddress->ReceiveBQ.IncreaseCurrentSize(receivedSize);
					}
					else
					{
						int rearSpace;

						rearSpace = fmClientSessionAddress->ReceiveBQ.GetMaximumSize() - fmClientSessionAddress->ReceiveBQ.GetNextIndex();

						receivedSize = recv(fmClientSessionAddress->Socket, fmClientSessionAddress->ReceiveBQ.GetNextAddress(), rearSpace, 0);
						fmClientSessionAddress->ReceiveBQ.IncreaseCurrentSize(receivedSize);

						if (rearSpace == receivedSize) --i;
					}
					*/
					
					if (receivedSize == SOCKET_ERROR || receivedSize == 0)
					{
						if (WSAGetLastError() == WSAEWOULDBLOCK);
						else
						{
							closesocket(fmClientSessionAddress->Socket);

							FM_CLIENT_SESSION_M.erase(fmClientSessionAddress->Socket);

							--LAST_FM_CLIENT_INDEX;
							CURRENT_FM_CLIENT_COUNT = LAST_FM_CLIENT_INDEX + 1;
						}
					}
				}
			}

			//FD_ZERO(&readSet);
			//FD_ZERO(&writeSet);

			readSet.fd_count = 0;
			writeSet.fd_count = 0;
		}
	}

	return 0;
}

int SendFM_ServerPacket()
{
	FD_SET readSet;
	FD_SET writeSet;

	FD_ZERO(&writeSet);

	for (FM_ClientSessionMI fmClientSessionMI = FM_CLIENT_SESSION_M.begin(); fmClientSessionMI != FM_CLIENT_SESSION_M.end();)
	{
		SOCKET fmClientSessionSocket;

		fmClientSessionSocket = fmClientSessionMI->second.Socket;

		if (FM_CLIENT_SESSION_M[fmClientSessionSocket].SendBQ.GetCurrentSize()) FD_SET(fmClientSessionSocket, &writeSet);

		++fmClientSessionMI;

		if (writeSet.fd_count == FD_SETSIZE || 0 < writeSet.fd_count && fmClientSessionMI == FM_CLIENT_SESSION_M.end())
		{
			IF_SOCKET_ERROR(select(0, nullptr, &writeSet, nullptr, &FM_SERVER_TIME_VALUE))
			ELSE
			{
				FOR(i, writeSet.fd_count)
				{
					FM_ClientSession* fmClientSessionAddress;
					int sendedSize;

					fmClientSessionAddress = &FM_CLIENT_SESSION_M[writeSet.fd_array[i]];

					sendedSize = fmClientSessionAddress->SendBQ.Send(send, fmClientSessionAddress->Socket);

					/*
					if (fmClientSessionAddress->SendBQ.GetFrontIndex() < fmClientSessionAddress->SendBQ.GetNextIndex())
					{
						sendedSize = send(fmClientSessionAddress->Socket, fmClientSessionAddress->SendBQ.GetFrontAddress(), fmClientSessionAddress->SendBQ.GetCurrentSize(), 0);
						fmClientSessionAddress->SendBQ.DecreaseCurrentSize(sendedSize);
					}
					else
					{
						int frontSize;

						frontSize = fmClientSessionAddress->SendBQ.GetMaximumSize() - fmClientSessionAddress->SendBQ.GetFrontIndex();

						sendedSize = send(fmClientSessionAddress->Socket, fmClientSessionAddress->SendBQ.GetFrontAddress(), frontSize, 0);
						fmClientSessionAddress->SendBQ.DecreaseCurrentSize(sendedSize);

						if (fmClientSessionAddress->SendBQ.GetCurrentSize()) --i;
					}
					*/

					if (sendedSize == SOCKET_ERROR) //if(sendedSize == SOCKET_ERROR || sendedSize == 0)
					{
						if (WSAGetLastError() == WSAEWOULDBLOCK);
						else
						{
							closesocket(fmClientSessionAddress->Socket);

							FM_CLIENT_SESSION_M.erase(fmClientSessionAddress->Socket);

							--LAST_FM_CLIENT_INDEX;
							CURRENT_FM_CLIENT_COUNT = LAST_FM_CLIENT_INDEX + 1;
						}
					}

					//keepalive or heardbeat check
				}
			}

			FD_ZERO(&writeSet);
		}
	}

	return 0;
}

int ProcessReceivedFM_ClientPacket()
{
	for (FM_ClientSessionMI fmClientSessionMI = FM_CLIENT_SESSION_M.begin(); fmClientSessionMI != FM_CLIENT_SESSION_M.end(); ++fmClientSessionMI)
	{
		FM_PacketHeader receivedPacketHeader;
		SerialByteQueue receivedPacketSBQ(sizeof(receivedPacketHeader), BUFFER_SIZE);

		if (sizeof(receivedPacketHeader) == fmClientSessionMI->second.ReceiveBQ.Peek((char*)&receivedPacketHeader, sizeof(receivedPacketHeader)))
		{
			if (sizeof(receivedPacketHeader) + receivedPacketHeader.PacketBodySize == fmClientSessionMI->second.ReceiveBQ.Peek(receivedPacketSBQ.GetQueueAddress(), sizeof(receivedPacketHeader) + receivedPacketHeader.PacketBodySize))
			{
				receivedPacketSBQ.IncreaseCurrentSize(receivedPacketHeader.PacketBodySize);

				if (CheckReceivedFM_ClientPacket(receivedPacketHeader, receivedPacketSBQ))
				{
					fmClientSessionMI->second.ReceiveBQ.DecreaseCurrentSize(sizeof(receivedPacketHeader) + receivedPacketHeader.PacketBodySize);

					switch (receivedPacketHeader.PacketType)
					{
					case FM_REQUEST_JOIN:
						{
							if (receivedPacketHeader.PacketBodySize < FM_MEMBER_NAME_SIZE && !(receivedPacketHeader.PacketBodySize % 2))
							{
								ProcessSendingFM_ServerPacket(FM_RESPONSE_JOIN, fmClientSessionMI, receivedPacketSBQ);
							}
						}
						break;
					case FM_REQUEST_LOGIN:
						{
							if (receivedPacketHeader.PacketBodySize == sizeof(UINT64))
							{
								ProcessSendingFM_ServerPacket(FM_RESPONSE_LOGIN, fmClientSessionMI, receivedPacketSBQ);
							}
						}
						break;

					case FM_REQUEST_MEMBER_LIST:
						{
							if (receivedPacketHeader.PacketBodySize == 0)
							{
								ProcessSendingFM_ServerPacket(FM_RESPONSE_MEMBER_LIST, fmClientSessionMI, receivedPacketSBQ);
							}
						}
						break;
					case FM_REQUEST_FRIEND_LIST:
						{
							if (receivedPacketHeader.PacketBodySize == 0)
							{
								ProcessSendingFM_ServerPacket(FM_RESPONSE_FRIEND_LIST, fmClientSessionMI, receivedPacketSBQ);
							}
						}
						break;
					case FM_REQUEST_FRIEND_OFFEREE_LIST:
						{
							if (receivedPacketHeader.PacketBodySize == 0)
							{
								ProcessSendingFM_ServerPacket(FM_RESPONSE_FRIEND_OFFEREE_LIST, fmClientSessionMI, receivedPacketSBQ);
							}
						}
						break;
					case FM_REQUEST_FRIEND_OFFERER_LIST:
						{
							if (receivedPacketHeader.PacketBodySize == 0)
							{
								ProcessSendingFM_ServerPacket(FM_RESPONSE_FRIEND_OFFERER_LIST, fmClientSessionMI, receivedPacketSBQ);
							}
						}
						break;
					case FM_REQUEST_FRIEND_BREACH:
						{
							if (receivedPacketHeader.PacketBodySize == sizeof(UINT64))
							{
								ProcessSendingFM_ServerPacket(FM_RESPONSE_FRIEND_BREACH, fmClientSessionMI, receivedPacketSBQ);
							}
						}
						break;
					case FM_REQUEST_FRIEND_OFFER:
						{
							if (receivedPacketHeader.PacketBodySize == sizeof(UINT64))
							{
								ProcessSendingFM_ServerPacket(FM_RESPONSE_FRIEND_OFFER, fmClientSessionMI, receivedPacketSBQ);
							}
						}
						break;
					case FM_REQUEST_FRIEND_OFFER_CANCEL:
						{
							if (receivedPacketHeader.PacketBodySize == sizeof(UINT64))
							{
								ProcessSendingFM_ServerPacket(FM_RESPONSE_FRIEND_OFFER_CANCEL, fmClientSessionMI, receivedPacketSBQ);
							}
						}
						break;
					case FM_REQUEST_FRIEND_DENY:
						{
							if (receivedPacketHeader.PacketBodySize == sizeof(UINT64))
							{
								ProcessSendingFM_ServerPacket(FM_RESPONSE_FRIEND_DENY, fmClientSessionMI, receivedPacketSBQ);
							}
						}
						break;
					case FM_REQUEST_FRIEND_AGREE:
						{
							if (receivedPacketHeader.PacketBodySize == sizeof(UINT64))
							{
								ProcessSendingFM_ServerPacket(FM_RESPONSE_FRIEND_AGREE, fmClientSessionMI, receivedPacketSBQ);
							}
						}
						break;
					case FM_REQUEST_STRESS_ECHO:
						{
							if (receivedPacketHeader.PacketBodySize)
							{
								ProcessSendingFM_ServerPacket(FM_RESPONSE_STRESS_ECHO, fmClientSessionMI, receivedPacketSBQ);
							}
						}
						break;
					default:
						break;
					}
				}
			}
		}
	}

	return 0;
}

int ProcessSendingFM_ServerPacket(FM_RESPONSE_INDEX fmResponseIndex, FM_ClientSessionMI fmClientSessionMI, SerialByteQueue& receivedPacketSBQ)
{
	FM_PacketHeader sendingPacketHeader;
	SerialByteQueue sendingPacketSBQ(sizeof(sendingPacketHeader), BUFFER_SIZE);

	ZeroMemory(&sendingPacketHeader, sizeof(sendingPacketHeader));

	sendingPacketHeader.PacketType = fmResponseIndex;

	switch (sendingPacketHeader.PacketType)
	{
	case FM_RESPONSE_JOIN:
		{
			UINT64 memberNumber;
			WCHAR* memberName;

			if (CURRENT_FM_CLIENT_COUNT < TOTAL_FM_CLIENT_COUNT)
			{
				++LAST_FM_MEMBER_INDEX;
				CURRENT_FM_MEMBER_COUNT = LAST_FM_MEMBER_INDEX + 1;

				memberNumber = FM_MEMBER_NUMBER_A[LAST_FM_MEMBER_INDEX];
				memberName = FM_MEMBER_NAME_M[memberNumber];

				FOR(i, FM_MEMBER_NAME_LENGTH - 12) //20
				{
					receivedPacketSBQ >> memberName[i];

					memberName[i + 1] = '\0\0'; //foolish

					if (memberName[i] == '\0\0') break;
				}

				FM_OFFEREE_MEMBER_NUMBER_LM[memberNumber];
				FM_OFFERER_MEMBER_NUMBER_LM[memberNumber];
				FM_FRIEND_MEMBER_NUMBER_LM[memberNumber];

				sendingPacketSBQ << memberNumber;
			}
			else
			{
				memberNumber = 0;

				sendingPacketSBQ << memberNumber;
			}
		}
		break;
	case FM_RESPONSE_LOGIN:
		{
			UINT64 memberNumber;
			WCHAR* memberName;

			receivedPacketSBQ >> memberNumber;

			if (FM_MEMBER_NAME_M.find(memberNumber) != FM_MEMBER_NAME_M.end())
			{
				memberName = FM_MEMBER_NAME_M[memberNumber];

				fmClientSessionMI->second.MemberNumber = memberNumber;

				sendingPacketSBQ << memberNumber;

				FOR(i, FM_MEMBER_NAME_LENGTH - 12) //20
				{
					sendingPacketSBQ << memberName[i];
					
					//if (memberName[i] == '\0\0') break;
				}
			}
			else sendingPacketSBQ << (UINT64)0;
		}
		break;
	case FM_RESPONSE_MEMBER_LIST:
		{
			sendingPacketSBQ << (UINT)FM_MEMBER_NAME_M.size();

			for (FM_MemberNameMI memberNameMI = FM_MEMBER_NAME_M.begin(); memberNameMI != FM_MEMBER_NAME_M.end(); ++memberNameMI)
			{
				sendingPacketSBQ << memberNameMI->first;

				FOR(i, FM_MEMBER_NAME_LENGTH - 12) //20
				{
					sendingPacketSBQ << memberNameMI->second[i];
					
					//if (memberNameMI->second[i] == '\0\0') break;
				}
			}
		}
		break;
	case FM_RESPONSE_FRIEND_LIST:
		{
			UINT64 memberNumber;
			WCHAR* memberName;
			FM_MemberNumberL* fmMemberNumberL;
			
			memberNumber = fmClientSessionMI->second.MemberNumber;

			if (FM_FRIEND_MEMBER_NUMBER_LM.find(memberNumber) != FM_FRIEND_MEMBER_NUMBER_LM.end())
			{
				fmMemberNumberL = &FM_FRIEND_MEMBER_NUMBER_LM[memberNumber];

				sendingPacketSBQ << (UINT)fmMemberNumberL->size();

				for (FM_MemberNumberLI memberNumberLI = fmMemberNumberL->begin(); memberNumberLI != fmMemberNumberL->end(); ++memberNumberLI)
				{
					memberNumber = (*memberNumberLI);

					if (FM_MEMBER_NAME_M.find(memberNumber) != FM_MEMBER_NAME_M.end())
					{
						memberName = FM_MEMBER_NAME_M[memberNumber];

						sendingPacketSBQ << memberNumber;

						FOR(i, FM_MEMBER_NAME_LENGTH - 12) //20
						{
							sendingPacketSBQ << memberName[i];

							//if (memberName[i] == '\0\0') break;
						}
					}
					else sendingPacketSBQ << (WCHAR)'\0\0';
				}

			}
			else sendingPacketSBQ << (UINT64)0;
		}
		break;
	case FM_RESPONSE_FRIEND_OFFEREE_LIST:
		{
			UINT64 memberNumber;
			WCHAR* memberName;
			FM_MemberNumberL* fmMemberNumberL;

			memberNumber = fmClientSessionMI->second.MemberNumber;

			if (FM_OFFEREE_MEMBER_NUMBER_LM.find(memberNumber) != FM_OFFEREE_MEMBER_NUMBER_LM.end())
			{
				fmMemberNumberL = &FM_OFFEREE_MEMBER_NUMBER_LM[memberNumber];

				sendingPacketSBQ << (UINT)fmMemberNumberL->size();

				for (FM_MemberNumberLI memberNumberLI = fmMemberNumberL->begin(); memberNumberLI != fmMemberNumberL->end(); ++memberNumberLI)
				{
					memberNumber = (*memberNumberLI);

					if (FM_MEMBER_NAME_M.find(memberNumber) != FM_MEMBER_NAME_M.end())
					{
						memberName = FM_MEMBER_NAME_M[memberNumber];

						sendingPacketSBQ << memberNumber;

						FOR(i, FM_MEMBER_NAME_LENGTH - 12) //20
						{
							sendingPacketSBQ << memberName[i];

							//if (memberName[i] == '\0\0') break;
						}
					}
					else sendingPacketSBQ << (WCHAR)'\0\0';
				}
			}
			else sendingPacketSBQ << (UINT64)0;
		}
		break;
	case FM_RESPONSE_FRIEND_OFFERER_LIST:
		{
			UINT64 memberNumber;
			WCHAR* memberName;
			FM_MemberNumberL* fmMemberNumberL;

			memberNumber = fmClientSessionMI->second.MemberNumber;

			if (FM_OFFERER_MEMBER_NUMBER_LM.find(memberNumber) != FM_OFFERER_MEMBER_NUMBER_LM.end())
			{
				fmMemberNumberL = &FM_OFFERER_MEMBER_NUMBER_LM[memberNumber];

				sendingPacketSBQ << (UINT)fmMemberNumberL->size();

				for (FM_MemberNumberLI memberNumberLI = fmMemberNumberL->begin(); memberNumberLI != fmMemberNumberL->end(); ++memberNumberLI)
				{
					memberNumber = (*memberNumberLI);

					if (FM_MEMBER_NAME_M.find(memberNumber) != FM_MEMBER_NAME_M.end())
					{
						memberName = FM_MEMBER_NAME_M[memberNumber];

						sendingPacketSBQ << memberNumber;

						FOR(i, FM_MEMBER_NAME_LENGTH - 12) //20
						{
							sendingPacketSBQ << memberName[i];

							//if (memberName[i] == '\0\0') break;
						}
					}
					else sendingPacketSBQ << (WCHAR)'\0\0';
				}
			}
			else sendingPacketSBQ << (UINT64)0;
		}
		break;
	case FM_RESPONSE_FRIEND_BREACH:
		{
			UINT64 memberNumber;
			UINT64 breachedFriendNumber;
			FM_MemberNumberL* fmMemberNumberL;
			BYTE result;

			memberNumber = fmClientSessionMI->second.MemberNumber;
			
			receivedPacketSBQ >> breachedFriendNumber;
			sendingPacketSBQ << breachedFriendNumber;

			result = FM_RESULT_NOT_EXIST;

			if (FM_MEMBER_NAME_M.find(breachedFriendNumber) != FM_MEMBER_NAME_M.end())
			{
				if (FM_FRIEND_MEMBER_NUMBER_LM.find(memberNumber) != FM_FRIEND_MEMBER_NUMBER_LM.end())
				{
					fmMemberNumberL = &FM_FRIEND_MEMBER_NUMBER_LM[memberNumber];

					for (FM_MemberNumberLI memberNumberLI = fmMemberNumberL->begin(); memberNumberLI != fmMemberNumberL->end(); ++memberNumberLI)
					{
						if ((*memberNumberLI) == breachedFriendNumber)
						{
							result = FM_RESULT_SUCCESS;

							fmMemberNumberL->erase(memberNumberLI);

							break;
						}
					}
				}
				else result = FM_RESULT_FAILURE;

				if (FM_FRIEND_MEMBER_NUMBER_LM.find(breachedFriendNumber) != FM_FRIEND_MEMBER_NUMBER_LM.end())
				{
					fmMemberNumberL = &FM_FRIEND_MEMBER_NUMBER_LM[breachedFriendNumber];

					for (FM_MemberNumberLI memberNumberLI = fmMemberNumberL->begin(); memberNumberLI != fmMemberNumberL->end(); ++memberNumberLI)
					{
						if ((*memberNumberLI) == memberNumber)
						{
							result = FM_RESULT_SUCCESS;

							fmMemberNumberL->erase(memberNumberLI);

							break;
						}
					}
				}
				else result = FM_RESULT_FAILURE;
			}
			else result = FM_RESULT_FAILURE;

			sendingPacketSBQ << result; //bug
		}
		break;
	case FM_RESPONSE_FRIEND_OFFER:
		{
			UINT64 memberNumber;
			UINT64 offeringFriendNumber;
			FM_MemberNumberL* fmMemberNumberL;
			BYTE result;

			memberNumber = fmClientSessionMI->second.MemberNumber;

			receivedPacketSBQ >> offeringFriendNumber;
			sendingPacketSBQ << offeringFriendNumber;

			result = FM_RESULT_SUCCESS;

			if (FM_MEMBER_NAME_M.find(offeringFriendNumber) != FM_MEMBER_NAME_M.end())
			{
				if (FM_OFFEREE_MEMBER_NUMBER_LM.find(memberNumber) != FM_OFFEREE_MEMBER_NUMBER_LM.end())
				{
					fmMemberNumberL = &FM_OFFEREE_MEMBER_NUMBER_LM[memberNumber];

					for (FM_MemberNumberLI memberNumberLI = fmMemberNumberL->begin(); memberNumberLI != fmMemberNumberL->end(); ++memberNumberLI)
					{
						if ((*memberNumberLI) == offeringFriendNumber)
						{
							result = FM_RESULT_FAILURE;

							break;
						}
					}

					if(result == FM_RESULT_SUCCESS) fmMemberNumberL->push_back(offeringFriendNumber);
				}
				else result = FM_RESULT_FAILURE;

				if (FM_OFFERER_MEMBER_NUMBER_LM.find(offeringFriendNumber) != FM_OFFEREE_MEMBER_NUMBER_LM.end())
				{
					fmMemberNumberL = &FM_OFFERER_MEMBER_NUMBER_LM[offeringFriendNumber];

					for (FM_MemberNumberLI memberNumberLI = fmMemberNumberL->begin(); memberNumberLI != fmMemberNumberL->end(); ++memberNumberLI)
					{
						if ((*memberNumberLI) == memberNumber)
						{
							result = FM_RESULT_FAILURE;

							break;
						}
					}

					if(result == FM_RESULT_SUCCESS) fmMemberNumberL->push_back(memberNumber);
				}
				else result = FM_RESULT_FAILURE;
			}
			else result = FM_RESULT_NOT_EXIST;
			
			sendingPacketSBQ << result; //bug
		}
		break;
	case FM_RESPONSE_FRIEND_OFFER_CANCEL:
		{
			UINT64 memberNumber;
			UINT64 cancelingFriendNumber;
			FM_MemberNumberL* fmMemberNumberL;
			BYTE result;

			memberNumber = fmClientSessionMI->second.MemberNumber;

			receivedPacketSBQ >> cancelingFriendNumber;
			sendingPacketSBQ << cancelingFriendNumber;

			result = FM_RESULT_NOT_EXIST;

			if (FM_MEMBER_NAME_M.find(cancelingFriendNumber) != FM_MEMBER_NAME_M.end())
			{
				if (FM_OFFEREE_MEMBER_NUMBER_LM.find(memberNumber) != FM_OFFEREE_MEMBER_NUMBER_LM.end())
				{
					fmMemberNumberL = &FM_OFFEREE_MEMBER_NUMBER_LM[memberNumber];

					for (FM_MemberNumberLI memberNumberLI = fmMemberNumberL->begin(); memberNumberLI != fmMemberNumberL->end(); ++memberNumberLI)
					{
						if ((*memberNumberLI) == cancelingFriendNumber)
						{
							result = FM_RESULT_SUCCESS;

							fmMemberNumberL->erase(memberNumberLI);

							break;
						}
					}
				}
				else result = FM_RESULT_FAILURE;

				if (FM_OFFERER_MEMBER_NUMBER_LM.find(cancelingFriendNumber) != FM_OFFERER_MEMBER_NUMBER_LM.end())
				{
					fmMemberNumberL = &FM_OFFERER_MEMBER_NUMBER_LM[cancelingFriendNumber];

					for (FM_MemberNumberLI memberNumberLI = fmMemberNumberL->begin(); memberNumberLI != fmMemberNumberL->end(); ++memberNumberLI)
					{
						if ((*memberNumberLI) == memberNumber)
						{
							result = FM_RESULT_SUCCESS;

							fmMemberNumberL->erase(memberNumberLI);

							break;
						}
					}
				}
				else result = FM_RESULT_FAILURE;
			}
			else result = FM_RESULT_FAILURE;

			sendingPacketSBQ << result; //bug
		}
		break;
	case FM_RESPONSE_FRIEND_DENY:
		{
			UINT64 memberNumber;
			UINT64 denyingFriendNumber;
			FM_MemberNumberL* fmMemberNumberL;
			BYTE result;

			memberNumber = fmClientSessionMI->second.MemberNumber;

			receivedPacketSBQ >> denyingFriendNumber;
			sendingPacketSBQ << denyingFriendNumber;

			result = FM_RESULT_NOT_EXIST;

			if (FM_MEMBER_NAME_M.find(denyingFriendNumber) != FM_MEMBER_NAME_M.end())
			{
				if (FM_OFFERER_MEMBER_NUMBER_LM.find(memberNumber) != FM_OFFERER_MEMBER_NUMBER_LM.end())
				{
					fmMemberNumberL = &FM_OFFERER_MEMBER_NUMBER_LM[memberNumber];

					for (FM_MemberNumberLI memberNumberLI = fmMemberNumberL->begin(); memberNumberLI != fmMemberNumberL->end(); ++memberNumberLI)
					{
						if ((*memberNumberLI) == denyingFriendNumber)
						{
							result = FM_RESULT_SUCCESS;

							fmMemberNumberL->erase(memberNumberLI);

							break;
						}
					}
				}
				else result = FM_RESULT_FAILURE;

				if (FM_OFFEREE_MEMBER_NUMBER_LM.find(denyingFriendNumber) != FM_OFFEREE_MEMBER_NUMBER_LM.end())
				{
					fmMemberNumberL = &FM_OFFEREE_MEMBER_NUMBER_LM[denyingFriendNumber];

					for (FM_MemberNumberLI memberNumberLI = fmMemberNumberL->begin(); memberNumberLI != fmMemberNumberL->end(); ++memberNumberLI)
					{
						if ((*memberNumberLI) == memberNumber)
						{
							result = FM_RESULT_SUCCESS;

							fmMemberNumberL->erase(memberNumberLI);

							break;
						}
					}
				}
				else result = FM_RESULT_FAILURE;
			}
			else result = FM_RESULT_FAILURE;

			sendingPacketSBQ << result; //bug
		}
		break;
	case FM_RESPONSE_FRIEND_AGREE:
		{
			UINT64 memberNumber;
			UINT64 agreeingFriendNumber;
			FM_MemberNumberL* fmMemberNumberL;
			BYTE result;

			memberNumber = fmClientSessionMI->second.MemberNumber;

			receivedPacketSBQ >> agreeingFriendNumber;
			sendingPacketSBQ << agreeingFriendNumber;

			if (FM_MEMBER_NAME_M.find(agreeingFriendNumber) != FM_MEMBER_NAME_M.end())
			{
				result = FM_RESULT_NOT_EXIST;

				if (FM_OFFERER_MEMBER_NUMBER_LM.find(memberNumber) != FM_OFFERER_MEMBER_NUMBER_LM.end())
				{
					fmMemberNumberL = &FM_OFFERER_MEMBER_NUMBER_LM[memberNumber];

					for (FM_MemberNumberLI memberNumberLI = fmMemberNumberL->begin(); memberNumberLI != fmMemberNumberL->end(); ++memberNumberLI)
					{
						if ((*memberNumberLI) == agreeingFriendNumber)
						{
							result = FM_RESULT_SUCCESS;

							fmMemberNumberL->erase(memberNumberLI);

							break;
						}
					}
				}
				else result = FM_RESULT_FAILURE;

				if (FM_OFFEREE_MEMBER_NUMBER_LM.find(agreeingFriendNumber) != FM_OFFEREE_MEMBER_NUMBER_LM.end())
				{
					fmMemberNumberL = &FM_OFFEREE_MEMBER_NUMBER_LM[agreeingFriendNumber];

					for (FM_MemberNumberLI memberNumberLI = fmMemberNumberL->begin(); memberNumberLI != fmMemberNumberL->end(); ++memberNumberLI)
					{
						if ((*memberNumberLI) == memberNumber)
						{
							result = FM_RESULT_SUCCESS;

							fmMemberNumberL->erase(memberNumberLI);

							break;
						}
					}
				}
				else result = FM_RESULT_FAILURE;

				result = FM_RESULT_SUCCESS;

				if (FM_FRIEND_MEMBER_NUMBER_LM.find(memberNumber) != FM_FRIEND_MEMBER_NUMBER_LM.end())
				{
					fmMemberNumberL = &FM_FRIEND_MEMBER_NUMBER_LM[memberNumber];

					for (FM_MemberNumberLI memberNumberLI = fmMemberNumberL->begin(); memberNumberLI != fmMemberNumberL->end(); ++memberNumberLI)
					{
						if ((*memberNumberLI) == agreeingFriendNumber)
						{
							result = FM_RESULT_FAILURE;

							break;
						}
					}

					if (result == FM_RESULT_SUCCESS) fmMemberNumberL->push_back(agreeingFriendNumber);
				}
				else result = FM_RESULT_NOT_EXIST;

				if (FM_FRIEND_MEMBER_NUMBER_LM.find(agreeingFriendNumber) != FM_FRIEND_MEMBER_NUMBER_LM.end())
				{
					fmMemberNumberL = &FM_FRIEND_MEMBER_NUMBER_LM[agreeingFriendNumber];

					for (FM_MemberNumberLI memberNumberLI = fmMemberNumberL->begin(); memberNumberLI != fmMemberNumberL->end(); ++memberNumberLI)
					{
						if ((*memberNumberLI) == memberNumber)
						{
							result = FM_RESULT_FAILURE;

							break;
						}
					}

					if(result == FM_RESULT_SUCCESS) fmMemberNumberL->push_back(memberNumber);
				}
				else result = FM_RESULT_NOT_EXIST;
			}
			else result = FM_RESULT_NOT_EXIST;

			sendingPacketSBQ << result; //bug
		}
		break;
	case FM_RESPONSE_STRESS_ECHO:
		{
			WORD size;

			receivedPacketSBQ >> size;
			sendingPacketSBQ << size;

			memcpy(sendingPacketSBQ.GetFrontAddress() + sizeof(size), receivedPacketSBQ.GetFrontAddress(), size); //make enqueue and dequeue function
			sendingPacketSBQ.IncreaseCurrentSize(size); //make increase and decrease function
		}
		break;
	default:
		break;
	}

	if (CheckSendingFM_ServerPacket(sendingPacketHeader, sendingPacketSBQ)) fmClientSessionMI->second.SendBQ.Enqueue(sendingPacketSBQ.GetQueueAddress(), sendingPacketSBQ.GetTotalSize());

	return 0;
}

bool CheckReceivedFM_ClientPacket(FM_PacketHeader& receivedPacketHeader, SerialByteQueue& receivedPacketSBQ)
{
	if (receivedPacketHeader.IdentificationCode == FM_PACKET_HEADER_IDENTIFICATION_CODE && receivedPacketHeader.PacketBodySize == receivedPacketSBQ.GetCurrentSize()) return 1;

	return 0;
}

bool CheckSendingFM_ServerPacket(FM_PacketHeader& sendingPacketHeader, SerialByteQueue& sendingPacketSBQ)
{
	sendingPacketHeader.IdentificationCode = FM_PACKET_HEADER_IDENTIFICATION_CODE;

	sendingPacketHeader.PacketBodySize = sendingPacketSBQ.GetCurrentSize();

	sendingPacketSBQ.SetHeader(sendingPacketHeader);

	return 1;
}