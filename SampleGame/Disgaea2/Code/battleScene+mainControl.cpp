#include "stdafx.h"
#include "battleScene.h"

void battleScene::battleSceneMainControl()
{
	//*******************************//
	//			X키를 눌럿을경우		 //
	//*******************************//

		if (KEYMANAGER->isOnceKeyDown('X'))
		{
			if (_UIManager->getCursorClass()->getCursorDirection() != CURSOR_STEADY)return;
			switch (_UIManager->getUI_Menus())
			{
				//배틀메뉴일떄~~~~
			case BATTLEMENU:
				if (_vHeroOnTheBattle.size() == 0)
				{
					if (_battleMap->getTiles(_UIManager->getCursorClass()->getCurrentCursorTileX(), _UIManager->getCursorClass()->getCurrentCursorTileY())->obj == OBJ_STARTPOTAL)
					{
						_UIManager->setMenuSwitch(MENU_ON);
						_UIManager->setUIMenus(HEROLIST);
					}
				}
				//영웅숫자만큼 반복
				for (int i = 0; i <_vHeroOnTheBattle.size(); i++)
				{
					//*******************************************//
					//   영웅의 타일숫자가 커서 타일과 같을경우		 //
					//*******************************************//
					if ((*(_vHeroOnTheBattle[i]))->GetTileX() == _UIManager->getCursorClass()->getCurrentCursorTileX() &&
						(*(_vHeroOnTheBattle[i]))->GetTileY() == _UIManager->getCursorClass()->getCurrentCursorTileY())
					{
						
						//메뉴스위치가 꺼졋니 켜졋니?>??
						switch (_UIManager->getMenuSwitch())
						{
							//메뉴가 꺼진상태
						case MENU_OFF:
							if (_UIManager->getInAction())break;
							//메뉴를 켜주고
							SOUNDMANAGER->play("menuSelect", 1.0f);
							_UIManager->setMenuSwitch(MENU_ON);
							_UIManager->setUIMenus(BATTLEMENU);
							_UIManager->getBattleMenu()->setMenus(MOVE);
							
							//에너미의 이동완료, 공격완료 부울 변수를 초기화
							//플레이어 턴이 왔는지 체크하는 부분에 위치해야 적절
							for (int i = 0; i < _EM.GetEnemiesV().size(); ++i)
							{
								_EM.SetEnemiesVMoved(i, 0);
								_EM.SetEnemiesVAttacked(i, 0);
							}
							//플레이어 턴이 왔는지 체크하는 부분에 위치해야 적절
							break;
							//메뉴가 켜져잇는 상태에서
						case MENU_ON:
							switch (_UIManager->getBattleMenu()->getMenus())
							{
							case NONE:
								break;
								//무브를 눌렷을경우!
							case MOVE:
								//
								SOUNDMANAGER->play("menuSelect", 1.0f);
								_UIManager->setInAction(true);
								//이동칸을 열어준다
								_battleMap->addOpenableTileV(
									PointMake(_UIManager->getCursorClass()->getCurrentCursorTileX(),
										_UIManager->getCursorClass()->getCurrentCursorTileY()),
									(*(_vHeroOnTheBattle[i]))->GetMovableDistance(), true);
								_selectedCharacter = i;
								//메뉴는 꺼준다
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
					//   영웅의 타일숫자가 커서 타일과 같지않을경우	 //
					//*******************************************//
					else
					{
						switch (_UIManager->getBattleMenu()->getMenus())
						{
						case NONE:
							//NONE일경우 아무 일도 하지않는다
							break;
							//무브상태일경우!
						case MOVE:
							//함수로 따로 빼는 것을 고려
							if (_battleMap->getTiles(_UIManager->getCursorClass()->getCurrentCursorTileX(), _UIManager->getCursorClass()->getCurrentCursorTileY())->activeTile)
							{
								//다른 영웅이 잇는 곳이라면 이동불가! 리턴해준다.
								for (int i = 0; i < _vHeroOnTheBattle.size(); i++)
								{
									if ((*(_vHeroOnTheBattle[i]))->GetTileX() == _UIManager->getCursorClass()->getCurrentCursorTileX() &&
										(*(_vHeroOnTheBattle[i]))->GetTileY() == _UIManager->getCursorClass()->getCurrentCursorTileY())
									{
										return;
									}
								}
								//길을 찾아주고
								SOUNDMANAGER->play("menuSelect", 1.0f);
								_battleMap->pathFinder(PointMake((*(_vHeroOnTheBattle[_selectedCharacter]))->GetTileX(), (*(_vHeroOnTheBattle[_selectedCharacter]))->GetTileY()),
									PointMake(_UIManager->getCursorClass()->getCurrentCursorTileX(), _UIManager->getCursorClass()->getCurrentCursorTileY())
									, (*(_vHeroOnTheBattle[_selectedCharacter]))->GetMovableDistance());
								(*(_vHeroOnTheBattle[_selectedCharacter]))->Walk(_battleMap->GetPathsV());
								//오픈타일들 클리어 해주고
								_battleMap->ClearMoveableTile();
								_battleMap->clearAllList();
								_UIManager->getBattleMenu()->setMenus(NONE);
								_UIManager->setInAction(false);
								_selectedCharacter = 0;
							}
							//함수로 따로 빼는 것을 고려, i에 현재 선택된 캐릭터가 들어가야 함
							break;
						case ATTACK:
								if (_selectedCharacter == i)
								{
									SOUNDMANAGER->play("menuSelect", 1.0f);
									_battleMap->ClearMoveableTile();
									_battleMap->clearAllList();
									_UIManager->getBattleMenu()->setMenus(NONE);
									//캐릭터에 해당하는 액션을 끝낸다
									_UIManager->setInAction(false);
									//공격타겟을 정한다
									_HM.SetHeroesVTargetTile(_selectedCharacter, PointMake(_UIManager->getCursorClass()->getCurrentCursorTileX(), _UIManager->getCursorClass()->getCurrentCursorTileY()));
									//엔트리에 넣어준다
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
				//턴메뉴일떄~~~~~~
			case TURNMENU:
				switch (_UIManager->getTurnMenu()->getTurnMenus())
				{
				case TURN_NONE:
					break;
				case EXECUTE:
					_UIManager->setMenuSwitch(MENU_OFF);
					_UIManager->setUIMenus(BATTLEMENU);
					SOUNDMANAGER->play("menuSelect", 1.0f);
					_HM.SetAct(1); //공격개시
					break;
				case END_TURN:
					_UIManager->setMenuSwitch(MENU_OFF);
					_UIManager->setUIMenus(BATTLEMENU);
					SOUNDMANAGER->play("menuSelect", 1.0f);

					//플레이어의 이동완료, 공격완료 부울 변수를 초기화
					//에너미 턴이 왔는지 체크하는 부분에 위치해야 적절
					for (int i = 0; i < _HM.GetHeroesV().size(); ++i)
					{
						_HM.SetHeroesVMoved(i, 0);
						_HM.SetHeroesVAttacked(i, 0);
					}
					//에너미 턴이 왔는지 체크하는 부분에 위치해야 적절
				
					EnemyTurn(); //적 턴 AI동작
					//_EM.SetAct(1); //공격개시
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
	//			C를 눌럿을경우		 //
	//*******************************//


	if (KEYMANAGER->isOnceKeyDown('C'))
	{
		if (_statView) _statView = false;
		if (_UIManager->getCursorClass()->getCursorDirection() != CURSOR_STEADY)return;
		switch (_UIManager->getMenuSwitch())
		{
			//메뉴가 꺼져있다면
		case MENU_OFF:
			switch (_UIManager->getBattleMenu()->getMenus())
			{
				//무브 상태라면
			case MOVE:
				SOUNDMANAGER->play("menucancel", 1.0f);
				//다시 메뉴 켜주고
				_UIManager->setMenuSwitch(MENU_ON);
				//이동영역표시 날려주고
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
			//메뉴가 켜져있다면
		case MENU_ON:
			SOUNDMANAGER->play("menucancel", 1.0f);
			_UIManager->setUIMenus(BATTLEMENU);
			//메뉴꺼주고
			_UIManager->setMenuSwitch(MENU_OFF);
			//메뉴상태 NONE으로 바꿔주고
			_UIManager->getBattleMenu()->setMenus(NONE);
			//턴메뉴상태 turn_none으로 바꿔주고
			_UIManager->getTurnMenu()->setTurnMenus(TURN_NONE);
			break;
		}
	}

	//*******************************//
	//			V를 눌럿을경우		//
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