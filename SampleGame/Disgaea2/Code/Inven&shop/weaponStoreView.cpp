
#include "stdafx.h"
#include "weaponStoreView.h"

weaponStoreView::weaponStoreView()
{

}

weaponStoreView::~weaponStoreView()
{

}

HRESULT weaponStoreView::init(void)
{
	_wsb = new weaponStoreBuy;
	_wsb->init();

	_wss = new weaponStoreSale;
	_wss->init();

	_inven = new inventory;
	_inven->init();

	_chooseBuy = RectMakeCenter(WINSIZEX/2, WINSIZEY/2 - 200, 300, 70);
	_chooseSale = RectMakeCenter(WINSIZEX/2, WINSIZEY/2 - 100, 300, 70);

	_cursorImage = IMAGEMANAGER->findImage("cursorImage");
	_cursorRc = RectMake(_chooseBuy.left - 15, (_chooseBuy.top + _chooseBuy.bottom) / 2 - 15, 50, 50);
	
	isPushButtonX = false;
	isChooseBox = CHOOSE_WEAPON_BUY;

	IMAGEMANAGER->addImage("slotimage", "UI_IMAGE/shopslot.bmp", 300, 70, true, RGB(255, 0, 255));

	return S_OK;
}
void weaponStoreView::release(void)
{
	_wsb->release();
	_wss->release();
	_inven->release();
}
void weaponStoreView::update(int money)
{
	_currentMoney = money;

	if(!isPushButtonX && KEYMANAGER->isOnceKeyDown(VK_DOWN))
	{
		SOUNDMANAGER->play("menuMove", 1.0f);
		_cursorRc = RectMake(_chooseSale.left - 15, (_chooseSale.top + _chooseSale.bottom) / 2 - 15, 50, 50);
		isChooseBox = CHOOSE_WEAPON_SALE;
	}
	if(!isPushButtonX && KEYMANAGER->isOnceKeyDown(VK_UP))
	{
		SOUNDMANAGER->play("menuMove", 1.0f);
		_cursorRc = RectMake(_chooseBuy.left - 15, (_chooseBuy.top + _chooseBuy.bottom) / 2 - 15, 50, 50);
		isChooseBox = CHOOSE_WEAPON_BUY;
	}
	if(!isPushButtonX && KEYMANAGER->isOnceKeyDown('X'))
	{
		SOUNDMANAGER->play("menuSelect", 1.0f);
		if(isChooseBox == CHOOSE_WEAPON_BUY) _wsb->setRegistItem(true);
		if(isChooseBox == CHOOSE_WEAPON_SALE) _wss->setFileLode(true); 
		isPushButtonX = true;
	}
	if(isPushButtonX && KEYMANAGER->isOnceKeyDown('C'))
	{
		SOUNDMANAGER->play("menucancel", 1.0f);
		if(isChooseBox == CHOOSE_WEAPON_BUY) _wsb->setPushDownC(true);
		if(isChooseBox == CHOOSE_WEAPON_SALE) _wss->setPushDownC(true);
		isPushButtonX = false;
	}
	if(!isPushButtonX && KEYMANAGER->isOnceKeyDown('C'))
	{
		//상점에서 나갈 때.
	}


	switch(isChooseBox)
	{
	case CHOOSE_WEAPON_BUY:
		_wsb->update(_currentMoney);
		_wsb->setPushDownC(false);
		_currentMoney = _wsb->getTotalMoney();
		break;
	case CHOOSE_WEAPON_SALE:
		_wss->update(_currentMoney);
		_wss->setPushDownC(false);
		_currentMoney = _wss->getTotalMoney();
		break;
	}
	_inven->update();
	_wsb->setInventoryDataLink(_inven);
}
void weaponStoreView::render(void)
{
	IMAGEMANAGER->findImage("storebg2")->render(getIMemDC(), 0, 0);
	HFONT font, oldfont;
	font = CreateFont(42, 0, 0, 0, 1000, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH, TEXT("맑은 고딕"));
	oldfont = (HFONT)SelectObject(getIMemDC(), font);
	SetTextColor(getIMemDC(), RGB(255, 255, 255));
	SetBkMode(getIMemDC(), TRANSPARENT);
	if(!isPushButtonX)
	{
		//Rectangle(getIMemDC(), _chooseBuy.left, _chooseBuy.top, _chooseBuy.right, _chooseBuy.bottom);
		IMAGEMANAGER->findImage("slotimage")->render(getIMemDC(), _chooseBuy.left, _chooseBuy.top);
		TextOut(getIMemDC(), _chooseBuy.left + 85, _chooseBuy.top + 10, "물건 사기", lstrlen("물건 사기"));
		//Rectangle(getIMemDC(), _chooseSale.left, _chooseSale.top, _chooseSale.right, _chooseSale.bottom);
		IMAGEMANAGER->findImage("slotimage")->render(getIMemDC(), _chooseSale.left, _chooseSale.top);
		TextOut(getIMemDC(), _chooseSale.left + 85, _chooseSale.top + 10, "물건 팔기", lstrlen("물건 팔기"));
		IMAGEMANAGER->findImage("cursorImage")->render(getIMemDC(), _cursorRc.left, _cursorRc.top);
	}
	else
	{
		switch(isChooseBox)
		{
			case CHOOSE_WEAPON_BUY:
				_wsb->render();
				break;
			case CHOOSE_WEAPON_SALE:
				_wss->render();
				break;
			case WEAPON_DEFAULT:
				break;
			default:
				break;
		}
	}
	SelectObject(getIMemDC(), oldfont);
	DeleteObject(font);
	//Rectangle(getMemDC(), _cursorRc.left, _cursorRc.top, _cursorRc.right, _cursorRc.bottom);
}
