
#include "stdafx.h"
#include "invenView.h"

invenView::invenView()
{

}

invenView::~invenView()
{

}

HRESULT invenView::init(void)
{

	_inven = new inventory;
	_inven->init();

	_house = new wareHouse;
	_house->init();


	return S_OK;
}

void invenView::release(void)
{
}

void invenView::update(void)
{
	_inven->update();
	_house->update();
	_inven->setWareHouseMemoryLink(_house);
	_house->setInventoryMemoryLink(_inven);
	
}

void invenView::render(void)
{
	IMAGEMANAGER->findImage("storebg1")->render(getIMemDC(), 0, 0);
	_inven->render();
	_house->render();
	//_info->render();
}