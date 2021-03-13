#include "stdafx.h"
#include "WorldMaps.h"

#define SCALE 128

WorldMaps::WorldMaps() {}
WorldMaps::~WorldMaps() {}

HRESULT WorldMaps::Initialize()
{
	Image = IMAGEMANAGER->findImage("WorldMinimap");
	ZoomIn = 0;

	LeftTopScrollX = 0;
	LeftTopScrollY = 0;

	Cursor.SetImage(IMAGEMANAGER->findImage("LinkHead"));
	Cursor.Initialize();

	UK = VK_UP;
	DK = VK_DOWN;
	LK = VK_LEFT;
	RK = VK_RIGHT;

	return S_OK;
}

void WorldMaps::Release()
{}

void WorldMaps::Update()
{
	Cursor.Update();

	if (ZoomIn)
	{
		if (KEYMANAGER->isStayKeyDown(UK))// && (LeftTopScrollY >= -512))
		{
			Cursor.SetCenterY(Cursor.GetCenterY() + 3.5);
			Cursor.SetCenterX(Cursor.GetCenterX() + 1.5);
		}
		if (KEYMANAGER->isStayKeyDown(DK))// && (LeftTopScrollY <= WINSIZEY - 256))
		{
			Cursor.SetCenterY(Cursor.GetCenterY() - 3.5);
			Cursor.SetCenterX(Cursor.GetCenterX() - 1.5);
		}
		if (KEYMANAGER->isStayKeyDown(LK) && (LeftTopScrollX >= -510)) Cursor.SetCenterX(Cursor.GetCenterX() + 18);
		if (KEYMANAGER->isStayKeyDown(RK) && (LeftTopScrollX <= WINSIZEX - 510)) Cursor.SetCenterX(Cursor.GetCenterX() - 18);
	}
}

void WorldMaps::Render()
{
	//for (int i = SCALE; i > 0; i--)
	//{ 
	//	StretchBlt(getMemDC(), 0, (SCALE - i) * WINSIZEY / SCALE, WINSIZEX, WINSIZEY / SCALE,
	//		Image->getMemDC(), LeftTopScrollX + Image->getCenterX() - ((i * Image->getWidth() / SCALE) / 2) / (1 + ZoomIn), LeftTopScrollY + (ZoomIn * Image->getHeight() / 4) + ((SCALE - i) * Image->getHeight() / SCALE) / (1 + ZoomIn),
	//		(i * Image->getWidth() / SCALE) / (1 + ZoomIn), (Image->getHeight() / SCALE) / (1 + ZoomIn), SRCCOPY);
	//}

	for (int i = SCALE; i > 0; i--)
	{
		StretchBlt(getMemDC(), 0, (SCALE - i) * WINSIZEY / SCALE, WINSIZEX, WINSIZEY / SCALE,
			Image->getMemDC(), LeftTopScrollX + Image->getCenterX() - (i * 20 / 2) - (SCALE - i) * 5 / (1 + ZoomIn), LeftTopScrollY + (ZoomIn * Image->getHeight() / 4) + ((SCALE - i) * Image->getHeight() / SCALE) / (1 + ZoomIn),
			(i * 20) / (1 + ZoomIn) + (SCALE - i) * 5, (Image->getHeight() / SCALE) / (1 + ZoomIn), SRCCOPY);
	}
	Cursor.Render();
}