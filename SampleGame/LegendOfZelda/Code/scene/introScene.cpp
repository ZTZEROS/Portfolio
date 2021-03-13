#include "stdafx.h"
#include "introScene.h"


introScene::introScene()
{
}


introScene::~introScene()
{
}
HRESULT introScene::init(void)
{
	STREAMMANAGER->startStream("../introMovie/introZelda.wmv");
	
	return S_OK;
}
void introScene::release(void)
{
	STREAMMANAGER->closeStream();
}
void introScene::update(void)
{
	
}
void introScene::render(void)
{

}