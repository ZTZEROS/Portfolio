#include "stdafx.h"
#include "UIManager.h"
#include "Actors\HeroesManagers.h"
#include "Actors\EnemiesManagers.h"

void UIManager::progressBarInit(int NumOfEnemyUnits)
{
	char str[256];
	char str1[256];
	//적수만큼 프로그래스바 생성!
	for (int i = 0; i < NumOfEnemyUnits; i++)
	{
		memset(str, 0, sizeof(str));
		memset(str1, 0, sizeof(str1));
		sprintf_s(str, "enemyCurrent%d", i);
		sprintf_s(str1, "enemyMax%d", i);
		_progressBar = new progressBar;
		_progressBar->enemyInit(
			str,
			str1,
			_enemyManager->GetEnemiesV()[i]->GetCenterX(),
			_enemyManager->GetEnemiesV()[i]->GetCenterY() +
			(_enemyManager->GetEnemiesV()[i]->GetDNA().Image[is_IDLE]->getFrameHeight() / 2));
		_vEnemyProgressBar.push_back(_progressBar);
	}
	
}

void UIManager:: playerProgressBarInit()
{
	char str[256];
	char str1[256];
	int tempNum =RND->getInt(10000);
	//플레이어수만큼 포문을 돌려서 프로그래스바 생성!
	memset(str, 0, sizeof(str));
	memset(str1, 0, sizeof(str1));
	sprintf_s(str, "heroCurrent%d", tempNum);
	tempNum++;
	sprintf_s(str1, "heroMax%d", tempNum);
	
	_progressBar = new progressBar;
	_progressBar->playerInit(
		str,
		str1,
		_vHero[_heroList->getSelectedOne() ]->GetCenterX(),
		_vHero[_heroList->getSelectedOne() ]->GetCenterY() +
		(_vHero[_heroList->getSelectedOne()]->GetDNA().Image[is_IDLE]->getFrameHeight() / 2));
	_vHeroProgressBar.push_back(_progressBar);
	
}
void UIManager::progressBarUpdate(int NumOfPlayerUnits, int NumOfEnemyUnits)
{
	for (int i = 0; i < NumOfPlayerUnits; i++)
	{

		_vHeroProgressBar[i]->update(
			_vHero[i]->GetCenterX(),
			_vHero[i]->GetCenterY() - (_vHero[i]->GetDNA().Image[is_IDLE]->getFrameHeight() / 2) - _vHero[i]->GetFakeAltitude(),
			_vHero[i]->GetCurrentHP(),
			_vHero[i]->GetMaxHP());

	}
	for (int i = 0; i < NumOfEnemyUnits; i++)
	{
		_vEnemyProgressBar[i]->update(
			
			_enemyManager->GetEnemiesV()[i]->GetCenterX(),
			_enemyManager->GetEnemiesV()[i]->GetCenterY() - (_enemyManager->GetEnemiesV()[i]->GetDNA().Image[is_IDLE]->getFrameHeight() / 2) - _enemyManager->GetEnemiesV()[i]->GetFakeAltitude(),
			_enemyManager->GetEnemiesV()[i]->GetCurrentHP(),
			_enemyManager->GetEnemiesV()[i]->GetMaxHP());

	}
}
	