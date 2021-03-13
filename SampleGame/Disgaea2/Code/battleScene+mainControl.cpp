#include "stdafx.h"
#include "battleScene.h"

void battleScene::battleSceneMainControl()
{
	//*******************************//
	//			XŰ�� ���������		 //
	//*******************************//

		if (KEYMANAGER->isOnceKeyDown('X'))
		{
			if (_UIManager->getCursorClass()->getCursorDirection() != CURSOR_STEADY)return;
			switch (_UIManager->getUI_Menus())
			{
				//��Ʋ�޴��ϋ�~~~~
			case BATTLEMENU:
				if (_vHeroOnTheBattle.size() == 0)
				{
					if (_battleMap->getTiles(_UIManager->getCursorClass()->getCurrentCursorTileX(), _UIManager->getCursorClass()->getCurrentCursorTileY())->obj == OBJ_STARTPOTAL)
					{
						_UIManager->setMenuSwitch(MENU_ON);
						_UIManager->setUIMenus(HEROLIST);
					}
				}
				//�������ڸ�ŭ �ݺ�
				for (int i = 0; i <_vHeroOnTheBattle.size(); i++)
				{
					//*******************************************//
					//   ������ Ÿ�ϼ��ڰ� Ŀ�� Ÿ�ϰ� �������		 //
					//*******************************************//
					if ((*(_vHeroOnTheBattle[i]))->GetTileX() == _UIManager->getCursorClass()->getCurrentCursorTileX() &&
						(*(_vHeroOnTheBattle[i]))->GetTileY() == _UIManager->getCursorClass()->getCurrentCursorTileY())
					{
						
						//�޴�����ġ�� ������ �Ѡ���?>??
						switch (_UIManager->getMenuSwitch())
						{
							//�޴��� ��������
						case MENU_OFF:
							if (_UIManager->getInAction())break;
							//�޴��� ���ְ�
							SOUNDMANAGER->play("menuSelect", 1.0f);
							_UIManager->setMenuSwitch(MENU_ON);
							_UIManager->setUIMenus(BATTLEMENU);
							_UIManager->getBattleMenu()->setMenus(MOVE);
							
							//���ʹ��� �̵��Ϸ�, ���ݿϷ� �ο� ������ �ʱ�ȭ
							//�÷��̾� ���� �Դ��� üũ�ϴ� �κп� ��ġ�ؾ� ����
							for (int i = 0; i < _EM.GetEnemiesV().size(); ++i)
							{
								_EM.SetEnemiesVMoved(i, 0);
								_EM.SetEnemiesVAttacked(i, 0);
							}
							//�÷��̾� ���� �Դ��� üũ�ϴ� �κп� ��ġ�ؾ� ����
							break;
							//�޴��� �����մ� ���¿���
						case MENU_ON:
							switch (_UIManager->getBattleMenu()->getMenus())
							{
							case NONE:
								break;
								//���긦 ���������!
							case MOVE:
								//
								SOUNDMANAGER->play("menuSelect", 1.0f);
								_UIManager->setInAction(true);
								//�̵�ĭ�� �����ش�
								_battleMap->addOpenableTileV(
									PointMake(_UIManager->getCursorClass()->getCurrentCursorTileX(),
										_UIManager->getCursorClass()->getCurrentCursorTileY()),
									(*(_vHeroOnTheBattle[i]))->GetMovableDistance(), true);
								_selectedCharacter = i;
								//�޴��� ���ش�
								_UIManager->setMenuSwitch(MENU_OFF);

								break;
							case ATTACK:
								SOUNDMANAGER->play("menuSelect", 1.0f);
								_HM.AddHeroesActionEntryV(i);
								_battleMap->SearchAttackableTile(PointMake(_UIManager->getCursorClass()->getCurrentCursorTileX(),
									_UIManager->getCursorClass()->getCurrentCursorTileY()));
								_selectedCharacter = i;
								_UIManager->setMenuSwitch(MENU_OFF);
								break;
							case DEFEND:
								break;
							case ITEM:
								break;
							case EQUIPMENT:
								SOUNDMANAGER->play("menuSelect", 1.0f);
								tagStat charStat;
								charStat.sName = _HM.GetHeroesV()[i]->GetName();
								charStat.lv = _HM.GetHeroesV()[i]->GetLevel();
								charStat.hp = _HM.GetHeroesV()[i]->GetMaxHP();
								charStat.sp = _HM.GetHeroesV()[i]->GetMaxSP();
								charStat.currentHp = _HM.GetHeroesV()[i]->GetCurrentHP();
								charStat.currentSp = _HM.GetHeroesV()[i]->GetCurrentSP();
								charStat.atk = _HM.GetHeroesV()[i]->GetAttack();
								charStat.def = _HM.GetHeroesV()[i]->GetDefence();
								charStat.inte = _HM.GetHeroesV()[i]->GetMagic();
								charStat.res = _HM.GetHeroesV()[i]->GetResistance();
								charStat.hit = _HM.GetHeroesV()[i]->GetHit();
								charStat.spd = _HM.GetHeroesV()[i]->GetSpeed();
								charStat.exp = 50;
								charStat.next = 100;
								_UIstat->setCharStat(charStat);
								_statView = true;
								break;
							}
							break;
						}
					}
					else if (_battleMap->getTiles(_UIManager->getCursorClass()->getCurrentCursorTileX(), _UIManager->getCursorClass()->getCurrentCursorTileY())->obj == OBJ_STARTPOTAL)
					{
						_UIManager->setMenuSwitch(MENU_ON);
						_UIManager->setUIMenus(HEROLIST);
						
					}
					//*******************************************//
					//   ������ Ÿ�ϼ��ڰ� Ŀ�� Ÿ�ϰ� �����������	 //
					//*******************************************//
					else
					{
						switch (_UIManager->getBattleMenu()->getMenus())
						{
						case NONE:
							//NONE�ϰ�� �ƹ� �ϵ� �����ʴ´�
							break;
							//��������ϰ��!
						case MOVE:
							//�Լ��� ���� ���� ���� ���
							if (_battleMap->getTiles(_UIManager->getCursorClass()->getCurrentCursorTileX(), _UIManager->getCursorClass()->getCurrentCursorTileY())->activeTile)
							{
								//�ٸ� ������ �մ� ���̶�� �̵��Ұ�! �������ش�.
								for (int i = 0; i < _vHeroOnTheBattle.size(); i++)
								{
									if ((*(_vHeroOnTheBattle[i]))->GetTileX() == _UIManager->getCursorClass()->getCurrentCursorTileX() &&
										(*(_vHeroOnTheBattle[i]))->GetTileY() == _UIManager->getCursorClass()->getCurrentCursorTileY())
									{
										return;
									}
								}
								//���� ã���ְ�
								SOUNDMANAGER->play("menuSelect", 1.0f);
								_battleMap->pathFinder(PointMake((*(_vHeroOnTheBattle[_selectedCharacter]))->GetTileX(), (*(_vHeroOnTheBattle[_selectedCharacter]))->GetTileY()),
									PointMake(_UIManager->getCursorClass()->getCurrentCursorTileX(), _UIManager->getCursorClass()->getCurrentCursorTileY())
									, (*(_vHeroOnTheBattle[_selectedCharacter]))->GetMovableDistance());
								(*(_vHeroOnTheBattle[_selectedCharacter]))->Walk(_battleMap->GetPathsV());
								//����Ÿ�ϵ� Ŭ���� ���ְ�
								_battleMap->ClearMoveableTile();
								_battleMap->clearAllList();
								_UIManager->getBattleMenu()->setMenus(NONE);
								_UIManager->setInAction(false);
								_selectedCharacter = 0;
							}
							//�Լ��� ���� ���� ���� ���, i�� ���� ���õ� ĳ���Ͱ� ���� ��
							break;
						case ATTACK:
								if (_selectedCharacter == i)
								{
									SOUNDMANAGER->play("menuSelect", 1.0f);
									_battleMap->ClearMoveableTile();
									_battleMap->clearAllList();
									_UIManager->getBattleMenu()->setMenus(NONE);
									//ĳ���Ϳ� �ش��ϴ� �׼��� ������
									_UIManager->setInAction(false);
									//����Ÿ���� ���Ѵ�
									_HM.SetHeroesVTargetTile(_selectedCharacter, PointMake(_UIManager->getCursorClass()->getCurrentCursorTileX(), _UIManager->getCursorClass()->getCurrentCursorTileY()));
									//��Ʈ���� �־��ش�
									_HM.AddHeroesActionEntryV(_selectedCharacter);
								}
							break;
						case DEFEND:
							break;
						case ITEM:
							break;
						case EQUIPMENT:
							break;
						}
					}
				}
				break;
				//�ϸ޴��ϋ�~~~~~~
			case TURNMENU:
				switch (_UIManager->getTurnMenu()->getTurnMenus())
				{
				case TURN_NONE:
					break;
				case EXECUTE:
					_UIManager->setMenuSwitch(MENU_OFF);
					_UIManager->setUIMenus(BATTLEMENU);
					SOUNDMANAGER->play("menuSelect", 1.0f);
					_HM.SetAct(1); //���ݰ���
					break;
				case END_TURN:
					_UIManager->setMenuSwitch(MENU_OFF);
					_UIManager->setUIMenus(BATTLEMENU);
					SOUNDMANAGER->play("menuSelect", 1.0f);

					//�÷��̾��� �̵��Ϸ�, ���ݿϷ� �ο� ������ �ʱ�ȭ
					//���ʹ� ���� �Դ��� üũ�ϴ� �κп� ��ġ�ؾ� ����
					for (int i = 0; i < _HM.GetHeroesV().size(); ++i)
					{
						_HM.SetHeroesVMoved(i, 0);
						_HM.SetHeroesVAttacked(i, 0);
					}
					//���ʹ� ���� �Դ��� üũ�ϴ� �κп� ��ġ�ؾ� ����
				
					EnemyTurn(); //�� �� AI����
					//_EM.SetAct(1); //���ݰ���
					_battleTurn = ENEMYTURN;
					break;
				case CHARACTER_LIST:
					SOUNDMANAGER->play("menuSelect", 1.0f);
					_UIManager->getTurnMenu()->setMenusInTurnMenu(RENDER_CHARACTER_LIST);
					break;
				case BONUS:
					break;
				case HELP:
					break;
				case GIVEUP:
					break;
				case SETTING:
					break;
				}
				break;
			case HEROLIST:
				if (_UIManager->getHeroList()->getVHeroList().size() <1) break;
				_vHeroOnTheBattle.push_back(&((*_HM.GetHeroesVAddress())[_UIManager->getHeroList()->getSelectedOne()]));
				_UIManager->playerProgressBarInit();
				_UIManager->deleteVHero();
				_UIManager->setMenuSwitch(MENU_OFF);
				_UIManager->setUIMenus(BATTLEMENU);
			
				break;
			}
		}

	
	//*******************************//
	//			C�� ���������		 //
	//*******************************//


	if (KEYMANAGER->isOnceKeyDown('C'))
	{
		if (_statView) _statView = false;
		if (_UIManager->getCursorClass()->getCursorDirection() != CURSOR_STEADY)return;
		switch (_UIManager->getMenuSwitch())
		{
			//�޴��� �����ִٸ�
		case MENU_OFF:
			switch (_UIManager->getBattleMenu()->getMenus())
			{
				//���� ���¶��
			case MOVE:
				SOUNDMANAGER->play("menucancel", 1.0f);
				//�ٽ� �޴� ���ְ�
				_UIManager->setMenuSwitch(MENU_ON);
				//�̵�����ǥ�� �����ְ�
				_battleMap->ClearMoveableTile();
				_UIManager->setInAction(false);
				break;
			case ATTACK:
				break;
			case DEFEND:
				break;
			case ITEM:
				break;
			case EQUIPMENT:
				
				break;
			}
			break;
			//�޴��� �����ִٸ�
		case MENU_ON:
			SOUNDMANAGER->play("menucancel", 1.0f);
			_UIManager->setUIMenus(BATTLEMENU);
			//�޴����ְ�
			_UIManager->setMenuSwitch(MENU_OFF);
			//�޴����� NONE���� �ٲ��ְ�
			_UIManager->getBattleMenu()->setMenus(NONE);
			//�ϸ޴����� turn_none���� �ٲ��ְ�
			_UIManager->getTurnMenu()->setTurnMenus(TURN_NONE);
			break;
		}
	}

	//*******************************//
	//			V�� ���������		//
	//*******************************//
	if (KEYMANAGER->isOnceKeyDown('V'))
	{
		if (_UIManager->getCursorClass()->getCursorDirection() != CURSOR_STEADY)return;
		if (_UIManager->getMenuSwitch() == MENU_ON)return;
		_UIManager->setMenuSwitch(MENU_ON);
		_UIManager->setUIMenus(TURNMENU);
		_UIManager->getTurnMenu()->setMenusInTurnMenu(RENDER_TURNMENU);
		_UIManager->getTurnMenu()->setTurnMenus(EXECUTE);
	}
}