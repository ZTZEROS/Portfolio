#pragma once
#include "singletonBase.h"

#include <Vfw.h>	// ���� ��� ���� ���̺귯��
#pragma comment(lib, "vfw32.lib")

class streamManager : public singletonBase<streamManager>
{
private:
	HWND _hWndAVI;
	bool _isPlay;
	float nowPlay;
public:
	HRESULT init(void);

	void startStream(const char* fileName);
	void closeStream(void);
	void closePlayEndStream(void);
	bool getisplay(void) { return _isPlay; }

	streamManager();
	~streamManager();
};

