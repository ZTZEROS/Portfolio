#include "stdafx.h"
#include "../stdafx.h"
#include "raining.h"

raining::raining()
	:_alphaCount(0),_alphaVal(50)
{

}

raining::~raining()
{

}

HRESULT raining::init(void)
{
	_rainingImage = IMAGEMANAGER->findImage("ºñ¿Â´Ù");
	_alphaImage = IMAGEMANAGER->findImage("¹ã");
	_aniRain = new animation;
	_aniRain->init(_rainingImage->getWidth(), _rainingImage->getHeight(),
		_rainingImage->getFrameWidth(), _rainingImage->getFrameHeight());
	_aniRain->setFPS(3);
	_aniRain->setDefPlayFrame(false, true);
	_aniRain->start();
	return S_OK;
}
void raining::release(void)
{
	SAFE_DELETE(_aniRain);
}
void raining::update(void)
{
	_aniRain->frameUpdate(0.1f);
	_alphaCount++;
	if (_alphaCount % 8 == 0)
	{
		if(_alphaVal==120)_alphaVal = 50;
		else _alphaVal = 120;
		_alphaCount = 0;
	}
}
void raining::render(void)
{
	_rainingImage->aniRender(getMemDC(), 0, 0, _aniRain);
	_alphaImage->alphaRender(getMemDC(), 0, 0, _alphaVal);
}