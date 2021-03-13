#include "stdafx.h"
#include "../stdafx.h"
#include "gamePlayScene.h"


gamePlayScene::gamePlayScene()
{
}


gamePlayScene::~gamePlayScene()
{
}

HRESULT gamePlayScene::init()
{
	_link = new link;
	_link->init();
	_mapManager = new mapManager;
	_mapManager->init();
	_mapManager->setLinkMapMangerAddressLink(_link);
	_link->setMapManagerAddressLink(_mapManager);
	_mm = new monstermanager;
	_mm->init();

	_link->setUIManagerAddressLink(&_UIM);
	_UIM.SetIMAddress(_mapManager->getIM());
	_UIM.GetIM()->SetUIMAddress(&_UIM);
	_UIM.SetLinkAddress(_link);
	_UIM.Initialize(); //UIManager ÃÊ±âÈ­
	
	return S_OK;
}

void gamePlayScene::release()
{

}
void gamePlayScene::update()
{
	if (!_UIM.GetPaused())
	{
		_mapManager->update();
		if(_mapManager->getSlidingMap()==MAPSTEADY && !_UIM.GetTalking())_link->update();
		_mm->update();
	}

	_UIM.Update();
}
void gamePlayScene::render()
{
	_mapManager->render();
	_link->render();
	_mm->render();
	_UIM.Render();
}

