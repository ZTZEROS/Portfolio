#pragma once
#include "image.h"

static image* _backBuffer = IMAGEMANAGER->addImage("backBuffer", BACKBUFFERSIZEX, BACKBUFFERSIZEY, FALSE);

//�� ���� ���ο� ����� �ǵ��� �����ּ���
static image* _invenbackBuffer = IMAGEMANAGER->addImage("invenbackBuffer", WINSIZEX, WINSIZEY, false);

class gameNode
{
private:
	HDC _hdc;
	bool _managerInit;
public:
	//HRESULT ���ҿ��� ���� �ǵ�
	//�Լ��� ȣ���� �� �Ǹ� S_OK, �����ϸ� E_FAIL������
	//�ʱ�ȭ�� �ߵǾ����� ���θ� ���������� �˷���
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

