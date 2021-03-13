#include "stdafx.h"
#include "PacketDefine.h"
#include "FighterServer.h"

int FighterResponseCreateMyCharacter(FighterClientSession& fighterClientSession, SerialByteQueue& receivedPacketSBQ)
{
	FighterPacketHeader sendingPacketHeader;
	SerialByteQueue sendingPacketSBQ;
	FighterPlayer fighterPlayer;

	sendingPacketHeader.PacketType = FIGHTER_RESPONSE_CREATE_MY_CHARACTER;

	sendingPacketSBQ.SetHeader(sendingPacketHeader);

	ZeroMemory(&fighterPlayer, sizeof(fighterPlayer));

	

	fighterPlayer.IdentificationNumber = fighterClientSession.IdentificationNumber;
	fighterPlayer.ActionIndex = FIGHTER_PLAYER_ACTION_MOVE_STOP;
	fighterPlayer.Direction = FIGHTER_MOVE_DIRECTION_LEFT;
	fighterPlayer.LeftRightDirection = fighterPlayer.Direction;
	fighterPlayer.PositionX = 1 + rand() % (FIGHTER_MAP_RANGE_RIGHT - 1);
	fighterPlayer.PositionY = 1 + rand() % (FIGHTER_MAP_RANGE_BOTTOM - 1);
	fighterPlayer.InitialPositionX = fighterPlayer.PositionX;
	fighterPlayer.InitialPositionY = fighterPlayer.PositionY;

	fighterPlayer.SectorIndexX = fighterPlayer.PositionX / FIGHTER_SECTOR_WIDTH;
	fighterPlayer.SectorIndexY = fighterPlayer.PositionY / FIGHTER_SECTOR_HEIGHT;
	fighterPlayer.PreviousSectorIndexX = fighterPlayer.SectorIndexX;
	fighterPlayer.PreviousSectorIndexY = fighterPlayer.SectorIndexY;
	fighterPlayer.Life = FIGHTER_PLAYER_LIFE;
	fighterPlayer.ClientSessionAddress = &fighterClientSession;

	FIGHTER_PLAYER_UM[fighterPlayer.IdentificationNumber] = fighterPlayer;



	FIGHTER_SECTOR_A[fighterPlayer.SectorIndexY][fighterPlayer.SectorIndexX].FighterPlayerL.push_back(&FIGHTER_PLAYER_UM[fighterPlayer.IdentificationNumber]);



	sendingPacketSBQ << fighterPlayer.IdentificationNumber;
	sendingPacketSBQ << fighterPlayer.Direction;
	sendingPacketSBQ << fighterPlayer.PositionX;
	sendingPacketSBQ << fighterPlayer.PositionY;
	sendingPacketSBQ << fighterPlayer.Life;

	if (CheckSendingFighterServerPacket(sendingPacketHeader, sendingPacketSBQ)) fighterClientSession.SendBQ.Enqueue(sendingPacketSBQ.GetQueueAddress(), sendingPacketSBQ.GetTotalSize());

	return 0;
}

int FighterResponseCreateOtherCharacter(FighterClientSession& fighterClientSession, SerialByteQueue& receivedPacketSBQ)
{
	FighterPacketHeader sendingPacketHeader;
	SerialByteQueue sendingPacketSBQ;
	FighterPlayer fighterPlayer;

	sendingPacketHeader.PacketType = FIGHTER_RESPONSE_CREATE_OTHER_CHARACTER;

	sendingPacketSBQ.SetHeader(sendingPacketHeader);

	fighterPlayer = FIGHTER_PLAYER_UM[fighterClientSession.IdentificationNumber];

	sendingPacketSBQ << fighterPlayer.IdentificationNumber;
	sendingPacketSBQ << fighterPlayer.LeftRightDirection;
	sendingPacketSBQ << fighterPlayer.PositionX;
	sendingPacketSBQ << fighterPlayer.PositionY;
	sendingPacketSBQ << fighterPlayer.Life;

	if (fighterPlayer.ActionIndex == FIGHTER_PLAYER_ACTION_MOVE_START)
	{
		FighterPacketHeader sendingSelfPacketHeader;
		SerialByteQueue sendingSelfPacketSBQ;
		FighterPlayerAddressL* fighterPlayerL[5];

		sendingSelfPacketHeader.PacketType = sendingPacketHeader.PacketType;

		sendingSelfPacketSBQ.SetHeader(sendingSelfPacketHeader);
		
		ZeroMemory(fighterPlayerL, sizeof(fighterPlayerL));

		if (CheckSendingFighterServerPacket(sendingPacketHeader, sendingPacketSBQ))
		{
			switch (3 * (fighterPlayer.SectorIndexX - fighterPlayer.PreviousSectorIndexX) + (fighterPlayer.SectorIndexY - fighterPlayer.PreviousSectorIndexY))
			{
			case FIGHTER_SECTOR_LEFT_UP:
				{
					if (FIGHTER_SECTOR_INDEX_LEFT < fighterPlayer.SectorIndexX && FIGHTER_SECTOR_INDEX_TOP < fighterPlayer.SectorIndexY)
					{
						fighterPlayerL[0] = &(FIGHTER_SECTOR_A[fighterPlayer.SectorIndexY - 1][fighterPlayer.SectorIndexX - 1].FighterPlayerL);
					}

					if (FIGHTER_SECTOR_INDEX_LEFT < fighterPlayer.SectorIndexX)
					{
						fighterPlayerL[1] = &(FIGHTER_SECTOR_A[fighterPlayer.SectorIndexY][fighterPlayer.SectorIndexX - 1].FighterPlayerL);
					}

					if (FIGHTER_SECTOR_INDEX_LEFT < fighterPlayer.SectorIndexX && fighterPlayer.SectorIndexY < FIGHTER_SECTOR_INDEX_BOTTOM)
					{
						fighterPlayerL[2] = &(FIGHTER_SECTOR_A[fighterPlayer.SectorIndexY + 1][fighterPlayer.SectorIndexX - 1].FighterPlayerL);
					}

					if (FIGHTER_SECTOR_INDEX_TOP < fighterPlayer.SectorIndexY)
					{
						fighterPlayerL[3] = &(FIGHTER_SECTOR_A[fighterPlayer.SectorIndexY - 1][fighterPlayer.SectorIndexX].FighterPlayerL);
					}

					if (fighterPlayer.SectorIndexX < FIGHTER_SECTOR_INDEX_RIGHT && FIGHTER_SECTOR_INDEX_TOP < fighterPlayer.SectorIndexY)
					{
						fighterPlayerL[4] = &(FIGHTER_SECTOR_A[fighterPlayer.SectorIndexY - 1][fighterPlayer.SectorIndexX + 1].FighterPlayerL);
					}
				}
				break;
			case FIGHTER_SECTOR_LEFT:
				{
					if (FIGHTER_SECTOR_INDEX_LEFT < fighterPlayer.SectorIndexX && FIGHTER_SECTOR_INDEX_TOP < fighterPlayer.SectorIndexY)
					{
						fighterPlayerL[0] = &(FIGHTER_SECTOR_A[fighterPlayer.SectorIndexY - 1][fighterPlayer.SectorIndexX - 1].FighterPlayerL);
					}

					if (FIGHTER_SECTOR_INDEX_LEFT < fighterPlayer.SectorIndexX)
					{
						fighterPlayerL[1] = &(FIGHTER_SECTOR_A[fighterPlayer.SectorIndexY][fighterPlayer.SectorIndexX - 1].FighterPlayerL);
					}

					if (FIGHTER_SECTOR_INDEX_LEFT < fighterPlayer.SectorIndexX && fighterPlayer.SectorIndexY < FIGHTER_SECTOR_INDEX_BOTTOM)
					{
						fighterPlayerL[2] = &(FIGHTER_SECTOR_A[fighterPlayer.SectorIndexY + 1][fighterPlayer.SectorIndexX - 1].FighterPlayerL);
					}
				}
				break;
			case FIGHTER_SECTOR_LEFT_DOWN:
				{
					if (FIGHTER_SECTOR_INDEX_LEFT < fighterPlayer.SectorIndexX && FIGHTER_SECTOR_INDEX_TOP < fighterPlayer.SectorIndexY)
					{
						fighterPlayerL[0] = &(FIGHTER_SECTOR_A[fighterPlayer.SectorIndexY - 1][fighterPlayer.SectorIndexX - 1].FighterPlayerL);
					}

					if (FIGHTER_SECTOR_INDEX_LEFT < fighterPlayer.SectorIndexX)
					{
						fighterPlayerL[1] = &(FIGHTER_SECTOR_A[fighterPlayer.SectorIndexY][fighterPlayer.SectorIndexX - 1].FighterPlayerL);
					}

					if (FIGHTER_SECTOR_INDEX_LEFT < fighterPlayer.SectorIndexX && fighterPlayer.SectorIndexY < FIGHTER_SECTOR_INDEX_BOTTOM)
					{
						fighterPlayerL[2] = &(FIGHTER_SECTOR_A[fighterPlayer.SectorIndexY + 1][fighterPlayer.SectorIndexX - 1].FighterPlayerL);
					}

					if (fighterPlayer.SectorIndexY < FIGHTER_SECTOR_INDEX_BOTTOM)
					{
						fighterPlayerL[3] = &(FIGHTER_SECTOR_A[fighterPlayer.SectorIndexY + 1][fighterPlayer.SectorIndexX].FighterPlayerL);
					}

					if (fighterPlayer.SectorIndexX < FIGHTER_SECTOR_INDEX_RIGHT && fighterPlayer.SectorIndexY < FIGHTER_SECTOR_INDEX_BOTTOM)
					{
						fighterPlayerL[4] = &(FIGHTER_SECTOR_A[fighterPlayer.SectorIndexY + 1][fighterPlayer.SectorIndexX + 1].FighterPlayerL);
					}
				}
				break;
			case FIGHTER_SECTOR_UP:
				{
					if (FIGHTER_SECTOR_INDEX_LEFT < fighterPlayer.SectorIndexX && FIGHTER_SECTOR_INDEX_TOP < fighterPlayer.SectorIndexY)
					{
						fighterPlayerL[0] = &(FIGHTER_SECTOR_A[fighterPlayer.SectorIndexY - 1][fighterPlayer.SectorIndexX - 1].FighterPlayerL);
					}

					if (FIGHTER_SECTOR_INDEX_TOP < fighterPlayer.SectorIndexY)
					{
						fighterPlayerL[1] = &(FIGHTER_SECTOR_A[fighterPlayer.SectorIndexY - 1][fighterPlayer.SectorIndexX].FighterPlayerL);
					}

					if (fighterPlayer.SectorIndexX < FIGHTER_SECTOR_INDEX_RIGHT && FIGHTER_SECTOR_INDEX_TOP < fighterPlayer.SectorIndexY)
					{
						fighterPlayerL[2] = &(FIGHTER_SECTOR_A[fighterPlayer.SectorIndexY - 1][fighterPlayer.SectorIndexX + 1].FighterPlayerL);
					}
				}
				break;
			case FIGHTER_SECTOR_DOWN:
				{
					if (FIGHTER_SECTOR_INDEX_LEFT < fighterPlayer.SectorIndexX && fighterPlayer.SectorIndexY < FIGHTER_SECTOR_INDEX_BOTTOM)
					{
						fighterPlayerL[0] = &(FIGHTER_SECTOR_A[fighterPlayer.SectorIndexY + 1][fighterPlayer.SectorIndexX - 1].FighterPlayerL);
					}

					if (fighterPlayer.SectorIndexY < FIGHTER_SECTOR_INDEX_BOTTOM)
					{
						fighterPlayerL[1] = &(FIGHTER_SECTOR_A[fighterPlayer.SectorIndexY + 1][fighterPlayer.SectorIndexX].FighterPlayerL);
					}

					if (fighterPlayer.SectorIndexX < FIGHTER_SECTOR_INDEX_RIGHT && fighterPlayer.SectorIndexY < FIGHTER_SECTOR_INDEX_BOTTOM)
					{
						fighterPlayerL[2] = &(FIGHTER_SECTOR_A[fighterPlayer.SectorIndexY + 1][fighterPlayer.SectorIndexX + 1].FighterPlayerL);
					}
				}
				break;
			case FIGHTER_SECTOR_RIGHT_UP:
				{
					if (fighterPlayer.SectorIndexX < FIGHTER_SECTOR_INDEX_RIGHT && FIGHTER_SECTOR_INDEX_TOP < fighterPlayer.SectorIndexY)
					{
						fighterPlayerL[0] = &(FIGHTER_SECTOR_A[fighterPlayer.SectorIndexY - 1][fighterPlayer.SectorIndexX + 1].FighterPlayerL);
					}

					if (fighterPlayer.SectorIndexX < FIGHTER_SECTOR_INDEX_RIGHT)
					{
						fighterPlayerL[1] = &(FIGHTER_SECTOR_A[fighterPlayer.SectorIndexY][fighterPlayer.SectorIndexX + 1].FighterPlayerL);
					}

					if (fighterPlayer.SectorIndexX < FIGHTER_SECTOR_INDEX_RIGHT && fighterPlayer.SectorIndexY < FIGHTER_SECTOR_INDEX_BOTTOM)
					{
						fighterPlayerL[2] = &(FIGHTER_SECTOR_A[fighterPlayer.SectorIndexY + 1][fighterPlayer.SectorIndexX + 1].FighterPlayerL);
					}

					if (FIGHTER_SECTOR_INDEX_TOP < fighterPlayer.SectorIndexY)
					{
						fighterPlayerL[3] = &(FIGHTER_SECTOR_A[fighterPlayer.SectorIndexY - 1][fighterPlayer.SectorIndexX].FighterPlayerL);
					}

					if (FIGHTER_SECTOR_INDEX_LEFT < fighterPlayer.SectorIndexX && FIGHTER_SECTOR_INDEX_TOP < fighterPlayer.SectorIndexY)
					{
						fighterPlayerL[4] = &(FIGHTER_SECTOR_A[fighterPlayer.SectorIndexY - 1][fighterPlayer.SectorIndexX - 1].FighterPlayerL);
					}
				}
				break;
			case FIGHTER_SECTOR_RIGHT:
				{
					if (fighterPlayer.SectorIndexX < FIGHTER_SECTOR_INDEX_RIGHT && FIGHTER_SECTOR_INDEX_TOP < fighterPlayer.SectorIndexY)
					{
						fighterPlayerL[0] = &(FIGHTER_SECTOR_A[fighterPlayer.SectorIndexY - 1][fighterPlayer.SectorIndexX + 1].FighterPlayerL);
					}

					if (fighterPlayer.SectorIndexX < FIGHTER_SECTOR_INDEX_RIGHT)
					{
						fighterPlayerL[1] = &(FIGHTER_SECTOR_A[fighterPlayer.SectorIndexY][fighterPlayer.SectorIndexX + 1].FighterPlayerL);
					}

					if (fighterPlayer.SectorIndexX < FIGHTER_SECTOR_INDEX_RIGHT && fighterPlayer.SectorIndexY < FIGHTER_SECTOR_INDEX_BOTTOM)
					{
						fighterPlayerL[2] = &(FIGHTER_SECTOR_A[fighterPlayer.SectorIndexY + 1][fighterPlayer.SectorIndexX + 1].FighterPlayerL);
					}
				}
				break;
			case FIGHTER_SECTOR_RIGHT_DOWN:
				{
					if (fighterPlayer.SectorIndexX < FIGHTER_SECTOR_INDEX_RIGHT && FIGHTER_SECTOR_INDEX_TOP < fighterPlayer.SectorIndexY)
					{
						fighterPlayerL[0] = &(FIGHTER_SECTOR_A[fighterPlayer.SectorIndexY - 1][fighterPlayer.SectorIndexX + 1].FighterPlayerL);
					}

					if (fighterPlayer.SectorIndexX < FIGHTER_SECTOR_INDEX_RIGHT)
					{
						fighterPlayerL[1] = &(FIGHTER_SECTOR_A[fighterPlayer.SectorIndexY][fighterPlayer.SectorIndexX + 1].FighterPlayerL);
					}

					if (fighterPlayer.SectorIndexX < FIGHTER_SECTOR_INDEX_RIGHT && fighterPlayer.SectorIndexY < FIGHTER_SECTOR_INDEX_BOTTOM)
					{
						fighterPlayerL[2] = &(FIGHTER_SECTOR_A[fighterPlayer.SectorIndexY + 1][fighterPlayer.SectorIndexX + 1].FighterPlayerL);
					}

					if (fighterPlayer.SectorIndexY < FIGHTER_SECTOR_INDEX_BOTTOM)
					{
						fighterPlayerL[4] = &(FIGHTER_SECTOR_A[fighterPlayer.SectorIndexY + 1][fighterPlayer.SectorIndexX].FighterPlayerL);
					}

					if (FIGHTER_SECTOR_INDEX_LEFT < fighterPlayer.SectorIndexX && fighterPlayer.SectorIndexY < FIGHTER_SECTOR_INDEX_BOTTOM)
					{
						fighterPlayerL[3] = &(FIGHTER_SECTOR_A[fighterPlayer.SectorIndexY + 1][fighterPlayer.SectorIndexX - 1].FighterPlayerL);
					}
				}
				break;
			default:
				break;
			}

			FOR(i, 5)
			{
				if (fighterPlayerL[i] && 0 < fighterPlayerL[i]->size())
				{
					for (FighterPlayerAddressLI fighterPlayerLI = fighterPlayerL[i]->begin(); fighterPlayerLI != fighterPlayerL[i]->end(); ++fighterPlayerLI)
					{
						++CREATE_OTHER_CHARACTER_COUNT;

						sendingSelfPacketSBQ.DecreaseCurrentSize(sendingSelfPacketSBQ.GetCurrentSize());

						(*fighterPlayerLI)->ClientSessionAddress->SendBQ.Enqueue(sendingPacketSBQ.GetQueueAddress(), sendingPacketSBQ.GetTotalSize());

						sendingSelfPacketSBQ << (*fighterPlayerLI)->IdentificationNumber;
						sendingSelfPacketSBQ << (*fighterPlayerLI)->Direction;
						sendingSelfPacketSBQ << (*fighterPlayerLI)->PositionX;
						sendingSelfPacketSBQ << (*fighterPlayerLI)->PositionY;
						sendingSelfPacketSBQ << (*fighterPlayerLI)->Life;

						if (CheckSendingFighterServerPacket(sendingSelfPacketHeader, sendingSelfPacketSBQ)) fighterClientSession.SendBQ.Enqueue(sendingSelfPacketSBQ.GetQueueAddress(), sendingSelfPacketSBQ.GetTotalSize());
					}
				}
			}
		}
	}
	else
	{
		BroadcastAndReverseUnicastFighter(fighterClientSession, sendingPacketHeader, sendingPacketSBQ);
	}

	return 0;
}

