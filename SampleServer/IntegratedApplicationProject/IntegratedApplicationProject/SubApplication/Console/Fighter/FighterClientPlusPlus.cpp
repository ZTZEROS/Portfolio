#include "stdafx.h"
#include "PacketDefine.h"
#include "FighterClient.h"

int FighterRequestMoveStart(FighterClientSession& fighterClientSession)
{
	FighterPacketHeader fighterPacketHeader;
	SerialByteQueue sendingPacketSBQ;
	FighterPlayer fighterPlayer;
	
	fighterPacketHeader.PacketType = FIGHTER_REQUEST_MOVE_START;

	if (FIGHTER_PLAYER_UM.find(fighterClientSession.IdentificationNumber) != FIGHTER_PLAYER_UM.end())
	{
		fighterPlayer = FIGHTER_PLAYER_UM[fighterPlayer.IdentificationNumber];

		sendingPacketSBQ << fighterPlayer.Direction;
		sendingPacketSBQ << fighterPlayer.PositionX;
		sendingPacketSBQ << fighterPlayer.PositionY;

		if (CheckSendingFighterClientPacket(fighterPacketHeader, sendingPacketSBQ)) fighterClientSession.SendBQ.Enqueue(sendingPacketSBQ.GetQueueAddress(), sendingPacketSBQ.GetTotalSize());
	}

	return 0;
}

int FighterRequestMoveStop(FighterClientSession& fighterClientSession)
{
	FighterPacketHeader fighterPacketHeader;
	SerialByteQueue sendingPacketSBQ;
	FighterPlayer fighterPlayer;

	fighterPacketHeader.PacketType = FIGHTER_REQUEST_MOVE_STOP;

	if (FIGHTER_PLAYER_UM.find(fighterClientSession.IdentificationNumber) != FIGHTER_PLAYER_UM.end())
	{
		fighterPlayer = FIGHTER_PLAYER_UM[fighterPlayer.IdentificationNumber];

		sendingPacketSBQ << fighterPlayer.Direction;
		sendingPacketSBQ << fighterPlayer.PositionX;
		sendingPacketSBQ << fighterPlayer.PositionY;

		if (CheckSendingFighterClientPacket(fighterPacketHeader, sendingPacketSBQ)) fighterClientSession.SendBQ.Enqueue(sendingPacketSBQ.GetQueueAddress(), sendingPacketSBQ.GetTotalSize());
	}

	return 0;
}

int FighterRequestAttack1(FighterClientSession& fighterClientSession)
{
	FighterPacketHeader fighterPacketHeader;
	SerialByteQueue sendingPacketSBQ;
	FighterPlayer fighterPlayer;

	fighterPacketHeader.PacketType = FIGHTER_REQUEST_ATTACK1;

	if (FIGHTER_PLAYER_UM.find(fighterClientSession.IdentificationNumber) != FIGHTER_PLAYER_UM.end())
	{
		fighterPlayer = FIGHTER_PLAYER_UM[fighterPlayer.IdentificationNumber];

		sendingPacketSBQ << fighterPlayer.Direction;
		sendingPacketSBQ << fighterPlayer.PositionX;
		sendingPacketSBQ << fighterPlayer.PositionY;

		if (CheckSendingFighterClientPacket(fighterPacketHeader, sendingPacketSBQ)) fighterClientSession.SendBQ.Enqueue(sendingPacketSBQ.GetQueueAddress(), sendingPacketSBQ.GetTotalSize());
	}

	return 0;
}

int FighterRequestAttack2(FighterClientSession& fighterClientSession)
{
	FighterPacketHeader fighterPacketHeader;
	SerialByteQueue sendingPacketSBQ;
	FighterPlayer fighterPlayer;

	fighterPacketHeader.PacketType = FIGHTER_REQUEST_ATTACK2;

	if (FIGHTER_PLAYER_UM.find(fighterClientSession.IdentificationNumber) != FIGHTER_PLAYER_UM.end())
	{
		fighterPlayer = FIGHTER_PLAYER_UM[fighterPlayer.IdentificationNumber];

		sendingPacketSBQ << fighterPlayer.Direction;
		sendingPacketSBQ << fighterPlayer.PositionX;
		sendingPacketSBQ << fighterPlayer.PositionY;

		if (CheckSendingFighterClientPacket(fighterPacketHeader, sendingPacketSBQ)) fighterClientSession.SendBQ.Enqueue(sendingPacketSBQ.GetQueueAddress(), sendingPacketSBQ.GetTotalSize());
	}

	return 0;
}

