#include "stdafx.h"
#include "dungeonMap.h"


dungeonMap::dungeonMap()
{
}


dungeonMap::~dungeonMap()
{
}

HRESULT dungeonMap::init(int x, int y)
{
	items::init(x, y);
	_Item->_Iname = "던전지도";
	_Item->_Itype = DUNGEONITEM;
	_Item->_itemimg = IMAGEMANAGER->findImage("inven_map");
	_Item->_dropimg = IMAGEMANAGER->findImage("dungeonmap");

	return S_OK;
}