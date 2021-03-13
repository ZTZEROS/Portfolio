#include "stdafx.h"
#include "PacketDefine.h"
#include "FighterClient.h"

bool InitializeFighterClient()
{
	WSADATA wsaDatum;
	u_long nonblockOn;
	//LINGER lingeringTime;
	BOOL nagleOn;
	//BOOL reuseOn;
	WCHAR ipAddress[32];
	UINT clientCount;

	FIGHTER_CLIENT_SESSION_V.reserve(TOTAL_FIGHTER_CLIENT_COUNT);

	CHECK_ERROR(WSAStartup(0x0202, &wsaDatum));

	printf("ipAddress = ");
	wscanf_s(L"%s", &ipAddress);
	printf(CRALF);
	printf("clientCount = ");
	wscanf_s(L"%d", &clientCount);
	printf(CRALF);

	FOR(i, clientCount)
	{
		FighterClientSession fighterClientSession;

		fighterClientSession.Socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
		CHECK_INVALID_SOCKET(fighterClientSession.Socket);

		nonblockOn = 1;
		CHECK_SOCKET_ERROR(ioctlsocket(fighterClientSession.Socket, FIONBIO, &nonblockOn));

		//lingeringTime.l_onoff = 1;
		//lingeringTime.l_linger = 0;
		//setsockopt(FIGHTER_CLIENT_SESSION.Socket, SOL_SOCKET, SO_LINGER, (char*)&lingeringTime, sizeof(lingeringTime));

		nagleOn = 0;
		setsockopt(fighterClientSession.Socket, IPPROTO_TCP, TCP_NODELAY, (char*)&nagleOn, sizeof(nagleOn));

		//reuseOn = 1;
		//setsockopt(FIGHTER_CLIENT_SESSION.Socket, SOL_SOCKET, SO_REUSEADDR, (char*)&reuseOn, sizeof(reuseOn));

		fighterClientSession.Address.sin_family = AF_INET;
		//WSAStringToAddress(ipAddress, AF_INET, nullptr, &fighterClientSession.Address, &addressLength)
		InetPton(AF_INET, ipAddress, &fighterClientSession.Address.sin_addr); //inet_addr(INADDR_ANY); //InetNtop();
		//FIGHTER_CLIENT_SESSION.Address.sin_addr.s_addr = htonl(INADDR_ANY); //WSAHtonl();
		fighterClientSession.Address.sin_port = htons(FIGHTER_SERVER_PORT);

		FIGHTER_CLIENT_SESSION_V.push_back(fighterClientSession);
	}

	FIGHTER_CLIENT_TIME_VALUE.tv_sec = 0;
	FIGHTER_CLIENT_TIME_VALUE.tv_usec = 0;

	srand(time(nullptr));


	//ConnectPhase:
	{
		ConnectFighterServer();
	}
	
	//TIMER.Initialize();

	return 0;
}

