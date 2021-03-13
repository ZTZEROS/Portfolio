#include "stdafx.h"
#include "../../stdafx.h"
#include "items.h"


items::items()
{
}


items::~items()
{
}

HRESULT items::init(int x, int y)
{
	_Item = new tagItem;
	_Item->_equip = false;
	_Item->CenterX = x;
	_Item->CenterY = y;
	_Item->_leftTopX = 0;
	_Item->_leftTopY = 0;
	_Item->LoopX = 0;
	_Item->LoopY = 0;
	_Item->_equip = 0;
	_Item->CreationPointX = x;
	_Item->CreationPointY = y;
	_Item->_itemRc = RectMakeCenter(_Item->CenterX, _Item->CenterY, 32, 32);

	Timer = 0;

	return S_OK;
}
void items::release(void)
{
}

void items::update(void)
{
	if (_Item->_Itype == UIITEM && !(_Item->_equip))
	{
		_Item->CenterX = _Item->CreationPointX - _Item->LoopX;
		_Item->CenterY = _Item->CreationPointY - _Item->LoopY;
	}

	if ((_Item->_Itype != UIITEM) && !(_Item->_equip)) AquisitionEffect();
	
	//if (_Item->_equip);
	_Item->_itemRc = RectMakeCenter(_Item->CenterX, _Item->CenterY, 32, 32);
}

void items::render(int leftTopX, int leftTopY)
{
	//Rectangle(getMemDC(), _Item->_itemRc.left, _Item->_itemRc.top, _Item->_itemRc.right, _Item->_itemRc.bottom);
	
	if (!(_Item->_equip))
	{
		_Item->_dropimg->render(getMemDC(), leftTopX, leftTopY);

		if (_Item->_Itype != UIITEM) IMAGEMANAGER->findImage("TextBox")->render(getMemDC(), 100, 100);
		if (_Item->_Itype != UIITEM) TextOut(getMemDC(), 200, 150, _Item->_IExplain, strlen(_Item->_IExplain));
	}
	
	if ((_Item->_equip) && (_Item->_Itype != UIITEM) && (_Item->_Itype != GOLDITEM)) _Item->_itemimg->render(getMemDC(), leftTopX, leftTopY);
}

void items::AquisitionEffect()
{
	if (_Item->CenterY > (_Item->CreationPointY - 70)) _Item->CenterY -= 5;

	if (_Item->CenterY <= (_Item->CreationPointY - 70))
	{
		Talking = 1;
	}
		
	if(Talking = 1 && KEYMANAGER->isOnceKeyDown('X')) _Item->_equip = 1;
}