int FighterRequestAttack3(FighterClientSession& fighterClientSession)
{
	FighterPacketHeader fighterPacketHeader;
	SerialByteQueue sendingPacketSBQ;
	FighterPlayer fighterPlayer;

	fighterPacketHeader.PacketType = FIGHTER_REQUEST_ATTACK3;

	if (FIGHTER_PLAYER_UM.find(fighterClientSession.IdentificationNumber) != FIGHTER_PLAYER_UM.end())
	{
		fighterPlayer = FIGHTER_PLAYER_UM[fighterPlayer.IdentificationNumber];

		sendingPacketSBQ << fighterPlayer.Direction;
		sendingPacketSBQ << fighterPlayer.PositionX;
		sendingPacketSBQ << fighterPlayer.PositionY;

		if (CheckSendingFighterClientPacket(fighterPacketHeader, sendingPacketSBQ)) fighterClientSession.SendBQ.Enqueue(sendingPacketSBQ.GetQueueAddress(), sendingPacketSBQ.GetTotalSize());
	}

	return 0;
}

int FighterRequestEcho(FighterClientSession& fighterClientSession)
{
	FighterPacketHeader fighterPacketHeader;
	SerialByteQueue sendingPacketSBQ;
	//DWORD time;

	fighterPacketHeader.PacketType = FIGHTER_REQUEST_ECHO;

	sendingPacketSBQ << (DWORD)time(nullptr);

	if (CheckSendingFighterClientPacket(fighterPacketHeader, sendingPacketSBQ)) fighterClientSession.SendBQ.Enqueue(sendingPacketSBQ.GetQueueAddress(), sendingPacketSBQ.GetTotalSize());

	return 0;
}



int FighterCreateMyCharacter(FighterClientSession& fighterClientSession, SerialByteQueue& receivedPacketSBQ)
{
	FighterPlayer fighterPlayer;

	receivedPacketSBQ >> fighterPlayer.IdentificationNumber;
	receivedPacketSBQ >> fighterPlayer.Direction;
	receivedPacketSBQ >> fighterPlayer.PositionX;
	receivedPacketSBQ >> fighterPlayer.PositionY;
	receivedPacketSBQ >> fighterPlayer.Life;

	if (FIGHTER_PLAYER_UM.find(fighterPlayer.IdentificationNumber) == FIGHTER_PLAYER_UM.end())
	{
		fighterClientSession.IdentificationNumber = fighterPlayer.IdentificationNumber;

		FIGHTER_PLAYER_UM[fighterPlayer.IdentificationNumber] = fighterPlayer;

		return 1;
	}

	return 0;
}

int FighterCreateOtherCharacter(FighterClientSession& fighterClientSession, SerialByteQueue& receivedPacketSBQ)
{
	FighterPlayer fighterPlayer;

	receivedPacketSBQ >> fighterPlayer.IdentificationNumber;
	receivedPacketSBQ >> fighterPlayer.Direction;
	receivedPacketSBQ >> fighterPlayer.PositionX;
	receivedPacketSBQ >> fighterPlayer.PositionY;
	receivedPacketSBQ >> fighterPlayer.Life;

	//어느 섹터에 추가해야 하는가?
	fighterPlayer.SectorIndexX = fighterPlayer.PositionX / FIGHTER_SECTOR_WIDTH;
	fighterPlayer.SectorIndexY = fighterPlayer.PositionY / FIGHTER_SECTOR_HEIGHT;

	FIGHTER_PLAYER_UM[fighterClientSession.IdentificationNumber].SectorIndexX;
	FIGHTER_PLAYER_UM[fighterClientSession.IdentificationNumber].SectorIndexY;

	//찾은 섹터에 이미 플레이어가 있지는 않은가?
	if(0)
	{
		//찾은 섹터의 플레이어 리스트에 플레이어 정보를 추가

		return 1;
	}

	return 0;
}

int FighterDeleteOtherCharacter(FighterClientSession& fighterClientSession, SerialByteQueue& receivedPacketSBQ)
{
	DWORD identificationNumber;

	receivedPacketSBQ >> identificationNumber;

	//주변 섹터에서 플레이어 검색
	if (0)
	{
		//찾은 섹터의 플레이어 리스트에서 플레이어 정보를 삭제

		return 1;
	}

	return 0;
}