int FighterResponseDeleteOtherCharacter(FighterClientSession& fighterClientSession, SerialByteQueue& receivedPacketSBQ)
{
	FighterPacketHeader sendingPacketHeader;
	SerialByteQueue sendingPacketSBQ;
	FighterPlayer fighterPlayer;

	sendingPacketHeader.PacketType = FIGHTER_RESPONSE_DELETE_OTHER_CHARACTER;

	sendingPacketSBQ.SetHeader(sendingPacketHeader);

	fighterPlayer = FIGHTER_PLAYER_UM[fighterClientSession.IdentificationNumber];

	sendingPacketSBQ << fighterPlayer.IdentificationNumber;

	{
		FighterPacketHeader sendingSelfPacketHeader;
		SerialByteQueue sendingSelfPacketSBQ;
		FighterPlayerAddressL* fighterPlayerL[5];

		sendingSelfPacketHeader.PacketType = sendingPacketHeader.PacketType;

		sendingSelfPacketSBQ.SetHeader(sendingSelfPacketHeader);

		ZeroMemory(fighterPlayerL, sizeof(fighterPlayerL));

		if (CheckSendingFighterServerPacket(sendingPacketHeader, sendingPacketSBQ))
		{
			switch (3 * (fighterPlayer.SectorIndexX - fighterPlayer.PreviousSectorIndexX) + (fighterPlayer.SectorIndexY - fighterPlayer.PreviousSectorIndexY))
			{
			case FIGHTER_SECTOR_LEFT_UP:
				{
					if (fighterPlayer.PreviousSectorIndexX < FIGHTER_SECTOR_INDEX_RIGHT && FIGHTER_SECTOR_INDEX_TOP < fighterPlayer.PreviousSectorIndexY)
					{
						fighterPlayerL[0] = &(FIGHTER_SECTOR_A[fighterPlayer.PreviousSectorIndexY - 1][fighterPlayer.PreviousSectorIndexX + 1].FighterPlayerL);
					}

					if (fighterPlayer.PreviousSectorIndexX < FIGHTER_SECTOR_INDEX_RIGHT)
					{
						fighterPlayerL[1] = &(FIGHTER_SECTOR_A[fighterPlayer.PreviousSectorIndexY][fighterPlayer.PreviousSectorIndexX + 1].FighterPlayerL);
					}

					if (fighterPlayer.PreviousSectorIndexX < FIGHTER_SECTOR_INDEX_RIGHT && fighterPlayer.PreviousSectorIndexY < FIGHTER_SECTOR_INDEX_BOTTOM)
					{
						fighterPlayerL[2] = &(FIGHTER_SECTOR_A[fighterPlayer.PreviousSectorIndexY + 1][fighterPlayer.PreviousSectorIndexX + 1].FighterPlayerL);
					}

					if (FIGHTER_SECTOR_INDEX_LEFT < fighterPlayer.PreviousSectorIndexX && fighterPlayer.PreviousSectorIndexY < FIGHTER_SECTOR_INDEX_BOTTOM)
					{
						fighterPlayerL[3] = &(FIGHTER_SECTOR_A[fighterPlayer.PreviousSectorIndexY + 1][fighterPlayer.PreviousSectorIndexX].FighterPlayerL);
					}

					if (fighterPlayer.PreviousSectorIndexY < FIGHTER_SECTOR_INDEX_BOTTOM)
					{
						fighterPlayerL[4] = &(FIGHTER_SECTOR_A[fighterPlayer.PreviousSectorIndexY + 1][fighterPlayer.PreviousSectorIndexX - 1].FighterPlayerL);
					}
				}
				break;
			case FIGHTER_SECTOR_LEFT:
				{
					if (fighterPlayer.PreviousSectorIndexX < FIGHTER_SECTOR_INDEX_RIGHT && FIGHTER_SECTOR_INDEX_TOP < fighterPlayer.PreviousSectorIndexY)
					{
						fighterPlayerL[0] = &(FIGHTER_SECTOR_A[fighterPlayer.PreviousSectorIndexY - 1][fighterPlayer.PreviousSectorIndexX + 1].FighterPlayerL);
					}

					if (fighterPlayer.PreviousSectorIndexX < FIGHTER_SECTOR_INDEX_RIGHT)
					{
						fighterPlayerL[1] = &(FIGHTER_SECTOR_A[fighterPlayer.PreviousSectorIndexY][fighterPlayer.PreviousSectorIndexX + 1].FighterPlayerL);
					}

					if (fighterPlayer.PreviousSectorIndexX < FIGHTER_SECTOR_INDEX_RIGHT && fighterPlayer.PreviousSectorIndexY < FIGHTER_SECTOR_INDEX_BOTTOM)
					{
						fighterPlayerL[2] = &(FIGHTER_SECTOR_A[fighterPlayer.PreviousSectorIndexY + 1][fighterPlayer.PreviousSectorIndexX + 1].FighterPlayerL);
					}
				}
				break;
			case FIGHTER_SECTOR_LEFT_DOWN:
				{
					if (fighterPlayer.PreviousSectorIndexX < FIGHTER_SECTOR_INDEX_RIGHT && FIGHTER_SECTOR_INDEX_TOP < fighterPlayer.PreviousSectorIndexY)
					{
						fighterPlayerL[0] = &(FIGHTER_SECTOR_A[fighterPlayer.PreviousSectorIndexY - 1][fighterPlayer.PreviousSectorIndexX + 1].FighterPlayerL);
					}

					if (fighterPlayer.PreviousSectorIndexX < FIGHTER_SECTOR_INDEX_RIGHT)
					{
						fighterPlayerL[1] = &(FIGHTER_SECTOR_A[fighterPlayer.PreviousSectorIndexY][fighterPlayer.PreviousSectorIndexX + 1].FighterPlayerL);
					}

					if (fighterPlayer.PreviousSectorIndexX < FIGHTER_SECTOR_INDEX_RIGHT && fighterPlayer.PreviousSectorIndexY < FIGHTER_SECTOR_INDEX_BOTTOM)
					{
						fighterPlayerL[2] = &(FIGHTER_SECTOR_A[fighterPlayer.PreviousSectorIndexY + 1][fighterPlayer.PreviousSectorIndexX + 1].FighterPlayerL);
					}

					if (FIGHTER_SECTOR_INDEX_LEFT < fighterPlayer.PreviousSectorIndexX && FIGHTER_SECTOR_INDEX_TOP < fighterPlayer.PreviousSectorIndexY)
					{
						fighterPlayerL[3] = &(FIGHTER_SECTOR_A[fighterPlayer.PreviousSectorIndexY - 1][fighterPlayer.PreviousSectorIndexX - 1].FighterPlayerL);
					}

					if (FIGHTER_SECTOR_INDEX_TOP < fighterPlayer.PreviousSectorIndexY)
					{
						fighterPlayerL[4] = &(FIGHTER_SECTOR_A[fighterPlayer.PreviousSectorIndexY - 1][fighterPlayer.PreviousSectorIndexX].FighterPlayerL);
					}
				}
				break;
			case FIGHTER_SECTOR_UP:
				{
					if (FIGHTER_SECTOR_INDEX_LEFT < fighterPlayer.PreviousSectorIndexX && fighterPlayer.PreviousSectorIndexY < FIGHTER_SECTOR_INDEX_BOTTOM)
					{
						fighterPlayerL[0] = &(FIGHTER_SECTOR_A[fighterPlayer.PreviousSectorIndexY + 1][fighterPlayer.PreviousSectorIndexX - 1].FighterPlayerL);
					}

					if (fighterPlayer.PreviousSectorIndexY < FIGHTER_SECTOR_INDEX_BOTTOM)
					{
						fighterPlayerL[1] = &(FIGHTER_SECTOR_A[fighterPlayer.PreviousSectorIndexY + 1][fighterPlayer.PreviousSectorIndexX].FighterPlayerL);
					}

					if (fighterPlayer.PreviousSectorIndexX < FIGHTER_SECTOR_INDEX_RIGHT && fighterPlayer.PreviousSectorIndexY < FIGHTER_SECTOR_INDEX_BOTTOM)
					{
						fighterPlayerL[2] = &(FIGHTER_SECTOR_A[fighterPlayer.PreviousSectorIndexY + 1][fighterPlayer.PreviousSectorIndexX + 1].FighterPlayerL);
					}
				}
				break;
			case FIGHTER_SECTOR_DOWN:
				{
					if (FIGHTER_SECTOR_INDEX_LEFT < fighterPlayer.PreviousSectorIndexX && FIGHTER_SECTOR_INDEX_TOP < fighterPlayer.PreviousSectorIndexY)
					{
						fighterPlayerL[0] = &(FIGHTER_SECTOR_A[fighterPlayer.PreviousSectorIndexY - 1][fighterPlayer.PreviousSectorIndexX - 1].FighterPlayerL);
					}

					if (FIGHTER_SECTOR_INDEX_TOP < fighterPlayer.PreviousSectorIndexY)
					{
						fighterPlayerL[1] = &(FIGHTER_SECTOR_A[fighterPlayer.PreviousSectorIndexY - 1][fighterPlayer.PreviousSectorIndexX].FighterPlayerL);
					}

					if (fighterPlayer.PreviousSectorIndexX < FIGHTER_SECTOR_INDEX_RIGHT && FIGHTER_SECTOR_INDEX_TOP < fighterPlayer.PreviousSectorIndexY)
					{
						fighterPlayerL[2] = &(FIGHTER_SECTOR_A[fighterPlayer.PreviousSectorIndexY - 1][fighterPlayer.PreviousSectorIndexX + 1].FighterPlayerL);
					}
				}
				break;
			case FIGHTER_SECTOR_RIGHT_UP:
				{
					if (FIGHTER_SECTOR_INDEX_LEFT < fighterPlayer.PreviousSectorIndexX && FIGHTER_SECTOR_INDEX_TOP < fighterPlayer.PreviousSectorIndexY)
					{
						fighterPlayerL[0] = &(FIGHTER_SECTOR_A[fighterPlayer.PreviousSectorIndexY - 1][fighterPlayer.PreviousSectorIndexX - 1].FighterPlayerL);
					}

					if (FIGHTER_SECTOR_INDEX_LEFT < fighterPlayer.PreviousSectorIndexX)
					{
						fighterPlayerL[1] = &(FIGHTER_SECTOR_A[fighterPlayer.PreviousSectorIndexY][fighterPlayer.PreviousSectorIndexX - 1].FighterPlayerL);
					}

					if (FIGHTER_SECTOR_INDEX_LEFT < fighterPlayer.PreviousSectorIndexX && fighterPlayer.PreviousSectorIndexY < FIGHTER_SECTOR_INDEX_BOTTOM)
					{
						fighterPlayerL[2] = &(FIGHTER_SECTOR_A[fighterPlayer.PreviousSectorIndexY + 1][fighterPlayer.PreviousSectorIndexX - 1].FighterPlayerL);
					}

					if (fighterPlayer.PreviousSectorIndexY < FIGHTER_SECTOR_INDEX_BOTTOM)
					{
						fighterPlayerL[3] = &(FIGHTER_SECTOR_A[fighterPlayer.PreviousSectorIndexY + 1][fighterPlayer.PreviousSectorIndexX].FighterPlayerL);
					}

					if (fighterPlayer.PreviousSectorIndexX < FIGHTER_SECTOR_INDEX_RIGHT && fighterPlayer.PreviousSectorIndexY < FIGHTER_SECTOR_INDEX_BOTTOM)
					{
						fighterPlayerL[4] = &(FIGHTER_SECTOR_A[fighterPlayer.PreviousSectorIndexY + 1][fighterPlayer.PreviousSectorIndexX + 1].FighterPlayerL);
					}
				}
				break;
			case FIGHTER_SECTOR_RIGHT:
				{
					if (FIGHTER_SECTOR_INDEX_LEFT < fighterPlayer.PreviousSectorIndexX && FIGHTER_SECTOR_INDEX_TOP < fighterPlayer.PreviousSectorIndexY)
					{
						fighterPlayerL[0] = &(FIGHTER_SECTOR_A[fighterPlayer.PreviousSectorIndexY - 1][fighterPlayer.PreviousSectorIndexX - 1].FighterPlayerL);
					}

					if (FIGHTER_SECTOR_INDEX_LEFT < fighterPlayer.PreviousSectorIndexX)
					{
						fighterPlayerL[1] = &(FIGHTER_SECTOR_A[fighterPlayer.PreviousSectorIndexY][fighterPlayer.PreviousSectorIndexX - 1].FighterPlayerL);
					}

					if (FIGHTER_SECTOR_INDEX_LEFT < fighterPlayer.PreviousSectorIndexX && fighterPlayer.PreviousSectorIndexY < FIGHTER_SECTOR_INDEX_BOTTOM)
					{
						fighterPlayerL[2] = &(FIGHTER_SECTOR_A[fighterPlayer.PreviousSectorIndexY + 1][fighterPlayer.PreviousSectorIndexX - 1].FighterPlayerL);
					}
				}
				break;
			case FIGHTER_SECTOR_RIGHT_DOWN:
				{
					if (FIGHTER_SECTOR_INDEX_LEFT < fighterPlayer.PreviousSectorIndexX && FIGHTER_SECTOR_INDEX_TOP < fighterPlayer.PreviousSectorIndexY)
					{
						fighterPlayerL[0] = &(FIGHTER_SECTOR_A[fighterPlayer.PreviousSectorIndexY - 1][fighterPlayer.PreviousSectorIndexX - 1].FighterPlayerL);
					}

					if (FIGHTER_SECTOR_INDEX_LEFT < fighterPlayer.PreviousSectorIndexX)
					{
						fighterPlayerL[1] = &(FIGHTER_SECTOR_A[fighterPlayer.PreviousSectorIndexY][fighterPlayer.PreviousSectorIndexX - 1].FighterPlayerL);
					}

					if (FIGHTER_SECTOR_INDEX_LEFT < fighterPlayer.PreviousSectorIndexX && fighterPlayer.PreviousSectorIndexY < FIGHTER_SECTOR_INDEX_BOTTOM)
					{
						fighterPlayerL[2] = &(FIGHTER_SECTOR_A[fighterPlayer.PreviousSectorIndexY + 1][fighterPlayer.PreviousSectorIndexX - 1].FighterPlayerL);
					}

					if (FIGHTER_SECTOR_INDEX_TOP < fighterPlayer.PreviousSectorIndexY)
					{
						fighterPlayerL[3] = &(FIGHTER_SECTOR_A[fighterPlayer.PreviousSectorIndexY - 1][fighterPlayer.PreviousSectorIndexX].FighterPlayerL);
					}

					if (fighterPlayer.PreviousSectorIndexX < FIGHTER_SECTOR_INDEX_RIGHT && FIGHTER_SECTOR_INDEX_TOP < fighterPlayer.PreviousSectorIndexY)
					{
						fighterPlayerL[4] = &(FIGHTER_SECTOR_A[fighterPlayer.PreviousSectorIndexY - 1][fighterPlayer.PreviousSectorIndexX + 1].FighterPlayerL);
					}
				}
				break;
			default:
				break;
			}

			FOR(i, 5)
			{
				if (fighterPlayerL[i] && 0 < fighterPlayerL[i]->size())
				{
					for (FighterPlayerAddressLI fighterPlayerLI = fighterPlayerL[i]->begin(); fighterPlayerLI != fighterPlayerL[i]->end(); ++fighterPlayerLI)
					{
						++DELETE_OTHER_CHARACTER_COUNT;

						sendingSelfPacketSBQ.DecreaseCurrentSize(sendingSelfPacketSBQ.GetCurrentSize());

						(*fighterPlayerLI)->ClientSessionAddress->SendBQ.Enqueue(sendingPacketSBQ.GetQueueAddress(), sendingPacketSBQ.GetTotalSize());

						sendingSelfPacketSBQ << (*fighterPlayerLI)->IdentificationNumber;

						if (CheckSendingFighterServerPacket(sendingSelfPacketHeader, sendingSelfPacketSBQ)) fighterClientSession.SendBQ.Enqueue(sendingSelfPacketSBQ.GetQueueAddress(), sendingSelfPacketSBQ.GetTotalSize());
					}
				}
			}
		}
	}

	return 0;
}

