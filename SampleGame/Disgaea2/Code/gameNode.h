#pragma once
#include "image.h"

static image* _backBuffer = IMAGEMANAGER->addImage("backBuffer", BACKBUFFERSIZEX, BACKBUFFERSIZEY, FALSE);

//내 전용 새로운 백버퍼 건들지 말아주세요
static image* _invenbackBuffer = IMAGEMANAGER->addImage("invenbackBuffer", WINSIZEX, WINSIZEY, false);

class gameNode
{
private:
	HDC _hdc;
	bool _managerInit;
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

	image* getInvenBackBuffer(void) {return _invenbackBuffer; }

	HDC getIMemDC() { return _invenbackBuffer->getMemDC(); }

	LRESULT MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);


	gameNode();
	virtual ~gameNode();
};

