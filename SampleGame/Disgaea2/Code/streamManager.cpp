#include "stdafx.h"
#include "streamManager.h"


streamManager::streamManager()
{
}


streamManager::~streamManager()
{
}

HRESULT streamManager::init(void)
{
	//타임 포멧을 밀리세컨드로 변경
	MCIWndSetTimeFormat(_hWndAVI, "ms");
	nowPlay = 0;
	return S_OK;
}

void streamManager::startStream(const char* fileName)
{
	_isPlay = true;

	if (_hWndAVI)
	{
		MCIWndClose(_hWndAVI);
		MCIWndDestroy(_hWndAVI);
		_hWndAVI = 0;
	}

	string str = fileName;

	// hWndAVI 생성
	_hWndAVI = MCIWndCreate(_hWnd, _hInstance,
		MCIWNDF_NOTIFYANSI | MCIWNDF_NOMENU | MCIWNDF_NOTIFYALL | MCIWNDF_NOPLAYBAR,
		fileName);

	// 사이즈 맞춤
	if (_hWndAVI)
	{
		RECT rc;
		GetClientRect(_hWnd, &rc);
		SetWindowPos(_hWndAVI, NULL, 0, 0, rc.right, rc.bottom, SWP_NOZORDER);
		MCIWndPlay(_hWndAVI);
	}

}
void streamManager::closePlayEndStream(void)
{
	nowPlay += TIMEMANAGER->getElapsedTime();
	if (MCIWndGetLength(_hWndAVI)*0.001 <= nowPlay)
	{
		if (_hWndAVI)
		{
			_isPlay = false;
			MCIWndClose(_hWndAVI);
			MCIWndDestroy(_hWndAVI);
			_hWndAVI = 0;
		}
		nowPlay = 0;
	}
}


void streamManager::closeStream(void)
{
	_isPlay = false;

	if (_hWndAVI)
	{
		MCIWndClose(_hWndAVI);
		MCIWndDestroy(_hWndAVI);
		_hWndAVI = 0;
	}
}

