#include "stdafx.h"
#include "UIManager.h"

UIManager::UIManager() {}
UIManager::~UIManager() {}

HRESULT UIManager::Initialize()
{	
	Inventory.Initialize();
	StatusDisplay.Initialize();
	WorldMap.Initialize();
	DungeonMap.Initialize();
	SaveLoad.Initialize();

	UK = VK_UP;
	DK = VK_DOWN;
	LK = VK_LEFT;
	RK = VK_RIGHT;
	
	ReceiveLinkInfo();

	CurrentMenu = isClosed;
	Paused = 0;
	Talking = 0;


	vector<string> LoadV = TXTDATA->txtLoad("UIManager/Slot0.txt");

	for (int i = 0; i < LoadV.size(); i++)
	{
		if ("Lamp" == LoadV[i])
		{
			Item = new lanterns;
			Item->init(250, 250);

			Inventory.SetItemV(Item);
			Inventory.SetEquip(i);

			//delete(Item);
		}

		if ("Boomerang" == LoadV[i])
		{
			Item = new boomerangs;
			Item->init(-100, -100);

			Inventory.SetItemV(Item);
			Inventory.SetEquip(i);
			//delete(Item);
		}
	}


	return S_OK;
}

void UIManager::Release() {}

void UIManager::Update()
{
	ReceiveLinkInfo();

	if (KEYMANAGER->isOnceKeyDown(VK_RSHIFT))
	{
		if (CurrentMenu == isClosed)
		{
			CurrentMenu = isSaveLoad;
			SaveLoad.SetCursorCenterX(185);
			SaveLoad.SetCursorCenterY(50 + WINSIZEY * 5 / 7);
		}
		else CurrentMenu = isClosed;
	}

	if (KEYMANAGER->isOnceKeyDown(VK_RETURN))
	{
		if (CurrentMenu == isClosed)
		{
			CurrentMenu = isInventory;
			Inventory.SetCursorCenterX(150);
			Inventory.SetCursorCenterY(160);
		}
		else CurrentMenu = isClosed;
	}
	
	if (KEYMANAGER->isOnceKeyDown('Q'))
	{
		//if (Link->getMapManager()->getWhereIsLinnk() == OUTSIDE_HOUSE_AREA)
		{
			if (CurrentMenu == isWorldMap && WorldMap.GetZoomIn())
			{
				CurrentMenu = isClosed;
				WorldMap.SetZoomIn(0);
				WorldMap.SetLeftTopScrollX(0);
				WorldMap.SetLeftTopScrollY(0);
			}

			if (CurrentMenu == isWorldMap)
			{
				WorldMap.SetZoomIn(1);
				WorldMap.SetCursorCenterX(70 * (1 + WorldMap.GetZoomIn()) + WINSIZEX / 2);
				WorldMap.SetCursorCenterY(70 * (1 + WorldMap.GetZoomIn()) + WINSIZEY / 2);
			}

			if (CurrentMenu == isClosed)
			{
				CurrentMenu = isWorldMap;
				WorldMap.SetCursorCenterX(70 * (1 + WorldMap.GetZoomIn()) + WINSIZEX / 2);//Link¸Ê·çÇÁ + Link->getShadowX() / ÇöÀç ¸Ê Å©±â); //after get the coordinate value
				WorldMap.SetCursorCenterY(70 * (1 + WorldMap.GetZoomIn()) + WINSIZEY / 2);//Link¸Ê·çÇÁ + Link->getBodyRect().bottom / ÇöÀç ¸Ê Å©±â); //after get the coordinate value
			}
		}
	}

	if (KEYMANAGER->isOnceKeyDown('W'))
	{
		//if (Link->getMapManager()->getWhereIsLinnk() != INSIDE_HOUSE_AREA && Link->getMapManager()->getWhereIsLinnk() != OUTSIDE_HOUSE_AREA)
		{
			if (CurrentMenu == isClosed)
			{
				CurrentMenu = isDungeonMap;
				DungeonMap.SetCursorCenterX(126);
				DungeonMap.SetCursorCenterY(380);
			}
		}
	}

	if (CurrentMenu == isInventory && (Inventory.GetLeftTopY() <= 0 && StatusDisplay.GetLeftTopY() <= WINSIZEY))
	{
		Inventory.SetLeftTopY(Inventory.GetLeftTopY() + 12);
		StatusDisplay.SetLeftTopY(StatusDisplay.GetLeftTopY() + 12);
	}
	else if (CurrentMenu == isClosed && (Inventory.GetLeftTopY() >= - WINSIZEY && StatusDisplay.GetLeftTopY() >= 0))
	{
		Inventory.SetLeftTopY(Inventory.GetLeftTopY() - 12);
		StatusDisplay.SetLeftTopY(StatusDisplay.GetLeftTopY() - 12);
	}

	if (CurrentMenu == isWorldMap && WorldMap.GetZoomIn())
	{
		if (KEYMANAGER->isStayKeyDown(UK) && (WorldMap.GetLeftTopScrollY() >= -512)) WorldMap.SetLeftTopScrollY(WorldMap.GetLeftTopScrollY() - 5);
		if (KEYMANAGER->isStayKeyDown(DK) && (WorldMap.GetLeftTopScrollY() <= WINSIZEY - 256)) WorldMap.SetLeftTopScrollY(WorldMap.GetLeftTopScrollY() + 5);
		if (KEYMANAGER->isStayKeyDown(LK) && (WorldMap.GetLeftTopScrollX() >= -510)) WorldMap.SetLeftTopScrollX(WorldMap.GetLeftTopScrollX() - 5);
		if (KEYMANAGER->isStayKeyDown(RK) && (WorldMap.GetLeftTopScrollX() <= WINSIZEX - 510)) WorldMap.SetLeftTopScrollX(WorldMap.GetLeftTopScrollX() + 5);

		WorldMap.Update();
	}

	if (CurrentMenu == isInventory)
	{
		if (KEYMANAGER->isOnceKeyDown(UK) && (Inventory.GetCursorCenterY() >= 260)) Inventory.SetCursorCenterY(Inventory.GetCursorCenterY() - 100);
		if (KEYMANAGER->isOnceKeyDown(DK) && (Inventory.GetCursorCenterY() <= 260)) Inventory.SetCursorCenterY(Inventory.GetCursorCenterY() + 100);
		if (KEYMANAGER->isOnceKeyDown(LK) && (Inventory.GetCursorCenterX() >= 250)) Inventory.SetCursorCenterX(Inventory.GetCursorCenterX() - 100);
		if (KEYMANAGER->isOnceKeyDown(RK) && (Inventory.GetCursorCenterX() <= 450)) Inventory.SetCursorCenterX(Inventory.GetCursorCenterX() + 100);
	
		Inventory.Update();
		StatusDisplay.SetSelectedSubWeapon(Inventory.GetSelectedSubWeapon());
		Link->setItemState((ITEMSTATE)Inventory.GetSelectedSubWeapon());
	}
		
	if (CurrentMenu == isDungeonMap)
	{
		if (KEYMANAGER->isOnceKeyDown(UK) && DungeonMap.GetCursorCenterY() >= 435) DungeonMap.SetCursorCenterY(DungeonMap.GetCursorCenterY() - 55);
		if (KEYMANAGER->isOnceKeyDown(DK) && DungeonMap.GetCursorCenterY() <= 545) DungeonMap.SetCursorCenterY(DungeonMap.GetCursorCenterY() + 55);
		DungeonMap.Update();
	}

	if (CurrentMenu == isSaveLoad)
	{
		if (KEYMANAGER->isOnceKeyDown(UK) && SaveLoad.GetCursorCenterY() >= 120 + WINSIZEY * 5 / 7) SaveLoad.SetCursorCenterY(SaveLoad.GetCursorCenterY() - 70);
		if (KEYMANAGER->isOnceKeyDown(DK) && SaveLoad.GetCursorCenterY() <= 50 + WINSIZEY * 5 / 7) SaveLoad.SetCursorCenterY(SaveLoad.GetCursorCenterY() + 70);
		
		if (KEYMANAGER->isOnceKeyDown('X'))
		{
			if (SaveLoad.GetCursorCenterY() == 50 + WINSIZEY * 5 / 7)
			{
				vector<string> SaveV;
				vector<string>::iterator SaveVI;

				//SaveV = TXTDATA->txtLoad("UIManager/SlotName0.txt");

				//int Storyline = Link->getMapManager()->getStoryLine();
				//
				//char Number[2];
				//
				//itoa(Storyline, Number, 10);
				//
				//SaveV.push_back(Number);
								
				//itoa(Inventory.GetItemV()[0]->GetItemNum(), Number, 10);
				//
				//SaveV.push_back(Number);

				for (int i = 0; i < Inventory.GetItemV().size(); i++)
				{
					SaveV.push_back(Inventory.GetItemV()[i]->GetItemName());
				}

				TXTDATA->txtSave("UIManager/Slot0.txt", SaveV); //UIManager/
				PostQuitMessage(0);
			}

			if (SaveLoad.GetCursorCenterY() == 120 + WINSIZEY * 5 / 7)
			{
				PostQuitMessage(0);
			}
			SaveLoad.Update();
		}		
	}

	if (CurrentMenu == isClosed) StatusDisplay.Update();
	
	Paused = 0;
	if (CurrentMenu) Paused = 1;
}

