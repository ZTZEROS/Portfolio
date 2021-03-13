#include "stdafx.h"
#include "../../stdafx.h"
#include "itemManager.h"


itemManager::itemManager()
{
}


itemManager::~itemManager()
{
}

HRESULT itemManager::init(void)
{
	//CreateSword(0, 0);
	//CreateShield(0, 0);
	//
	//CreateLamp(0, 0);
	//CreateBoomerang(0, 0);
	//CreateDungeonMap(0, 0);
	//CreateMasterKey(0, 0);

	//CreateLifeHeart(100, 100);
	//Createkey(200, 200);
	//CreateGreenCrystal(300, 300);
	//CreatePurpleCrystal(300, 300);
	//UIM->GetInventory()->SetItemV(greenCrystal);
	Timer = 0;
	
	//vector<string> LoadV = TXTDATA->txtLoad("UIManager/Slot0.txt");
	
	//for (int i = 0; i < LoadV.size(); i++)
	//{
	//	if ("Lamp" == LoadV[i])
	//	{
	//		CreateLamp(-100, -100);
	//		UIM->GetInventory()->SetItemV(_vItem[i]);
	//		//_vItem.clear();
	//	}
	//
	//	if ("Boomerang" == LoadV[i])
	//	{
	//		CreateBoomerang(-100, -100);
	//		UIM->GetInventory()->SetItemV(_vItem[i]);
	//		//_vItem.clear();
	//	}
	//}
	return S_OK;
}
void itemManager::release(void)
{
	SAFE_DELETE(lamp);
	SAFE_DELETE(boomerang);
	SAFE_DELETE(dungeonmap);
	SAFE_DELETE(lifeHeart);
	SAFE_DELETE(key);
	SAFE_DELETE(masterkey);
	SAFE_DELETE(greenCrystal);

	_vItem.clear();
}
void itemManager::update(void)
{
	for (_viItem = _vItem.begin(); _viItem != _vItem.end(); ++_viItem) 
	{
		(*_viItem)->SetLoopX(LoopX);
		(*_viItem)->SetLoopY(LoopY);

		(*_viItem)->update();
		
		if ((*_viItem)->getEquip())
		{
			//UIM->GetInventory()->GetItemV().push_back((*_viItem));
			if ((*_viItem)->GetItemType() != UIITEM && (*_viItem)->GetItemType() != GOLDITEM) UIM->GetInventory()->SetItemV((*_viItem));
			if ((*_viItem)->GetItemType() == GOLDITEM) UIM->GetInventory()->GetItemV()[0]->SetItemNum(UIM->GetInventory()->GetItemV()[0]->GetItemNum() + 1);
			
			_vItem.erase(_viItem);
			
			break;
		}
	}
}

void itemManager::render()
{
	for (_viItem = _vItem.begin(); _viItem != _vItem.end(); ++_viItem)
	{
		(*_viItem)->render((*_viItem)->GetCenterX() - (*_viItem)->GetDropImage()->getFrameWidth() / 2, (*_viItem)->GetCenterY() - (*_viItem)->GetDropImage()->getFrameHeight() / 2);
	}
}

void itemManager::buyItem(int itemnum)
{

}

void itemManager::equipItem(int itemnum)
{
	_vItem[itemnum]->setEquip(true);
}

void itemManager::CreateSword(float leftTopX, float leftTopY)
{
	sword = new swords;
	sword->init(leftTopX, leftTopY);
	_vItem.push_back(sword);
}
void itemManager::CreateShield(float leftTopX, float leftTopY)
{
	shield = new shields;
	shield->init(leftTopX, leftTopY);
	_vItem.push_back(shield);
}

void itemManager::CreateLamp(float leftTopX, float leftTopY)
{
	lamp = new lanterns;
	lamp->init(leftTopX, leftTopY);
	_vItem.push_back(lamp);
}
void itemManager::CreateBoomerang(float leftTopX, float leftTopY)
{
	boomerang = new boomerangs;
	boomerang->init(leftTopX, leftTopY);
	_vItem.push_back(boomerang);
}

void itemManager::CreateDungeonMap(float leftTopX, float leftTopY)
{
	dungeonmap = new dungeonMap;
	dungeonmap->init(0, 0);
	_vItem.push_back(dungeonmap);
}
void itemManager::CreateMasterKey(float leftTopX, float leftTopY)
{
	masterkey = new masterkeys;
	masterkey->init(leftTopX, leftTopY);
	_vItem.push_back(masterkey);
}

void itemManager::CreateLifeHeart(float leftTopX, float leftTopY)
{
	lifeHeart = new lifeHearts;
	lifeHeart->init(leftTopX, leftTopY);
	_vItem.push_back(lifeHeart);
}

void itemManager::Createkey(float leftTopX, float leftTopY)
{
	key = new keys;
	key->init(leftTopX, leftTopY);
	_vItem.push_back(key);
}

void itemManager::CreateGreenCrystal(float leftTopX, float leftTopY)
{
	greenCrystal = new greenCrystals;
	greenCrystal->init(leftTopX, leftTopY);
	_vItem.push_back(greenCrystal);
}

void itemManager::CreatePurpleCrystal(float leftTopX, float leftTopY)
{
	purpleCrystal = new purpleCrystals;
	purpleCrystal->init(leftTopX, leftTopY);
	_vItem.push_back(purpleCrystal);
}