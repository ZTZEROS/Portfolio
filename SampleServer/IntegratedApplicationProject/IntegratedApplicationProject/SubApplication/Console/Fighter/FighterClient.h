#pragma once

#include "Fighter.h"



extern FighterServerSession FIGHTER_SERVER_SESSION;

extern FighterClientSessionV FIGHTER_CLIENT_SESSION_V;

extern FighterPlayerUM FIGHTER_PLAYER_UM;
extern FighterPlayerV FIGHTER_PLAYER_V;
//extern FighterPlayer FIGHTER_PLAYER_A[TOTAL_FIGHTER_CLIENT_COUNT];

extern TIMEVAL FIGHTER_CLIENT_TIME_VALUE;



int FighterClient(int argumentCount, char* argumentVector[]);

bool InitializeFighterClient();
bool UpdateFighterClient();
bool RenderFighterClient();
bool TerminalizeFighterClient();

int ConnectFighterServer();
int DisconnectFighterServer();

int ReceiveFighterServerPacket();
int SendFighterClientPacket();

int ProcessReceivedFighterServerPacket();
int ProcessSendingFighterClientPacket();

bool CheckReceivedFighterServerPacket(FighterPacketHeader& receivedPacketHeader, SerialByteQueue& receivedPacketSBQ);
bool CheckSendingFighterClientPacket(FighterPacketHeader& sendingPacketHeader, SerialByteQueue& sendingPacketSBQ);



int FighterRequestMoveStart(FighterClientSession& fighterClientSession);
int FighterRequestMoveStop(FighterClientSession& fighterClientSession);
int FighterRequestAttack1(FighterClientSession& fighterClientSession);
int FighterRequestAttack2(FighterClientSession& fighterClientSession);
int FighterRequestAttack3(FighterClientSession& fighterClientSession);
int FighterRequestEcho(FighterClientSession& fighterClientSession);



int FighterCreateMyCharacter(FighterClientSession& fighterClientSession, SerialByteQueue& receivedPacketSBQ);
int FighterCreateOtherCharacter(FighterClientSession& fighterClientSession, SerialByteQueue& receivedPacketSBQ);
int FighterDeleteOtherCharacter(FighterClientSession& fighterClientSession, SerialByteQueue& receivedPacketSBQ);
int FighterMoveStart(FighterClientSession& fighterClientSession, SerialByteQueue& receivedPacketSBQ);
int FighterMoveStop(FighterClientSession& fighterClientSession, SerialByteQueue& receivedPacketSBQ);
int FighterAttack1(FighterClientSession& fighterClientSession, SerialByteQueue& receivedPacketSBQ);
int FighterAttack2(FighterClientSession& fighterClientSession, SerialByteQueue& receivedPacketSBQ);
int FighterAttack3(FighterClientSession& fighterClientSession, SerialByteQueue& receivedPacketSBQ);
int FighterDamage(FighterClientSession& fighterClientSession, SerialByteQueue& receivedPacketSBQ);
int FighterSync(FighterClientSession& fighterClientSession, SerialByteQueue& receivedPacketSBQ);
int FighterEcho(FighterClientSession& fighterClientSession, SerialByteQueue& receivedPacketSBQ);

void CalculateRoundTripTime();