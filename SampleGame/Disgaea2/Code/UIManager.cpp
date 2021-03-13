#include "stdafx.h"
#include "UIManager.h"
#include "Actors\HeroesManagers.h"
#include "Actors\EnemiesManagers.h"
#include "Actors\Heroes\Heroes.h"

UIManager::UIManager()
	:_menuSwitch(MENU_OFF),
	_menus(BATTLEMENU),
	_statusPanel(PANEL_OFF),
	_inAction(false)
{
}


UIManager::~UIManager()
{
}

HRESULT UIManager::init(int NumOfPlayerUnits, int NumOfEnemyUnits)
{
	addUIImage(); //UI �̹��� ���~~
	_cursor = new cursor;
	_cursor->init(); //Ŀ�� Ŭ���� �̴�
	_battleMenu = new battleMenu;
	_battleMenu->init(); //��Ʋ�޴� Ŭ���� �̴�
	_turnMenu = new turnMenu;
	_turnMenu->init();
	_overviewPanel = new statusOverviewPanel;//��Ʋ�г� �̴�~~
	_overviewPanel->init(_cursor->getCursorCoordinateX() - WINSIZEX / 2, _cursor->getCursorCoordinateY() + WINSIZEY / 4);
	progressBarInit(NumOfEnemyUnits);//���α׷����� �̴�~~~
	_vHero = _heroManager->GetHeroesV();
	_heroList = new HeroList;
	_heroList->init(_vHero.size(), _cursor->getCursorCoordinateX() + WINSIZEX / 4, _cursor->getCursorCoordinateY() - WINSIZEY / 4);
	return S_OK;
}
void UIManager::release()
{
	SAFE_DELETE(_cursor);
	SAFE_DELETE(_battleMenu);
	SAFE_DELETE(_overviewPanel);
}
void UIManager::update(int NumOfPlayerUnits, int NumOfEnemyUnits, int numberOfHeroes)
{
	progressBarUpdate(numberOfHeroes, NumOfEnemyUnits); //���α׷����� ������Ʈ~~~
	//�г� ������Ʈ~~~
	_overviewPanel->update(
		_currentExp,_maxExp, 
		_currentHp, _maxHp, 
		_currentSp, _maxSp,
		_cursor->getCursorCoordinateX() - WINSIZEX / 2, 
		_cursor->getCursorCoordinateY() + WINSIZEY / 4);
	//�޴��� ������? �Ѡ���?
	switch (_menuSwitch)
	{
		//�޴��� ������?
	case MENU_OFF:_cursor->update(); //Ŀ���� ������Ʈ ���ְ�~~~
		break;
	case MENU_ON:
		//�޴��� �Ѡ���?
		switch (_menus)
		{
			//��Ʋ�޴���?
		case BATTLEMENU:
			_battleMenu->update(); //�޴��� �Ѡ��ٸ� ��Ʋ�Ŵ��� ������Ʈ ���ְ�~~
			break;
			//�ϸ޴���?
		case TURNMENU:
			switch(_turnMenu->getMenusInTurnMenu())
			{
			case RENDER_TURNMENU:
				_turnMenu->update(numberOfHeroes + NumOfEnemyUnits, _cursor->getCursorCoordinateX() + WINSIZEX / 4, _cursor->getCursorCoordinateY() - WINSIZEY / 4);
				break;
				//ĳ���͸���Ʈ â �׷���~~
			case RENDER_CHARACTER_LIST:
				_turnMenu->update(numberOfHeroes + NumOfEnemyUnits, _cursor->getCursorCoordinateX() + WINSIZEX / 4, _cursor->getCursorCoordinateY() - WINSIZEY / 4);
				_cursor->cursorDirection();
				if (_turnMenu->getCursorPlace() < _enemyManager->GetEnemiesV().size())
				{
					_cursor->setCursorCoordinateX(
						(_enemyManager->GetEnemiesV()[_turnMenu->getCursorPlace()]->GetTileX() -
							_enemyManager->GetEnemiesV()[_turnMenu->getCursorPlace()]->GetTileY()) * TILEWIDTH* 0.5 + BACKBUFFERSIZEX * 0.5);
					_cursor->setCursorCoordinateY(
						(_enemyManager->GetEnemiesV()[_turnMenu->getCursorPlace()]->GetTileX() +
							_enemyManager->GetEnemiesV()[_turnMenu->getCursorPlace()]->GetTileY()) * TILEHEIGHT* 0.5 + (TILEHEIGHT / 2) + BACKBUFFERSIZEY/3 - (_enemyManager->GetEnemiesV()[_turnMenu->getCursorPlace()]->GetFakeAltitude())/18);
					_cursor->setCursorDirection(CURSOR_STEADY);
				}
				else
				{
					_cursor->setCursorCoordinateX(
						(_heroManager->GetHeroesV()[_turnMenu->getCursorPlace() - _enemyManager->GetEnemiesV().size()]->GetTileX() -
							_heroManager->GetHeroesV()[_turnMenu->getCursorPlace() - _enemyManager->GetEnemiesV().size()]->GetTileY()) * TILEWIDTH* 0.5 + BACKBUFFERSIZEX * 0.5);
					_cursor->setCursorCoordinateY(
						(_heroManager->GetHeroesV()[_turnMenu->getCursorPlace() - _enemyManager->GetEnemiesV().size()]->GetTileX() +
							_heroManager->GetHeroesV()[_turnMenu->getCursorPlace() - _enemyManager->GetEnemiesV().size()]->GetTileY()) * TILEHEIGHT* 0.5 + (TILEHEIGHT / 2) + BACKBUFFERSIZEY/3 - _heroManager->GetHeroesV()[_turnMenu->getCursorPlace() - _enemyManager->GetEnemiesV().size()]->GetFakeAltitude());
					_cursor->setCursorDirection(CURSOR_STEADY);
				}
				break;
			}
		case HEROLIST:
			if(_heroList->getVHeroList().size()>0)_heroList->update();
			break;
		}
		break;
	}
}
void UIManager::render(int numberOfHeroes)
{
	//�޴��� ������ ������?
	switch (_menuSwitch)
	{
		//�޴��� ������?
	case MENU_OFF:
		break;
		//�޴��� �Ѡ���?
	case MENU_ON:
		//���� �޴���?
		switch (_menus)
		{
			//��Ʋ�޴���?
		case BATTLEMENU:
			_battleMenu->render(_cursor->getCursorCoordinateX() + WINSIZEX / 4, _cursor->getCursorCoordinateY() - WINSIZEY / 4);
			break;
			//�ϸ޴���?
		case TURNMENU:
			switch (_turnMenu->getMenusInTurnMenu())
			{
				//�ϸ޴�â �׷���~
			case RENDER_TURNMENU:
				_turnMenu->render(_cursor->getCursorCoordinateX() + WINSIZEX / 4, _cursor->getCursorCoordinateY() - WINSIZEY / 4);
				break;
				//ĳ���͸���Ʈ â �׷���~~
			case RENDER_CHARACTER_LIST:
				_turnMenu->render(_cursor->getCursorCoordinateX() + WINSIZEX / 4, _cursor->getCursorCoordinateY() - WINSIZEY / 4);
				renderCharacterList(numberOfHeroes);
				break;
			}
			break;
		case HEROLIST:
			_heroList->render(_cursor->getCursorCoordinateX() + WINSIZEX / 4, _cursor->getCursorCoordinateY() - WINSIZEY / 4);
			renderHeroList();
		}
		break;
	}
	//�г� ����ġ ����!
	switch (_statusPanel)
	{
		//����� ������!
	case PANEL_OFF:
		break;
		//����� �Ѡ���!
	case PANEL_ON:
		_overviewPanel->render(_currentName);
		break;
	}
	
}

