#include "stdafx.h"
#include "battleScene.h"

void battleScene:: battleSceneAndUI()
{
	
	//���࿡ Ŀ���� ������ �������
	if (_UIManager->getCursorClass()->getCursorDirection() == CURSOR_STEADY)
	{
		//���� ���ڸ�ŭ �ݺ�
		for (int i = 0; i < _vHeroOnTheBattle.size(); i++)
		{
			//���࿡ �������ڰ� 0�̰ų� �����ϰ�� �ݺ��� ��������
			if (_vHeroOnTheBattle.size() <= 0)break;
			//���࿡ ������ Ÿ�ϳѹ��� Ŀ�� �ѹ��� �������
			if ((*_vHeroOnTheBattle[i])->GetTileX() == _UIManager->getCursorClass()->getCurrentCursorTileX() &&
				(*_vHeroOnTheBattle[i])->GetTileY() == _UIManager->getCursorClass()->getCurrentCursorTileY())
			{
				//*******************************//
				//			��Ʋ����â ������Ʈ		 //
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
		//�� ���ڸ�ŭ �ݺ�
		for (int i = 0; i < _EM.GetEnemiesV().size(); i++)
		{
			//���� �����ڰ� 0�̰ų� �����ϰ�� �ݺ��� ������
			if (_EM.GetEnemiesV().size() <= 0)break;
			//���࿡ ���� Ÿ�ϳѹ��� Ŀ�� �ѹ��� �������
			if (_EM.GetEnemiesV()[i]->GetTileX() == _UIManager->getCursorClass()->getCurrentCursorTileX() &&
				_EM.GetEnemiesV()[i]->GetTileY() == _UIManager->getCursorClass()->getCurrentCursorTileY())
			{
				//*******************************//
				//			��Ʋ����â ������Ʈ		 //
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
	//Ŀ���� ������ �����ʰ� �����̴� ��� 
	else
	{
		_UIManager->setStatusPanel(PANEL_OFF);
	}
	
	//UI �Ŵ��� ������Ʈ!
	_UIManager->update(_HM.GetHeroesV().size(), _EM.GetEnemiesV().size(), _vHeroOnTheBattle.size());
}