#include "stdafx.h"
#include "Inventories.h"

Inventories::Inventories() {}
Inventories::~Inventories() {}

HRESULT Inventories::Initialize()
{
	LeftTopX = 0;
	LeftTopY = - WINSIZEY;

	Cursor.SetImage(IMAGEMANAGER->findImage("InventoryCursor"));
	Cursor.Initialize();
	
	SelectedSubWeapon = 0;

	

	//Item[0] = { "Lantern" ,IMAGEMANAGER->findImage("Lantern"), 0, 0, 0 };
	//Item[0] = { "Boomerang" ,IMAGEMANAGER->findImage("Boomerang"), 0, 0, 0 };
	//Item[0] = { "Sword" ,IMAGEMANAGER->findImage("Sword"), 0, 0, 0 };
	//Item[0] = { "Armour" ,IMAGEMANAGER->findImage("Armour"), 0, 0, 0 };
	//Item[0] = { "Shield" ,IMAGEMANAGER->findImage("Shield"), 0, 0, 0 };
	
	//ItemV = TXTDATA->txtLoad("ItemStructure.txt");

	//for (int i = 0; i < 20; i++) { ItemV.push_back(Item[i]); }

	return S_OK;
}

void Inventories::Release() {}

void Inventories::Update()
{
	for (ItemVI = ItemV.begin(); ItemVI != ItemV.end(); ++ItemVI)
	{
		if ((*ItemVI)->GetItemName() == "Lamp")
		{
			(*ItemVI)->SetCenterX(250);
			(*ItemVI)->SetCenterY(250);
		}

		if ((*ItemVI)->GetItemName() == "ºÎ¸Þ¶û")
		{
			(*ItemVI)->SetCenterX(350);
			(*ItemVI)->SetCenterY(150);
		}
		
		if ((*ItemVI)->GetItemName() == "½¯µå")
		{
			(*ItemVI)->SetCenterX(350);
			(*ItemVI)->SetCenterY(150);
		}

		if ((*ItemVI)->GetItemName() == "¼Òµå")
		{
			(*ItemVI)->SetCenterX(350);
			(*ItemVI)->SetCenterY(150);
		}

		if ((*ItemVI)->GetItemName() == "´øÀüÁöµµ")
		{
			(*ItemVI)->SetCenterX(350);
			(*ItemVI)->SetCenterY(150);
		}

		if ((*ItemVI)->GetItemName() == "¸¶½ºÅÍ¿­¼è")
		{
			(*ItemVI)->SetCenterX(350);
			(*ItemVI)->SetCenterY(150);
		}

		if (PtInRect(&(*ItemVI)->GetVirtualItem(), PointMake(Cursor.GetCenterX(), Cursor.GetCenterY())))
		{
			if (KEYMANAGER->isOnceKeyDown('X'))
			{
				if ((*ItemVI)->GetItemName() == "Lamp") SelectedSubWeapon = 1;
				
				if ((*ItemVI)->GetItemName() == "ºÎ¸Þ¶û") SelectedSubWeapon = 2;
			}
		}


		(*ItemVI)->update();
	}

	if (LeftTopY >= 0)
	{
		Cursor.Update();
	}
}

void Inventories::Render()
{
	IMAGEMANAGER->render("Inventory", getMemDC(), LeftTopX, LeftTopY);

	for (ItemVI = ItemV.begin(); ItemVI != ItemV.end(); ++ItemVI)
	{
		(*ItemVI)->render(LeftTopX + (*ItemVI)->GetCenterX() - (*ItemVI)->GetInvenImage()->getWidth() / 2, LeftTopY + (*ItemVI)->GetCenterY() - (*ItemVI)->GetInvenImage()->getHeight() / 2);

		if (PtInRect(&(*ItemVI)->GetVirtualItem(), PointMake(Cursor.GetCenterX(), Cursor.GetCenterY())))
		{
			if (((*ItemVI)->GetItemName() == "Lamp") || ((*ItemVI)->GetItemName() == "ºÎ¸Þ¶û"))
			{
				(*ItemVI)->GetNameImage()->render(getMemDC(), LeftTopX + 688, LeftTopY + 77);

				break;
			}
		}
	}


	if(LeftTopY >= 0)
	{
		if (Cursor.GetCurrentFrameX() > 20) Cursor.Render();
	}
}