int FighterResponseMoveStart(FighterClientSession& fighterClientSession, SerialByteQueue& receivedPacketSBQ)
{
	FighterPacketHeader sendingPacketHeader;
	SerialByteQueue sendingPacketSBQ;
	FighterPlayer fighterPlayer;
	BYTE direction;

	++MOVE_START_COUNT;

	sendingPacketHeader.PacketType = FIGHTER_RESPONSE_MOVE_START;

	sendingPacketSBQ.SetHeader(sendingPacketHeader);

	if (0 < receivedPacketSBQ.GetTotalSize())
	{
		receivedPacketSBQ >> direction;
		//receivedPacketSBQ >> fighterPlayer.PositionX;
		//receivedPacketSBQ >> fighterPlayer.PositionY;
		//assume player position is same with server exactly.
		
		DeadReckoning(fighterClientSession, receivedPacketSBQ);

		fighterPlayer = FIGHTER_PLAYER_UM[fighterClientSession.IdentificationNumber];

		QueryPerformanceCounter(&fighterPlayer.InitialPerformanceCounter);

		fighterPlayer.Direction = direction;
		fighterPlayer.ActionIndex = FIGHTER_PLAYER_ACTION_MOVE_START;
		fighterPlayer.InitialPositionX = fighterPlayer.PositionX;
		fighterPlayer.InitialPositionY = fighterPlayer.PositionY;

		//fighterPlayer.PreviousSectorIndexX = fighterPlayer.SectorIndexX;
		//fighterPlayer.PreviousSectorIndexY = fighterPlayer.SectorIndexY;
		//fighterPlayer.SectorIndexX = fighterPlayer.PositionX / FIGHTER_SECTOR_WIDTH;
		//fighterPlayer.SectorIndexY = fighterPlayer.PositionY / FIGHTER_SECTOR_HEIGHT;

		FIGHTER_PLAYER_UM[fighterPlayer.IdentificationNumber] = fighterPlayer;
	}
	else fighterPlayer = FIGHTER_PLAYER_UM[fighterClientSession.IdentificationNumber];
	
	sendingPacketSBQ << fighterPlayer.IdentificationNumber;
	sendingPacketSBQ << fighterPlayer.Direction;
	sendingPacketSBQ << fighterPlayer.PositionX;
	sendingPacketSBQ << fighterPlayer.PositionY;

	BroadcastFighter(fighterClientSession, sendingPacketHeader, sendingPacketSBQ);

	return 0;
}

