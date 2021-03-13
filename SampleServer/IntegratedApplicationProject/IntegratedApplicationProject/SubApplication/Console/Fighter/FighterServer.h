#pragma once

#include "Fighter.h"



extern FighterServerSession FIGHTER_SERVER_SESSION;

extern FighterClientSessionV FIGHTER_CLIENT_SESSION_V;
//extern FighterClientSessionA FIGHTER_CLIENT_SESSION_A[TOTAL_FIGHTER_CLIENT_COUNT];

extern FighterPlayerUM FIGHTER_PLAYER_UM;
extern FighterPlayerV FIGHTER_PLAYER_V;
//extern FighterPlayer FIGHTER_PLAYER_A[TOTAL_FIGHTER_CLIENT_COUNT];
extern FighterSector FIGHTER_SECTOR_A[FIGHTER_MAP_HEIGHT / FIGHTER_SECTOR_HEIGHT][FIGHTER_MAP_WIDTH / FIGHTER_SECTOR_WIDTH];

extern TIMEVAL FIGHTER_SERVER_TIME_VALUE;

extern unsigned int FIGHTER_IDENTIFICATION_NUMBER;

extern unsigned int LOOP_COUNT;
extern unsigned int UPDATE_COUNT;
extern unsigned int RECEIVE_COUNT;
extern unsigned int SEND_COUNT;

extern unsigned int CREATE_OTHER_CHARACTER_COUNT;
extern unsigned int DELETE_OTHER_CHARACTER_COUNT;
extern unsigned int MOVE_START_COUNT;
extern unsigned int MOVE_STOP_COUNT;
extern unsigned int ATTACK1_COUNT;
extern unsigned int ATTACK2_COUNT;
extern unsigned int ATTACK3_COUNT;
extern unsigned int CALCULATE_FIGHTER_SECTOR_COUNT;
extern unsigned int DEADRECKONING_COUNT;


int FighterServer(int argumentCount, char* argumentVector[]);

bool InitializeFighterServer();
bool UpdateFighterServer();
bool RenderFighterServer();
bool TerminalizeFighterServer();

int AcceptFighterClient();
int DisconnectFighterClient();

int ReceiveFighterClientPacket();
int SendFighterServerPacket();

int ProcessReceivedFighterClientPacket();
int ProcessSendingFighterServerPacket();

bool CheckReceivedFighterClientPacket(FighterPacketHeader& receivedPacketHeader, SerialByteQueue& receivedPacketSBQ);
bool CheckSendingFighterServerPacket(FighterPacketHeader& receivedPacketHeader, SerialByteQueue& receivedPacketSBQ);



int FighterResponseCreateMyCharacter(FighterClientSession& fighterClientSession, SerialByteQueue& receivedPacketSBQ);
int FighterResponseCreateOtherCharacter(FighterClientSession& fighterClientSession, SerialByteQueue& receivedPacketSBQ);
int FighterResponseDeleteOtherCharacter(FighterClientSession& fighterClientSession, SerialByteQueue& receivedPacketSBQ);
int FighterResponseMoveStart(FighterClientSession& fighterClientSession, SerialByteQueue& receivedPacketSBQ);
int FighterResponseMoveStop(FighterClientSession& fighterClientSession, SerialByteQueue& receivedPacketSBQ);
int FighterResponseAttack1(FighterClientSession& fighterClientSession, SerialByteQueue& receivedPacketSBQ);
int FighterResponseAttack2(FighterClientSession& fighterClientSession, SerialByteQueue& receivedPacketSBQ);
int FighterResponseAttack3(FighterClientSession& fighterClientSession, SerialByteQueue& receivedPacketSBQ);
int FighterResponseDamage(FighterClientSession& fighterClientSession, SerialByteQueue& receivedPacketSBQ);
int FighterResponseSynchronize(FighterClientSession& fighterClientSession, SerialByteQueue& receivedPacketSBQ);
int FighterResponseEcho(FighterClientSession& fighterClientSession, SerialByteQueue& receivedPacketSBQ);


void CalculateFighterSector(FighterClientSession& fighterClientSession, SerialByteQueue& receivedPacketSBQ);
void CalculateFighterPosition(FighterClientSession& fighterClientSession, SerialByteQueue& receivedPacketSBQ);
void DeadReckoning(FighterClientSession& fighterClientSession, SerialByteQueue& receivedPacketSBQ);

void PartialcastFighter(FighterClientSession& fighterClientSession, FighterPacketHeader& sendingPacketHeader, SerialByteQueue& sendingPacketSBQ);
void BroadcastFighter(FighterClientSession& fighterClientSession, FighterPacketHeader& sendingPacketHeader, SerialByteQueue& sendingPacketSBQ);
void ReverseUnicastFighter(FighterClientSession& fighterClientSession, FighterPacketHeader& sendingPacketHeader, SerialByteQueue& sendingPacketSBQ);
void BroadcastAndReverseUnicastFighter(FighterClientSession& fighterClientSession, FighterPacketHeader& sendingPacketHeader, SerialByteQueue& sendingPacketSBQ);




/*
���� �迭ȭ
ProcessSendingFighterServerPacket �ɰ���
���� ���� ����ȭ
Broadcast ��������
Ÿ�̸� ����
�������
���ú����

�������� ���, �ؽ�Ʈ �α� - ����ť, ���ú�ť�� ���� ���� Ŭ���̾�Ʈ�� �ȹ޾Ҵٴ� �������� Ȯ���ϱ� ���� �α� ����, Ÿ�Ӿƿ��� ����
������ ó��
������ ���� ��� �ִ� �ּ� ����`
���� ���� - �� ���� ����, ����, �ð� ������ ��ǥ ����



192.168.10.34
*/