#include "stdafx.h"
#include "Protocol.h"
#include "FriendManagementClient.h"

bool InitializeFM_Client()
{
	WSADATA wsaDatum;
	u_long nonblockOn;
	//LINGER lingeringTime;
	//BOOL nagleOn;
	//BOOL reuseOn;
	WCHAR ipAddress[32];

	FM_SERVER_SESSION_M.reserve(TOTAL_FM_CLIENT_COUNT);

	CHECK_ERROR(WSAStartup(0x0202, &wsaDatum));

	wscanf_s(L"%s", &ipAddress);

	FOR(i, TOTAL_FM_CLIENT_COUNT)
	{
		SOCKET currentClientSocket;
		SOCKADDR_IN currentClientAddress;

		currentClientSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
		CHECK_INVALID_SOCKET(currentClientSocket);

		



		nonblockOn = 1;
		CHECK_SOCKET_ERROR(ioctlsocket(currentClientSocket, FIONBIO, &nonblockOn));

		//lingeringTime.l_onoff = 1;
		//lingeringTime.l_linger = 0;
		//setsockopt(currentClientSocket, SOL_SOCKET, SO_LINGER, (char*)&lingeringTime, sizeof(lingeringTime));

		//nagleOn = 1;
		//setsockopt(currentClientSocket, IPPROTO_TCP, TCP_NODELAY, (char*)&nagleOn, sizeof(nagleOn));

		//reuseOn = 1;
		//setsockopt(currentClientSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&reuseOn, sizeof(reuseOn));



		currentClientAddress.sin_family = AF_INET;
		InetPton(AF_INET, ipAddress, &currentClientAddress.sin_addr); //inet_addr(INADDR_ANY); //InetNtop();
		//currentClientAddress.sin_addr.s_addr = htonl(INADDR_ANY); //WSAHtonl();
		currentClientAddress.sin_port = htons(FM_SERVER_PORT);

		FM_SERVER_SESSION_M[currentClientSocket].Socket = currentClientSocket;
		FM_SERVER_SESSION_M[currentClientSocket].Address = currentClientAddress;
	}


	FM_CLIENT_TIME_VALUE.tv_sec = 0;
	FM_CLIENT_TIME_VALUE.tv_usec = 0;

	LAST_FM_SERVER_SESSION_INDEX = ~0;
	CURRENT_FM_SERVER_SESSION_COUNT = 0;

	//ConnectPhase:
	{
		ConnectFM_Client();
	}
	
	for (FM_ServerSessionMI fmClientSessionMI = FM_SERVER_SESSION_M.begin(); fmClientSessionMI != FM_SERVER_SESSION_M.end(); ++fmClientSessionMI)
	{
		SerialByteQueue receivedPacketSBQ;

		ProcessSendingFM_ClientPacket(FM_REQUEST_STRESS_ECHO, fmClientSessionMI, receivedPacketSBQ);
	}

	return 0;
}

bool UpdateFM_Client()
{
	//ReceivePhase:
	{
		ReceiveFM_ServerPacket();
		ProcessReceivedFM_ServerPacket();
	}

	
	{
		//ProcessSendingFM_ServerPacket(FM_RESPONSE_JOIN, fmClientSessionMI, receivedPacketSBQ);
		//ProcessSendingFM_ServerPacket(FM_RESPONSE_LOGIN, fmClientSessionMI, receivedPacketSBQ);
		//ProcessSendingFM_ServerPacket(FM_RESPONSE_MEMBER_LIST, fmClientSessionMI, receivedPacketSBQ);
		//ProcessSendingFM_ServerPacket(FM_RESPONSE_FRIEND_LIST, fmClientSessionMI, receivedPacketSBQ);
		//ProcessSendingFM_ServerPacket(FM_RESPONSE_FRIEND_OFFEREE_LIST, fmClientSessionMI, receivedPacketSBQ);
		//ProcessSendingFM_ServerPacket(FM_RESPONSE_FRIEND_OFFERER_LIST, fmClientSessionMI, receivedPacketSBQ);
		//ProcessSendingFM_ServerPacket(FM_RESPONSE_FRIEND_BREACH, fmClientSessionMI, receivedPacketSBQ);
		//ProcessSendingFM_ServerPacket(FM_RESPONSE_FRIEND_OFFER, fmClientSessionMI, receivedPacketSBQ);
		//ProcessSendingFM_ServerPacket(FM_RESPONSE_FRIEND_OFFER_CANCEL, fmClientSessionMI, receivedPacketSBQ);
		//ProcessSendingFM_ServerPacket(FM_RESPONSE_FRIEND_DENY, fmClientSessionMI, receivedPacketSBQ);
		//ProcessSendingFM_ServerPacket(FM_RESPONSE_FRIEND_AGREE, fmClientSessionMI, receivedPacketSBQ);
		//ProcessSendingFM_ServerPacket(FM_RESPONSE_STRESS_ECHO, fmClientSessionMI, receivedPacketSBQ);
	}
	
	{
		//ProcessSendingFM_ClientPacket();
	}

	//SendPhase:
	{
		
		SendFM_ClientPacket();
	}

	return 0;
}

