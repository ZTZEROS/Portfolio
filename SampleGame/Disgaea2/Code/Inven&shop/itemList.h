
#pragma once
#include "../stdafx.h"
#include "../image.h"
#include <string>

enum itemRarity
{
	ITEM_COMMON = 1,
	ITEM_UNCOMMON,
	ITEM_RARE,
	ITEM_SUPERRARE,
	ITEM_UNIQUE
};

enum itemState
{
	STATE_RECOVERY_HP,			//HPȸ��
	STATE_RECOVERY_SP,			//SPȸ��
	STATE_REGULATER_HP,			//�ִ�HP����
	STATE_REGULATER_SP,			//�ִ�SP����
	STATE_REGULATER_ATK,		//Atk����
	STATE_REGULATER_DEF,		//Def����
	STATE_REGULATER_HIT,		//HitPoint����
	STATE_REGULATER_SPEED,		//ȸ���� ����
	STATE_REGULATER_MAGIC,		//int ����
	STATE_REGULATER_RES			//�������׷� ����
};

enum weaponList
{
	//���߿� ������ �� ����Ʈ �߰�
};

enum armorList
{
	LIST_ARMOR_AMULET,
	LIST_ARMOR_PROTECTOR,
	LIST_ARMOR_BULLETPROOF_VEST,
	LIST_ARMOR_NIKE,
	LIST_ARMOR_ONE_LING,
	LIST_ARMOR_PENDENT,
	LIST_ARMOR_NULL
};

enum consumeItemList
{
	LIST_CONSUME_CHUGUM,
	LIST_CONSUME_GUM,
	LIST_CONSUME_BETTER_GUM,
	LIST_CONSUME_CANDY,
	LIST_CONSUME_DRINK,
	LIST_CONSUME_MERSULE,
	LIST_CONSUME_NULL
};

struct tagItem
{
	image* img;					//������ �̹��� �ε�
	LPTSTR name;				//������ �̸�
	LPTSTR itemKind;			//������ ����
	LPTSTR itemIndex;			//������ ����
	RECT itemRc;				//������ ��Ʈ

	int regulaterHP;			//�ִ�HP����
	int recoveryHP;				//HPȸ��
	int regulaterSP;			//�ִ�SP����
	int recoverySP;				//SPȸ��
	int regulaterAttack;		//Atk����
	int regulaterDefence;		//Def����
	int regulaterHit;			//HitPoint����
	int regulaterSpeed;			//ȸ���� ����
	int regulaterMagic;			//int ����
	int regulaterResistance;	//�������׷� ����

	int itemHell;				//������ ����
	char str[128];				//������ ǥ���ϱ� ���� ĳ������ �迭
	itemRarity itemClass;		//������ ���(1 = ���, 2 = ������� ����, 3 = ���, 4 = ���� �����, 5 = ����)
};

struct strItemInfo
{
	char regulaterHP[32];				//�ִ�HP����
	char recoveryHP[32];				//HPȸ��
	char regulaterSP[32];				//�ִ�SP����
	char recoverySP[32];				//SPȸ��
	char regulaterAttack[32];			//Atk����
	char regulaterDefence[32];			//Def����
	char regulaterHit[32];				//HitPoint����
	char regulaterSpeed[32];			//ȸ���� ����
	char regulaterMagic[32];			//int ����
	char regulaterResistance[32];		//�������׷� ����
	char itemName[32];					//�������̸�
	char itemIndex[128];				//�������ε���
};