void UIManager::cursorRender(int tileHeight)
{
	//Ŀ�� ������~~~~~
	_cursor->render(tileHeight);

}

void UIManager::progressBarRender(int numberOfCharacters)
{
	//���� ���α׷����� ������~~
	for (int i = 0; i < numberOfCharacters; i++)
	{
		_vHeroProgressBar[i]->render();
	}
	//�� ���α׷����� ������~~~
	for (int i = 0; i < _enemyManager->GetEnemiesV().size(); i++)
	{
		_vEnemyProgressBar[i]->render();
	}
}

void UIManager::renderCharacterList(int numberOfHeroes)
{
	int tempNum;
	for (int i = 0; i < numberOfHeroes; i++)
	{
		for (int j = 0; j < _heroManager->GetHeroesV()[i]->GetName().size(); j++)
		{
			string strName;
			ZeroMemory(&strName, sizeof(strName));
			strName = _heroManager->GetHeroesV()[i]->GetName();
			string chrName;
			ZeroMemory(&chrName, sizeof(strName));
			chrName = strName.at(j);
			if (chrName < "a")
			{
				tempNum = (int)strName.at(j);

				chrName = tempNum + 32;
			}
			if (_turnMenu->getCharacterList().size() == 0)break;
			IMAGEMANAGER->findImage(chrName.c_str())->render(getMemDC(),
				_turnMenu->getCharacterList()[_enemyManager->GetEnemiesV().size()].rc.left + j * 25, _turnMenu->getCharacterList()[_enemyManager->GetEnemiesV().size()].rc.top + 20 + (i * 50));
		}
	}
	for (int i = 0; i < _enemyManager->GetEnemiesV().size(); i++)
	{
		for (int j = 0; j < _enemyManager->GetEnemiesV()[i]->GetName().size(); j++)
		{
			if (j > 8)break;
			string strName;
			ZeroMemory(&strName, sizeof(strName));
			strName = _enemyManager->GetEnemiesV()[i]->GetName();
			string chrName;
			ZeroMemory(&chrName, sizeof(strName));
			chrName = strName.at(j);
			if (chrName < "a")
			{
				tempNum = (int)strName.at(j);

				chrName = tempNum + 32;
			}
			if (j > 5) chrName = "dot";
			if (_turnMenu->getCharacterList().size() == 0)break;
			IMAGEMANAGER->findImage(chrName.c_str())->render(getMemDC(),
				_turnMenu->getCharacterList()[0].rc.left + j * 25, _turnMenu->getCharacterList()[0].rc.top + 20 + (i * 50));
		}
	}
	
}
void UIManager::renderHeroList()
{
	int tempNum;


	for (int i =0 ; i < _heroList->getNumberOfHeroes(); i++)
	{
		for (int j = 0; j < _vHero[i]->GetName().size(); j++)
		{
			
			string strName;
			ZeroMemory(&strName, sizeof(strName));
			strName = _vHero[i]->GetName();
			string chrName;
			ZeroMemory(&chrName, sizeof(strName));
			chrName = strName.at(j); 
			if (chrName < "a")
			{
				tempNum = (int)strName.at(j);

				chrName = tempNum + 32;
			}
			if (_heroList->getVHeroList().size() == 0)break;
		IMAGEMANAGER->findImage(chrName.c_str())->render(getMemDC(),
				_heroList->getVHeroList()[0].rc.left + j * 25, _heroList->getVHeroList()[0].rc.top + i * 50);
		}
	}

}

void UIManager:: deleteVHero(void)
{
	_heroList->removeList(_heroList->getSelectedOne(),_cursor->getCursorCoordinateX() + WINSIZEX / 4, _cursor->getCursorCoordinateY() - WINSIZEY / 4);
}