bool UpdateFighterClient()
{
	{
		FD_SET readSet;
		FD_SET writeSet;
		FD_SET exceptionSet;

		UINT clientSessionCount;

		UINT currentClientSessionIndex;
		UINT correctedReceivingIndex;
		UINT correctedSendingIndex;

		clientSessionCount = FIGHTER_CLIENT_SESSION_V.size();
		currentClientSessionIndex = 0;

		FOR(i, FIGHTER_FD_SET_COUNT)
		{
			//FD_ZERO(&readSet);
			//FD_ZERO(&writeSet);
			readSet.fd_count = 0;
			writeSet.fd_count = 0;

			correctedReceivingIndex = 0;
			correctedSendingIndex = 0;

			FOR(j, FIGHTER_FD_COUNT)
			{
				if (currentClientSessionIndex < clientSessionCount && j < FIGHTER_FD_COUNT)
				{
					//FD_SET(FIGHTER_CLIENT_SESSION_V[currentClientSessionIndex].Socket, &readSet);
					readSet.fd_array[readSet.fd_count] = FIGHTER_CLIENT_SESSION_V[currentClientSessionIndex].Socket;
					++readSet.fd_count;

					if (0 < FIGHTER_CLIENT_SESSION_V[currentClientSessionIndex].SendBQ.GetCurrentSize())
					{
						//FD_SET(FIGHTER_CLIENT_SESSION_V[currentClientSessionIndex].Socket, &writeSet);
						writeSet.fd_array[writeSet.fd_count] = FIGHTER_CLIENT_SESSION_V[currentClientSessionIndex].Socket;
						++writeSet.fd_count;
					}

					++currentClientSessionIndex;
				}
				else break;
			}

			if (0 < readSet.fd_count || 0 < writeSet.fd_count)
			{
				IF_SOCKET_ERROR(select(0, &readSet, &writeSet, nullptr, &FIGHTER_CLIENT_TIME_VALUE))
				ELSE
				{
					//SendPhase:
					{
						correctedSendingIndex = i * FIGHTER_FD_COUNT;

						FOR(j, writeSet.fd_count)
						{
							int sendedSize;

							if (FIGHTER_CLIENT_SESSION_V[correctedSendingIndex].Socket == writeSet.fd_array[j])
							{
								sendedSize = FIGHTER_CLIENT_SESSION_V[correctedSendingIndex].SendBQ.Send(send, FIGHTER_CLIENT_SESSION_V[correctedSendingIndex].Socket);

								if (sendedSize == SOCKET_ERROR) //if(sendedSize == SOCKET_ERROR || sendedSize == 0)
								{
									if (WSAGetLastError() == WSAEWOULDBLOCK);
									else
									{
										//SerialByteQueue nullSBQ;
										//
										//closesocket(FIGHTER_CLIENT_SESSION_V[correctedReceivingIndex].Socket);
										//
										//FighterResponseDeleteOtherCharacter(FIGHTER_CLIENT_SESSION_V[correctedReceivingIndex], nullSBQ);
										//
										//FIGHTER_PLAYER_UM.erase(FIGHTER_CLIENT_SESSION_V[correctedReceivingIndex].IdentificationNumber);
										//
										//FIGHTER_CLIENT_SESSION_V[correctedReceivingIndex] = FIGHTER_CLIENT_SESSION_V[FIGHTER_CLIENT_SESSION_V.size() - 1];
										//FIGHTER_CLIENT_SESSION_V.pop_back();
									}
								}
							}
							else --j;

							++correctedSendingIndex;
						}
					}



					//ReceivePhase:
					{
						correctedReceivingIndex = i * FIGHTER_FD_COUNT;

						FOR(k, readSet.fd_count)
						{
							int receivedSize;

							if (FIGHTER_CLIENT_SESSION_V[correctedReceivingIndex].Socket == readSet.fd_array[k])
							{
								while (1)
								{
									receivedSize = FIGHTER_CLIENT_SESSION_V[correctedReceivingIndex].ReceiveBQ.Receive(recv, FIGHTER_CLIENT_SESSION_V[correctedReceivingIndex].Socket);

									if (receivedSize < 1)
									{
										if (WSAGetLastError() == WSAEWOULDBLOCK);
										else
										{
											FIGHTER_PLAYER_UM.erase(FIGHTER_CLIENT_SESSION_V[correctedReceivingIndex].IdentificationNumber);

											closesocket(FIGHTER_CLIENT_SESSION_V[correctedReceivingIndex].Socket);

											FIGHTER_CLIENT_SESSION_V[correctedReceivingIndex] = FIGHTER_CLIENT_SESSION_V[FIGHTER_CLIENT_SESSION_V.size() - 1];
											FIGHTER_CLIENT_SESSION_V.pop_back();

											--clientSessionCount;

											if (i == clientSessionCount / FD_SETSIZE) --readSet.fd_count;
										}

										break;
									}
								}
							}
							else --k;

							++correctedReceivingIndex;
						}
					}
				}
			}
			else break;

		}

		ProcessReceivedFighterServerPacket();
	}

	{
		FOR(i, FIGHTER_CLIENT_SESSION_V.size())
		{
			switch (rand() % FIGHTER_REQUEST_COUNT)
			{
			case 0:
				{
					FighterRequestMoveStart(FIGHTER_CLIENT_SESSION_V[i]);
				}
				break;
			case 1:
				{
					FighterRequestMoveStop(FIGHTER_CLIENT_SESSION_V[i]);
				}
				break;
			case 2:
				{
					FighterRequestAttack1(FIGHTER_CLIENT_SESSION_V[i]);
				}
				break;
			case 3:
				{
					FighterRequestAttack2(FIGHTER_CLIENT_SESSION_V[i]);
				}
				break;
			case 4:
				{
					FighterRequestAttack3(FIGHTER_CLIENT_SESSION_V[i]);
				}
				break;
			case 5:
				{
					FighterRequestEcho(FIGHTER_CLIENT_SESSION_V[i]);
				}
				break;
			}
			
		}
	}

	return 0;
}

