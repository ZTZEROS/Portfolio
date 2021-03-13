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

//메뉴가 꺼졋는지 켜졋는지
enum MENU_SWITCH
{
	MENU_OFF,
	MENU_ON
};
//베틀메뉴인가 턴종료메뉴인가
enum UI_MENUS
{
	BATTLEMENU,
	TURNMENU,
	HEROLIST
};
//페널이 꺼짐? 켜짐?
enum STATUS_PANEL
{
	PANEL_OFF,
	PANEL_ON
};
class UIManager :public gameNode
{
private:
	//프로그레스바 벡터에 담아놓자
	vector<progressBar*> _vHeroProgressBar;
	vector<progressBar*> _vEnemyProgressBar;
	vector<Heroes*> _vHero;
private:
	//****************************//
	//			클래스 변수		  //
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
	//			이넘문 변수		  //
	//****************************//
	MENU_SWITCH _menuSwitch;
	UI_MENUS _menus;
	STATUS_PANEL _statusPanel;
private:
	//****************************//
	//		캐릭터 능력치 변수		  //
	//****************************//
	int _currentHp, _maxHp, _currentExp, _maxExp, _currentSp, _maxSp;
	string _currentName; //현재커서가 가르키는 캐릭터 이름을 담을 변수
	bool _inAction; //캐릭터가 선택되서 행동준비중 판단 변수
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
	//			겟터		셋터		  //
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

