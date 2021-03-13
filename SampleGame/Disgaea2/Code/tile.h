#pragma once
#include "stdafx.h"
#define TILESIZE 32

#define TILEX 5
#define TILEY 5

#define TILESIZEX TILESIZE * TILEX
#define TILESIZEY TILESIZE * TILEY

#define SAMPLETILEX 20
#define SAMPLETILEY 9

#define ATTR_UNMOVE		0x00000001
#define ATTR_SWAMP		0x00000002
#define ATTR_POISON		0x00000004

//아이소 메트릭스용 디파인문
#define ISOMETRIC_STARTING_POINT_X WINSIZEX/2
#define ISOMETRIC_STARTING_POINT_Y 100
#define OFFSET_X TILESIZE*2
#define OFFSET_Y TILESIZE
enum CTRL
{
	CTRL_SAVE, CTRL_ERASER, CTRL_LOAD,
	CTRL_OBJDRAW, CTRL_TERRAIN, CTRL_END
};

enum TERRAIN
{
	TR_CEMENT, TR_DESERT, TR_GRASS, TR_WATER, TR_END
};

enum OBJECT
{
	OBJ_BLOCK1, OBJ_BLOCK3, OBJ_BLOCKS, OBJ_FLAG1,
	OBJ_FLAG2, OBJ_PACKMAN, OBJ_ENEMY, OBJ_NONE
};

enum POS
{
	POS_FLAG1, POS_FLAG2, POS_TANK1, POS_TANK2
};

struct tagTile
{
	TERRAIN terrain;
	OBJECT obj;
	RECT rc;
	int terrainFrameX;
	int terrainFrameY;
	int objFrameX;
	int objFrameY;
	POINT index;		//타일넘버
	POINT pivot;		//아이소메트릭용 피봇
};

struct tagSampleTile
{
	RECT rcTile;
	int terrainFrameX;
	int terrainFrameY;
};

struct tagCurrentTile
{
	int x;
	int y;
};
