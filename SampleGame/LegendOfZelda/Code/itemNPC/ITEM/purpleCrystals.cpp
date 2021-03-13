#include "stdafx.h"
#include "../../stdafx.h"
#include "purpleCrystals.h"


purpleCrystals::purpleCrystals()
{
}


purpleCrystals::~purpleCrystals()
{
}

HRESULT purpleCrystals::init(int x, int y)
{
	items::init(x, y);
	_Item->_Iname = "퍼플크리스탈";
	_Item->_Itype = GOLDITEM;
	_Item->_dropimg = IMAGEMANAGER->findImage("Purple_Crystal");
	_Item->_Iprice = 5;

	return S_OK;
}