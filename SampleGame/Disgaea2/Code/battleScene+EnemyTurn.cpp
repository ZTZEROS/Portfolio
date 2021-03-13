#include "stdafx.h"
#include "battleScene.h"

 //턴 종료 조작시 1회 실행
//MoveEnemies(i);

void battleScene::EnemyTurn() //반복 실행
{

	//if(_EM.GetEnemiesVMoved(i)) EnemiesAttack(i);
	//
	//if (_EM.GetEnemiesVAttacked(i))
	//{
	//	++i;
	//	if(i < _EM.GetEnemiesV().size() ) MoveEnemies(i);
	//	else i = 0; //턴종료 추가
	//}
}

void battleScene::MoveEnemies(int index)
{
	POINT destination;
	int movingDistance;
	int distance;
	//POINT correction;
	//
	//correction = PointMake(0, 0);
	//
	//while ((*_battleMap->getTiles(_EM.GetIAIAddress()->GetCenterOfEnemies().x + correction.x, _EM.GetIAIAddress()->GetCenterOfEnemies().y + correction.y)).terrain == TR_WATER)
	//{
	//	correction.x += RND->getFromIntTo(-3, 3);
	//	correction.y += RND->getFromIntTo(-3, 3);
	//} 

	//for (int i = 0; i < _EM.GetEnemiesV().size(); i++)
	{
		//이하 조건 적타일, 이동불가능 타일, 타일 바깥이 되지 않도록 예외처리 필요
		destination = PointMake(0, 0);
		movingDistance = 0;
		distance = 100;

		for (int j = 0; j < _HM.GetHeroesV().size(); j++)
		{
			if (distance > (abs(_HM.GetHeroesVTileX(j) - _EM.GetEnemiesVTileX(index)) + abs(_HM.GetHeroesVTileY(j) - _EM.GetEnemiesVTileY(index))))
			{
				distance = abs(_HM.GetHeroesVTileX(j) - _EM.GetEnemiesVTileX(index)) + abs(_HM.GetHeroesVTileY(j) - _EM.GetEnemiesVTileY(index));
				destination.x = _HM.GetHeroesVTileX(j);
				destination.y = _HM.GetHeroesVTileY(j);
			}
		}

		if (destination.x > _EM.GetEnemiesVTileX(index)) destination.x -= 1;
		else if (destination.x < _EM.GetEnemiesVTileX(index)) destination.x += 1;
		else if (destination.y > _EM.GetEnemiesVTileY(index)) destination.y -= 1;
		else if (destination.y < _EM.GetEnemiesVTileY(index)) destination.y += 1;
		
		//destination = PointMake(15, 15);

		_battleMap->addOpenableTileV(PointMake(_EM.GetEnemiesVTileX(index), _EM.GetEnemiesVTileY(index)), 40, 1);
		_battleMap->pathFinder(PointMake(_EM.GetEnemiesVTileX(index), _EM.GetEnemiesVTileY(index)), destination, 40);
				
		if (_EM.GetEnemiesVMovableDistance(index) < _battleMap->GetPathsV().size()) movingDistance = _EM.GetEnemiesVMovableDistance(index);
		else movingDistance = _battleMap->GetPathsV().size() - 1;
		destination = PointMake(_battleMap->GetPathsV()[movingDistance].x, _battleMap->GetPathsV()[movingDistance].y);
		
		_battleMap->ClearMoveableTile();
		_battleMap->clearAllList();



		_battleMap->addOpenableTileV(PointMake(_EM.GetEnemiesVTileX(index), _EM.GetEnemiesVTileY(index)), _EM.GetEnemiesVMovableDistance(index), 1);
		_battleMap->pathFinder(PointMake(_EM.GetEnemiesVTileX(index), _EM.GetEnemiesVTileY(index)), destination, _EM.GetEnemiesVMovableDistance(index));

		_EM.RunEnemiesVWalk(index, _battleMap->GetPathsV());

		_battleMap->ClearMoveableTile();
		_battleMap->clearAllList();
	}
}

void battleScene::EnemiesAttack(int index)
{
	//for (int i = 0; i < _EM.GetEnemiesV().size(); i++)
	{
		//_battleMap->addOpenableTileV(PointMake(_EM.GetEnemiesVTileX(i), _EM.GetEnemiesVTileY(i)), 1, 0);
		_EM.AddEnemiesActionEntryV(index);
		//_EM.SetEnemiesVTargetTile(i, PointMake(_HM.GetHeroesVTileX(i), _HM.GetHeroesVTileX(i));
		//_EM.RunEnemiesVAttack(i, PointMake(_HM.GetHeroesVTileX(i), _HM.GetHeroesVTileX(i)));
	}
}

bool battleScene::CheckTurnEnd()
{
	bool turnEnd = 1;

	for (int i = 0; i < _EM.GetEnemiesV().size(); i++)
	{
		if (!_EM.GetEnemiesVMoved(i) || !_EM.GetEnemiesVAttacked(i)) turnEnd = 0;
	}

	return turnEnd;
}