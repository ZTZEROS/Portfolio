#include "stdafx.h"
#include "../../stdafx.h"
#include "swords.h"


swords::swords()
{
}


swords::~swords()
{
}
HRESULT swords :: init(int x, int y)
{
	items::init(x, y);
	_Item->_Iname = "¼Òµå";
	_Item->_Itype = EQUIPITEM;
	_Item->_itemimg = IMAGEMANAGER->findImage("sword_01");

	return S_OK;
}