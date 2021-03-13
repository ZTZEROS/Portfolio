#include "stdafx.h"
#include "battleScene.h"

battleScene::battleScene()
	:_selectedCharacter(0)
{
}


battleScene::~battleScene()
{
}

HRESULT battleScene::init()
{
	_battleMap = new battlemap;
	_battleMap->init();
	
	_UIstat = new UIstat;
	_UIstat->init();
	
	_HM.Initialize();
	_HM.SetEMAddress(&_EM);
	//_HM.SetInventoryAddress();

	this->putStat();
	_EM.Initialize();
	_EM.SetHMAddress(&_HM);
	_EM.SetIAIAddress(&_IAI);
	_IAI.Initialize();
	_IAI.SetEMAddress(&_EM);
	_vHero = _HM.GetHeroesV();
	_UIManager = new UIManager;
	_UIManager->setHMAddress(&_HM);
	_UIManager->setEMAddress(&_EM);
	_UIManager->init(_HM.GetHeroesV().size(),_EM.GetEnemiesV().size());

	
	CAMERA->addressUIManager(_UIManager);

	_characterMove = false;
	_statView = false;

	return S_OK;
}
void battleScene::release()
{		

}
void battleScene::update()
{
	CAMERA->setFocusX(_UIManager->getCursorClass()->getCursorCoordinateX());
	CAMERA->setFocusY(_UIManager->getCursorClass()->getCursorCoordinateY());

	_battleMap->update();
	_battleMap->onTheTileReset();

	for (int i = 0; i < _HM.GetHeroesV().size(); i++)
	{
		_battleMap->getTiles(_HM.GetHeroesV()[i]->GetTileX(), _HM.GetHeroesV()[i]->GetTileY())->onTile = ON_HERO;
	}

	for (int i = 0; i < _EM.GetEnemiesV().size(); i++)
	{
		_battleMap->getTiles(_EM.GetEnemiesV()[i]->GetTileX(), _EM.GetEnemiesV()[i]->GetTileY())->onTile = ON_ENEMY;
	}

	battleSceneAndUI();
	battleSceneMainControl();
	battleSceneCamera();
	CAMERA->update();
	

	KEYANIMANAGER->update();
	_HM.Update();
	_EM.Update();
	
	//Correct altitude of Heroes
	for (int i = 0; i < _HM.GetHeroesV().size(); i++)
	{
		_HM.SetHeroesVFakeAltitude(i, 48 * (*_battleMap->getTiles(_HM.GetHeroesVTileX(i), _HM.GetHeroesVTileY(i))).altitude);
	}

	//Correct altitude of Enemies
	for (int i = 0; i < _EM.GetEnemiesV().size(); i++)
	{
		_EM.SetEnemiesVFakeAltitude(i, 48 * (*_battleMap->getTiles(_EM.GetEnemiesVTileX(i), _EM.GetEnemiesVTileY(i))).altitude);
	}
}

