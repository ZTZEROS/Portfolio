#include "stdafx.h"
#include "battleScene.h"

void battleScene:: battleSceneAndUI()
{
	
	//만약에 커서가 가만히 잇을경우
	if (_UIManager->getCursorClass()->getCursorDirection() == CURSOR_STEADY)
	{
		//영웅 숫자만큼 반복
		for (int i = 0; i < _vHeroOnTheBattle.size(); i++)
		{
			//만약에 영웅숫자가 0이거나 이하일경우 반복문 빠져나감
			if (_vHeroOnTheBattle.size() <= 0)break;
			//만약에 영웅의 타일넘버가 커서 넘버랑 같을경우
			if ((*_vHeroOnTheBattle[i])->GetTileX() == _UIManager->getCursorClass()->getCurrentCursorTileX() &&
				(*_vHeroOnTheBattle[i])->GetTileY() == _UIManager->getCursorClass()->getCurrentCursorTileY())
			{
				//*******************************//
				//			배틀상태창 업데이트		 //
				//*******************************//
				
				_UIManager->setStatusPanel(PANEL_ON); 
				_UIManager->setCurrentExp(100.0f); 
				_UIManager->setMaxExp(100.0f);	
				_UIManager->setCurrentHp((*_vHeroOnTheBattle[i])->GetCurrentHP());
				_UIManager->setMaxHp((*_vHeroOnTheBattle[i])->GetMaxHP());
				_UIManager->setCurrentSp((*_vHeroOnTheBattle[i])->GetCurrentSP());
				_UIManager->setMaxSp((*_vHeroOnTheBattle[i])->GetMaxSP());
				_UIManager->setCurrentName((*_vHeroOnTheBattle[i])->GetName());
			}	
		}
		//적 숫자만큼 반복
		for (int i = 0; i < _EM.GetEnemiesV().size(); i++)
		{
			//만약 적숫자가 0이거나 이하일경우 반복문 나가기
			if (_EM.GetEnemiesV().size() <= 0)break;
			//만약에 적의 타일넘버가 커서 넘버와 같을경우
			if (_EM.GetEnemiesV()[i]->GetTileX() == _UIManager->getCursorClass()->getCurrentCursorTileX() &&
				_EM.GetEnemiesV()[i]->GetTileY() == _UIManager->getCursorClass()->getCurrentCursorTileY())
			{
				//*******************************//
				//			배틀상태창 업데이트		 //
				//*******************************//
				_UIManager->setStatusPanel(PANEL_ON);
				_UIManager->setCurrentExp(10.0f);
				_UIManager->setMaxExp(10.0f);
				_UIManager->setCurrentHp(_EM.GetEnemiesV()[i]->GetCurrentHP());
				_UIManager->setMaxHp(_EM.GetEnemiesV()[i]->GetMaxHP());
				_UIManager->setCurrentSp(_EM.GetEnemiesV()[i]->GetCurrentSP());
				_UIManager->setMaxSp(_EM.GetEnemiesV()[i]->GetMaxSP());
				_UIManager->setCurrentName(_EM.GetEnemiesV()[i]->GetName());
			}
		}

	}
	//커서가 가만히 있지않고 움직이는 경우 
	else
	{
		_UIManager->setStatusPanel(PANEL_OFF);
	}
	
	//UI 매니저 업데이트!
	_UIManager->update(_HM.GetHeroesV().size(), _EM.GetEnemiesV().size(), _vHeroOnTheBattle.size());
}