int FighterResponseMoveStop(FighterClientSession& fighterClientSession, SerialByteQueue& receivedPacketSBQ)
{
	FighterPacketHeader sendingPacketHeader;
	SerialByteQueue sendingPacketSBQ;
	FighterPlayer fighterPlayer;
	LARGE_INTEGER finalPerformacneCounter;
	LARGE_INTEGER elapsedPerformanceCounter;
	UINT elapsedFrameCount;
	BYTE leftRightDirection;
	WORD clientPositionX;
	WORD clientPositionY;
	WORD finalPositionX;
	WORD finalPositionY;

	++MOVE_STOP_COUNT;
	
	sendingPacketHeader.PacketType = FIGHTER_RESPONSE_MOVE_STOP;

	sendingPacketSBQ.SetHeader(sendingPacketHeader);
	
	receivedPacketSBQ >> leftRightDirection;

	DeadReckoning(fighterClientSession, receivedPacketSBQ);

	fighterPlayer = FIGHTER_PLAYER_UM[fighterClientSession.IdentificationNumber];
	
	sendingPacketSBQ << fighterPlayer.IdentificationNumber;
	sendingPacketSBQ << fighterPlayer.LeftRightDirection;
	sendingPacketSBQ << fighterPlayer.PositionX;
	sendingPacketSBQ << fighterPlayer.PositionY;

	BroadcastFighter(fighterClientSession, sendingPacketHeader, sendingPacketSBQ);

	return 0;
}

int FighterResponseAttack1(FighterClientSession& fighterClientSession, SerialByteQueue& receivedPacketSBQ)
{
	FighterPacketHeader sendingPacketHeader;
	SerialByteQueue sendingPacketSBQ;
	FighterPlayer fighterPlayer;
	BYTE leftRightDirection;

	++ATTACK1_COUNT;

	sendingPacketHeader.PacketType = FIGHTER_RESPONSE_ATTACK1;

	sendingPacketSBQ.SetHeader(sendingPacketHeader);

	receivedPacketSBQ >> leftRightDirection;

	DeadReckoning(fighterClientSession, receivedPacketSBQ);

	fighterPlayer = FIGHTER_PLAYER_UM[fighterClientSession.IdentificationNumber];

	//FighterResponseMoveStop(fighterClientSession, receivedPacketSBQ);

	sendingPacketSBQ << fighterPlayer.IdentificationNumber;
	sendingPacketSBQ << fighterPlayer.LeftRightDirection;
	sendingPacketSBQ << fighterPlayer.PositionX;
	sendingPacketSBQ << fighterPlayer.PositionY;

	BroadcastFighter(fighterClientSession, sendingPacketHeader, sendingPacketSBQ);

	//insert hit judgement
	//FighterResponseDamage(attackeePlayerClientSession, receivedPacketSBQ);

	return 0;
}

int FighterResponseAttack2(FighterClientSession& fighterClientSession, SerialByteQueue& receivedPacketSBQ)
{
	FighterPacketHeader sendingPacketHeader;
	SerialByteQueue sendingPacketSBQ;
	FighterPlayer fighterPlayer;
	BYTE leftRightDirection;

	++ATTACK2_COUNT;

	sendingPacketHeader.PacketType = FIGHTER_RESPONSE_ATTACK2;

	sendingPacketSBQ.SetHeader(sendingPacketHeader);

	receivedPacketSBQ >> leftRightDirection;

	DeadReckoning(fighterClientSession, receivedPacketSBQ);

	fighterPlayer = FIGHTER_PLAYER_UM[fighterClientSession.IdentificationNumber];

	//FighterResponseMoveStop(fighterClientSession, receivedPacketSBQ);

	sendingPacketSBQ << fighterPlayer.IdentificationNumber;
	sendingPacketSBQ << fighterPlayer.LeftRightDirection;
	sendingPacketSBQ << fighterPlayer.PositionX;
	sendingPacketSBQ << fighterPlayer.PositionY;

	BroadcastFighter(fighterClientSession, sendingPacketHeader, sendingPacketSBQ);

	//insert hit judgement
	//FighterResponseDamage(attackeePlayerClientSession, receivedPacketSBQ);

	return 0;
}

int FighterResponseAttack3(FighterClientSession& fighterClientSession, SerialByteQueue& receivedPacketSBQ)
{
	FighterPacketHeader sendingPacketHeader;
	SerialByteQueue sendingPacketSBQ;
	FighterPlayer fighterPlayer;
	BYTE leftRightDirection;

	++ATTACK3_COUNT;

	sendingPacketHeader.PacketType = FIGHTER_RESPONSE_ATTACK3;

	sendingPacketSBQ.SetHeader(sendingPacketHeader);

	receivedPacketSBQ >> leftRightDirection;

	DeadReckoning(fighterClientSession, receivedPacketSBQ);

	fighterPlayer = FIGHTER_PLAYER_UM[fighterClientSession.IdentificationNumber];

	//FighterResponseMoveStop(fighterClientSession, receivedPacketSBQ);

	sendingPacketSBQ << fighterPlayer.IdentificationNumber;
	sendingPacketSBQ << fighterPlayer.LeftRightDirection;
	sendingPacketSBQ << fighterPlayer.PositionX;
	sendingPacketSBQ << fighterPlayer.PositionY;

	BroadcastFighter(fighterClientSession, sendingPacketHeader, sendingPacketSBQ);

	//insert hit judgement
	//FighterResponseDamage(attackeePlayerClientSession, receivedPacketSBQ);

	return 0;
}

int FighterResponseDamage(FighterClientSession& fighterClientSession, SerialByteQueue& receivedPacketSBQ)
{
	FighterPacketHeader sendingPacketHeader;
	SerialByteQueue sendingPacketSBQ;

	sendingPacketHeader.PacketType = FIGHTER_RESPONSE_DAMAGE;

	sendingPacketSBQ.SetHeader(sendingPacketHeader);

	FighterPlayer fighterPlayer;

	fighterPlayer = FIGHTER_PLAYER_UM[fighterClientSession.IdentificationNumber];

	sendingPacketSBQ << fighterPlayer.IdentificationNumber;
	sendingPacketSBQ << fighterPlayer.LeftRightDirection;
	sendingPacketSBQ << fighterPlayer.PositionX;
	sendingPacketSBQ << fighterPlayer.PositionY;

	BroadcastFighter(fighterClientSession, sendingPacketHeader, sendingPacketSBQ);

	return 0;
}

int FighterResponseSynchronize(FighterClientSession& fighterClientSession, SerialByteQueue& receivedPacketSBQ)
{
	FighterPacketHeader sendingPacketHeader;
	SerialByteQueue sendingPacketSBQ;
	FighterPlayer fighterPlayer;
	static UINT syncCount;

	sendingPacketHeader.PacketType = FIGHTER_RESPONSE_SYNCHRONIZE;

	sendingPacketSBQ.SetHeader(sendingPacketHeader);

	fighterPlayer = FIGHTER_PLAYER_UM[fighterClientSession.IdentificationNumber];

	sendingPacketSBQ << fighterPlayer.IdentificationNumber;
	sendingPacketSBQ << fighterPlayer.PositionX;
	sendingPacketSBQ << fighterPlayer.PositionY;

	//BroadcastFighter(fighterClientSession, sendingPacketHeader, sendingPacketSBQ); //CalculateSector() before

	if (CheckSendingFighterServerPacket(sendingPacketHeader, sendingPacketSBQ)) fighterClientSession.SendBQ.Enqueue(sendingPacketSBQ.GetQueueAddress(), sendingPacketSBQ.GetTotalSize());

	printf("Sync %d : Player %d, Position [ %d, %d ] -> [ %d, %d ], Sector [ %d, %d ] -> [ %d, %d ], %d" CRALF, ++syncCount,
		fighterPlayer.IdentificationNumber, fighterPlayer.InitialPositionX, fighterPlayer.InitialPositionY, fighterPlayer.PositionX, fighterPlayer.PositionY,
		fighterPlayer.PreviousSectorIndexX, fighterPlayer.PreviousSectorIndexY, fighterPlayer.SectorIndexX, fighterPlayer.SectorIndexY, fighterPlayer.ActionIndex);

	return 0;
}

int FighterResponseEcho(FighterClientSession& fighterClientSession, SerialByteQueue& receivedPacketSBQ)
{
	FighterPacketHeader sendingPacketHeader;
	SerialByteQueue sendingPacketSBQ;

	DWORD time;

	sendingPacketHeader.PacketType = FIGHTER_RESPONSE_ECHO;

	sendingPacketSBQ.SetHeader(sendingPacketHeader);

	receivedPacketSBQ >> time;
	sendingPacketSBQ << time;

	if (CheckSendingFighterServerPacket(sendingPacketHeader, sendingPacketSBQ)) fighterClientSession.SendBQ.Enqueue(sendingPacketSBQ.GetQueueAddress(), sendingPacketSBQ.GetTotalSize());

	return 0;
}



