#include "stdafx.h"
#include "effect.h"
#include "animation.h"

effect::effect()
	: _effectImage(NULL),
	_effectAnimation(NULL),
	_isRunning(false),
	_x(0),
	_y(0)
{
}


effect::~effect()
{
}

HRESULT effect::init(image* effectImage, int frameW, int frameH, int fps, float elapsedTime)
{
	//만약에 이펙트 이미지가 없으면 리턴
	if (!effectImage) return E_FAIL;

	//활성화 여부 초기화
	_isRunning = false;

	//이펙트 이미지 정보 대입
	_effectImage = effectImage;

	//이펙트 경과 시간
	_elapsedTime = elapsedTime;

	//이미지 애니메이션 객체가 없다면 생성해준다
	if (!_effectAnimation) _effectAnimation = new animation;

	//애니메이션 셋팅
	_effectAnimation->init(_effectImage->getWidth(), _effectImage->getHeight(), frameW, frameH);
	_effectAnimation->setDefPlayFrame(false, false);
	_effectAnimation->setFPS(fps);
	_effectAnimation->stop();


	return S_OK;
}

void effect::release(void)
{
	_effectImage = NULL;
	SAFE_DELETE(_effectAnimation);
}

void effect::update(void)
{
	//애니메이션이 활성화 되어있지않으면 재생하지 않는다
	if (!_isRunning) return;
	_effectAnimation->frameUpdate(_elapsedTime);

	//이펙트 애니메이션이 종료되면 -> 꺼라
	if (!_effectAnimation->isPlay()) killEffect();
}

void effect::render(void)
{
	if (!_isRunning) return;

	_effectImage->aniRender(getMemDC(), _x, _y, _effectAnimation);
}

void effect::startEffect(int x, int y)
{
	//이펙트 정보가 초기화 되어있지 않으면? 실행하면 안됨
	if (!_effectImage || !_effectAnimation) return;

	_x = x - (_effectAnimation->getFrameWidth() / 2);
	_y = y - (_effectAnimation->getFrameHeight() / 2);

	_isRunning = true;
	_effectAnimation->start();
}

void effect::killEffect(void)
{
	_isRunning = false;
}
