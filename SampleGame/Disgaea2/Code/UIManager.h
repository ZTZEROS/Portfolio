#pragma once
#include "gameNode.h"
#include "cursor.h"
#include "progressBar.h"
#include "battleMenu.h"
#include "statusOverviewPanel.h"
#include "turnMenu.h"
#include "HeroList.h"


class HeroesManagers;
class EnemiesManagers;
class Heroes;

//�޴��� �������� �Ѡ�����
enum MENU_SWITCH
{
	MENU_OFF,
	MENU_ON
};
//��Ʋ�޴��ΰ� ������޴��ΰ�
enum UI_MENUS
{
	BATTLEMENU,
	TURNMENU,
	HEROLIST
};
//����� ����? ����?
enum STATUS_PANEL
{
	PANEL_OFF,
	PANEL_ON
};
class UIManager :public gameNode
{
private:
	//���α׷����� ���Ϳ� ��Ƴ���
	vector<progressBar*> _vHeroProgressBar;
	vector<progressBar*> _vEnemyProgressBar;
	vector<Heroes*> _vHero;
private:
	//****************************//
	//			Ŭ���� ����		  //
	//****************************//
	cursor* _cursor;
	progressBar* _progressBar;
	battleMenu* _battleMenu;
	statusOverviewPanel* _overviewPanel;
	HeroesManagers* _heroManager;
	EnemiesManagers* _enemyManager;

	turnMenu* _turnMenu;
	HeroList* _heroList;
private:
	//****************************//
	//			�̳ѹ� ����		  //
	//****************************//
	MENU_SWITCH _menuSwitch;
	UI_MENUS _menus;
	STATUS_PANEL _statusPanel;
private:
	//****************************//
	//		ĳ���� �ɷ�ġ ����		  //
	//****************************//
	int _currentHp, _maxHp, _currentExp, _maxExp, _currentSp, _maxSp;
	string _currentName; //����Ŀ���� ����Ű�� ĳ���� �̸��� ���� ����
	bool _inAction; //ĳ���Ͱ� ���õǼ� �ൿ�غ��� �Ǵ� ����
	bool boolhero1, boolhero2, boolhero3;
public:
	HRESULT init( int NumOfPlayerUnits, int NumOfEnemyUnits);
	void release();
	void update(int NumOfPlayerUnits, int NumOfEnemyUnits, int numberOfHeroes);
	void render(int numberOfHeroes);
	void addUIImage();
	void renderCharacterList(int numberOfHeroes);
	void renderHeroList();
	void cursorRender(int tileHeight);
	void progressBarInit(int NumOfEnemyUnits);
	void progressBarUpdate(int NumOfPlayerUnits, int NumOfEnemyUnits);
	void progressBarRender(int numberOfCharacters);
	void deleteVHero(void);
	void playerProgressBarInit(void);
public:
	//****************************//
	//			����		����		  //
	//****************************//
	UI_MENUS getUI_Menus(void) { return _menus; }
	battleMenu* getBattleMenu(void) { return _battleMenu; }
	turnMenu* getTurnMenu() { return _turnMenu; }
	cursor* getCursorClass(void) { return _cursor; }
	HeroList* getHeroList(void) { return _heroList; }
	progressBar* getProgressBar(void) { return _progressBar; }
	MENU_SWITCH getMenuSwitch(void) { return _menuSwitch; }
	vector<Heroes*> getVHero(void) { return _vHero; }
	bool getInAction(void) { return _inAction; }
	void setHMAddress(HeroesManagers* heroManager) { _heroManager = heroManager; }
	void setEMAddress(EnemiesManagers* enemyManager) { _enemyManager = enemyManager; }
	void setMenuSwitch(MENU_SWITCH menuswitch) { _menuSwitch = menuswitch; }
	void setUIMenus(UI_MENUS menus) { _menus = menus; }
	void setStatusPanel(STATUS_PANEL statusPanel) { _statusPanel = statusPanel; }
	void setCurrentHp(int currentHP) { _currentHp = currentHP; }
	void setMaxHp(int maxHP) { _maxHp = maxHP; }
	void setCurrentExp(int currentExp) { _currentExp = currentExp; }
	void setMaxExp(int maxExp) { _maxExp = maxExp; }
	void setCurrentSp(int currentSp) { _currentSp = currentSp; }
	void setMaxSp(int maxSp) { _maxSp = maxSp; }
	void setCurrentName(string name) { _currentName = name; }
	void setInAction(bool inAction) { _inAction = inAction; }
	
	UIManager();
	~UIManager();
};

