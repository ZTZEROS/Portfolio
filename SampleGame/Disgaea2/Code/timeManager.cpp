#include "stdafx.h"
#include "timeManager.h"


timeManager::timeManager()
	: _timer(NULL)
{
}


timeManager::~timeManager()
{
}

HRESULT timeManager::init(void)
{
	_timer = new timer;
	_timer->init();

	return S_OK;
}

void timeManager::release(void)
{
	if (_timer != NULL)
	{
		SAFE_DELETE(_timer);
		_timer = NULL;
	}
}

void timeManager::update(float lock)
{
	if (_timer != NULL)
	{
		_timer->tick(lock);
	}
}

void timeManager::render(HDC hdc, float x, float y)
{
	char str[256];
	string strFrame;

	//���� ����� (TRANSPARENT : ����, OPAQUE : ������)
	SetBkMode(hdc, TRANSPARENT);

#ifdef _DEBUG
	if (_timer != NULL)
	{
		//FPS
		wsprintf(str, "framePerSec : %d", _timer->getFrameRate());
		TextOut(hdc, x, y, str, strlen(str));

		//����Ÿ��
		sprintf_s(str, "worldTime : %f", _timer->getWorldTime());
		TextOut(hdc, x, y+20, str, strlen(str));

		//���� tick
		sprintf_s(str, "elapsedTime : %f", _timer->getElapsedTime());
		TextOut(hdc, x, y+40, str, strlen(str));
	}
#else
	{
		if (_timer != NULL)
		{
			wsprintf(str, "framePerSec : %d", _timer->getFrameRate());
			TextOut(hdc, x, y, str, strlen(str));
		}
	}
#endif
}