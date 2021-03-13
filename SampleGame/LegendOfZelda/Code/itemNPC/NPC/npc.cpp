#include "stdafx.h"
#include "../../stdafx.h"
#include "npc.h"

npc::npc()
{
}

npc::~npc()
{
}

HRESULT npc::init(void)
{
	//_framecount = 0;
	//_npcRc1 = RectMakeCenter(_x, _y, _npcFImg[STATE_END].img->getFrameWidth(), _npcFImg[STATE_END].img->getFrameHeight());
	textframe = 0;
	Talking = 0;
	CurrentMenu = 0;
	return S_OK;
}
void npc::release(void)
{

}
void npc::update(void)
{
	//_npcRc1 = RectMakeCenter(_x, _y, _npcFImg[STATE_END].img->getFrameWidth(), _npcFImg[STATE_END].img->getFrameHeight());

}
void npc::render(void)
{
	HFONT normal = CreateFont(25, 0, 0, 0, 600, 0, 0, 0, HANGUL_CHARSET, 0, 0, 0, 0, TEXT("HY¿±¼­L"));
	SetTextColor(getMemDC(), RGB(255, 255, 255));
	SelectObject(getMemDC(), normal);
	//_npcFImg[_nState].img->frameRender(getMemDC(), _npcRc1.left, _npcRc1.top, _npcFImg[_nState].img->getFrameX(), 0);
}
void npc::npcUP()
{}
void npc::npcDOWN()
{}
void npc::npcLEFT()
{}
void npc::npcRIGHT()
{}
void npc::npcWEAPON()
{}
void npc::npcDIE()
{}
void npc::npcSIT()
{}
void npc::npcSITLEFT()
{}

void npc::npcWALKDOWN()
{}
void npc::npcWALKLEFT()
{}
void npc::npcWALKRIGHT()
{}

void npc::framePlay()
{
	//for (int i = 0; i < STATE_END; i++)
	//{
	//if (_framecount % 2 == 0)
	//{
	//	_npcFImg[_nState].img->setFrameX(_npcFImg[_nState].img->getFrameX() + 1);
	//	if (_npcFImg[_nState].img->getFrameX() >= _npcFImg[_nState].img->getMaxFrameX()) _npcFImg[_nState].img->setFrameX(0);
	//	_framecount = 0;
	//}
	//}
}