int FighterMoveStart(FighterClientSession& fighterClientSession, SerialByteQueue& receivedPacketSBQ)
{
	FighterPlayer fighterPlayer;

	fighterPlayer.ActionIndex = FIGHTER_PLAYER_ACTION_MOVE_START;

	receivedPacketSBQ >> fighterPlayer.IdentificationNumber;
	receivedPacketSBQ >> fighterPlayer.Direction;
	receivedPacketSBQ >> fighterPlayer.PositionX;
	receivedPacketSBQ >> fighterPlayer.PositionY;

	QueryPerformanceCounter(&fighterPlayer.InitialPerformanceCounter);
	fighterPlayer.InitialPositionX = fighterPlayer.PositionX;
	fighterPlayer.InitialPositionY = fighterPlayer.PositionY;

	//어느 섹터의 플레이어인가?
	fighterPlayer.SectorIndexX = fighterPlayer.PositionX / FIGHTER_SECTOR_WIDTH;
	fighterPlayer.SectorIndexY = fighterPlayer.PositionY / FIGHTER_SECTOR_HEIGHT;

	FIGHTER_PLAYER_UM[fighterClientSession.IdentificationNumber].SectorIndexX;
	FIGHTER_PLAYER_UM[fighterClientSession.IdentificationNumber].SectorIndexY;

	//찾은 섹터에 플레이어가 없지는 않은가?
	if (0)
	{
		//찾은 섹터의 플레이어 리스트의 플레이어 데이터를 교정한다.

		return 1;
	}

	return 0;
}

int FighterMoveStop(FighterClientSession& fighterClientSession, SerialByteQueue& receivedPacketSBQ)
{
	FighterPlayer fighterPlayer;

	fighterPlayer.ActionIndex = FIGHTER_PLAYER_ACTION_MOVE_STOP;

	receivedPacketSBQ >> fighterPlayer.IdentificationNumber;
	receivedPacketSBQ >> fighterPlayer.Direction;
	receivedPacketSBQ >> fighterPlayer.PositionX;
	receivedPacketSBQ >> fighterPlayer.PositionY;

	//어느 섹터의 플레이어인가?
	fighterPlayer.SectorIndexX = fighterPlayer.PositionX / FIGHTER_SECTOR_WIDTH;
	fighterPlayer.SectorIndexY = fighterPlayer.PositionY / FIGHTER_SECTOR_HEIGHT;

	FIGHTER_PLAYER_UM[fighterClientSession.IdentificationNumber].SectorIndexX;
	FIGHTER_PLAYER_UM[fighterClientSession.IdentificationNumber].SectorIndexY;

	//찾은 섹터에 플레이어가 없지는 않은가?
	if (0)
	{
		//찾은 섹터의 플레이어 리스트의 플레이어 데이터를 교정한다.

		return 1;
	}


	return 0;
}

int FighterAttack1(FighterClientSession& fighterClientSession, SerialByteQueue& receivedPacketSBQ)
{
	FighterPlayer fighterPlayer;

	fighterPlayer.ActionIndex = FIGHTER_PLAYER_ACTION_ATTACK1;

	receivedPacketSBQ >> fighterPlayer.IdentificationNumber;
	receivedPacketSBQ >> fighterPlayer.Direction;
	receivedPacketSBQ >> fighterPlayer.PositionX;
	receivedPacketSBQ >> fighterPlayer.PositionY;

	//어느 섹터의 플레이어인가?
	fighterPlayer.SectorIndexX = fighterPlayer.PositionX / FIGHTER_SECTOR_WIDTH;
	fighterPlayer.SectorIndexY = fighterPlayer.PositionY / FIGHTER_SECTOR_HEIGHT;

	FIGHTER_PLAYER_UM[fighterClientSession.IdentificationNumber].SectorIndexX;
	FIGHTER_PLAYER_UM[fighterClientSession.IdentificationNumber].SectorIndexY;

	//찾은 섹터에 플레이어가 없지는 않은가?
	if (0)
	{
		//찾은 섹터의 플레이어 리스트의 플레이어 데이터를 교정한다.

		return 1;
	}

	return 0;
}

