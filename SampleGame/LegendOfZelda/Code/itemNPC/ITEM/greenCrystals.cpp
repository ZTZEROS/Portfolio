#include "stdafx.h"
#include "../../stdafx.h"
#include "greenCrystals.h"


greenCrystals::greenCrystals()
{
}


greenCrystals::~greenCrystals()
{
}

HRESULT greenCrystals::init(int x, int y)
{
	items::init(x, y);
	_Item->_Iname = "그린크리스탈";
	_Item->_Itype = GOLDITEM;
	_Item->_dropimg = IMAGEMANAGER->findImage("Green_Crystal");
	_Item->_Iprice = 1;

	return S_OK;
}