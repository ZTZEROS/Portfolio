#include "stdafx.h"
#include "PacketDefine.h"
#include "FighterServer.h"

bool InitializeFighterServer()
{
	WSADATA wsaDatum;
	u_long nonblockOn;
	//LINGER lingeringTime;
	BOOL nagleOff;
	//BOOL reuseOn;

	ZeroMemory(&FIGHTER_SERVER_SESSION, sizeof(FIGHTER_SERVER_SESSION));
	FIGHTER_CLIENT_SESSION_V.reserve(TOTAL_FIGHTER_CLIENT_COUNT);

	CHECK_ERROR(WSAStartup(0x0202, &wsaDatum));

	FIGHTER_SERVER_SESSION.Socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	CHECK_INVALID_SOCKET(FIGHTER_SERVER_SESSION.Socket);

	nonblockOn = 1;
	CHECK_SOCKET_ERROR(ioctlsocket(FIGHTER_SERVER_SESSION.Socket, FIONBIO, &nonblockOn));

	//lingeringTime.l_onoff = 1;
	//lingeringTime.l_linger = 0;
	//setsockopt(FM_SERVER_SESSION.Socket, SOL_SOCKET, SO_LINGER, (char*)&lingeringTime, sizeof(lingeringTime));

	nagleOff = 1;
	setsockopt(FIGHTER_SERVER_SESSION.Socket, IPPROTO_TCP, TCP_NODELAY, (char*)&nagleOff, sizeof(nagleOff));

	//reuseOn = 1;
	//setsockopt(FM_SERVER_SESSION.Socket, SOL_SOCKET, SO_REUSEADDR, (char*)&reuseOn, sizeof(reuseOn));

	FIGHTER_SERVER_SESSION.Address.sin_family = AF_INET;
	FIGHTER_SERVER_SESSION.Address.sin_addr.s_addr = htonl(INADDR_ANY); //WSAHtonl();
	//InetPton(AF_INET, INADDR_ANY, &serverAddress.sin_addr); //inet_addr(INADDR_ANY); //InetNtop();
	FIGHTER_SERVER_SESSION.Address.sin_port = htons(FIGHTER_SERVER_PORT);
	CHECK_SOCKET_ERROR(bind(FIGHTER_SERVER_SESSION.Socket, (SOCKADDR*)&FIGHTER_SERVER_SESSION.Address, sizeof(FIGHTER_SERVER_SESSION.Address)));
	CHECK_SOCKET_ERROR(listen(FIGHTER_SERVER_SESSION.Socket, SOMAXCONN));

	FIGHTER_SERVER_TIME_VALUE.tv_sec = 0;
	FIGHTER_SERVER_TIME_VALUE.tv_usec = 0;

	FIGHTER_IDENTIFICATION_NUMBER = 0;
	
	LOOP_COUNT = 0;
	UPDATE_COUNT = 0;
	RECEIVE_COUNT = 0;
	SEND_COUNT = 0;

	MOVE_START_COUNT = 0;
	MOVE_STOP_COUNT = 0;
	ATTACK1_COUNT = 0;
	ATTACK2_COUNT = 0;
	ATTACK3_COUNT = 0;
	CALCULATE_FIGHTER_SECTOR_COUNT = 0;
	DEADRECKONING_COUNT = 0;

	//srand(time(nullptr));
	srand(27);
	
	TIMER.Initialize();

	return 0;
}

