
#include "stdafx.h"
#include "armorStoreView.h"

armorStoreView::armorStoreView()
{

}

armorStoreView::~armorStoreView()
{

}

HRESULT armorStoreView::init(void)
{
	_asb = new armorStoreBuy;
	_asb->init();

	_ass = new armorStoreSale;
	_ass->init();

	_inven = new inventory;
	_inven->init();

	_chooseBuy = RectMakeCenter(WINSIZEX/2, WINSIZEY/2 - 200, 300, 70);
	_chooseSale = RectMakeCenter(WINSIZEX/2, WINSIZEY/2 - 100, 300, 70);

	_cursorImage = IMAGEMANAGER->findImage("cursorImage");
	_cursorRc = RectMake(_chooseBuy.left - 15, (_chooseBuy.top + _chooseBuy.bottom) / 2 - 15, 50, 50);

	IMAGEMANAGER->addImage("slotimage", "UI_IMAGE/shopslot.bmp", 300, 70, true, RGB(255, 0, 255));
	
	isPushButtonX = false;
	isChooseBox = CHOOSE_BUY;

	return S_OK;
}
void armorStoreView::release(void)
{
	_asb->release();
	_ass->release();
	_inven->release();
}
void armorStoreView::update(int money)
{
	_currentMoney = money;
	if(!isPushButtonX && KEYMANAGER->isOnceKeyDown(VK_DOWN))
	{
		SOUNDMANAGER->play("menuMove", 1.0f);
		_cursorRc = RectMake(_chooseSale.left - 15, (_chooseSale.top + _chooseSale.bottom) / 2 - 15, 50, 50);
		isChooseBox = CHOOSE_SALE;
	}
	if(!isPushButtonX && KEYMANAGER->isOnceKeyDown(VK_UP))
	{
		SOUNDMANAGER->play("menuMove", 1.0f);
		_cursorRc = RectMake(_chooseBuy.left - 15, (_chooseBuy.top + _chooseBuy.bottom) / 2 - 15, 50, 50);
		isChooseBox = CHOOSE_BUY;
	}
	if(!isPushButtonX && KEYMANAGER->isOnceKeyDown('X'))
	{
		SOUNDMANAGER->play("menuSelect", 1.0f);
		if(isChooseBox == CHOOSE_BUY) _asb->setRegistItem(true);
		if(isChooseBox == CHOOSE_SALE) _ass->setFileLode(true); 
		isPushButtonX = true;
	}
	if(isPushButtonX && KEYMANAGER->isOnceKeyDown('C'))
	{
		SOUNDMANAGER->play("menucancel", 1.0f);
		if(isChooseBox == CHOOSE_BUY) _asb->setPushDownC(true);
		if(isChooseBox == CHOOSE_SALE) _ass->setPushDownC(true);
		isPushButtonX = false;
	}
	if(!isPushButtonX && KEYMANAGER->isOnceKeyDown('C'))
	{
		//상점에서 나갈 때.
	}

	

	switch(isChooseBox)
	{
	case CHOOSE_BUY:
		_asb->update(_currentMoney);
		_asb->setPushDownC(false);
		_currentMoney = _asb->getTotalMoney();
		break;
	case CHOOSE_SALE:
		_ass->update(_currentMoney);
		_ass->setPushDownC(false);
		_currentMoney = _ass->getTotalMoney();
		break;
	}
	_inven->update();
	_asb->setInventoryDataLink(_inven);
}
void armorStoreView::render(void)
{
	IMAGEMANAGER->findImage("storebg1")->render(getIMemDC(), 0, 0);
	HFONT font, oldfont;
	font = CreateFont(42, 0, 0, 0, 1000, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH, TEXT("맑은 고딕"));
	oldfont = (HFONT)SelectObject(getIMemDC(), font);
	SetTextColor(getIMemDC(), RGB(255, 255, 255));
	SetBkMode(getIMemDC(), TRANSPARENT);
	if(!isPushButtonX)
	{
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
			case CHOOSE_BUY:
				_asb->render();
				break;
			case CHOOSE_SALE:
				_ass->render();
				break;
			case DEFAULT:
				break;
			default:
				break;
		}
	}
	SelectObject(getIMemDC(), oldfont);
	DeleteObject(font);
	//Rectangle(getMemDC(), _cursorRc.left, _cursorRc.top, _cursorRc.right, _cursorRc.bottom);
}
