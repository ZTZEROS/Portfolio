#include "stdafx.h"
#include "../stdafx.h"
#include "grass.h"


grass::grass()
{
}


grass::~grass()
{
}
HRESULT grass::init(int x, int y)
{
	
	ZeroMemory(&_objectInfo, sizeof(tagObject));
	_objectInfo.isFire = false;
	_objectInfo.isLift = false;
	_objectInfo.throwObjectDestroy = false;
	_objectInfo.centerX = x;
	_objectInfo.centerY = y;
	_objectInfo.throwCenterX = _objectInfo.centerX;
	_objectInfo.throwCenterY = _objectInfo.centerY;
	_objectInfo.speed = 10;
	_objectInfo.frameX = 0;
	_objectInfo.objectImage = IMAGEMANAGER->findImage("풀");
	_objectInfo.throwObjectImage = IMAGEMANAGER->findImage("풀던지기용");
	_objectInfo.rcObject = new RECT;
	_objectInfo.rcThrowObject = new RECT;
	_objectInfo.objectState = UNTOUCHED;
	*_objectInfo.rcObject = RectMakeCenter(_objectInfo.centerX, _objectInfo.centerY,
		_objectInfo.objectImage->getFrameWidth(),
		_objectInfo.objectImage->getFrameHeight());
	*_objectInfo.rcThrowObject = RectMakeCenter(_objectInfo.centerX, _objectInfo.centerY,
		_objectInfo.throwObjectImage->getWidth(),
		_objectInfo.throwObjectImage->getHeight());
	_objectInfo.objectImage->setX(_objectInfo.centerX - (_objectInfo.rcObject->right - _objectInfo.rcObject->left) / 2);
	_objectInfo.objectImage->setY(_objectInfo.centerY - (_objectInfo.rcObject->bottom - _objectInfo.rcObject->top) / 2);
	_objectInfo.destroyObjectImage = new image;
	_objectInfo.destroyObjectImage = IMAGEMANAGER->findImage("풀날리기");
	_objectInfo.objectDestroyedAni = new animation;
	_objectInfo.objectDestroyedAni->init(
		_objectInfo.destroyObjectImage->getWidth(),
		_objectInfo.destroyObjectImage->getHeight(),
		_objectInfo.destroyObjectImage->getFrameWidth(),
		_objectInfo.destroyObjectImage->getFrameHeight());
	_objectInfo.objectDestroyedAni->setDefPlayFrame(false, false);
	_objectInfo.objectDestroyedAni->setFPS(3);
	return S_OK;
}
void grass::release()
{

}
void grass::update()
{
	*_objectInfo.rcObject = RectMakeCenter(_objectInfo.centerX, _objectInfo.centerY,
		_objectInfo.objectImage->getFrameWidth(),
		_objectInfo.objectImage->getFrameHeight());
	if (!_objectInfo.throwObjectDestroy)*_objectInfo.rcThrowObject = RectMakeCenter(_objectInfo.throwCenterX, _objectInfo.throwCenterY,
		_objectInfo.objectImage->getFrameWidth(),
		_objectInfo.objectImage->getFrameHeight());
	switch (_objectInfo.objectState)
	{
	case UNTOUCHED:
		_objectInfo.frameX = 0;
		break;
	case TOUCHED:
		_objectInfo.frameX = 1;
		break;
	case ATTACKED:
		_objectInfo.frameX = 1;
		_objectInfo.objectDestroyedAni->start();
		_objectInfo.objectState = TOUCHED;
		objectDestroy();
		break;
	}
	_objectInfo.objectDestroyedAni->frameUpdate(0.1f);
}
void grass::render()
{
	_objectInfo.objectImage->frameRender(getMemDC(),
		_objectInfo.rcObject->left,
		_objectInfo.rcObject->top,
		_objectInfo.frameX, 0);
	if (_objectInfo.isFire)_objectInfo.throwObjectImage->render(getMemDC(),
		_objectInfo.rcThrowObject->left,
		_objectInfo.rcThrowObject->top);
	if (_objectInfo.objectDestroyedAni->isPlay())
	{
		_objectInfo.destroyObjectImage->aniRender(getMemDC(),
			_objectInfo.throwCenterX - (_objectInfo.objectImage->getWidth() / 2),
			_objectInfo.throwCenterY - (_objectInfo.objectImage->getHeight() / 2),
			_objectInfo.objectDestroyedAni);
	}
	if (!_objectInfo.throwObjectDestroy)
	{
		Rectangle(getMemDC(), _objectInfo.rcThrowObject->left,
			_objectInfo.rcThrowObject->top,
			_objectInfo.rcThrowObject->right,
			_objectInfo.rcThrowObject->bottom);
	}
	
}