void battleScene::render()
{
	for (int i = 0; i < TILENUMY; i++)
	{
		for (int j = 0; j < TILENUMX; j++)
		{
			//if ((j - i) * TILEWIDTH * 0.5 + TILEWIDTH * 0.5 + BACKBUFFERSIZEX * 0.5  >-50 + CAMERA->getCameraX() &&
			//	(j + i) * TILEHEIGHT * 0.5 + 200< WINSIZEY+50 + CAMERA->getCameraY() &&
			//	(j - i) * TILEWIDTH * 0.5 + TILEWIDTH * 0.5 - BACKBUFFERSIZEX * 0.5  < WINSIZEX+50 + CAMERA->getCameraX() &&
			//	(j + i) * TILEHEIGHT * 0.5 + TILEHEIGHT + 200 > -50 + CAMERA->getCameraY())
	
			if ((j - i) * TILEWIDTH * 0.5 + TILEWIDTH * 0.5 + BACKBUFFERSIZEX * 0.5 + TILEWIDTH * 0.5 < -80 + CAMERA->getCameraX() + CAMERA->getWheelValue() / 4) continue;
			if ((j + i) * TILEHEIGHT * 0.5 + BACKBUFFERSIZEY / 3 > CAMERA->getCameraY() - CAMERA->getWheelValue() / 4 + WINSIZEY + 80) continue;
			if ((j - i) * TILEWIDTH * 0.5 + TILEWIDTH * 0.5 + BACKBUFFERSIZEX * 0.5 - TILEWIDTH * 0.5 > 80 + CAMERA->getCameraX() - CAMERA->getWheelValue() / 3 + WINSIZEX) continue;
			if ((j + i) * TILEHEIGHT * 0.5 + BACKBUFFERSIZEY / 3 + TILEHEIGHT < CAMERA->getCameraY() + CAMERA->getWheelValue() / 3 - 80) continue;

			{
				_battleMap->render(j, i); //배틀맵 렌더링
				for (int k = 0; k < _vHeroOnTheBattle.size(); k++)
				{
					if ((*_vHeroOnTheBattle[k])->GetTileX() == j && (*_vHeroOnTheBattle[k])->GetTileY() == i) (*_vHeroOnTheBattle[k])->Render();
					//if (_HM.GetHeroesVTileX(k) == j && _HM.GetHeroesVTileY(k) == i) _HM.GetHeroesV()[k]->Render();
				}
				for (int k = 0; k < _EM.GetEnemiesV().size(); k++)
				{
					if (_EM.GetEnemiesVTileX(k) == j && _EM.GetEnemiesVTileY(k) == i) _EM.GetEnemiesV()[k]->Render();
				}
				//만약에 현재 커서 타일X 와 현재 커서 타일Y 가 j+i*TILENUMX 가 같을때
				if (_UIManager->getCursorClass()->getCurrentCursorTileX() + _UIManager->getCursorClass()->getCurrentCursorTileY() * TILENUMX == j + i*TILENUMX)
				{
					_UIManager->cursorRender(_battleMap->getTiles(_UIManager->getCursorClass()->getCurrentCursorTileX(), _UIManager->getCursorClass()->getCurrentCursorTileY())->altitude * (TILEHEIGHT / 2.1));
				}
			}
		}
	}
	
	//_HM.Render();
	//_EM.Render();

	_UIManager->progressBarRender(_vHeroOnTheBattle.size());
	//커서 상태가 steady일때만 UI 그려주기~
	if (_UIManager->getCursorClass()->getCursorDirection() == CURSOR_STEADY)
	{
		_UIManager->render(_vHeroOnTheBattle.size());
	}
	if (_statView) _UIstat->render(CAMERA->getCameraX(), CAMERA->getCameraY());
	
}

void battleScene::putStat(void)
{
	for (int i = 0; i < 3; i++)
	{
		_HM.SetHeroesVLevel(i, _UIstat->getCharStatIndex(i).lv);
		_HM.SetHeroesVMaxHP(i, _UIstat->getCharStatIndex(i).hp);
		_HM.SetHeroesVMaxSP(i, _UIstat->getCharStatIndex(i).sp);
		_HM.SetHeroesVCurrentHP(i, _UIstat->getCharStatIndex(i).currentHp);
		_HM.SetHeroesVCurrentSP(i, _UIstat->getCharStatIndex(i).currentSp);
		_HM.SetHeroesVAttack(i, _UIstat->getCharStatIndex(i).atk);
		_HM.SetHeroesVDefence(i, _UIstat->getCharStatIndex(i).def);
		_HM.SetHeroesVHit(i, _UIstat->getCharStatIndex(i).hit);
		_HM.SetHeroesVSpeed(i, _UIstat->getCharStatIndex(i).spd);
		_HM.SetHeroesVMagic(i, _UIstat->getCharStatIndex(i).inte);
		_HM.SetHeroesVResistance(i, _UIstat->getCharStatIndex(i).res);

	}
}