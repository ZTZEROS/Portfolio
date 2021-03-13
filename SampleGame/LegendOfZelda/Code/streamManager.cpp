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
	return S_OK;
}

void streamManager::startStream(const char* fileName)
{
	if (_hWndAVI)
	{
		MCIWndClose(_hWndAVI);
		MCIWndDestroy(_hWndAVI);
		_hWndAVI = 0;
	}

	string str = fileName;

	// hWndAVI ����
	_hWndAVI = MCIWndCreate(_hWnd, _hInstance,
		MCIWNDF_NOTIFYANSI | MCIWNDF_NOMENU | MCIWNDF_NOTIFYALL | MCIWNDF_NOPLAYBAR,
		fileName);

	// ������ ����
	if (_hWndAVI)
	{
		RECT rc;
		GetClientRect(_hWnd, &rc);
		SetWindowPos(_hWndAVI, NULL, 0, 0, rc.right, rc.bottom, SWP_NOZORDER);
		MCIWndPlay(_hWndAVI);
	}
}
void streamManager::closeStream(void)
{
	if (_hWndAVI)
	{
		MCIWndClose(_hWndAVI);
		MCIWndDestroy(_hWndAVI);
		_hWndAVI = 0;
	}
}