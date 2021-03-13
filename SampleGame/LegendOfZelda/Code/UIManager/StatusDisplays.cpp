#include "stdafx.h"
#include "StatusDisplays.h"

StatusDisplays::StatusDisplays() {}
StatusDisplays::~StatusDisplays() {}

HRESULT StatusDisplays::Initialize()
{
	LeftTopX = LeftTopY = 0;
	Talking = 0;

	return S_OK;
}
void StatusDisplays::Release() {}

void StatusDisplays::Update() {}

void StatusDisplays::Render()
{
	switch (SelectedSubWeapon)
	{
	case 1:
		IMAGEMANAGER->render("inven_lamp", getMemDC(), 160 + LeftTopX, 75 + LeftTopY);
		break;
	case 2:
		IMAGEMANAGER->render("inven_boomerang", getMemDC(), 100 + LeftTopX, 100 + LeftTopY);
		break;
	default:
		break;
	}

	IMAGEMANAGER->render("StatusDisplay", getMemDC(), LeftTopX, LeftTopY);

	IMAGEMANAGER->render("MagicPower", getMemDC(), 94, 80 + LeftTopY);
	
	for (int i = 0; i < _maxHp / 2; i++) IMAGEMANAGER->render("Heart_Empty", getMemDC(), 644 + i * 32, 82 + LeftTopY);
	for (int i = 0; i < _currentHp / 2; i++)
	{
		IMAGEMANAGER->render("Heart_Full", getMemDC(), 644 + i * 32, 82 + LeftTopY);
		if(i > (_currentHp / 2) - 1 &&_currentHp % 2) IMAGEMANAGER->render("Heart_Half", getMemDC(), 644 + i * 32, 82 + LeftTopY);
	}
			
	if (Talking) IMAGEMANAGER->render("TextBox", getMemDC(), WINSIZEX / 2 - 378, WINSIZEY / 2 - 105);
}