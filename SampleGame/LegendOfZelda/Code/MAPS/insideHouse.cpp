#include "stdafx.h"
#include "../stdafx.h"
#include "insideHouse.h"

insideHouse::insideHouse()
	:_alphaNight(200), _alphaCount(0)
{

}

insideHouse::~insideHouse()
{

}

HRESULT insideHouse::init(void)
{
	_backgroundImage = IMAGEMANAGER->findImage("Áý");
	_backgroundPixel = IMAGEMANAGER->findImage("Áý¾ÈÇÈ¼¿");
	_interDoor = new RECT;
	_centerInterDoorX = WINSIZEX/2;
	_centerInterDoorY =810;
	_linkIntheMap = true;
	*_interDoor = RectMakeCenter(_centerInterDoorX, _centerInterDoorY, DOORWIDTH, DOORHEIGHT);
	return S_OK;
}
void insideHouse::release(void)
{
	delete _interDoor;
}
void insideHouse::update(void)
{
	alphaNight();
	
	*_interDoor = RectMakeCenter(_centerInterDoorX, _centerInterDoorY, DOORWIDTH, DOORHEIGHT);

}
void insideHouse::render(void)
{
	_backgroundPixel->render(getMemDC(),0,0,0,0,_backgroundPixel->getWidth(),_backgroundPixel->getHeight());
	_backgroundImage->render(getMemDC(),0,0,_loopX,_loopY,_backgroundImage->getWidth(),_backgroundImage->getHeight());
	IMAGEMANAGER->findImage("¹ã")->alphaRender(getMemDC(), _alphaNight);
	Rectangle(getMemDC(), _interDoor->left, _interDoor->top, _interDoor->right, _interDoor->bottom);
}

void insideHouse::alphaNight()
{
	if (_alphaNight >3)_alphaCount++;
	if (_alphaCount % 5 == 0)_alphaNight -= 3, _alphaCount = 1;
}
