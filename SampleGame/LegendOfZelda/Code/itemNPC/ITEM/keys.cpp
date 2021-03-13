#include "stdafx.h"
#include "../../stdafx.h"
#include "keys.h"


keys::keys()
{
}


keys::~keys()
{
}

HRESULT keys::init(int x, int y)
{
	items::init(x, y);
	_Item->_Iname = "¿­¼è";
	_Item->_Itype = UIITEM;
	_Item->_dropimg = IMAGEMANAGER->findImage("small_key");

	return S_OK;
}

masterkeys::masterkeys()
{
}
masterkeys::~masterkeys()
{
}

HRESULT masterkeys::init(int x, int y)
{
	items::init(x, y);
	_Item->_Iname = "¸¶½ºÅÍ¿­¼è";
	_Item->_Itype = DUNGEONITEM;
	_Item->_itemimg = IMAGEMANAGER->findImage("inven_bigkey");
	_Item->_dropimg = IMAGEMANAGER->findImage("big_key");

	return S_OK;
}