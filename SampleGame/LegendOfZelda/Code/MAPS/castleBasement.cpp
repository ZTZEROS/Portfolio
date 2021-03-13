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
	_backgroundImage = IMAGEMANAGER->findImage("하수구");
	_backgroundPixel = IMAGEMANAGER->findImage("하수구픽셀");

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

