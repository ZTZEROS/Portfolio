#include "stdafx.h"
#include "../../stdafx.h"
#include "redCrystals.h"


redCrystals::redCrystals()
{
}


redCrystals::~redCrystals()
{
}

HRESULT redCrystals::init(int x, int y)
{
	items::init(x, y);
	_Item->_Iname = "����ũ����Ż";
	_Item->_Itype = GOLDITEM;
	_Item->_dropimg = IMAGEMANAGER->findImage("Red_Crystal");
	_Item->_Iprice = 10;

	return S_OK;
}