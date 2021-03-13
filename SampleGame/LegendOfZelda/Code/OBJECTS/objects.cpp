#include "stdafx.h"
#include "../stdafx.h"
#include "objects.h"


objects::objects()
{
}


objects::~objects()
{
}



void objects::isFire(int x, int y)
{
	if (_objectInfo.isFire)return;
	_objectInfo.isFire = true;
	_objectInfo.startX = x;
	_objectInfo.startY = y;
	_objectInfo.throwCenterX = x;
	_objectInfo.throwCenterY = y;
	*_objectInfo.rcThrowObject = RectMakeCenter(
		_objectInfo.startX,
		_objectInfo.startY,
		ITEMSIZE,
		ITEMSIZE);
	_objectInfo.throwObjectImage->setX(_objectInfo.startX);
	_objectInfo.throwObjectImage->setY(_objectInfo.startY);
}
void objects::objectMove(FIREDIRECTION fireDirection) 
{
	if (!_objectInfo.isFire)return;

	switch (fireDirection)
	{
	case FIRE_UP:
		_objectInfo.throwCenterY -= _objectInfo.speed;
		break;
	case FIRE_DOWN:
		_objectInfo.throwCenterY += _objectInfo.speed;
		break;
	case FIRE_LEFT:
		_objectInfo.throwCenterX -= _objectInfo.speed;
		break;
	case FIRE_RIGHT:
		_objectInfo.throwCenterX += _objectInfo.speed;
		break;
	}
	*_objectInfo.rcThrowObject = RectMakeCenter(
		_objectInfo.throwCenterX,
		_objectInfo.throwCenterY,
		_objectInfo.throwObjectImage->getWidth(),
		_objectInfo.throwObjectImage->getHeight());
	if (fireDirection == FIRE_UP || fireDirection == FIRE_DOWN)
	{
		if (getDistance(_objectInfo.startX, _objectInfo.startY, _objectInfo.throwCenterX, _objectInfo.throwCenterY) > VERTICALRANGE)
		{
			_objectInfo.isFire = false;
			_objectInfo.objectDestroyedAni->start();
			_objectInfo.throwObjectDestroy = true;
			deleteObject();
		}
	}
	else
	{
		if (getDistance(_objectInfo.startX, _objectInfo.startY, _objectInfo.throwCenterX, _objectInfo.throwCenterY) > HORIZONTALRANGE)
		{
			_objectInfo.isFire = false;
			_objectInfo.objectDestroyedAni->start();
			_objectInfo.throwObjectDestroy = true;
			deleteObject();
		}
	}

}
void objects::deleteObject() 
{
	SAFE_DELETE( _objectInfo.rcThrowObject);
}
void objects::objectLifting(int x, int y) 
{
	_objectInfo.throwCenterX = x;
	_objectInfo.throwCenterY = y;
	*_objectInfo.rcThrowObject = RectMakeCenter(
		_objectInfo.throwCenterX, 
		_objectInfo.throwCenterY,
		ITEMSIZE,
		ITEMSIZE);
}

void objects::objectDestroy(void) 
{
	_objectInfo.isFire = false;
	_objectInfo.objectDestroyedAni->start();
	_objectInfo.throwObjectDestroy = true;
	deleteObject();
}