bool UpdateFighterServer()
{
	//AcceptPhase:
	{
		AcceptFighterClient();
	}

	//ReceivePhase:
	//{
	//	ReceiveFighterClientPacket();
	//	ProcessReceivedFighterClientPacket();
	//}

	//SendPhase:
	//{
	//	ProcessSendingFighterServerPacket();
	//	SendFighterServerPacket();
	//}

	//ReceiveAndSendPhase:
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
				IF_SOCKET_ERROR(select(0, &readSet, &writeSet, nullptr, &FIGHTER_SERVER_TIME_VALUE))
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

								++SEND_COUNT;
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
											//FighterResponseDeleteOtherCharacter()
											{
												FighterPacketHeader sendingPacketHeader;
												SerialByteQueue sendingPacketSBQ;
												FighterPlayer fighterPlayer;

												ZeroMemory(&sendingPacketHeader, sizeof(sendingPacketHeader));

												sendingPacketSBQ.SetHeader(sendingPacketHeader);

												sendingPacketHeader.PacketType = FIGHTER_RESPONSE_DELETE_OTHER_CHARACTER;

												fighterPlayer = FIGHTER_PLAYER_UM[FIGHTER_CLIENT_SESSION_V[correctedReceivingIndex].IdentificationNumber];

												sendingPacketSBQ << fighterPlayer.IdentificationNumber;


												BroadcastFighter(FIGHTER_CLIENT_SESSION_V[correctedReceivingIndex], sendingPacketHeader, sendingPacketSBQ);

												FIGHTER_SECTOR_A[fighterPlayer.PreviousSectorIndexY][fighterPlayer.PreviousSectorIndexX].FighterPlayerL.remove(&FIGHTER_PLAYER_UM[fighterPlayer.IdentificationNumber]);
												FIGHTER_SECTOR_A[fighterPlayer.SectorIndexY][fighterPlayer.SectorIndexX].FighterPlayerL.remove(&FIGHTER_PLAYER_UM[fighterPlayer.IdentificationNumber]);

												FIGHTER_PLAYER_UM.erase(fighterPlayer.IdentificationNumber);
											}

											closesocket(FIGHTER_CLIENT_SESSION_V[correctedReceivingIndex].Socket);

											FIGHTER_CLIENT_SESSION_V[correctedReceivingIndex] = FIGHTER_CLIENT_SESSION_V[FIGHTER_CLIENT_SESSION_V.size() - 1];
											FIGHTER_CLIENT_SESSION_V.pop_back();

											--clientSessionCount;
											
											if(i == clientSessionCount / FD_SETSIZE) --readSet.fd_count;
										}

										break;
									}

									++RECEIVE_COUNT;
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
	
		ProcessReceivedFighterClientPacket();
	}



	//UpdateClientStatePhase: 
	{
		//++LOOP_COUNT;

		while (TIMER.CheckUpdate())
		{
			UINT clientSessionCount;
			SerialByteQueue nullSBQ;
			
			++UPDATE_COUNT;

			clientSessionCount = FIGHTER_CLIENT_SESSION_V.size();
			
			FOR(i, clientSessionCount)
			{
				//CalculateFighterPosition(FIGHTER_CLIENT_SESSION_V[i], nullSBQ);
				CalculateFighterSector(FIGHTER_CLIENT_SESSION_V[i], nullSBQ);
			}
		}
	}



	//if (GetAsyncKeyState('T') && GetAsyncKeyState('I') && GetAsyncKeyState('U') && GetAsyncKeyState('Q')
	//	|| GetAsyncKeyState('T') && GetAsyncKeyState('I') && GetAsyncKeyState('X') && GetAsyncKeyState('E')) return 1;

	return 0;
}

bool RenderFighterServer()
{
	//if(TIMER.CheckElapsedTime())
	//if(TIMER.CheckRender())
	if (TIMER.ShowCounter())
	{
		//printf("LOOP_COUNT : %d, UPDATE_COUNT : %d, RECEIVE_COUNT : %d, SEND_COUNT : %d," CRALF, LOOP_COUNT, UPDATE_COUNT, RECEIVE_COUNT, SEND_COUNT);
		printf("UPDATE_COUNT : %d" CRALF, UPDATE_COUNT);
		printf("RECEIVE_COUNT : %d, SEND_COUNT : %d," CRALF, RECEIVE_COUNT, SEND_COUNT);
		printf("MOVE_START_COUNT: %d, MOVE_STOP_COUNT : %d, ATTACK1_COUNT : %d, ATTACK2_COUNT : %d, ATTACK3_COUNT : %d" CRALF, MOVE_START_COUNT, MOVE_STOP_COUNT, ATTACK1_COUNT, ATTACK2_COUNT, ATTACK3_COUNT);
		printf("CALCULATE_FIGHTER_SECTOR_COUNT : %d, DEADRECKONING_COUNT : %d" CRALF, CALCULATE_FIGHTER_SECTOR_COUNT, DEADRECKONING_COUNT);
		printf("CREATE_OTHER_CHARACTER_COUNT : %d, DELETE_OTHER_CHARACTER_COUNT : %d" CRALF CRALF, CREATE_OTHER_CHARACTER_COUNT, DELETE_OTHER_CHARACTER_COUNT);

		//LOOP_COUNT = 0;
		UPDATE_COUNT = 0;
		RECEIVE_COUNT = 0;
		SEND_COUNT = 0;

		CREATE_OTHER_CHARACTER_COUNT = 0;
		DELETE_OTHER_CHARACTER_COUNT = 0;
		MOVE_START_COUNT = 0;
		MOVE_STOP_COUNT = 0;
		ATTACK1_COUNT = 0;
		ATTACK2_COUNT = 0;
		ATTACK3_COUNT = 0;
		CALCULATE_FIGHTER_SECTOR_COUNT = 0;
		DEADRECKONING_COUNT = 0;
	}

	return 0;
}

