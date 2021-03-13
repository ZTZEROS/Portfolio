#include "stdafx.h"
#include "../stdafx.h"
#include "blackCircle.h"

blackCircle::blackCircle()
	:_aniProgress(STANDBY), _animationOn(false)
{

}

blackCircle::~blackCircle()
{

}

HRESULT blackCircle::init()
{
	_blackCircleImage = new image;
	_blackCircleImage = IMAGEMANAGER->findImage("°ËÀº¿ø");
	_blackCircleAni = new animation;
	_blackCircleAni->init(_blackCircleImage->getWidth(), _blackCircleImage->getHeight(),
		_blackCircleImage->getFrameWidth(), _blackCircleImage->getFrameHeight());
	_blackCircleAni->setFPS(2);

	return S_OK;
}

void blackCircle::startAni(bool reverse)
{
	
	_blackCircleAni->setDefPlayFrame(reverse, false);
	_blackCircleAni->start();
	_aniProgress = START;
	_animationOn = true;
}
void blackCircle::release(void)
{

}
void blackCircle::update(void)
{
	switch (_aniProgress)
	{
	case STANDBY:
		break;
	case START:
		_blackCircleAni->frameUpdate(0.1f);
		if (!_blackCircleAni->isPlay())_aniProgress = COMPLETE;
		break;
	case COMPLETE:
		_animationOn = false;
		_aniProgress = STANDBY;
		break;
	}
}
void blackCircle::render(int x, int y)
{
	switch (_aniProgress)
	{
	case STANDBY:
		break;
	case START:
		_blackCircleImage->aniRender(getMemDC(), x, y, _blackCircleAni);
		break;
	case COMPLETE:
		break;
	}	
}
