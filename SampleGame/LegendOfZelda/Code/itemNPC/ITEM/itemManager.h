#pragma once
#include "../../gameNode.h"
#include "items.h"
#include "../../UIManager/UIManager.h"

#include "swords.h"
#include "shields.h"

#include "boomerangs.h"
#include "lanterns.h"
#include "dungeonMap.h"

#include "lifeHearts.h"
#include "keys.h"
#include "greenCrystals.h"
#include "purpleCrystals.h"
#include "bombs.h"

#include <vector>

class UIManager;

typedef vector<items*> vItem;
typedef vector<items*>::iterator viItem;

class itemManager : public gameNode
{
private:
	vItem _vItem;
	viItem _viItem;

	swords* sword;
	shields* shield;

	lanterns* lamp;
	dungeonMap* dungeonmap;
	masterkeys* masterkey;

	lifeHearts* lifeHeart;
	keys* key;
	greenCrystals* greenCrystal;
	purpleCrystals* purpleCrystal;
	boomerangs* boomerang;
	//bombs* bomb;

	UIManager* UIM;

	float LoopX;
	float LoopY;

	bool Talking;
	int Timer;

public:
	void buyItem(int itemnum);
	void equipItem(int itemnum);
	
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);
	
	void CreateSword(float leftTopX, float leftTopY);
	void CreateShield(float leftTopX, float leftTopY);

	void CreateLamp(float leftTopX, float leftTopY);
	void CreateBoomerang(float leftTopX, float leftTopY);
	void CreateDungeonMap(float leftTopX, float leftTopY);
	void CreateMasterKey(float leftTopX, float leftTopY);

	void CreateLifeHeart(float leftTopX, float leftTopY);
	void Createkey(float leftTopX, float leftTopY);
	void CreateGreenCrystal(float leftTopX, float leftTopY);
	void CreatePurpleCrystal(float leftTopX, float leftTopY);

	bool GetTalking() { return Talking; }
	float GetLoopX() { return LoopX; }
	float GetLoopY() { return LoopY; }
	vItem Get_vItem() { return _vItem; }
	viItem Get_viItem() { return _viItem; }

	void SetUIMAddress(UIManager* uim) { UIM = uim; }
	void SetLoopX(float loopX) { LoopX = loopX; }
	void SetLoopY(float loopY) { LoopY = loopY; }
	void Set_vItem(vItem vitem) { _vItem = vitem; }
	void Set_viItem(viItem viItem) { _viItem = viItem; }
	void SetTalking(bool talking) { Talking = talking; }

	itemManager();
	~itemManager();
};