bool TerminalizeFighterServer()
{
	TIMER.Termimalize();

	closesocket(FIGHTER_SERVER_SESSION.Socket);
	
	WSACleanup();

	return 0;
}

int AcceptFighterClient()
{
	FD_SET readSet;
	//FD_SET writeSet;
	//FD_SET exceptionSet;

	readSet.fd_count = 0;
	//writeSet.fd_count = 0;
	//exceptionSet.fd_count = 0;

	//FD_SET(FIGHTER_SERVER_SESSION.Socket, &readSet);
	readSet.fd_array[readSet.fd_count] = FIGHTER_SERVER_SESSION.Socket;
	++readSet.fd_count;

	IF_SOCKET_ERROR(select(0, &readSet, nullptr, nullptr, &FIGHTER_SERVER_TIME_VALUE))
	ELSE
	{
		FighterClientSession fighterClientSession;
		int addressLength;

		ZeroMemory(&fighterClientSession, sizeof(fighterClientSession));
		addressLength = sizeof(fighterClientSession.Address);

		while (0 < readSet.fd_count && fighterClientSession.Socket != INVALID_SOCKET)
		{
			fighterClientSession.Socket = accept(FIGHTER_SERVER_SESSION.Socket, (SOCKADDR*)&fighterClientSession.Address, &addressLength);
			IF_INVALID_SOCKET(fighterClientSession.Socket)
			ELSE
			{
				UINT fighterClientSessionIndex;
				SerialByteQueue nullSBQ;

				fighterClientSession.IdentificationNumber = ++FIGHTER_IDENTIFICATION_NUMBER;

				FIGHTER_CLIENT_SESSION_V.push_back(fighterClientSession);

				fighterClientSessionIndex = FIGHTER_CLIENT_SESSION_V.size() - 1;

				FIGHTER_CLIENT_SESSION_V[fighterClientSessionIndex].ReceiveBQ.Clear();
				FIGHTER_CLIENT_SESSION_V[fighterClientSessionIndex].SendBQ.Clear();

				FighterResponseCreateMyCharacter(FIGHTER_CLIENT_SESSION_V[fighterClientSessionIndex], nullSBQ);
				FighterResponseCreateOtherCharacter(FIGHTER_CLIENT_SESSION_V[fighterClientSessionIndex], nullSBQ);
			}
		}
	}

	return 0;
}

int DisconnectFighterClient()
{
	return 0;
}

int ReceiveFighterClientPacket()
{
	return 0;
}

int SendFighterServerPacket()
{
	return 0;
}

