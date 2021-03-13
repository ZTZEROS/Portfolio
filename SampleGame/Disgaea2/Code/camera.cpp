#include "stdafx.h"
#include "camera.h"
#include "UIManager.h"
camera::camera()
{
}


camera::~camera()
{
}


HRESULT camera::init(void)
{
	_cameraX = _cameraY = 0;
	_focus.x = _focus.y = 0;

	_zoomIn = _zoomOut = false;
	
	_shakeOn = false;
	_shakeTime = 0;

	_wheelValue = 0;
	_maxValue = 0;

	return S_OK;
}

void camera::release(void)
{

}

void camera::update(void)
{
	_cameraX = -((WINSIZEX / 2) - _focus.x);
	_cameraY = -((WINSIZEY / 2) - _focus.y);
}

void camera::setShake(bool shake, int shakeTime)
{
	_shakeOn = shake;
	_shakeTime = shakeTime;
}