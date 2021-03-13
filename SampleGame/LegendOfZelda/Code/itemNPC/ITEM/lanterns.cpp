#include "stdafx.h"
#include "../../stdafx.h"
#include "lanterns.h"


lanterns::lanterns()
{
}


lanterns::~lanterns()
{
}

HRESULT lanterns::init(int x, int y)
{
	items::init(x, y);
	_Item->_Iname = "Lamp";
	_Item->_IExplain = "램프를 얻었습니다! 어두움을 밝힐 수 있습니다.";
	_Item->_Itype = SUBITEM;
	_Item->_itemimg = IMAGEMANAGER->findImage("inven_lamp");
	_Item->_dropimg = IMAGEMANAGER->findImage("lamp");
	_Item->_nameimg = IMAGEMANAGER->findImage("name_lamp");

	return S_OK;
}