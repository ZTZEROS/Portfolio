#include "stdafx.h"
#include "DungeonMaps.h"

DungeonMaps::DungeonMaps() {}
DungeonMaps::~DungeonMaps() {}

HRESULT DungeonMaps::Initialize()
{
	LeftTopX = 480;
	LeftTopY = 78;

	Cursor.SetImage(IMAGEMANAGER->findImage("LinkHead"));
	Cursor.Initialize();
	Cursor.SetCenterX(126);
	Cursor.SetCenterY(380);
	
	return S_OK;
}

void DungeonMaps::Release() {}

void DungeonMaps::Update()
{
	if (Cursor.GetCenterY() == 380)
	{
		if (LeftTopY < 78) LeftTopY += 6;
		if (LeftTopY > 78) LeftTopY -= 6;

		//LeftTopY = 78;
	}

	if (Cursor.GetCenterY() == 435)
	{
		if (LeftTopY < 78) LeftTopY += 6;
		if (LeftTopY > 78) LeftTopY -= 6;
		
		//LeftTopY = 78;
	}

	if (Cursor.GetCenterY() == 490)
	{
		if (LeftTopY < -250) LeftTopY += 6;
		if (LeftTopY > -250) LeftTopY -= 6;

		//LeftTopY = -250;
	}
		
	if (Cursor.GetCenterY() == 545)
	{
		if (LeftTopY < -579) LeftTopY += 6;
		if (LeftTopY > -579) LeftTopY -= 6;
		
		//LeftTopY = -579;
	}

	if (Cursor.GetCenterY() == 600)
	{
		if (LeftTopY < -908) LeftTopY += 6;
		if (LeftTopY > -908) LeftTopY -= 6;

		//LeftTopY = -908;
	}
}

void DungeonMaps::Render()
{
	IMAGEMANAGER->findImage("DungeonMinimapDepth")->render(getMemDC(), 0, 0);
	IMAGEMANAGER->findImage("DungeonMinimap2FtoB3")->render(getMemDC(), LeftTopX, LeftTopY);
	IMAGEMANAGER->findImage("DungeonMinimapSurface")->render(getMemDC(), 0, 0);
	
	Cursor.Render();
}