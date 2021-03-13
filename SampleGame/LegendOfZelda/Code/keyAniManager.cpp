#include "stdafx.h"
#include "keyAniManager.h"
#include "image.h"
#include "animation.h"


keyAniManager::keyAniManager()
{
}


keyAniManager::~keyAniManager()
{
}

HRESULT keyAniManager::init(void)
{

	return S_OK;
}

void keyAniManager::release(void)
{
	deleteAll();
}

void keyAniManager::update(void)
{
	iterAnimations iter = _mTotalAnimations.begin();

	for (iter; iter != _mTotalAnimations.end(); ++iter)
	{
		if (!iter->second->isPlay()) continue;
		//초당 60프레임으로!
		iter->second->frameUpdate(TIMEMANAGER->getElapsedTime() * 1.0f);
	}
}

void keyAniManager::render(void)
{

}

void keyAniManager::addDefaultFrameAnimation(string animationKeyName, char* imageKeyName, int fps, bool reverse, bool loop)
{
	image* img = IMAGEMANAGER->findImage(imageKeyName);
	animation* ani = new animation;

	ani->init(img->getWidth(), img->getHeight(), img->getFrameWidth(), img->getFrameHeight());
	ani->setDefPlayFrame(reverse, loop);
	ani->setFPS(fps);

	_mTotalAnimations.insert(pair<string, animation*>(animationKeyName, ani));
}

void keyAniManager::addDefaultFrameAnimation(string animationKeyName, char* imageKeyName, int fps, bool reverse, bool loop, void* cbFunction)
{
	image* img = IMAGEMANAGER->findImage(imageKeyName);
	animation* ani = new animation;

	ani->init(img->getWidth(), img->getHeight(), img->getFrameWidth(), img->getFrameHeight());
	ani->setDefPlayFrame(reverse, loop, (CALLBACKFUNCTION)cbFunction);
	ani->setFPS(fps);

	_mTotalAnimations.insert(pair<string, animation*>(animationKeyName, ani));
}

void keyAniManager::addDefaultFrameAnimation(string animationKeyName, char* imageKeyName, int fps, bool reverse, bool loop, void* cbFunction, void* obj)
{
	image* img = IMAGEMANAGER->findImage(imageKeyName);
	animation* ani = new animation;

	ani->init(img->getWidth(), img->getHeight(), img->getFrameWidth(), img->getFrameHeight());
	ani->setDefPlayFrame(reverse, loop, (CALLBACKFUNCTION_PARAMETER)cbFunction, obj);
	ani->setFPS(fps);

	_mTotalAnimations.insert(pair<string, animation*>(animationKeyName, ani));
}

void keyAniManager::addArrayFrameAnimation(string animationKeyName, char* imageKeyName, int* arr, int arrLen, int fps, bool loop)
{
	image* img = IMAGEMANAGER->findImage(imageKeyName);
	animation* ani = new animation;

	ani->init(img->getWidth(), img->getHeight(), img->getFrameWidth(), img->getFrameHeight());
	ani->setPlayFrame(arr, arrLen, loop);
	ani->setFPS(fps);

	_mTotalAnimations.insert(pair<string, animation*>(animationKeyName, ani));
}

void keyAniManager::addArrayFrameAnimation(string animationKeyName, char* imageKeyName, int* arr, int arrLen, int fps, bool loop, void* cbFunction)
{
	image* img = IMAGEMANAGER->findImage(imageKeyName);
	animation* ani = new animation;

	ani->init(img->getWidth(), img->getHeight(), img->getFrameWidth(), img->getFrameHeight());
	ani->setPlayFrame(arr, arrLen, loop, CALLBACKFUNCTION(cbFunction));
	ani->setFPS(fps);

	_mTotalAnimations.insert(pair<string, animation*>(animationKeyName, ani));
}

void keyAniManager::addArrayFrameAnimation(string animationKeyName, char* imageKeyName, int* arr, int arrLen, int fps, bool loop, void* cbFunction, void* obj)
{
	image* img = IMAGEMANAGER->findImage(imageKeyName);
	animation* ani = new animation;

	ani->init(img->getWidth(), img->getHeight(), img->getFrameWidth(), img->getFrameHeight());
	ani->setPlayFrame(arr, arrLen, loop, CALLBACKFUNCTION_PARAMETER(cbFunction), obj);
	ani->setFPS(fps);

	_mTotalAnimations.insert(pair<string, animation*>(animationKeyName, ani));
}

void keyAniManager::addCoordinateFrameAnimation(string animationKeyName, char* imageKeyName, int start, int end, int fps, bool reverse, bool loop)
{
	image* img = IMAGEMANAGER->findImage(imageKeyName);
	animation* ani = new animation;

	ani->init(img->getWidth(), img->getHeight(), img->getFrameWidth(), img->getFrameHeight());
	ani->setPlayFrame(start, end, reverse, loop);
	ani->setFPS(fps);

	_mTotalAnimations.insert(pair<string, animation*>(animationKeyName, ani));
}

void keyAniManager::addCoordinateFrameAnimation(string animationKeyName, char* imageKeyName, int start, int end, int fps, bool reverse, bool loop, void* cbFunction)
{
	image* img = IMAGEMANAGER->findImage(imageKeyName);
	animation* ani = new animation;

	ani->init(img->getWidth(), img->getHeight(), img->getFrameWidth(), img->getFrameHeight());
	ani->setPlayFrame(start, end, reverse, loop, CALLBACKFUNCTION(cbFunction));
	ani->setFPS(fps);

	_mTotalAnimations.insert(pair<string, animation*>(animationKeyName, ani));
}

void keyAniManager::addCoordinateFrameAnimation(string animationKeyName, char* imageKeyName, int start, int end, int fps, bool reverse, bool loop, void* cbFunction, void* obj)
{
	image* img = IMAGEMANAGER->findImage(imageKeyName);
	animation* ani = new animation;

	ani->init(img->getWidth(), img->getHeight(), img->getFrameWidth(), img->getFrameHeight());
	ani->setPlayFrame(start, end, reverse, loop, CALLBACKFUNCTION_PARAMETER(cbFunction), obj);
	ani->setFPS(fps);

	_mTotalAnimations.insert(pair<string, animation*>(animationKeyName, ani));
}

void keyAniManager::start(string animationKeyName)
{
	iterAnimations iter = _mTotalAnimations.find(animationKeyName);
	iter->second->start();
}

void keyAniManager::stop(string animationKeyName)
{
	iterAnimations iter = _mTotalAnimations.find(animationKeyName);
	iter->second->stop();
}

void keyAniManager::pause(string animationKeyName)
{
	iterAnimations iter = _mTotalAnimations.find(animationKeyName);
	iter->second->pause();
}

void keyAniManager::resume(string animationKeyName)
{
	iterAnimations iter = _mTotalAnimations.find(animationKeyName);
	iter->second->resume();
}

animation* keyAniManager::findAnimation(string animationKeyName)
{
	iterAnimations iter = _mTotalAnimations.find(animationKeyName);

	if (iter != _mTotalAnimations.end())
	{
		return iter->second;
	}

	return NULL;
}

void keyAniManager::deleteAll(void)
{
	iterAnimations iter = _mTotalAnimations.begin();

	for (; iter != _mTotalAnimations.end();)
	{
		if (iter->second != NULL)
		{
			iter->second->release();
			SAFE_DELETE(iter->second);
			iter = _mTotalAnimations.erase(iter);
		}
		else
		{
			++iter;
		}
	}

	_mTotalAnimations.clear();
}
