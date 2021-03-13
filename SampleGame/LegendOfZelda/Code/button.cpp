#include "stdafx.h"
#include "button.h"


button::button()
{
}


button::~button()
{
}

HRESULT button::init(const char* imageName, int x, int y,
	POINT btnUpFramePoint, POINT btnDownFramePoint,
	CALLBACKFUNCTION cbFunction)
{
	//콜백함수
	_callbackFunction = static_cast<CALLBACKFUNCTION>(cbFunction);

	_state = BUTTONSTATE_NULL;

	_x = x;
	_y = y;

	_imageName = imageName;
	_image = IMAGEMANAGER->findImage(imageName);

	_btnDownFramePoint = btnDownFramePoint;
	_btnUpFramePoint = btnUpFramePoint;

	_rc = RectMakeCenter(x, y, _image->getFrameWidth(), _image->getFrameHeight());

	return S_OK;
}

void button::release(void)
{

}

void button::update(void)
{
	if (PtInRect(&_rc, _ptMouse))
	{
		if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			_state = BUTTONSTATE_DOWN;
		}
		else if (KEYMANAGER->isOnceKeyUp(VK_LBUTTON) && _state == BUTTONSTATE_DOWN)
		{
			_state = BUTTONSTATE_UP;
			_callbackFunction();
		}
	}
	else
	{
		_state = BUTTONSTATE_NULL;
	}
}

void button::render(void)
{
	switch (_state)
	{
		case BUTTONSTATE_NULL: case BUTTONSTATE_UP:
			_image->frameRender(getMemDC(), _rc.left, _rc.top, _btnUpFramePoint.x, _btnUpFramePoint.y);
		break;
		case BUTTONSTATE_DOWN:
			_image->frameRender(getMemDC(), _rc.left, _rc.top, _btnDownFramePoint.x, _btnDownFramePoint.y);
		break;
	}
}
