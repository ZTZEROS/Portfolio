
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
	STATE_RECOVERY_HP,			//HP회복
	STATE_RECOVERY_SP,			//SP회복
	STATE_REGULATER_HP,			//최대HP증감
	STATE_REGULATER_SP,			//최대SP증감
	STATE_REGULATER_ATK,		//Atk증감
	STATE_REGULATER_DEF,		//Def증감
	STATE_REGULATER_HIT,		//HitPoint증감
	STATE_REGULATER_SPEED,		//회피율 증감
	STATE_REGULATER_MAGIC,		//int 증감
	STATE_REGULATER_RES			//마법저항력 증감
};

enum weaponList
{
	//나중에 무기할 때 리스트 추가
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
	image* img;					//아이템 이미지 로드
	LPTSTR name;				//아이템 이름
	LPTSTR itemKind;			//아이템 종류
	LPTSTR itemIndex;			//아이템 설명
	RECT itemRc;				//아이템 랙트

	int regulaterHP;			//최대HP증감
	int recoveryHP;				//HP회복
	int regulaterSP;			//최대SP증감
	int recoverySP;				//SP회복
	int regulaterAttack;		//Atk증감
	int regulaterDefence;		//Def증감
	int regulaterHit;			//HitPoint증감
	int regulaterSpeed;			//회피율 증감
	int regulaterMagic;			//int 증감
	int regulaterResistance;	//마법저항력 증감

	int itemHell;				//아이템 가격
	char str[128];				//가격을 표시하기 위한 캐릭터형 배열
	itemRarity itemClass;		//아이템 등급(1 = 평범, 2 = 평범하지 않은, 3 = 희귀, 4 = 열라 희귀한, 5 = 유일)
};

struct strItemInfo
{
	char regulaterHP[32];				//최대HP증감
	char recoveryHP[32];				//HP회복
	char regulaterSP[32];				//최대SP증감
	char recoverySP[32];				//SP회복
	char regulaterAttack[32];			//Atk증감
	char regulaterDefence[32];			//Def증감
	char regulaterHit[32];				//HitPoint증감
	char regulaterSpeed[32];			//회피율 증감
	char regulaterMagic[32];			//int 증감
	char regulaterResistance[32];		//마법저항력 증감
	char itemName[32];					//아이템이름
	char itemIndex[128];				//아이템인덱스
};
