#pragma once
#include "image.h"

static image* _backBuffer = IMAGEMANAGER->addImage("backBuffer", WINSIZEX, WINSIZEY, FALSE);

class gameNode
{
private:
	HDC _hdc;
	bool _managerInit;
	bool _introOver;
public:
	//HRESULT 마소에서 만든 건데
	//함수가 호출이 잘 되면 S_OK, 실패하면 E_FAIL등으로
	//초기화가 잘되었는지 여부를 실패했을때 알려줌
	virtual HRESULT init(void);
	virtual HRESULT init(bool managerInit);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	image* getBackBuffer(void) { return _backBuffer; }

	HDC getMemDC() { return _backBuffer->getMemDC(); }
	HDC getHDC() { return _hdc; }

	LRESULT MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

	bool getIntroOver(void) { return _introOver; }

	gameNode();
	virtual ~gameNode();
};

