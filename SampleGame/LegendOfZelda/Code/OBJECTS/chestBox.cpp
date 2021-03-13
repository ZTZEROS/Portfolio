#include "stdafx.h"
#include "../stdafx.h"
#include "chestBox.h"


chestBox::chestBox()
{
}


chestBox::~chestBox()
{
}
HRESULT chestBox::init(int x, int y)
{
	ZeroMemory(&_objectInfo, sizeof(tagObject));
	_objectInfo.centerX = x;
	_objectInfo.centerY = y;
	_objectInfo.speed = 5;
	_objectInfo.objectImage = new image;
	_objectInfo.objectImage = IMAGEMANAGER->findImage("보물상자");
	_objectInfo.rcObject = new RECT;
	_objectInfo.objectState = UNTOUCHED;
	*_objectInfo.rcObject = RectMakeCenter(_objectInfo.centerX, _objectInfo.centerY, 
		_objectInfo.objectImage->getFrameWidth(), 
		_objectInfo.objectImage->getFrameHeight());
	 _objectInfo.objectImage->setX(_objectInfo.centerX - (_objectInfo.rcObject->right - _objectInfo.rcObject->left)/2);
	 _objectInfo.objectImage->setY(_objectInfo.centerY - (_objectInfo.rcObject->bottom - _objectInfo.rcObject->top) / 2);
	return S_OK;
}
void chestBox::release()
{

}
void chestBox::update()
{
	*_objectInfo.rcObject = RectMakeCenter(_objectInfo.centerX, _objectInfo.centerY,
		_objectInfo.objectImage->getFrameWidth(),
		_objectInfo.objectImage->getFrameHeight());
	switch (_objectInfo.objectState)
	{
	case UNTOUCHED:
		_objectInfo.objectImage->setFrameX(0);
		break;
	case TOUCHED:
		_objectInfo.objectImage->setFrameX(1);
		break;
	}
}
void chestBox::render()
{
	Rectangle(getMemDC(), _objectInfo.rcObject->left, _objectInfo.rcObject->top, _objectInfo.rcObject->right, _objectInfo.rcObject->bottom);
	_objectInfo.objectImage->frameRender(getMemDC(), _objectInfo.rcObject->left, _objectInfo.rcObject->top);
}