void CalculateFighterSector(FighterClientSession& fighterClientSession, SerialByteQueue& receivedPacketSBQ)
{
	FighterPlayerUMI fighterPlayerMI;
	FighterPlayer fighterPlayer;

	fighterPlayerMI = FIGHTER_PLAYER_UM.find(fighterClientSession.IdentificationNumber);

	if (fighterPlayerMI != FIGHTER_PLAYER_UM.end())
	{
		fighterPlayer = fighterPlayerMI->second;

		if (fighterPlayer.ActionIndex == FIGHTER_PLAYER_ACTION_MOVE_START) //check moving or attacking
		{
			++CALCULATE_FIGHTER_SECTOR_COUNT;

			if (fighterPlayer.Direction == FIGHTER_MOVE_DIRECTION_LEFT_UP || fighterPlayer.Direction == FIGHTER_MOVE_DIRECTION_LEFT || fighterPlayer.Direction == FIGHTER_MOVE_DIRECTION_LEFT_DOWN)
			{
				fighterPlayer.LeftRightDirection = FIGHTER_MOVE_DIRECTION_LEFT;
			}
			else if (fighterPlayer.Direction == FIGHTER_MOVE_DIRECTION_RIGHT_UP || fighterPlayer.Direction == FIGHTER_MOVE_DIRECTION_RIGHT || fighterPlayer.Direction == FIGHTER_MOVE_DIRECTION_RIGHT_DOWN)
			{
				fighterPlayer.LeftRightDirection = FIGHTER_MOVE_DIRECTION_RIGHT;
			}

			switch (fighterPlayer.Direction)
			{
			case FIGHTER_MOVE_DIRECTION_LEFT:
				{
					if (FIGHTER_MOVE_HORIZONTAL_VELOCITY < fighterPlayer.PositionX)
					{
						fighterPlayer.PositionX -= FIGHTER_MOVE_HORIZONTAL_VELOCITY;
						fighterPlayer.PositionY;
					}
				}
				break;
			case FIGHTER_MOVE_DIRECTION_LEFT_UP:
				{
					if (FIGHTER_MOVE_HORIZONTAL_VELOCITY < fighterPlayer.PositionX && FIGHTER_MOVE_VERTICAL_VELOCITY < fighterPlayer.PositionY)
					{
						fighterPlayer.PositionX -= FIGHTER_MOVE_HORIZONTAL_VELOCITY;
						fighterPlayer.PositionY -= FIGHTER_MOVE_VERTICAL_VELOCITY;
					}
				}
				break;
			case FIGHTER_MOVE_DIRECTION_UP:
				{
					if (FIGHTER_MOVE_VERTICAL_VELOCITY < fighterPlayer.PositionY)
					{
						fighterPlayer.PositionX;
						fighterPlayer.PositionY -= FIGHTER_MOVE_VERTICAL_VELOCITY;
					}
				}
				break;
			case FIGHTER_MOVE_DIRECTION_RIGHT_UP:
				{
					if (fighterPlayer.PositionX < FIGHTER_MAP_RANGE_RIGHT - FIGHTER_MOVE_HORIZONTAL_VELOCITY && FIGHTER_MOVE_VERTICAL_VELOCITY < fighterPlayer.PositionY)
					{
						fighterPlayer.PositionX += FIGHTER_MOVE_HORIZONTAL_VELOCITY;
						fighterPlayer.PositionY -= FIGHTER_MOVE_VERTICAL_VELOCITY;
					}
				}
				break;
			case FIGHTER_MOVE_DIRECTION_RIGHT:
				{
					if (fighterPlayer.PositionX < FIGHTER_MAP_RANGE_RIGHT - FIGHTER_MOVE_HORIZONTAL_VELOCITY)
					{
						fighterPlayer.PositionX += FIGHTER_MOVE_HORIZONTAL_VELOCITY;
						fighterPlayer.PositionY;
					}
				}
				break;
			case FIGHTER_MOVE_DIRECTION_RIGHT_DOWN:
				{
					if (fighterPlayer.PositionX < FIGHTER_MAP_RANGE_RIGHT - FIGHTER_MOVE_HORIZONTAL_VELOCITY && fighterPlayer.PositionY < FIGHTER_MAP_RANGE_BOTTOM - FIGHTER_MOVE_VERTICAL_VELOCITY)
					{
						fighterPlayer.PositionX += FIGHTER_MOVE_HORIZONTAL_VELOCITY;
						fighterPlayer.PositionY += FIGHTER_MOVE_VERTICAL_VELOCITY;
					}
				}
				break;
			case FIGHTER_MOVE_DIRECTION_DOWN:
				{
					if (fighterPlayer.PositionY < FIGHTER_MAP_RANGE_BOTTOM - FIGHTER_MOVE_VERTICAL_VELOCITY)
					{
						fighterPlayer.PositionX;
						fighterPlayer.PositionY += FIGHTER_MOVE_VERTICAL_VELOCITY;
					}
				}
				break;
			case FIGHTER_MOVE_DIRECTION_LEFT_DOWN:
				{
					if (FIGHTER_MOVE_HORIZONTAL_VELOCITY < fighterPlayer.PositionX && fighterPlayer.PositionY < FIGHTER_MAP_RANGE_BOTTOM - FIGHTER_MOVE_VERTICAL_VELOCITY)
					{
						fighterPlayer.PositionX -= FIGHTER_MOVE_HORIZONTAL_VELOCITY;
						fighterPlayer.PositionY += FIGHTER_MOVE_VERTICAL_VELOCITY;
					}
				}
				break;
			default:
				break;
			}

			fighterPlayer.SectorIndexX = fighterPlayer.PositionX / FIGHTER_SECTOR_WIDTH;
			fighterPlayer.SectorIndexY = fighterPlayer.PositionY / FIGHTER_SECTOR_HEIGHT;

			//wprintf_s(L"[PLAYER] X %05d Y %05d\n", fighterPlayer.PositionX, fighterPlayer.PositionY);
		}

		fighterPlayerMI->second = fighterPlayer;

		//separate moving and sector change

		if (fighterPlayer.PreviousSectorIndexX != fighterPlayer.SectorIndexX || fighterPlayer.PreviousSectorIndexY != fighterPlayer.SectorIndexY)
		{
			FighterResponseDeleteOtherCharacter(fighterClientSession, receivedPacketSBQ);

			FIGHTER_SECTOR_A[fighterPlayer.PreviousSectorIndexY][fighterPlayer.PreviousSectorIndexX].FighterPlayerL.remove(&FIGHTER_PLAYER_UM[fighterPlayer.IdentificationNumber]);
			FIGHTER_SECTOR_A[fighterPlayer.SectorIndexY][fighterPlayer.SectorIndexX].FighterPlayerL.push_back(&FIGHTER_PLAYER_UM[fighterPlayer.IdentificationNumber]);

			FighterResponseCreateOtherCharacter(fighterClientSession, receivedPacketSBQ);

			if (fighterPlayer.ActionIndex == FIGHTER_PLAYER_ACTION_MOVE_START) FighterResponseMoveStart(fighterClientSession, receivedPacketSBQ);
		}

		fighterPlayer.PreviousSectorIndexX = fighterPlayer.SectorIndexX;
		fighterPlayer.PreviousSectorIndexY = fighterPlayer.SectorIndexY;

		fighterPlayerMI->second = fighterPlayer;
	}
}

void DeadReckoning(FighterClientSession& fighterClientSession, SerialByteQueue& receivedPacketSBQ) //slow?
{
	FighterPlayer fighterPlayer;
	LARGE_INTEGER finalPerformacneCounter;
	LARGE_INTEGER elapsedPerformanceCounter;
	UINT elapsedFrameCount;

	WORD clientPositionX;
	WORD clientPositionY;
	WORD finalPositionX;
	WORD finalPositionY;

	++DEADRECKONING_COUNT;

	fighterPlayer = FIGHTER_PLAYER_UM[fighterClientSession.IdentificationNumber];

	//start-start
	//start-stop
	//start-attack
	//stop-attack

	if (fighterPlayer.ActionIndex == FIGHTER_PLAYER_ACTION_MOVE_START)
	{
		receivedPacketSBQ >> clientPositionX;
		receivedPacketSBQ >> clientPositionY;

		//if (clientPositionX < FIGHTER_MAP_RANGE_LEFT || FIGHTER_MAP_RANGE_RIGHT < clientPositionX);
		//if (clientPositionY < FIGHTER_MAP_RANGE_TOP || FIGHTER_MAP_RANGE_BOTTOM < clientPositionY);

		fighterPlayer.ActionIndex = FIGHTER_PLAYER_ACTION_MOVE_STOP;

		if ((15 < clientPositionX - fighterPlayer.PositionX || clientPositionX - fighterPlayer.PositionX < -15)
			|| (10 < clientPositionY - fighterPlayer.PositionY || clientPositionY - fighterPlayer.PositionY < -10))
		{
			QueryPerformanceCounter(&finalPerformacneCounter);

			elapsedPerformanceCounter.QuadPart = finalPerformacneCounter.QuadPart - fighterPlayer.InitialPerformanceCounter.QuadPart;
			elapsedFrameCount = elapsedPerformanceCounter.QuadPart / TIMER.GetFramePeriod().QuadPart;

			switch (fighterPlayer.Direction)
			{
			case FIGHTER_MOVE_DIRECTION_LEFT:
				{
					finalPositionX = fighterPlayer.InitialPositionX - elapsedFrameCount * FIGHTER_MOVE_HORIZONTAL_VELOCITY;
					finalPositionY;
				}
				break;
			case FIGHTER_MOVE_DIRECTION_LEFT_UP:
				{
					finalPositionX = fighterPlayer.InitialPositionX - elapsedFrameCount * FIGHTER_MOVE_HORIZONTAL_VELOCITY;
					finalPositionY = fighterPlayer.InitialPositionY - elapsedFrameCount * FIGHTER_MOVE_VERTICAL_VELOCITY;
				}
				break;
			case FIGHTER_MOVE_DIRECTION_UP:
				{
					finalPositionX;
					finalPositionY = fighterPlayer.InitialPositionY - elapsedFrameCount * FIGHTER_MOVE_VERTICAL_VELOCITY;
				}
				break;
			case FIGHTER_MOVE_DIRECTION_RIGHT_UP:
				{
					finalPositionX = fighterPlayer.InitialPositionX + elapsedFrameCount * FIGHTER_MOVE_HORIZONTAL_VELOCITY;
					finalPositionY = fighterPlayer.InitialPositionY - elapsedFrameCount * FIGHTER_MOVE_VERTICAL_VELOCITY;
				}
				break;
			case FIGHTER_MOVE_DIRECTION_RIGHT:
				{
					finalPositionX = fighterPlayer.InitialPositionX + elapsedFrameCount * FIGHTER_MOVE_HORIZONTAL_VELOCITY;
					finalPositionY;
				}
				break;
			case FIGHTER_MOVE_DIRECTION_RIGHT_DOWN:
				{
					finalPositionX = fighterPlayer.InitialPositionX + elapsedFrameCount * FIGHTER_MOVE_HORIZONTAL_VELOCITY;
					finalPositionY = fighterPlayer.InitialPositionY + elapsedFrameCount * FIGHTER_MOVE_VERTICAL_VELOCITY;
				}
				break;
			case FIGHTER_MOVE_DIRECTION_DOWN:
				{
					finalPositionX;
					finalPositionY = fighterPlayer.InitialPositionY + elapsedFrameCount * FIGHTER_MOVE_VERTICAL_VELOCITY;
				}
				break;
			case FIGHTER_MOVE_DIRECTION_LEFT_DOWN:
				{
					finalPositionX = fighterPlayer.InitialPositionX - elapsedFrameCount * FIGHTER_MOVE_HORIZONTAL_VELOCITY;
					finalPositionY = fighterPlayer.InitialPositionY + elapsedFrameCount * FIGHTER_MOVE_VERTICAL_VELOCITY;
				}
				break;
			default:
				//LogError()
				break;
			}

			if (finalPositionX < FIGHTER_MAP_RANGE_LEFT || FIGHTER_MAP_RANGE_RIGHT < finalPositionX
				|| finalPositionY < FIGHTER_MAP_RANGE_TOP || FIGHTER_MAP_RANGE_BOTTOM < finalPositionY)
			{
				finalPositionX = fighterPlayer.PositionX; //cheating solution
				finalPositionY = fighterPlayer.PositionY; //cheating solution
			}

			//fighterPlayer.LeftRightDirection = leftRightdirection;

			if (15 < clientPositionX - finalPositionX || clientPositionX - finalPositionX < -15) fighterPlayer.PositionX = finalPositionX;
			else fighterPlayer.PositionX = clientPositionX; 

			if (10 < clientPositionY - finalPositionY || clientPositionY - finalPositionY < -10) fighterPlayer.PositionY = finalPositionY;
			else fighterPlayer.PositionY = clientPositionY;

			//fighterPlayer.PreviousSectorIndexX = fighterPlayer.SectorIndexX;
			//fighterPlayer.PreviousSectorIndexY = fighterPlayer.SectorIndexY;
			fighterPlayer.SectorIndexX = fighterPlayer.PositionX / FIGHTER_SECTOR_WIDTH;
			fighterPlayer.SectorIndexY = fighterPlayer.PositionY / FIGHTER_SECTOR_HEIGHT;

			FIGHTER_PLAYER_UM[fighterPlayer.IdentificationNumber] = fighterPlayer;

			FighterResponseSynchronize(fighterClientSession, receivedPacketSBQ);
		}
		else
		{
			//fighterPlayer.LeftRightDirection = leftRightdirection;

			fighterPlayer.PositionX = clientPositionX;
			fighterPlayer.PositionY = clientPositionY;

			//fighterPlayer.PreviousSectorIndexX = fighterPlayer.SectorIndexX;
			//fighterPlayer.PreviousSectorIndexY = fighterPlayer.SectorIndexY;
			fighterPlayer.SectorIndexX = fighterPlayer.PositionX / FIGHTER_SECTOR_WIDTH;
			fighterPlayer.SectorIndexY = fighterPlayer.PositionY / FIGHTER_SECTOR_HEIGHT;

			FIGHTER_PLAYER_UM[fighterPlayer.IdentificationNumber] = fighterPlayer;
		}

		CalculateFighterSector(fighterClientSession, receivedPacketSBQ);
	}
}

