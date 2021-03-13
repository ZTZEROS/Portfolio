#pragma once
#include "singletonBase.h"

#include <Vfw.h>	// ���� ��� ���� ���̺귯��
#pragma comment(lib, "vfw32.lib")

class streamManager : public singletonBase<streamManager>
{
private:
	HWND _hWndAVI;
public:
	HRESULT init(void);

	void startStream(const char* fileName);
	void closeStream(void);

	streamManager();
	~streamManager();
};

