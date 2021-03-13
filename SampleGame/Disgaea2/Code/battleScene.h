#pragma once
#include "gameNode.h"
#include "battlemap.h"
#include "tileNode.h"
#include "Actors/HeroesManagers.h"
#include "Actors/EnemiesManagers.h"
#include "Actors/IntegratedAIs.h"
#include "UIManager.h"
#include "cursor.h"
#include "UIstat.h"

//*******************************//
//			한영아 힘내라~		     //
//*******************************//

//지금 누구턴이니~~~?
enum TURN
{
	PLAYERTURN,
	ENEMYTURN
};

#define ATTACKRANGE 1

class battleScene :public gameNode
{
private:
	vector<Heroes**> _vHeroOnTheBattle;
	vector<Heroes*> _vHero;
	//*******************************//
	//			클래스 변수			 //
	//*******************************//
	battlemap* _battleMap; //배틀맵 클래스 자료형 변수 선언
	UIManager* _UIManager;
	UIstat* _UIstat;

private:
	//*******************************//
	//			캐릭터 매니저			 //
	//*******************************//
	HeroesManagers _HM;
	EnemiesManagers _EM;
	IntegratedAIs _IAI;
	TURN _battleTurn;
	bool _characterMove;
	bool _statView;
	int _selectedCharacter;

public:
	HRESULT init();
	void release();
	void update();
	void render();
public:
	void battleSceneAndUI(); //배틀씬안 UI에 변수들을 바꿔줄 함수
	void battleSceneMainControl(); //배틀씬 메인 컨트롤 함수
	void battleSceneCamera(); //배틀씬 카메라
public:
	void EnemyTurn();
	void MoveEnemies(int index);
	void EnemiesAttack(int index);
	bool CheckTurnEnd();

	void putStat(void);

	battleScene();
	~battleScene();
};