bool RenderFM_Client()
{
	printf("CURRENT_FM_CLINET_COUNT : %d", CURRENT_FM_SERVER_SESSION_COUNT);

	return 0;
}

bool TerminalizeFM_Client()
{
	for (FM_ServerSessionMI fmServerSessionMI = FM_SERVER_SESSION_M.begin(); fmServerSessionMI != FM_SERVER_SESSION_M.end(); ++fmServerSessionMI)
	{
		closesocket(fmServerSessionMI->second.Socket);
	}

	WSACleanup();

	return 0;
}

int ConnectFM_Client()
{
	FD_SET writeSet;
	FD_SET exceptionSet;

	FD_ZERO(&writeSet);
	FD_ZERO(&exceptionSet);

	for (FM_ServerSessionMI fmServerSessionMI = FM_SERVER_SESSION_M.begin(); fmServerSessionMI != FM_SERVER_SESSION_M.end(); ++fmServerSessionMI)
	{
		SOCKET fmServerSessionSocket;

		fmServerSessionSocket = fmServerSessionMI->second.Socket;

		++fmServerSessionMI;
		
		FD_SET(fmServerSessionSocket, &writeSet);
		FD_SET(fmServerSessionSocket, &exceptionSet);

		CHECK_WSAEWOULDBLOCK(connect(fmServerSessionSocket, (SOCKADDR*)&FM_SERVER_SESSION_M[fmServerSessionSocket].Address, sizeof(SOCKADDR_IN)))

		if (writeSet.fd_count == FD_SETSIZE || 0 < writeSet.fd_count && fmServerSessionMI == FM_SERVER_SESSION_M.end())
		{
			IF_SOCKET_ERROR(select(0, nullptr, &writeSet, &exceptionSet, &FM_CLIENT_TIME_VALUE))
			ELSE
			{
				FOR(i, writeSet.fd_count)
				{
					++LAST_FM_SERVER_SESSION_INDEX;
					CURRENT_FM_SERVER_SESSION_COUNT = LAST_FM_SERVER_SESSION_INDEX + 1;
				}
			}
		}
	}

	return 0;
}

/*
int DisconnectFM_Client()
{


	return 0;
}
*/

int ReceiveFM_ServerPacket()
{
	FD_SET readSet;
	FD_SET writeSet;

	FD_ZERO(&readSet);

	for (FM_ServerSessionMI fmServerSessionMI = FM_SERVER_SESSION_M.begin(); fmServerSessionMI != FM_SERVER_SESSION_M.end();)
	{
		SOCKET fmClientSessionSocket;

		fmClientSessionSocket = fmServerSessionMI->second.Socket;

		FD_SET(fmClientSessionSocket, &readSet);

		++fmServerSessionMI;

		if (readSet.fd_count == FM_CLIENT_COUNT || 0 < readSet.fd_count && fmServerSessionMI == FM_SERVER_SESSION_M.end())
		{
			IF_SOCKET_ERROR(select(0, &readSet, nullptr, nullptr, &FM_CLIENT_TIME_VALUE))
			ELSE
			{
				FOR(i, readSet.fd_count)
				{
					FM_ServerSession* fmClientSessionAddress;
					int receivedSize;

					fmClientSessionAddress = &FM_SERVER_SESSION_M[readSet.fd_array[i]];

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

					if (receivedSize == SOCKET_ERROR || receivedSize == 0)
					{
						if (WSAGetLastError() == WSAEWOULDBLOCK);
						else
						{
							closesocket(fmClientSessionAddress->Socket);

							FM_SERVER_SESSION_M.erase(fmClientSessionAddress->Socket);

							--LAST_FM_SERVER_SESSION_INDEX;
							CURRENT_FM_SERVER_SESSION_COUNT = LAST_FM_SERVER_SESSION_INDEX + 1;
						}
					}
				}
			}

			FD_ZERO(&readSet);
		}
	}

	return 0;
}

