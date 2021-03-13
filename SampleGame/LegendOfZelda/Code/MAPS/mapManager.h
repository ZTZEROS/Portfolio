#pragma once
#include "../gameNode.h"
#include "../OBJECTS/chestBox.h"
#include "../OBJECTS/grass.h"
#include "../OBJECTS/jar.h"
#include "../itemNPC/ITEM/itemManager.h"
#include "../itemNPC/NPC/uncle.h"
#include "../itemNPC/NPC/princess.h"
#include "../itemNPC/NPC/soldier.h"
#include "insideHouse.h"
#include "outsideHouse.h"
#include "../effect/blackCircle.h"
#include "../effect/raining.h"
#include "../monster/monstermanager.h"

class link;
class itemManager;
class uncle;
class princess;
class soldier;

#define MAPLOOPSPEED 5

enum CHANGEMAP
{
	COMPLETECHANGE,
	CHANGING
};

enum STORYLINE
{
	PROLOGUE,
	BEFOREHOLE,
	AFTERHOLE
};
enum OUTSIDEAREA
{
	HOUSEAREA,
	CASTLEAREA
};
enum WHERE_IS_LINK
{
	INSIDE_HOUSE_AREA,
	OUTSIDE_HOUSE_AREA,
	CASTLE_BASEMENT,
	CASTLE_1F,
	CASTLE_2F,
	CASTLE_B1F,
	STAIRS_TO_PRINCESS,
	BOSS_ROOM
};

enum SLIDINGMAP
{
	MAPSTEADY,
	MAPSLIDING
};
enum EXITANDENTER
{
	EXIT,
	ENTER
};

class mapManager : public gameNode
{
private:
	vector<objects*> _vJar;
	vector<objects*> _vGrass;

private:
	//************************************//
	//				불러올 클래스		  //
	//************************************//
	chestBox* _chest;
	grass* _grass;
	jar* _jar;
	link* _linkAndMap;
	insideHouse* _houseMap;
	outsideHouse* _outsideHouseMap;
	blackCircle* _blackCircle;
	raining* _raining;
	itemManager* _IM;
	STORYLINE _storyline;

	soldier* _soldier1;	//downleft_01
	soldier* _soldier2;	//downleft_02
	soldier* _soldier3;	//down_01
	soldier* _soldier4;	//down_02
	soldier* _soldier5;	//right_01
	soldier* _soldier6;	//right_02
	soldier* _soldier7;	//castle_door
	soldier* _soldier8;	//castle_left_01
	soldier* _soldier9;	//castle_left_02
	uncle* _uncle;
	princess* _princess;

	monstermanager* _MM;

	RECT Collision;

private:
	WHERE_IS_LINK _whereIsLink;
	SLIDINGMAP _slidingMap;
	OUTSIDEAREA _outsideArea;
	RECT _rcHorizontal;
	RECT _rcVertical;
	RECT _linkArea;
	EXITANDENTER _exitAndEnter;
	int _linkCenterX;
	int _linkCenterY;
	int _mapPixelLoopX;
	int _mapPixelLoopY;
	int _mapLoopLimitUp;
	int _mapLoopLimitDown;
	bool _playerMove;
	CHANGEMAP _changeMap;
	RECT temp;

	RECT rt; //= { 165, 600, 860, 700 };
	int textframe = 0;
	int speed = 0;
	int procedure;
	int TimerVariable = 0;
public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);
public:
	void exitAndEnterMap(void);
	void mapSliding(void);
public:
	//GET,SET
	WHERE_IS_LINK getWhereIsLinnk(void) { return _whereIsLink; }
	void setLinkMapMangerAddressLink(link* linkAndMap) { _linkAndMap = linkAndMap; }
	STORYLINE getStoryLine(void) { return _storyline; }
	int getMapPixelLoopX(void) { return _mapPixelLoopX; }
	int getMapPixelLoopY(void) { return _mapPixelLoopY; }
	SLIDINGMAP getSlidingMap(void) { return _slidingMap; }
	itemManager* getIM() { return _IM; }

	bool getPlayerMove(void) { return _playerMove; }

	vector<objects*> getVJar(void) { return _vJar; }
	vector<objects*> getVGrass(void) { return _vGrass; }
	//************************************//
	//				집안에서...			  //
	//************************************//
	void houseInit(void);
	void houseLoop(void);
	void houseItemUpdate(void);
	void houseRender(void);
	
	//************************************//
	//				집밖에서...			  //
	//************************************//
	void outsideHouseInit(void);
	void outsideHouseLoop(void);
	void outsideHouseItemUpdate(void);
	void outsideHouseRender(void);
	
	mapManager();
	~mapManager();
};