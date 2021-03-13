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
//			�ѿ��� ������~		     //
//*******************************//

//���� �������̴�~~~?
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
	//			Ŭ���� ����			 //
	//*******************************//
	battlemap* _battleMap; //��Ʋ�� Ŭ���� �ڷ��� ���� ����
	UIManager* _UIManager;
	UIstat* _UIstat;

private:
	//*******************************//
	//			ĳ���� �Ŵ���			 //
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
	void battleSceneAndUI(); //��Ʋ���� UI�� �������� �ٲ��� �Լ�
	void battleSceneMainControl(); //��Ʋ�� ���� ��Ʈ�� �Լ�
	void battleSceneCamera(); //��Ʋ�� ī�޶�
public:
	void EnemyTurn();
	void MoveEnemies(int index);
	void EnemiesAttack(int index);
	bool CheckTurnEnd();

	void putStat(void);

	battleScene();
	~battleScene();
};