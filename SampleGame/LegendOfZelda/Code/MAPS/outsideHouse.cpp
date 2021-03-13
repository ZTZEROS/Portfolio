#include "stdafx.h"
#include "../stdafx.h"
#include "outsideHouse.h"

outsideHouse::outsideHouse()
{

}

outsideHouse::~outsideHouse()
{

}

HRESULT outsideHouse::init(void)
{
	_backgroundImage = IMAGEMANAGER->findImage("Áý¹Û");
	_backgroundPixel = IMAGEMANAGER->findImage("Áý¹ÛÇÈ¼¿");
	_exteriorGate = new image;
	_exteriorGate->init("../ObjectSprites/ExteriorGate.bmp", 38 * IMAGESCALE, 25 * IMAGESCALE, false,true, MAGENTA);
	_exteriorGateX = -400;
	_exteriorGateY = -3800;
	_rcGate = RectMake(_exteriorGateX, _exteriorGateY, _exteriorGate->getWidth(), _exteriorGate->getHeight());
	_interDoor = new RECT;
	_loopRectX = 500;
	_loopRectY = -950;
	_centerInterDoorX = 510;
	_centerInterDoorY = 340;
	*_interDoor = RectMakeCenter(_centerInterDoorX, _centerInterDoorY,DOORWIDTH,DOORHEIGHT);
	//Áý³ª°«À»‹š ¹Ù±ùÃ³À½ÁÂÇ¥~
	_loopX = 2300;
	_loopY = 4800;
	return S_OK;
}
void outsideHouse::release(void)
{
	delete _interDoor;
}
void outsideHouse::update(void)
{
	*_interDoor = RectMakeCenter(_centerInterDoorX, _centerInterDoorY, DOORWIDTH, DOORHEIGHT);
	_loopRect = RectMake(_loopRectX, _loopRectY, 1100, 40);
	_rcGate = RectMake(_exteriorGateX, _exteriorGateY, _exteriorGate->getWidth(), _exteriorGate->getHeight());
}
void outsideHouse::render(void)
{
	_backgroundPixel->render(getMemDC(), 0, 0, 0, 0, _backgroundPixel->getWidth(), _backgroundPixel->getHeight());
	_backgroundImage->render(getMemDC(), 0, 0, _loopX, _loopY, _backgroundImage->getWidth(), _backgroundImage->getHeight());
	Rectangle(getMemDC(), _interDoor->left, _interDoor->top, _interDoor->right, _interDoor->bottom);
	Rectangle(getMemDC(), _loopRect.left, _loopRect.top, _loopRect.right, _loopRect.bottom);
	_exteriorGate->render(getMemDC(), _rcGate.left, _rcGate.top);
}

