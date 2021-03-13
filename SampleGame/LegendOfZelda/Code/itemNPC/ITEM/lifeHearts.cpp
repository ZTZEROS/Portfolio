#include "stdafx.h"
#include "../../stdafx.h"
#include "lifeHearts.h"


lifeHearts::lifeHearts()
{
}


lifeHearts::~lifeHearts()
{
}

HRESULT lifeHearts::init(int x, int y)
{
	items::init(x, y);
	_Item->_Iname = "하트";
	_Item->_Itype = UIITEM;
	_Item->_dropimg = IMAGEMANAGER->findImage("heart_one");
	_Item->_Iprice = 10;

	return S_OK;
}

halflifeHearts::halflifeHearts() {};
halflifeHearts::~halflifeHearts() {};

HRESULT halflifeHearts::init(int x, int y)
{
	items::init(x, y);
	_Item->_Iname = "반하트";
	_Item->_Itype = UIITEM;
	_Item->_dropimg = IMAGEMANAGER->findImage("heart_half");
	_Item->_Iprice = 5;

	return S_OK;
}