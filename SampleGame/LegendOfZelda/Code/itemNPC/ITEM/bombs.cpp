#include "stdafx.h"
#include "../../stdafx.h"
#include "bombs.h"


bombs::bombs() {}
bombs::~bombs() {}

HRESULT bombs::init(int x, int y)
{
	items::init(x, y);
	_Item->_Iname = "�θ޶�";
	_Item->_Itype = SUBITEM;
	_Item->_itemimg = IMAGEMANAGER->findImage("inven_boomerang");
	_Item->_dropimg = IMAGEMANAGER->findImage("boomerang");
	_Item->_nameimg = IMAGEMANAGER->findImage("name_boomerang");

	return S_OK;
}

