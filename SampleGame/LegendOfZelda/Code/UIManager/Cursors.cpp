#include "stdafx.h"
#include "Cursors.h"

Cursors::Cursors() {}
Cursors::~Cursors() {}

HRESULT Cursors::Initialize()
{
	Cursor.CenterX = 150;
	Cursor.CenterY = 160;
	//Cursor.InventoryCursorLeftTopX = Cursor.InventoryCursorLeftTopY = 96;
	//Cursor.LinkHeadLeftTopX = Cursor.LinkHeadLeftTopY = 96;
	//Cursor.LocationAlertLeftTopX = 692;
	//Cursor.LocationAlertLeftTopY = 210;
	//
	//Cursor.XMarkLeftTopX = Cursor.XMarkLeftTopY = 96;

	Cursor.CurrentFrameX = Cursor.CurrentFrameY = 0;

	return S_OK;
}

void Cursors::Release()
{}

void Cursors::Update()
{
	Cursor.CurrentFrameX++;
	if (Cursor.CurrentFrameX > 40) Cursor.CurrentFrameX = 0;
}

void Cursors::Render()
{
	Cursor.Image->render(getMemDC(), Cursor.CenterX - Cursor.Image->getWidth() / 2, Cursor.CenterY - Cursor.Image->getHeight() / 2);

	//IMAGEMANAGER->render("LocationAlert", getMemDC(), Cursor.LocationAlertLeftTopX, Cursor.LocationAlertLeftTopY);
}