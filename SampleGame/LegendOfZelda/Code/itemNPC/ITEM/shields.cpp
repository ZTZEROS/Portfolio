#include "stdafx.h"
#include "../../stdafx.h"
#include "shields.h"


shields::shields()
{
}


shields::~shields()
{
}

HRESULT shields::init(int x, int y)
{
	items::init(x, y);
	_Item->_Iname = "½¯µå";
	_Item->_Itype = EQUIPITEM;
	_Item->_itemimg = IMAGEMANAGER->findImage("shield_01");

	return S_OK;
}