bool RenderFighterClient()
{
	return 0;
}

bool TerminalizeFighterClient()
{
	return 0;
}

int ConnectFighterServer()
{
	FD_SET writeSet;
	FD_SET exceptionSet;
	UINT fighterClientCount;
	UINT connectedClient;

	writeSet.fd_count = 0;
	exceptionSet.fd_count = 0;
	fighterClientCount = FIGHTER_CLIENT_SESSION_V.size();
	connectedClient = 0;

	FOR(i, fighterClientCount)
	{
		writeSet.fd_array[i % FD_SETSIZE] = FIGHTER_CLIENT_SESSION_V[i].Socket;
		exceptionSet.fd_array[i % FD_SETSIZE] = FIGHTER_CLIENT_SESSION_V[i].Socket;

		++writeSet.fd_count;
		++exceptionSet.fd_count;

		CHECK_WSAEWOULDBLOCK(connect(FIGHTER_CLIENT_SESSION_V[i].Socket, (SOCKADDR*)&FIGHTER_CLIENT_SESSION_V[i].Address, sizeof(SOCKADDR_IN)))

		if (writeSet.fd_count == FD_SETSIZE || i == fighterClientCount - 1)
		{
			IF_SOCKET_ERROR(select(0, nullptr, &writeSet, &exceptionSet, &FIGHTER_CLIENT_TIME_VALUE))
			ELSE connectedClient += writeSet.fd_count;
		}
	}

	printf("connectedClient : %d" CRALF, connectedClient);

	if(fighterClientCount == connectedClient) printf("fighterClientCount == connectedClient" CRALF);

	return 0;
}

int DisconnectFighterServer()
{
	return 0;
}

int ReceiveFighterServerPacket()
{
	return 0;
}

int SendFighterClientPacket()
{
	return 0;
}