void PartialcastFighter(FighterClientSession& fighterClientSession, FighterPacketHeader& sendingPacketHeader, SerialByteQueue& sendingPacketSBQ)
{
	FighterPlayerAddressL* fighterPlayerL;
	FighterPlayer fighterPlayer;
	FighterPlayerUMI fighterPlayerMI;

	fighterPlayerMI = FIGHTER_PLAYER_UM.find(fighterClientSession.IdentificationNumber);

	if (fighterPlayerMI != FIGHTER_PLAYER_UM.end())
	{
		fighterPlayer = fighterPlayerMI->second;

		if (CheckSendingFighterServerPacket(sendingPacketHeader, sendingPacketSBQ))
		{
			switch (fighterPlayer.Direction)
			{
			case FIGHTER_MOVE_DIRECTION_LEFT:
				break;
			case FIGHTER_MOVE_DIRECTION_LEFT_UP:
				break;
			case FIGHTER_MOVE_DIRECTION_UP:
				break;
			case FIGHTER_MOVE_DIRECTION_RIGHT_UP:
				break;
			case FIGHTER_MOVE_DIRECTION_RIGHT:
				break;
			case FIGHTER_MOVE_DIRECTION_RIGHT_DOWN:
				break;
			case FIGHTER_MOVE_DIRECTION_DOWN:
				break;
			case FIGHTER_MOVE_DIRECTION_LEFT_DOWN:
				break;
			default:
				break;
			}
		}
	}
}

void BroadcastFighter(FighterClientSession& fighterClientSession, FighterPacketHeader& sendingPacketHeader, SerialByteQueue& sendingPacketSBQ)
{
	FighterPlayerAddressL* fighterPlayerL;
	FighterPlayer fighterPlayer;
	FighterPlayerUMI fighterPlayerMI;

	fighterPlayerMI = FIGHTER_PLAYER_UM.find(fighterClientSession.IdentificationNumber);

	if (fighterPlayerMI != FIGHTER_PLAYER_UM.end())
	{
		fighterPlayer = fighterPlayerMI->second;

		if (CheckSendingFighterServerPacket(sendingPacketHeader, sendingPacketSBQ))
		{
			if (FIGHTER_SECTOR_INDEX_LEFT < fighterPlayer.SectorIndexX && FIGHTER_SECTOR_INDEX_TOP < fighterPlayer.SectorIndexY)
			{
				fighterPlayerL = &(FIGHTER_SECTOR_A[fighterPlayer.SectorIndexY - 1][fighterPlayer.SectorIndexX - 1].FighterPlayerL);
				for (FighterPlayerAddressLI fighterPlayerLI = fighterPlayerL->begin(); fighterPlayerLI != fighterPlayerL->end(); ++fighterPlayerLI)
				{
					(*fighterPlayerLI)->ClientSessionAddress->SendBQ.Enqueue(sendingPacketSBQ.GetQueueAddress(), sendingPacketSBQ.GetTotalSize());
				}
			}

			if (FIGHTER_SECTOR_INDEX_LEFT < fighterPlayer.SectorIndexX)
			{
				fighterPlayerL = &(FIGHTER_SECTOR_A[fighterPlayer.SectorIndexY][fighterPlayer.SectorIndexX - 1].FighterPlayerL);
				for (FighterPlayerAddressLI fighterPlayerLI = fighterPlayerL->begin(); fighterPlayerLI != fighterPlayerL->end(); ++fighterPlayerLI)
				{
					(*fighterPlayerLI)->ClientSessionAddress->SendBQ.Enqueue(sendingPacketSBQ.GetQueueAddress(), sendingPacketSBQ.GetTotalSize());
				}
			}

			if (FIGHTER_SECTOR_INDEX_LEFT < fighterPlayer.SectorIndexX && fighterPlayer.SectorIndexY < FIGHTER_SECTOR_INDEX_BOTTOM)
			{
				fighterPlayerL = &(FIGHTER_SECTOR_A[fighterPlayer.SectorIndexY + 1][fighterPlayer.SectorIndexX - 1].FighterPlayerL);
				for (FighterPlayerAddressLI fighterPlayerLI = fighterPlayerL->begin(); fighterPlayerLI != fighterPlayerL->end(); ++fighterPlayerLI)
				{
					(*fighterPlayerLI)->ClientSessionAddress->SendBQ.Enqueue(sendingPacketSBQ.GetQueueAddress(), sendingPacketSBQ.GetTotalSize());
				}
			}

			if (FIGHTER_SECTOR_INDEX_TOP < fighterPlayer.SectorIndexY)
			{
				fighterPlayerL = &(FIGHTER_SECTOR_A[fighterPlayer.SectorIndexY - 1][fighterPlayer.SectorIndexX].FighterPlayerL);
				for (FighterPlayerAddressLI fighterPlayerLI = fighterPlayerL->begin(); fighterPlayerLI != fighterPlayerL->end(); ++fighterPlayerLI)
				{
					(*fighterPlayerLI)->ClientSessionAddress->SendBQ.Enqueue(sendingPacketSBQ.GetQueueAddress(), sendingPacketSBQ.GetTotalSize());
				}
			}

			if (1)
			{
				fighterPlayerL = &(FIGHTER_SECTOR_A[fighterPlayer.SectorIndexY][fighterPlayer.SectorIndexX].FighterPlayerL);
				for (FighterPlayerAddressLI fighterPlayerLI = fighterPlayerL->begin(); fighterPlayerLI != fighterPlayerL->end(); ++fighterPlayerLI)
				{
					if (fighterClientSession.IdentificationNumber != (*fighterPlayerLI)->IdentificationNumber)
					{
						(*fighterPlayerLI)->ClientSessionAddress->SendBQ.Enqueue(sendingPacketSBQ.GetQueueAddress(), sendingPacketSBQ.GetTotalSize());
					}
				}
			}

			if (fighterPlayer.SectorIndexY < FIGHTER_SECTOR_INDEX_BOTTOM)
			{
				fighterPlayerL = &(FIGHTER_SECTOR_A[fighterPlayer.SectorIndexY + 1][fighterPlayer.SectorIndexX].FighterPlayerL);
				for (FighterPlayerAddressLI fighterPlayerLI = fighterPlayerL->begin(); fighterPlayerLI != fighterPlayerL->end(); ++fighterPlayerLI)
				{
					(*fighterPlayerLI)->ClientSessionAddress->SendBQ.Enqueue(sendingPacketSBQ.GetQueueAddress(), sendingPacketSBQ.GetTotalSize());
				}
			}

			if (fighterPlayer.SectorIndexX < FIGHTER_SECTOR_INDEX_RIGHT && FIGHTER_SECTOR_INDEX_TOP < fighterPlayer.SectorIndexY)
			{
				fighterPlayerL = &(FIGHTER_SECTOR_A[fighterPlayer.SectorIndexY - 1][fighterPlayer.SectorIndexX + 1].FighterPlayerL);
				for (FighterPlayerAddressLI fighterPlayerLI = fighterPlayerL->begin(); fighterPlayerLI != fighterPlayerL->end(); ++fighterPlayerLI)
				{
					(*fighterPlayerLI)->ClientSessionAddress->SendBQ.Enqueue(sendingPacketSBQ.GetQueueAddress(), sendingPacketSBQ.GetTotalSize());
				}
			}

			if (fighterPlayer.SectorIndexX < FIGHTER_SECTOR_INDEX_RIGHT)
			{
				fighterPlayerL = &(FIGHTER_SECTOR_A[fighterPlayer.SectorIndexY][fighterPlayer.SectorIndexX + 1].FighterPlayerL);
				for (FighterPlayerAddressLI fighterPlayerLI = fighterPlayerL->begin(); fighterPlayerLI != fighterPlayerL->end(); ++fighterPlayerLI)
				{
					(*fighterPlayerLI)->ClientSessionAddress->SendBQ.Enqueue(sendingPacketSBQ.GetQueueAddress(), sendingPacketSBQ.GetTotalSize());
				}
			}

			if (fighterPlayer.SectorIndexX < FIGHTER_SECTOR_INDEX_RIGHT && fighterPlayer.SectorIndexY < FIGHTER_SECTOR_INDEX_BOTTOM)
			{
				fighterPlayerL = &(FIGHTER_SECTOR_A[fighterPlayer.SectorIndexY + 1][fighterPlayer.SectorIndexX + 1].FighterPlayerL);
				for (FighterPlayerAddressLI fighterPlayerLI = fighterPlayerL->begin(); fighterPlayerLI != fighterPlayerL->end(); ++fighterPlayerLI)
				{
					(*fighterPlayerLI)->ClientSessionAddress->SendBQ.Enqueue(sendingPacketSBQ.GetQueueAddress(), sendingPacketSBQ.GetTotalSize());
				}
			}
		}
	}
}