int SendFM_ClientPacket()
{
	FD_SET readSet;
	FD_SET writeSet;

	FD_ZERO(&writeSet);

	for (FM_ServerSessionMI fmServerSessionMI = FM_SERVER_SESSION_M.begin(); fmServerSessionMI != FM_SERVER_SESSION_M.end();)
	{
		SOCKET fmServerSessionSocket;

		fmServerSessionSocket = fmServerSessionMI->second.Socket;

		if (FM_SERVER_SESSION_M[fmServerSessionSocket].SendBQ.GetCurrentSize()) FD_SET(fmServerSessionSocket, &writeSet);

		++fmServerSessionMI;

		if (writeSet.fd_count == FD_SETSIZE || 0 < writeSet.fd_count && fmServerSessionMI == FM_SERVER_SESSION_M.end())
		{
			IF_SOCKET_ERROR(select(0, nullptr, &writeSet, nullptr, &FM_CLIENT_TIME_VALUE))
			ELSE
			{
				FOR(i, writeSet.fd_count)
				{
					FM_ServerSession* fmClientSessionAddress;
					int sendedSize;

					fmClientSessionAddress = &FM_SERVER_SESSION_M[writeSet.fd_array[i]];

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

					if (sendedSize == SOCKET_ERROR) //if(sendedSize == SOCKET_ERROR || sendedSize == 0)
					{
						if (WSAGetLastError() == WSAEWOULDBLOCK);
						else
						{
							closesocket(fmClientSessionAddress->Socket);

							FM_SERVER_SESSION_M.erase(fmClientSessionAddress->Socket);

							--LAST_FM_SERVER_SESSION_INDEX;
							CURRENT_FM_SERVER_SESSION_COUNT = LAST_FM_SERVER_SESSION_INDEX + 1;
						}
					}
				}
			}

			FD_ZERO(&writeSet);
		}
	}

	return 0;
}

