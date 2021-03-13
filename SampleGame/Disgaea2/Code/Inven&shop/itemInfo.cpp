
#include "stdafx.h"
#include "itemInfo.h"


itemInfo::itemInfo()
{

}

itemInfo::~itemInfo()
{

}

HRESULT itemInfo::init(void)
{
	_itemInfoImage = IMAGEMANAGER->findImage("itemInfoImage");
	_textEx = IMAGEMANAGER->findImage("itemex");

	return S_OK;
}
void itemInfo::update(strItemInfo sinfo)
{
	_sinfo = sinfo;
}

void itemInfo::render(void)
{
	IMAGEMANAGER->findImage("itemInfoImage")->render(getIMemDC(), 660, 90);
	IMAGEMANAGER->findImage("itemex")->render(getIMemDC(), 50, 650);
	HFONT font, oldfont;
	font = CreateFont(32, 0, 0, 0, 1000, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH, TEXT("¸¼Àº °íµñ"));
	oldfont = (HFONT)SelectObject(getIMemDC(), font);
	SetTextColor(getIMemDC(), RGB(255, 255, 255));
	SetBkMode(getIMemDC(), TRANSPARENT);

	TextOut(getIMemDC(), 750, 103, _sinfo.itemName, lstrlen(_sinfo.itemName));
	TextOut(getIMemDC(), 800, 138, _sinfo.recoveryHP, lstrlen(_sinfo.recoveryHP));
	TextOut(getIMemDC(), 800, 158, _sinfo.recoverySP, lstrlen(_sinfo.recoverySP));
	//TextOut(getIMemDC(), 750, 180, _sinfo.regulaterHP, lstrlen(_sinfo.regulaterHP));
	//TextOut(getIMemDC(), 750, 210, _sinfo.regulaterSP, lstrlen(_sinfo.regulaterSP));
	TextOut(getIMemDC(), 800, 186, _sinfo.regulaterAttack, lstrlen(_sinfo.regulaterAttack));
	TextOut(getIMemDC(), 950, 186, _sinfo.regulaterDefence, lstrlen(_sinfo.regulaterDefence));
	TextOut(getIMemDC(), 800, 226, _sinfo.regulaterHit, lstrlen(_sinfo.regulaterHit));
	TextOut(getIMemDC(), 800, 206, _sinfo.regulaterMagic, lstrlen(_sinfo.regulaterMagic));
	TextOut(getIMemDC(), 950, 226, _sinfo.regulaterSpeed, lstrlen(_sinfo.regulaterSpeed));
	TextOut(getIMemDC(), 950, 206, _sinfo.regulaterResistance, lstrlen(_sinfo.regulaterResistance));
	TextOut(getIMemDC(), 180, 660, _sinfo.itemIndex, lstrlen(_sinfo.itemIndex));

	SelectObject(getIMemDC(), oldfont);
	DeleteObject(font);
}