void ReverseUnicastFighter(FighterClientSession& fighterClientSession, FighterPacketHeader& sendingPacketHeader, SerialByteQueue& sendingPacketSBQ)
{
	FighterPacketHeader sendingSelfPacketHeader;
	SerialByteQueue sendingSelfPacketSBQ;

	FighterPlayerAddressL* fighterPlayerL;
	FighterPlayer fighterPlayer;
	FighterPlayer currentFighterPlayer;
	FighterPlayerUMI fighterPlayerMI;

	fighterPlayerMI = FIGHTER_PLAYER_UM.find(fighterClientSession.IdentificationNumber);

	if (fighterPlayerMI != FIGHTER_PLAYER_UM.end())
	{
		fighterPlayer = fighterPlayerMI->second;

		sendingSelfPacketHeader.PacketType = sendingPacketHeader.PacketType;

		sendingSelfPacketSBQ.SetHeader(sendingSelfPacketHeader);

		if (FIGHTER_SECTOR_INDEX_LEFT < fighterPlayer.SectorIndexX && FIGHTER_SECTOR_INDEX_TOP < fighterPlayer.SectorIndexY)
		{
			fighterPlayerL = &(FIGHTER_SECTOR_A[fighterPlayer.SectorIndexY - 1][fighterPlayer.SectorIndexX - 1].FighterPlayerL);
			for (FighterPlayerAddressLI fighterPlayerLI = fighterPlayerL->begin(); fighterPlayerLI != fighterPlayerL->end(); ++fighterPlayerLI)
			{
				sendingSelfPacketSBQ.DecreaseCurrentSize(sendingSelfPacketSBQ.GetCurrentSize());

				currentFighterPlayer = *(*fighterPlayerLI);

				sendingSelfPacketSBQ << currentFighterPlayer.IdentificationNumber;
				sendingSelfPacketSBQ << currentFighterPlayer.Direction;
				sendingSelfPacketSBQ << currentFighterPlayer.PositionX;
				sendingSelfPacketSBQ << currentFighterPlayer.PositionY;
				sendingSelfPacketSBQ << currentFighterPlayer.Life;

				if (CheckSendingFighterServerPacket(sendingSelfPacketHeader, sendingSelfPacketSBQ)) fighterClientSession.SendBQ.Enqueue(sendingSelfPacketSBQ.GetQueueAddress(), sendingSelfPacketSBQ.GetTotalSize());
			}
		}

		if (FIGHTER_SECTOR_INDEX_LEFT < fighterPlayer.SectorIndexX)
		{
			fighterPlayerL = &(FIGHTER_SECTOR_A[fighterPlayer.SectorIndexY][fighterPlayer.SectorIndexX - 1].FighterPlayerL);
			for (FighterPlayerAddressLI fighterPlayerLI = fighterPlayerL->begin(); fighterPlayerLI != fighterPlayerL->end(); ++fighterPlayerLI)
			{
				sendingSelfPacketSBQ.DecreaseCurrentSize(sendingSelfPacketSBQ.GetCurrentSize());

				currentFighterPlayer = *(*fighterPlayerLI);

				sendingSelfPacketSBQ << currentFighterPlayer.IdentificationNumber;
				sendingSelfPacketSBQ << currentFighterPlayer.Direction;
				sendingSelfPacketSBQ << currentFighterPlayer.PositionX;
				sendingSelfPacketSBQ << currentFighterPlayer.PositionY;
				sendingSelfPacketSBQ << currentFighterPlayer.Life;

				if (CheckSendingFighterServerPacket(sendingSelfPacketHeader, sendingSelfPacketSBQ)) fighterClientSession.SendBQ.Enqueue(sendingSelfPacketSBQ.GetQueueAddress(), sendingSelfPacketSBQ.GetTotalSize());
			}
		}

		if (FIGHTER_SECTOR_INDEX_LEFT < fighterPlayer.SectorIndexX && fighterPlayer.SectorIndexY < FIGHTER_SECTOR_INDEX_BOTTOM)
		{
			fighterPlayerL = &(FIGHTER_SECTOR_A[fighterPlayer.SectorIndexY + 1][fighterPlayer.SectorIndexX - 1].FighterPlayerL);
			for (FighterPlayerAddressLI fighterPlayerLI = fighterPlayerL->begin(); fighterPlayerLI != fighterPlayerL->end(); ++fighterPlayerLI)
			{
				sendingSelfPacketSBQ.DecreaseCurrentSize(sendingSelfPacketSBQ.GetCurrentSize());

				currentFighterPlayer = *(*fighterPlayerLI);

				sendingSelfPacketSBQ << currentFighterPlayer.IdentificationNumber;
				sendingSelfPacketSBQ << currentFighterPlayer.Direction;
				sendingSelfPacketSBQ << currentFighterPlayer.PositionX;
				sendingSelfPacketSBQ << currentFighterPlayer.PositionY;
				sendingSelfPacketSBQ << currentFighterPlayer.Life;

				if (CheckSendingFighterServerPacket(sendingSelfPacketHeader, sendingSelfPacketSBQ)) fighterClientSession.SendBQ.Enqueue(sendingSelfPacketSBQ.GetQueueAddress(), sendingSelfPacketSBQ.GetTotalSize());
			}
		}

		if (FIGHTER_SECTOR_INDEX_TOP < fighterPlayer.SectorIndexY)
		{
			fighterPlayerL = &(FIGHTER_SECTOR_A[fighterPlayer.SectorIndexY - 1][fighterPlayer.SectorIndexX].FighterPlayerL);
			for (FighterPlayerAddressLI fighterPlayerLI = fighterPlayerL->begin(); fighterPlayerLI != fighterPlayerL->end(); ++fighterPlayerLI)
			{
				sendingSelfPacketSBQ.DecreaseCurrentSize(sendingSelfPacketSBQ.GetCurrentSize());

				currentFighterPlayer = *(*fighterPlayerLI);

				sendingSelfPacketSBQ << currentFighterPlayer.IdentificationNumber;
				sendingSelfPacketSBQ << currentFighterPlayer.Direction;
				sendingSelfPacketSBQ << currentFighterPlayer.PositionX;
				sendingSelfPacketSBQ << currentFighterPlayer.PositionY;
				sendingSelfPacketSBQ << currentFighterPlayer.Life;

				if (CheckSendingFighterServerPacket(sendingSelfPacketHeader, sendingSelfPacketSBQ)) fighterClientSession.SendBQ.Enqueue(sendingSelfPacketSBQ.GetQueueAddress(), sendingSelfPacketSBQ.GetTotalSize());
			}
		}

		if (1)
		{
			fighterPlayerL = &(FIGHTER_SECTOR_A[fighterPlayer.SectorIndexY][fighterPlayer.SectorIndexX].FighterPlayerL);
			for (FighterPlayerAddressLI fighterPlayerLI = fighterPlayerL->begin(); fighterPlayerLI != fighterPlayerL->end(); ++fighterPlayerLI)
			{
				sendingSelfPacketSBQ.DecreaseCurrentSize(sendingSelfPacketSBQ.GetCurrentSize());

				currentFighterPlayer = *(*fighterPlayerLI);

				if (fighterClientSession.IdentificationNumber != currentFighterPlayer.IdentificationNumber)
				{
					sendingSelfPacketSBQ << currentFighterPlayer.IdentificationNumber;
					sendingSelfPacketSBQ << currentFighterPlayer.Direction;
					sendingSelfPacketSBQ << currentFighterPlayer.PositionX;
					sendingSelfPacketSBQ << currentFighterPlayer.PositionY;
					sendingSelfPacketSBQ << currentFighterPlayer.Life;

					if (CheckSendingFighterServerPacket(sendingSelfPacketHeader, sendingSelfPacketSBQ)) fighterClientSession.SendBQ.Enqueue(sendingSelfPacketSBQ.GetQueueAddress(), sendingSelfPacketSBQ.GetTotalSize());
				}
			}
		}

		if (fighterPlayer.SectorIndexY < FIGHTER_SECTOR_INDEX_BOTTOM)
		{
			fighterPlayerL = &(FIGHTER_SECTOR_A[fighterPlayer.SectorIndexY + 1][fighterPlayer.SectorIndexX].FighterPlayerL);
			for (FighterPlayerAddressLI fighterPlayerLI = fighterPlayerL->begin(); fighterPlayerLI != fighterPlayerL->end(); ++fighterPlayerLI)
			{
				sendingSelfPacketSBQ.DecreaseCurrentSize(sendingSelfPacketSBQ.GetCurrentSize());

				currentFighterPlayer = *(*fighterPlayerLI);

				sendingSelfPacketSBQ << currentFighterPlayer.IdentificationNumber;
				sendingSelfPacketSBQ << currentFighterPlayer.Direction;
				sendingSelfPacketSBQ << currentFighterPlayer.PositionX;
				sendingSelfPacketSBQ << currentFighterPlayer.PositionY;
				sendingSelfPacketSBQ << currentFighterPlayer.Life;

				if (CheckSendingFighterServerPacket(sendingSelfPacketHeader, sendingSelfPacketSBQ)) fighterClientSession.SendBQ.Enqueue(sendingSelfPacketSBQ.GetQueueAddress(), sendingSelfPacketSBQ.GetTotalSize());
			}
		}

		if (fighterPlayer.SectorIndexX < FIGHTER_SECTOR_INDEX_RIGHT && FIGHTER_SECTOR_INDEX_TOP < fighterPlayer.SectorIndexY)
		{
			fighterPlayerL = &(FIGHTER_SECTOR_A[fighterPlayer.SectorIndexY - 1][fighterPlayer.SectorIndexX + 1].FighterPlayerL);
			for (FighterPlayerAddressLI fighterPlayerLI = fighterPlayerL->begin(); fighterPlayerLI != fighterPlayerL->end(); ++fighterPlayerLI)
			{
				sendingSelfPacketSBQ.DecreaseCurrentSize(sendingSelfPacketSBQ.GetCurrentSize());

				currentFighterPlayer = *(*fighterPlayerLI);

				sendingSelfPacketSBQ << currentFighterPlayer.IdentificationNumber;
				sendingSelfPacketSBQ << currentFighterPlayer.Direction;
				sendingSelfPacketSBQ << currentFighterPlayer.PositionX;
				sendingSelfPacketSBQ << currentFighterPlayer.PositionY;
				sendingSelfPacketSBQ << currentFighterPlayer.Life;

				if (CheckSendingFighterServerPacket(sendingSelfPacketHeader, sendingSelfPacketSBQ)) fighterClientSession.SendBQ.Enqueue(sendingSelfPacketSBQ.GetQueueAddress(), sendingSelfPacketSBQ.GetTotalSize());
			}
		}

		if (fighterPlayer.SectorIndexX < FIGHTER_SECTOR_INDEX_RIGHT)
		{
			fighterPlayerL = &(FIGHTER_SECTOR_A[fighterPlayer.SectorIndexY][fighterPlayer.SectorIndexX + 1].FighterPlayerL);
			for (FighterPlayerAddressLI fighterPlayerLI = fighterPlayerL->begin(); fighterPlayerLI != fighterPlayerL->end(); ++fighterPlayerLI)
			{
				sendingSelfPacketSBQ.DecreaseCurrentSize(sendingSelfPacketSBQ.GetCurrentSize());

				currentFighterPlayer = *(*fighterPlayerLI);

				sendingSelfPacketSBQ << currentFighterPlayer.IdentificationNumber;
				sendingSelfPacketSBQ << currentFighterPlayer.Direction;
				sendingSelfPacketSBQ << currentFighterPlayer.PositionX;
				sendingSelfPacketSBQ << currentFighterPlayer.PositionY;
				sendingSelfPacketSBQ << currentFighterPlayer.Life;

				if (CheckSendingFighterServerPacket(sendingSelfPacketHeader, sendingSelfPacketSBQ)) fighterClientSession.SendBQ.Enqueue(sendingSelfPacketSBQ.GetQueueAddress(), sendingSelfPacketSBQ.GetTotalSize());
			}
		}

		if (fighterPlayer.SectorIndexX < FIGHTER_SECTOR_INDEX_RIGHT && fighterPlayer.SectorIndexY < FIGHTER_SECTOR_INDEX_BOTTOM)
		{
			fighterPlayerL = &(FIGHTER_SECTOR_A[fighterPlayer.SectorIndexY + 1][fighterPlayer.SectorIndexX + 1].FighterPlayerL);
			for (FighterPlayerAddressLI fighterPlayerLI = fighterPlayerL->begin(); fighterPlayerLI != fighterPlayerL->end(); ++fighterPlayerLI)
			{
				sendingSelfPacketSBQ.DecreaseCurrentSize(sendingSelfPacketSBQ.GetCurrentSize());

				currentFighterPlayer = *(*fighterPlayerLI);

				sendingSelfPacketSBQ << currentFighterPlayer.IdentificationNumber;
				sendingSelfPacketSBQ << currentFighterPlayer.Direction;
				sendingSelfPacketSBQ << currentFighterPlayer.PositionX;
				sendingSelfPacketSBQ << currentFighterPlayer.PositionY;
				sendingSelfPacketSBQ << currentFighterPlayer.Life;

				if (CheckSendingFighterServerPacket(sendingSelfPacketHeader, sendingSelfPacketSBQ)) fighterClientSession.SendBQ.Enqueue(sendingSelfPacketSBQ.GetQueueAddress(), sendingSelfPacketSBQ.GetTotalSize());
			}
		}
	}
}

