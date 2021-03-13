#pragma once
#include "stdafx.h"

#define TILEWIDTH 192			//Ÿ�� ���α���
#define TILEHEIGHT 96			//Ÿ�� ���α���

#define TILENUMX 20				//����Ÿ�ϰ���
#define TILENUMY 20				//����Ÿ�ϰ���

enum TERRAIN
{
	TR_ROCK, 
	TR_STONES, 
	TR_GRASS, 
	TR_WATER,

	TR_DEEPTILE,
	TR_LIGHTTILE,
	TR_GRAVELTILE,

	TR_NONE

};

enum OBJECT
{
	OBJ_WHITE_FLOWER,
	OBJ_YELLOW_FLOWER,
	OBJ_PUPLE_FLOWER,
	OBJ_WEEDS,
	OBJ_PEBBLE,
	OBJ_STARTPOTAL,
	OBJ_NONE
};

enum WHO_ON_THE_TILE
{
	ON_HERO,
	ON_ENEMY,
	ON_NONE
};


//Ÿ�������� ����ִ� ����ü
struct tagTile
{
	TERRAIN terrain;
	OBJECT obj;
	WHO_ON_THE_TILE onTile;
	int altitude;
	bool activeTile;

	int X, Y;
	POINT Center;
	float TotalCost;
	float CostFromStart;
	float CostToGoal;

	tagTile* ParentTile;
};

