#include "stdafx.h"
#include "explosionManager.h"


explosionManager::explosionManager()
{
}


explosionManager::~explosionManager()
{
}


HRESULT explosionManager::init(void)
{

	return S_OK;
}
void explosionManager::release(void)
{

}
void explosionManager::update(void)
{

}
void explosionManager::render(void)
{

}

void explosionManager::setExplosion(HDC hdc, int x, int y, int width, int height)
{
	_rc = RectMakeCenter(x, y, width, height);



	HPEN myPen, oldPen;
	myPen = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));
	oldPen = (HPEN)SelectObject(hdc, myPen);
	HBRUSH myBrush, oldBrush;
	myBrush = CreateSolidBrush(RGB(255, 0, 255));
	oldBrush = (HBRUSH)SelectObject(hdc, myBrush);
	EllipseMakeCenter(hdc, _rc.left + (_rc.right - _rc.left) / 2, _rc.top + (_rc.bottom - _rc.top) / 2, width, height);
	SelectObject(hdc, oldBrush);
	DeleteObject(myBrush);
	SelectObject(hdc, oldPen);
	DeleteObject(myPen);

}