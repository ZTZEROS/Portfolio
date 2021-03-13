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
	_Item->_IExplain = "������ ������ϴ�! ��ο��� ���� �� �ֽ��ϴ�.";
	_Item->_Itype = SUBITEM;
	_Item->_itemimg = IMAGEMANAGER->findImage("inven_lamp");
	_Item->_dropimg = IMAGEMANAGER->findImage("lamp");
	_Item->_nameimg = IMAGEMANAGER->findImage("name_lamp");

	return S_OK;
}