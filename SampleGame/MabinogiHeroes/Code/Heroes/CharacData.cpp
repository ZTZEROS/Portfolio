#include "stdafx.h"
#include "CharacData.h"


CharacData::CharacData()
{
}


CharacData::~CharacData()
{
}

void CharacData::Initialize()
{
	player = new Lethita;
	player->InitXFile();

	chUnEquipInfo.resize(CH_END);
	chEquipInfo.resize(CH_END);
	chBattleInfo.resize(CH_END);

	chUnEquipInfo[CH_LEVEL] = 1;
	chUnEquipInfo[CH_EXP] = 0;
	chUnEquipInfo[CH_GOLD] = 100;
	chUnEquipInfo[CH_HP] = 100 + chUnEquipInfo[CH_LEVEL]*20;
	chUnEquipInfo[CH_MP] = 100 + chUnEquipInfo[CH_LEVEL] * 20;
	chUnEquipInfo[CH_SP] = 100 + chUnEquipInfo[CH_LEVEL] * 10;
	chUnEquipInfo[CH_ATK] = 10 + chUnEquipInfo[CH_LEVEL];
	chUnEquipInfo[CH_DEF] = 10 + chUnEquipInfo[CH_LEVEL];

	for (int i = 0; i < Item_Kind::Item_End ; ++i)
	{
		chEquipName[i] = "Null";
	}

	for (int i = 0; i < CH_END; ++i)
	{
		chEquipInfo[i] = chUnEquipInfo[i];
	}
	InfoUpdate();
}

void CharacData::Destroy()
{
	chUnEquipInfo.clear();
	chEquipInfo.clear();
	chBattleInfo.clear();
	SAFE_DELETE(player);
}

//void CharacData::SetInstanceEqip(Item_Kind kind)
//{
//	
//}

void CharacData::InfoUpdate()
{
	for (int i = 0; i < CH_INFOMATION::CH_END; ++i)
	{
		chBattleInfo[i] = chEquipInfo[i];
	}
}

void CharacData::SetInstanceInfomation(CH_INFOMATION type, int num, char* name /*= nullptr*/)
{
	if (type == CH_GOLD) chUnEquipInfo[type] += num;
	
	else if (type == CH_LEVEL)
	{
		for (int i = 0; i < CH_END; ++i)
		{
			chEquipInfo[i] -= chUnEquipInfo[i];
		}
		chUnEquipInfo[CH_LEVEL] += num;
		chUnEquipInfo[CH_HP] = 100 + chUnEquipInfo[CH_LEVEL] * 20;
		chUnEquipInfo[CH_MP] = 100 + chUnEquipInfo[CH_LEVEL] * 20;
		chUnEquipInfo[CH_SP] = 100 + chUnEquipInfo[CH_LEVEL] * 10;
		chUnEquipInfo[CH_ATK] = 10 + chUnEquipInfo[CH_LEVEL];
		chUnEquipInfo[CH_DEF] = 10 + chUnEquipInfo[CH_LEVEL];

		for (int i = CH_LEVEL; i < CH_END; ++i)
		{
			chEquipInfo[i] += chUnEquipInfo[i];
		}
	}

	else if (type == CH_EXP)
	{
		chUnEquipInfo[CH_EXP] += num;
		if (chUnEquipInfo[CH_EXP] > chUnEquipInfo[CH_LEVEL] * 20 + 100)
		{
			chUnEquipInfo[CH_EXP] = 0;
			SetInstanceInfomation(CH_LEVEL,1);
		}
	}

	else if (type == CH_HP)
	{
		if (num > 0)
		{
			if (chBattleInfo[CH_HP] < chEquipInfo[CH_HP])
			{
				for (int i = 1; i < num + 1; ++i)
				{
					++chBattleInfo[CH_HP];
					if (chBattleInfo[CH_HP] == chEquipInfo[CH_HP]) break;
				}
			}
		}

		else
		{
			if (chBattleInfo[CH_HP] > 0)
			{
				for (int i = 1; i < -num + 1; ++i)
				{
					--chBattleInfo[CH_HP];
					if (chBattleInfo[CH_HP] == 0) break;
				}
			}
		}
	

		
	}
	else
	{
		chEquipInfo[type] = chUnEquipInfo[type] + num;
		InfoUpdate();
	}
}