int ProcessReceivedFighterClientPacket()
{
	UINT fighterClientSessionCount;

	fighterClientSessionCount = FIGHTER_CLIENT_SESSION_V.size();

	FOR(i, fighterClientSessionCount)
	{
		FighterPacketHeader receivedPacketHeader;
		SerialByteQueue receivedPacketSBQ;

		if (sizeof(receivedPacketHeader) == FIGHTER_CLIENT_SESSION_V[i].ReceiveBQ.Peek((char*)&receivedPacketHeader, sizeof(receivedPacketHeader)))
		{
			if (sizeof(receivedPacketHeader) + receivedPacketHeader.PacketBodySize == FIGHTER_CLIENT_SESSION_V[i].ReceiveBQ.Peek(receivedPacketSBQ.GetQueueAddress(), sizeof(receivedPacketHeader) + receivedPacketHeader.PacketBodySize))
			{
				receivedPacketSBQ.SetHeader(receivedPacketHeader);

				receivedPacketSBQ.IncreaseCurrentSize(receivedPacketHeader.PacketBodySize);

				if (CheckReceivedFighterClientPacket(receivedPacketHeader, receivedPacketSBQ)) //upgradable?
				{
					FIGHTER_CLIENT_SESSION_V[i].ReceiveBQ.DecreaseCurrentSize(sizeof(receivedPacketHeader) + receivedPacketHeader.PacketBodySize + 1); //+1 for FIGHTER_PACKET_TRAILER_IDENTIFICATION_CODE
					
					switch (receivedPacketHeader.PacketType)
					{
					case FIGHTER_REQUEST_MOVE_START :
						{
							FighterResponseMoveStart(FIGHTER_CLIENT_SESSION_V[i], receivedPacketSBQ);

							//SHOW_TEXT(STRINGIZING(FIGHTER_REQUEST_MOVE_START));
						}
						break;
					case FIGHTER_REQUEST_MOVE_STOP :
						{
							FighterResponseMoveStop(FIGHTER_CLIENT_SESSION_V[i], receivedPacketSBQ);

							//SHOW_TEXT(STRINGIZING(FIGHTER_REQUEST_MOVE_STOP));
						}
						break;
					case FIGHTER_REQUEST_ATTACK1 :
						{
							FighterResponseAttack1(FIGHTER_CLIENT_SESSION_V[i], receivedPacketSBQ);

							//SHOW_TEXT(STRINGIZING(FIGHTER_REQUEST_ATTACK1));
						}
						break;
					case FIGHTER_REQUEST_ATTACK2 :
						{
							FighterResponseAttack2(FIGHTER_CLIENT_SESSION_V[i], receivedPacketSBQ);

							//SHOW_TEXT(STRINGIZING(FIGHTER_REQUEST_ATTACK2));
						}
						break;
					case FIGHTER_REQUEST_ATTACK3 :
						{
							FighterResponseAttack3(FIGHTER_CLIENT_SESSION_V[i], receivedPacketSBQ);

							//SHOW_TEXT(STRINGIZING(FIGHTER_REQUEST_ATTACK3));
						}
						break;
					case FIGHTER_REQUEST_ECHO:
						{
							FighterResponseEcho(FIGHTER_CLIENT_SESSION_V[i], receivedPacketSBQ);
						}
						break;
					default :
						{
							//log and disconnect client
						}
						break;
					}

					--i;
				}
			}
		}
	}

	return 0;
}

int ProcessSendingFighterServerPacket()
{
	return 0;
}

bool CheckReceivedFighterClientPacket(FighterPacketHeader& receivedPacketHeader, SerialByteQueue& receivedPacketSBQ)
{
	if (receivedPacketHeader.IdentificationCode == FIGHTER_PACKET_HEADER_IDENTIFICATION_CODE
		&& receivedPacketHeader.PacketBodySize == receivedPacketSBQ.GetCurrentSize()) return 1;

	//not checked FIGHTER_PACKET_TRAILER_IDENTIFICATION_CODE

	return 0;
}

bool CheckSendingFighterServerPacket(FighterPacketHeader& sendingPacketHeader, SerialByteQueue& sendingPacketSBQ)
{
	sendingPacketHeader.IdentificationCode = FIGHTER_PACKET_HEADER_IDENTIFICATION_CODE;

	sendingPacketHeader.PacketBodySize = sendingPacketSBQ.GetCurrentSize();

	sendingPacketSBQ.SetHeader(sendingPacketHeader);

	sendingPacketSBQ << BYTE(FIGHTER_PACKET_TRAILER_IDENTIFICATION_CODE);

	return 1;
}