void BroadcastAndReverseUnicastFighter(FighterClientSession& fighterClientSession, FighterPacketHeader& sendingPacketHeader, SerialByteQueue& sendingPacketSBQ)
{
	FighterPacketHeader sendingSelfPacketHeader;
	SerialByteQueue sendingSelfPacketSBQ;

	FighterPlayerAddressL* fighterPlayerL[9];
	FighterPlayer fighterPlayer;
	FighterPlayer currentFighterPlayer;
	FighterPlayerUMI fighterPlayerMI;

	ZeroMemory(fighterPlayerL, sizeof(fighterPlayerL));
	ZeroMemory(&fighterPlayer, sizeof(fighterPlayer));

	fighterPlayerMI = FIGHTER_PLAYER_UM.find(fighterClientSession.IdentificationNumber);

	if (fighterPlayerMI != FIGHTER_PLAYER_UM.end())
	{
		fighterPlayer = fighterPlayerMI->second;

		sendingSelfPacketHeader.PacketType = sendingPacketHeader.PacketType;

		sendingSelfPacketSBQ.SetHeader(sendingSelfPacketHeader);

		if (CheckSendingFighterServerPacket(sendingPacketHeader, sendingPacketSBQ))
		{
			if (FIGHTER_SECTOR_INDEX_LEFT < fighterPlayer.SectorIndexX && FIGHTER_SECTOR_INDEX_TOP < fighterPlayer.SectorIndexY)
			{
				fighterPlayerL[0] = &(FIGHTER_SECTOR_A[fighterPlayer.SectorIndexY - 1][fighterPlayer.SectorIndexX - 1].FighterPlayerL);
			}

			if (FIGHTER_SECTOR_INDEX_LEFT < fighterPlayer.SectorIndexX)
			{
				fighterPlayerL[1] = &(FIGHTER_SECTOR_A[fighterPlayer.SectorIndexY][fighterPlayer.SectorIndexX - 1].FighterPlayerL);
			}

			if (FIGHTER_SECTOR_INDEX_LEFT < fighterPlayer.SectorIndexX && fighterPlayer.SectorIndexY < FIGHTER_SECTOR_INDEX_BOTTOM)
			{
				fighterPlayerL[2] = &(FIGHTER_SECTOR_A[fighterPlayer.SectorIndexY + 1][fighterPlayer.SectorIndexX - 1].FighterPlayerL);
			}

			if (FIGHTER_SECTOR_INDEX_TOP < fighterPlayer.SectorIndexY)
			{
				fighterPlayerL[3] = &(FIGHTER_SECTOR_A[fighterPlayer.SectorIndexY - 1][fighterPlayer.SectorIndexX].FighterPlayerL);
			}

			if (1)
			{
				fighterPlayerL[4] = &(FIGHTER_SECTOR_A[fighterPlayer.SectorIndexY][fighterPlayer.SectorIndexX].FighterPlayerL);
				
				for (FighterPlayerAddressLI fighterPlayerLI = fighterPlayerL[4]->begin(); fighterPlayerLI != fighterPlayerL[4]->end(); ++fighterPlayerLI)
				{
					if (fighterClientSession.IdentificationNumber != (*fighterPlayerLI)->IdentificationNumber)
					{
						sendingSelfPacketSBQ.DecreaseCurrentSize(sendingSelfPacketSBQ.GetCurrentSize());

						(*fighterPlayerLI)->ClientSessionAddress->SendBQ.Enqueue(sendingPacketSBQ.GetQueueAddress(), sendingPacketSBQ.GetTotalSize());

						currentFighterPlayer = *(*fighterPlayerLI);

						sendingSelfPacketSBQ << currentFighterPlayer.IdentificationNumber;
						sendingSelfPacketSBQ << currentFighterPlayer.Direction;
						sendingSelfPacketSBQ << currentFighterPlayer.PositionX;
						sendingSelfPacketSBQ << currentFighterPlayer.PositionY;
						sendingSelfPacketSBQ << currentFighterPlayer.Life;

						if (CheckSendingFighterServerPacket(sendingSelfPacketHeader, sendingSelfPacketSBQ)) fighterClientSession.SendBQ.Enqueue(sendingSelfPacketSBQ.GetQueueAddress(), sendingSelfPacketSBQ.GetTotalSize());
					}
				}

				fighterPlayerL[4] = nullptr;
			}

			if (fighterPlayer.SectorIndexY < FIGHTER_SECTOR_INDEX_BOTTOM)
			{
				fighterPlayerL[5] = &(FIGHTER_SECTOR_A[fighterPlayer.SectorIndexY + 1][fighterPlayer.SectorIndexX].FighterPlayerL);
			}
			else fighterPlayerL[5] = nullptr;

			if (fighterPlayer.SectorIndexX < FIGHTER_SECTOR_INDEX_RIGHT && FIGHTER_SECTOR_INDEX_TOP < fighterPlayer.SectorIndexY)
			{
				fighterPlayerL[6] = &(FIGHTER_SECTOR_A[fighterPlayer.SectorIndexY - 1][fighterPlayer.SectorIndexX + 1].FighterPlayerL);
			}
			else fighterPlayerL[6] = nullptr;

			if (fighterPlayer.SectorIndexX < FIGHTER_SECTOR_INDEX_RIGHT)
			{
				fighterPlayerL[7] = &(FIGHTER_SECTOR_A[fighterPlayer.SectorIndexY][fighterPlayer.SectorIndexX + 1].FighterPlayerL);
			}
			else fighterPlayerL[7] = nullptr;

			if (fighterPlayer.SectorIndexX < FIGHTER_SECTOR_INDEX_RIGHT && fighterPlayer.SectorIndexY < FIGHTER_SECTOR_INDEX_BOTTOM)
			{
				fighterPlayerL[8] = &(FIGHTER_SECTOR_A[fighterPlayer.SectorIndexY + 1][fighterPlayer.SectorIndexX + 1].FighterPlayerL);
			}
			else fighterPlayerL[8] = nullptr;

			FOR(i, 9)
			{
				if (fighterPlayerL[i])
				{
					for (FighterPlayerAddressLI fighterPlayerLI = fighterPlayerL[i]->begin(); fighterPlayerLI != fighterPlayerL[i]->end(); ++fighterPlayerLI)
					{
						sendingSelfPacketSBQ.DecreaseCurrentSize(sendingSelfPacketSBQ.GetCurrentSize());

						(*fighterPlayerLI)->ClientSessionAddress->SendBQ.Enqueue(sendingPacketSBQ.GetQueueAddress(), sendingPacketSBQ.GetTotalSize());

						currentFighterPlayer = *(*fighterPlayerLI);

						sendingSelfPacketSBQ << currentFighterPlayer.IdentificationNumber;
						sendingSelfPacketSBQ << currentFighterPlayer.Direction;
						sendingSelfPacketSBQ << currentFighterPlayer.PositionX;
						sendingSelfPacketSBQ << currentFighterPlayer.PositionY;
						sendingSelfPacketSBQ << currentFighterPlayer.Life;

						if (CheckSendingFighterServerPacket(sendingSelfPacketHeader, sendingSelfPacketSBQ)) fighterClientSession.SendBQ.Enqueue(sendingSelfPacketSBQ.GetQueueAddress(), sendingSelfPacketSBQ.GetTotalSize());
					}
				}
			}
		}
	}
}


/*
void CalculateFighterPosition(FighterClientSession& fighterClientSession, SerialByteQueue& receivedPacketSBQ)
{
FighterPlayerUMI fighterPlayerMI;
FighterPlayer fighterPlayer;

++CALCULATE_FIGHTER_SECTOR_COUNT;

fighterPlayerMI = FIGHTER_PLAYER_UM.find(fighterClientSession.IdentificationNumber);

if (fighterPlayerMI != FIGHTER_PLAYER_UM.end())
{
fighterPlayer = fighterPlayerMI->second;

if (fighterPlayer.ActionIndex == FIGHTER_PLAYER_ACTION_MOVE_START) //check moving or attacking
{
if (fighterPlayer.Direction == FIGHTER_MOVE_DIRECTION_LEFT_UP || fighterPlayer.Direction == FIGHTER_MOVE_DIRECTION_LEFT || fighterPlayer.Direction == FIGHTER_MOVE_DIRECTION_LEFT_DOWN)
{
fighterPlayer.LeftRightDirection = FIGHTER_MOVE_DIRECTION_LEFT;
}
else if (fighterPlayer.Direction == FIGHTER_MOVE_DIRECTION_RIGHT_UP || fighterPlayer.Direction == FIGHTER_MOVE_DIRECTION_RIGHT || fighterPlayer.Direction == FIGHTER_MOVE_DIRECTION_RIGHT_DOWN)
{
fighterPlayer.LeftRightDirection = FIGHTER_MOVE_DIRECTION_RIGHT;
}

switch (fighterPlayer.Direction)
{
case FIGHTER_MOVE_DIRECTION_LEFT:
{
if (FIGHTER_MOVE_HORIZONTAL_VELOCITY < fighterPlayer.PositionX)
{
fighterPlayer.PositionX -= FIGHTER_MOVE_HORIZONTAL_VELOCITY;
fighterPlayer.PositionY;
}
}
break;
case FIGHTER_MOVE_DIRECTION_LEFT_UP:
{
if (FIGHTER_MOVE_HORIZONTAL_VELOCITY < fighterPlayer.PositionX && FIGHTER_MOVE_VERTICAL_VELOCITY < fighterPlayer.PositionY)
{
fighterPlayer.PositionX -= FIGHTER_MOVE_HORIZONTAL_VELOCITY;
fighterPlayer.PositionY -= FIGHTER_MOVE_VERTICAL_VELOCITY;
}
}
break;
case FIGHTER_MOVE_DIRECTION_UP:
{
if (FIGHTER_MOVE_VERTICAL_VELOCITY < fighterPlayer.PositionY)
{
fighterPlayer.PositionX;
fighterPlayer.PositionY -= FIGHTER_MOVE_VERTICAL_VELOCITY;
}
}
break;
case FIGHTER_MOVE_DIRECTION_RIGHT_UP:
{
if (fighterPlayer.PositionX < FIGHTER_MAP_RANGE_RIGHT - FIGHTER_MOVE_HORIZONTAL_VELOCITY && FIGHTER_MOVE_VERTICAL_VELOCITY < fighterPlayer.PositionY)
{
fighterPlayer.PositionX += FIGHTER_MOVE_HORIZONTAL_VELOCITY;
fighterPlayer.PositionY -= FIGHTER_MOVE_VERTICAL_VELOCITY;
}
}
break;
case FIGHTER_MOVE_DIRECTION_RIGHT:
{
if (fighterPlayer.PositionX < FIGHTER_MAP_RANGE_RIGHT - FIGHTER_MOVE_HORIZONTAL_VELOCITY)
{
fighterPlayer.PositionX += FIGHTER_MOVE_HORIZONTAL_VELOCITY;
fighterPlayer.PositionY;
}
}
break;
case FIGHTER_MOVE_DIRECTION_RIGHT_DOWN:
{
if (fighterPlayer.PositionX < FIGHTER_MAP_RANGE_RIGHT - FIGHTER_MOVE_HORIZONTAL_VELOCITY && fighterPlayer.PositionY < FIGHTER_MAP_RANGE_BOTTOM - FIGHTER_MOVE_VERTICAL_VELOCITY)
{
fighterPlayer.PositionX += FIGHTER_MOVE_HORIZONTAL_VELOCITY;
fighterPlayer.PositionY += FIGHTER_MOVE_VERTICAL_VELOCITY;
}
}
break;
case FIGHTER_MOVE_DIRECTION_DOWN:
{
if (fighterPlayer.PositionY < FIGHTER_MAP_RANGE_BOTTOM - FIGHTER_MOVE_VERTICAL_VELOCITY)
{
fighterPlayer.PositionX;
fighterPlayer.PositionY += FIGHTER_MOVE_VERTICAL_VELOCITY;
}
}
break;
case FIGHTER_MOVE_DIRECTION_LEFT_DOWN:
{
if (FIGHTER_MOVE_HORIZONTAL_VELOCITY < fighterPlayer.PositionX && fighterPlayer.PositionY < FIGHTER_MAP_RANGE_BOTTOM - FIGHTER_MOVE_VERTICAL_VELOCITY)
{
fighterPlayer.PositionX -= FIGHTER_MOVE_HORIZONTAL_VELOCITY;
fighterPlayer.PositionY += FIGHTER_MOVE_VERTICAL_VELOCITY;
}
}
break;
default:
break;
}

fighterPlayer.SectorIndexX = fighterPlayer.PositionX / FIGHTER_SECTOR_WIDTH;
fighterPlayer.SectorIndexY = fighterPlayer.PositionY / FIGHTER_SECTOR_HEIGHT;
}

fighterPlayerMI->second = fighterPlayer;

CalculateFighterSector(fighterClientSession, receivedPacketSBQ);
}
}

void CalculateFighterSector(FighterClientSession& fighterClientSession, SerialByteQueue& receivedPacketSBQ)
{
FighterPlayerUMI fighterPlayerMI;
FighterPlayer fighterPlayer;

++CALCULATE_FIGHTER_SECTOR_COUNT;

fighterPlayerMI = FIGHTER_PLAYER_UM.find(fighterClientSession.IdentificationNumber);

if (fighterPlayerMI != FIGHTER_PLAYER_UM.end())
{
fighterPlayer = fighterPlayerMI->second;

if (fighterPlayer.PreviousSectorIndexX != fighterPlayer.SectorIndexX || fighterPlayer.PreviousSectorIndexY != fighterPlayer.SectorIndexY)
{
FighterResponseDeleteOtherCharacter(fighterClientSession, receivedPacketSBQ);

FIGHTER_SECTOR_A[fighterPlayer.PreviousSectorIndexY][fighterPlayer.PreviousSectorIndexX].FighterPlayerL.remove(&FIGHTER_PLAYER_UM[fighterPlayer.IdentificationNumber]);
FIGHTER_SECTOR_A[fighterPlayer.SectorIndexY][fighterPlayer.SectorIndexX].FighterPlayerL.push_back(&FIGHTER_PLAYER_UM[fighterPlayer.IdentificationNumber]);

FighterResponseCreateOtherCharacter(fighterClientSession, receivedPacketSBQ);

if (fighterPlayer.ActionIndex == FIGHTER_PLAYER_ACTION_MOVE_START) FighterResponseMoveStart(fighterClientSession, receivedPacketSBQ);
}

fighterPlayer.PreviousSectorIndexX = fighterPlayer.SectorIndexX;
fighterPlayer.PreviousSectorIndexY = fighterPlayer.SectorIndexY;

fighterPlayerMI->second = fighterPlayer;
}
}
*/