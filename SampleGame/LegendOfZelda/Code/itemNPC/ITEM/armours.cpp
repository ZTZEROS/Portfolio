#include "stdafx.h"
#include "../../stdafx.h"
#include "armours.h"


armours::armours()
{
}


armours::~armours()
{
}

HRESULT armours::init(int x, int y)
{
	items::init(x, y);
	_Item->_Iname = "ºÎ¸Þ¶û";
	_Item->_Itype = SUBITEM;
	_Item->_itemimg = IMAGEMANAGER->findImage("inven_boomerang");
	_Item->_dropimg = IMAGEMANAGER->findImage("boomerang");
	_Item->_nameimg = IMAGEMANAGER->findImage("name_boomerang");

	return S_OK;
}