int ProcessReceivedFM_ServerPacket()
{
	for (FM_ServerSessionMI fmClientSessionMI = FM_SERVER_SESSION_M.begin(); fmClientSessionMI != FM_SERVER_SESSION_M.end(); ++fmClientSessionMI)
	{
		FM_PacketHeader receivedPacketHeader;
		SerialByteQueue receivedPacketSBQ(sizeof(receivedPacketHeader), BUFFER_SIZE);

		if (sizeof(receivedPacketHeader) <= fmClientSessionMI->second.ReceiveBQ.GetCurrentSize())
		{
			fmClientSessionMI->second.ReceiveBQ.Peek((char*)&receivedPacketHeader, sizeof(receivedPacketHeader));

			if (sizeof(receivedPacketHeader) + receivedPacketHeader.PacketBodySize <= fmClientSessionMI->second.ReceiveBQ.GetCurrentSize())
			{
				fmClientSessionMI->second.ReceiveBQ.Peek(receivedPacketSBQ.GetQueueAddress(), sizeof(receivedPacketHeader) + receivedPacketHeader.PacketBodySize);
				//fmClientSessionMI->second.ReceiveBQ.Peek(receivedPacketSBQ.GetQueueAddress(), receivedPacketSBQ.GetMaximumSize());

				receivedPacketSBQ.IncreaseCurrentSize(receivedPacketHeader.PacketBodySize);

				if (CheckReceivedFM_ServerPacket(receivedPacketHeader, receivedPacketSBQ))
				{
					fmClientSessionMI->second.ReceiveBQ.Dequeue(receivedPacketSBQ.GetQueueAddress(), sizeof(receivedPacketHeader) + receivedPacketHeader.PacketBodySize);

					switch (receivedPacketHeader.PacketType)
					{
					case FM_RESPONSE_JOIN:
						{
						}
						break;
					case FM_RESPONSE_LOGIN:
						{
						}
						break;

					case FM_RESPONSE_MEMBER_LIST:
						{
						}
						break;
					case FM_RESPONSE_FRIEND_LIST:
						{
						}
						break;
					case FM_RESPONSE_FRIEND_OFFEREE_LIST:
						{
						}
						break;
					case FM_RESPONSE_FRIEND_OFFERER_LIST:
						{
						}
						break;
					case FM_RESPONSE_FRIEND_BREACH:
						{
						}
						break;
					case FM_RESPONSE_FRIEND_OFFER:
						{
						}
						break;
					case FM_RESPONSE_FRIEND_OFFER_CANCEL:
						{
						}
						break;
					case FM_RESPONSE_FRIEND_DENY:
						{
						}
						break;
					case FM_RESPONSE_FRIEND_AGREE:
						{
						}
						break;
					case FM_RESPONSE_STRESS_ECHO:
						{
							if (!memcmp(fmClientSessionMI->second.SendBQ.GetFrontAddress(), receivedPacketSBQ.GetQueueAddress(), receivedPacketSBQ.GetTotalSize()))
							{
								ProcessSendingFM_ClientPacket(FM_REQUEST_STRESS_ECHO, fmClientSessionMI, receivedPacketSBQ);
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

int ProcessSendingFM_ClientPacket(FM_REQUEST_INDEX fmRequestIndex, FM_ServerSessionMI fmServerSessionMI, SerialByteQueue& receivedPacketSBQ)
{
	FM_PacketHeader sendingPacketHeader;
	SerialByteQueue sendingPacketSBQ(sizeof(sendingPacketHeader), BUFFER_SIZE);

	ZeroMemory(&sendingPacketHeader, sizeof(sendingPacketHeader));

	sendingPacketHeader.PacketType = fmRequestIndex;

	switch (sendingPacketHeader.PacketType)
	{
	case FM_REQUEST_JOIN:
		{
		}
		break;
	case FM_REQUEST_LOGIN:
		{
		}
		break;
	case FM_REQUEST_MEMBER_LIST:
		{
		}
		break;
	case FM_REQUEST_FRIEND_LIST:
		{
		}
		break;
	case FM_REQUEST_FRIEND_OFFEREE_LIST:
		{
		}
		break;
	case FM_REQUEST_FRIEND_OFFERER_LIST:
		{
		}
		break;
	case FM_REQUEST_FRIEND_BREACH:
		{
		}
		break;
	case FM_REQUEST_FRIEND_OFFER:
		{
		}
		break;
	case FM_REQUEST_FRIEND_OFFER_CANCEL:
		{
		}
		break;
	case FM_REQUEST_FRIEND_DENY:
		{
		}
		break;
	case FM_REQUEST_FRIEND_AGREE:
		{
		}
		break;
	case FM_REQUEST_STRESS_ECHO:
		{
			WORD size;
			WCHAR stress[BUFFER_SIZE];

			size = rand() % 79;

			sendingPacketSBQ << size;

			memcpy(sendingPacketSBQ.GetFrontAddress() + sizeof(size), "AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz!@#$%^&*()-=[];',./_+{}:<>?", size); //make enqueue and dequeue function
			sendingPacketSBQ.IncreaseCurrentSize(size);
		}
		break;
	default:
		break;
	}

	if (CheckSendingFM_ClientPacket(sendingPacketHeader, sendingPacketSBQ)) fmServerSessionMI->second.SendBQ.Enqueue(sendingPacketSBQ.GetQueueAddress(), sendingPacketSBQ.GetTotalSize());

	return 0;
}

bool CheckReceivedFM_ServerPacket(FM_PacketHeader& receivedPacketHeader, SerialByteQueue& receivedPacketSBQ)
{
	if (receivedPacketHeader.IdentificationCode == FM_PACKET_HEADER_IDENTIFICATION_CODE && receivedPacketHeader.PacketBodySize == receivedPacketSBQ.GetCurrentSize()) return 1;

	return 0;
}

bool CheckSendingFM_ClientPacket(FM_PacketHeader& sendingPacketHeader, SerialByteQueue& sendingPacketSBQ)
{
	sendingPacketHeader.IdentificationCode = FM_PACKET_HEADER_IDENTIFICATION_CODE;

	sendingPacketHeader.PacketBodySize = sendingPacketSBQ.GetCurrentSize();

	sendingPacketSBQ.SetHeader(sendingPacketHeader);

	return 1;
}