#include "stdafx.h"
#include "effectManager.h"
#include "effect.h"


effectManager::effectManager()
{
}


effectManager::~effectManager()
{
}

HRESULT effectManager::init(void)
{

	return S_OK;
}

void effectManager::release(void)
{
	iterTotalEffect vIter;
	iterEffect mIter;

	vIter = _vTotalEffects.begin();

	for (; vIter != _vTotalEffects.end(); ++vIter)
	{
		mIter = vIter->begin();
		for (; mIter != vIter->end();)
		{
			if (mIter->second.size() != 0)
			{
				iterEffects vArrIter = mIter->second.begin();
				for (; vArrIter != mIter->second.end(); ++vArrIter)
				{
					(*vArrIter)->release();
					delete *vArrIter;
					vArrIter = mIter->second.erase(vArrIter);
				}
			}
			else
			{
				++mIter;
			}
		}
	}
}

void effectManager::update(void)
{
	iterTotalEffect vIter;
	iterEffect mIter;

	for (vIter = _vTotalEffects.begin(); vIter != _vTotalEffects.end(); ++vIter)
	{
		for (mIter = vIter->begin(); mIter != vIter->end(); ++mIter)
		{
			iterEffects vArrIter;
			for (vArrIter = mIter->second.begin(); vArrIter != mIter->second.end(); ++vArrIter)
			{
				(*vArrIter)->update();
			}
		}
	}
}

void effectManager::render(void)
{
	iterTotalEffect vIter;
	iterEffect mIter;

	for (vIter = _vTotalEffects.begin(); vIter != _vTotalEffects.end(); ++vIter)
	{
		for (mIter = vIter->begin(); mIter != vIter->end(); ++mIter)
		{
			iterEffects vArrIter;
			for (vArrIter = mIter->second.begin(); vArrIter != mIter->second.end(); ++vArrIter)
			{
				(*vArrIter)->render();
			}
		}
	}


}

void effectManager::addEffect(string effectName, const char* imageName,
	int imageWidth, int imageHeight, int effectWidth, int effectHeight,
	int fps, float elapsedTime, int buffer)
{
	image* img;
	arrEffects vEffectBuffer; //이펙트 버퍼
	arrEffect mArrEffect;

	if (IMAGEMANAGER->findImage(imageName))
	{
		img = IMAGEMANAGER->findImage(imageName);
	}
	else
	{
		img = IMAGEMANAGER->addImage(imageName, imageName, imageWidth, imageHeight, true, RGB(255, 0, 255));
	}

	for (int i = 0; i < buffer; i++)
	{
		vEffectBuffer.push_back(new effect);
		vEffectBuffer[i]->init(img, effectWidth, effectHeight, fps, elapsedTime);
	}

	mArrEffect.insert(pair<string, arrEffects>(effectName, vEffectBuffer));

	_vTotalEffects.push_back(mArrEffect);

}


void effectManager::play(string effectName, int x, int y)
{
	iterTotalEffect vIter;
	iterEffect mIter;

	//토탈 이펙트를 담은 벡터를 돌아서
	for (vIter = _vTotalEffects.begin(); vIter != _vTotalEffects.end(); ++vIter)
	{
		//그 안에 담긴 맵의 이펙트 검사
		for (mIter = vIter->begin(); mIter != vIter->end(); ++mIter)
		{
			if (!(mIter->first == effectName)) break;

			iterEffects vArrIter;
			for (vArrIter = mIter->second.begin(); vArrIter != mIter->second.end(); ++vArrIter)
			{
				//이펙트가 동작하고 있으면 건너뜀!
				if ((*vArrIter)->getIsRunning()) continue;
				(*vArrIter)->startEffect(x, y);
				return;
			}
		}
	}
}
