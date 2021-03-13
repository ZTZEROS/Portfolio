#include "stdafx.h"
#include "testScene.h"


testScene::testScene()
{
}

testScene::~testScene()
{
}

HRESULT testScene::init(void)
{
	_npc = new npc;
	_npc->init();

	_fakePlayer.rc = RectMake(100, WINSIZEY/2 - 100, 100, 100);
	_fakePlayer.x = _fakePlayer.rc.left;
	_fakePlayer.y = WINSIZEY / 2;
	
	return S_OK;
}
void testScene::update(void)
{

}
void testScene::release(void)
{

}
void testScene::render(void)
{

}