void UIManager::Render()
{
	if (Talking && !CurrentMenu) IMAGEMANAGER->findImage("TextBox")->render(getMemDC(), 135, WINSIZEY * 5 / 7);
	if (Inventory.GetLeftTopY() >= - WINSIZEY) Inventory.Render();
	if (CurrentMenu == isWorldMap) WorldMap.Render();
	if (CurrentMenu == isDungeonMap) DungeonMap.Render();
	if (CurrentMenu == isSaveLoad) SaveLoad.Render();

	if ((Inventory.GetLeftTopY() <= 0 && StatusDisplay.GetLeftTopY() <= WINSIZEY) && CurrentMenu != isDungeonMap && CurrentMenu != isWorldMap) StatusDisplay.Render();
}

void UIManager::ReceiveLinkInfo()
{
	CloneLink._image = Link->getImage();
	CloneLink._bodyRc = Link->getBodyRect();
	CloneLink._currentHp = Link->getCurrentHp();
	CloneLink._currentSp = Link->getCurrentSp();
	CloneLink._itemState = Link->getLinkItemState();
	CloneLink._linkDir = Link->getLinkDirection();
	CloneLink._linkState = Link->getLinkState();
	CloneLink._maxHp = Link->getMaxHp();
	CloneLink._maxSp = Link->getMaxSp();
	CloneLink._shadowX = Link->getShadowX();
	CloneLink._shadowY = Link->getShadowY();
	CloneLink._weaponEquip = Link->getWeaponEquip();
	
	StatusDisplay.Set_currentHp(CloneLink._currentHp);
	StatusDisplay.Set_maxHp(CloneLink._maxHp);
}