int FighterAttack2(FighterClientSession& fighterClientSession, SerialByteQueue& receivedPacketSBQ)
{
	FighterPlayer fighterPlayer;

	fighterPlayer.ActionIndex = FIGHTER_PLAYER_ACTION_ATTACK2;

	receivedPacketSBQ >> fighterPlayer.IdentificationNumber;
	receivedPacketSBQ >> fighterPlayer.Direction;
	receivedPacketSBQ >> fighterPlayer.PositionX;
	receivedPacketSBQ >> fighterPlayer.PositionY;

	//어느 섹터의 플레이어인가?
	fighterPlayer.SectorIndexX = fighterPlayer.PositionX / FIGHTER_SECTOR_WIDTH;
	fighterPlayer.SectorIndexY = fighterPlayer.PositionY / FIGHTER_SECTOR_HEIGHT;

	FIGHTER_PLAYER_UM[fighterClientSession.IdentificationNumber].SectorIndexX;
	FIGHTER_PLAYER_UM[fighterClientSession.IdentificationNumber].SectorIndexY;

	//찾은 섹터에 플레이어가 없지는 않은가?
	if (0)
	{
		//찾은 섹터의 플레이어 리스트의 플레이어 데이터를 교정한다.

		return 1;
	}

	return 0;
}

int FighterAttack3(FighterClientSession& fighterClientSession, SerialByteQueue& receivedPacketSBQ)
{
	FighterPlayer fighterPlayer;

	fighterPlayer.ActionIndex = FIGHTER_PLAYER_ACTION_ATTACK3;

	receivedPacketSBQ >> fighterPlayer.IdentificationNumber;
	receivedPacketSBQ >> fighterPlayer.Direction;
	receivedPacketSBQ >> fighterPlayer.PositionX;
	receivedPacketSBQ >> fighterPlayer.PositionY;

	//어느 섹터의 플레이어인가?
	fighterPlayer.SectorIndexX = fighterPlayer.PositionX / FIGHTER_SECTOR_WIDTH;
	fighterPlayer.SectorIndexY = fighterPlayer.PositionY / FIGHTER_SECTOR_HEIGHT;

	FIGHTER_PLAYER_UM[fighterClientSession.IdentificationNumber].SectorIndexX;
	FIGHTER_PLAYER_UM[fighterClientSession.IdentificationNumber].SectorIndexY;

	//찾은 섹터에 플레이어가 없지는 않은가?
	if (0)
	{
		//찾은 섹터의 플레이어 리스트의 플레이어 데이터를 교정한다.

		return 1;
	}

	return 0;
}

int FighterDamage(FighterClientSession& fighterClientSession, SerialByteQueue& receivedPacketSBQ)
{
	DWORD attackerIdentificationNumber;
	FighterPlayer attackeePlayer; //victim //target

	receivedPacketSBQ >> attackerIdentificationNumber;
	receivedPacketSBQ >> attackeePlayer.IdentificationNumber;
	receivedPacketSBQ >> attackeePlayer.Life;

	if (fighterClientSession.IdentificationNumber == attackeePlayer.IdentificationNumber)
	{
		FIGHTER_PLAYER_UM[fighterClientSession.IdentificationNumber].Life = attackeePlayer.Life;

		return 1;
	}
	else
	{
		//어느 섹터의 플레이어인가?
		attackeePlayer.SectorIndexX = attackeePlayer.PositionX / FIGHTER_SECTOR_WIDTH;
		attackeePlayer.SectorIndexY = attackeePlayer.PositionY / FIGHTER_SECTOR_HEIGHT;

		FIGHTER_PLAYER_UM[fighterClientSession.IdentificationNumber].SectorIndexX;
		FIGHTER_PLAYER_UM[fighterClientSession.IdentificationNumber].SectorIndexY;

		//찾은 섹터에 플레이어가 없지는 않은가?
		if (0)
		{
			//찾은 섹터의 플레이어 리스트의 플레이어 데이터를 교정한다.

			return 1;
		}
	}

	return 0;
}

int FighterSync(FighterClientSession& fighterClientSession, SerialByteQueue& receivedPacketSBQ)
{
	FighterPlayer fighterPlayer;

	receivedPacketSBQ >> fighterPlayer.IdentificationNumber;
	receivedPacketSBQ >> fighterPlayer.PositionX;
	receivedPacketSBQ >> fighterPlayer.PositionY;

	if (fighterPlayer.IdentificationNumber == fighterClientSession.IdentificationNumber)
	{
		FIGHTER_PLAYER_UM[fighterPlayer.IdentificationNumber].PositionX = fighterPlayer.PositionX;
		FIGHTER_PLAYER_UM[fighterPlayer.IdentificationNumber].PositionY = fighterPlayer.PositionY;

		return 1;
	}

	return 0;
}

int FighterEcho(FighterClientSession& fighterClientSession, SerialByteQueue& receivedPacketSBQ)
{
	DWORD time;

	receivedPacketSBQ >> time;

	//TIME - time;

	CalculateRoundTripTime();

	return 0;
}

void CalculateRoundTripTime()
{

}