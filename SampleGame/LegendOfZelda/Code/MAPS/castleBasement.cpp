#include "stdafx.h"
#include "../stdafx.h"
#include "castleBasement.h"

castleBasement::castleBasement()
{

}

castleBasement::~castleBasement()
{

}

HRESULT castleBasement::init(void)
{
	_backgroundImage = IMAGEMANAGER->findImage("�ϼ���");
	_backgroundPixel = IMAGEMANAGER->findImage("�ϼ����ȼ�");

	return S_OK;
}
void castleBasement::release(void)
{

}
void castleBasement::update(void)
{

}
void castleBasement::render(void)
{
	_backgroundPixel->render(getMemDC());
	_backgroundImage->render(getMemDC());
}