int ProcessReceivedFighterServerPacket()
{
	UINT fighterClientSessionCount;

	fighterClientSessionCount = FIGHTER_CLIENT_SESSION_V.size();

	FOR(i, fighterClientSessionCount)
	{
		FighterPacketHeader receivedPacketHeader;
		SerialByteQueue receivedPacketSBQ(sizeof(receivedPacketHeader), BUFFER_SIZE);

		if (sizeof(receivedPacketHeader) == FIGHTER_CLIENT_SESSION_V[i].ReceiveBQ.Peek((char*)&receivedPacketHeader, sizeof(receivedPacketHeader)))
		{
			if (sizeof(receivedPacketHeader) + receivedPacketHeader.PacketBodySize == FIGHTER_CLIENT_SESSION_V[i].ReceiveBQ.Peek(receivedPacketSBQ.GetQueueAddress(), sizeof(receivedPacketHeader) + receivedPacketHeader.PacketBodySize))
			{
				receivedPacketSBQ.IncreaseCurrentSize(receivedPacketHeader.PacketBodySize);

				if (CheckReceivedFighterServerPacket(receivedPacketHeader, receivedPacketSBQ))
				{
					FIGHTER_CLIENT_SESSION_V[i].ReceiveBQ.DecreaseCurrentSize(sizeof(receivedPacketHeader) + receivedPacketHeader.PacketBodySize + 1); //+1 for FIGHTER_PACKET_TRAILER_IDENTIFICATION_CODE

					switch (receivedPacketHeader.PacketType)
					{
					case FIGHTER_RESPONSE_CREATE_MY_CHARACTER:
						{
							FighterCreateMyCharacter(FIGHTER_CLIENT_SESSION_V[i], receivedPacketSBQ);
						}
						break;
					case FIGHTER_RESPONSE_CREATE_OTHER_CHARACTER:
						{
							//FighterCreateOtherCharacter(FIGHTER_CLIENT_SESSION_V[i], receivedPacketSBQ); //not necessary for dummy client
						}
						break;
					case FIGHTER_RESPONSE_DELETE_OTHER_CHARACTER:
						{
							//FighterDeleteOtherCharacter(FIGHTER_CLIENT_SESSION_V[i], receivedPacketSBQ); //not necessary for dummy client
						}
						break;
					case FIGHTER_RESPONSE_MOVE_START:
						{
							//FighterMoveStart(FIGHTER_CLIENT_SESSION_V[i], receivedPacketSBQ); //not necessary for dummy client
						}
						break;
					case FIGHTER_RESPONSE_MOVE_STOP:
						{
							//FighterMoveStop(FIGHTER_CLIENT_SESSION_V[i], receivedPacketSBQ); //not necessary for dummy client
						}
						break;
					case FIGHTER_RESPONSE_ATTACK1:
						{
							//FighterAttack1(FIGHTER_CLIENT_SESSION_V[i], receivedPacketSBQ); //not necessary for dummy client
						}
						break;
					case FIGHTER_RESPONSE_ATTACK2:
						{
							//FighterAttack2(FIGHTER_CLIENT_SESSION_V[i], receivedPacketSBQ); //not necessary for dummy client
						}
						break;
					case FIGHTER_RESPONSE_ATTACK3:
						{
							//FighterAttack3(FIGHTER_CLIENT_SESSION_V[i], receivedPacketSBQ); //not necessary for dummy client
						}
						break;
					case FIGHTER_RESPONSE_DAMAGE:
						{
							FighterDamage(FIGHTER_CLIENT_SESSION_V[i], receivedPacketSBQ);
						}
						break;
					case FIGHTER_RESPONSE_SYNC:
						{
							FighterSync(FIGHTER_CLIENT_SESSION_V[i], receivedPacketSBQ);
						}
						break;
					case FIGHTER_RESPONSE_ECHO:
						{
							FighterEcho(FIGHTER_CLIENT_SESSION_V[i], receivedPacketSBQ);
						}
						break;
					default:
						{
							//log and disconnect client
						}
						break;
					}
				}
			}
		}
	}


	return 0;
}

int ProcessSendingFighterClientPacket()
{
	FOR(i, FIGHTER_CLIENT_SESSION_V.size())
	{

	}

	return 0;
}

bool CheckReceivedFighterServerPacket(FighterPacketHeader& receivedPacketHeader, SerialByteQueue& receivedPacketSBQ)
{
	if (receivedPacketHeader.IdentificationCode == FIGHTER_PACKET_HEADER_IDENTIFICATION_CODE
		&& receivedPacketHeader.PacketBodySize == receivedPacketSBQ.GetCurrentSize()) return 1;

	return 0;
}

bool CheckSendingFighterClientPacket(FighterPacketHeader& sendingPacketHeader, SerialByteQueue& sendingPacketSBQ)
{
	sendingPacketHeader.IdentificationCode = FIGHTER_PACKET_HEADER_IDENTIFICATION_CODE;

	sendingPacketHeader.PacketBodySize = sendingPacketSBQ.GetCurrentSize();

	sendingPacketSBQ.SetHeader(sendingPacketHeader);

	sendingPacketSBQ << BYTE(FIGHTER_PACKET_TRAILER_IDENTIFICATION_CODE);

	return 1;
}

