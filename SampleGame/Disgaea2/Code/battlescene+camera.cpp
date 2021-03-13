#include "stdafx.h"
#include "battleScene.h"

void battleScene::battleSceneCamera()
{
	//enemy, hero 걸을 때 카메라 회전 막기
	for (int i = 0; i < _EM.GetEnemiesV().size(); i++)
	{
		if (_EM.GetEnemiesV()[i]->GetCurrentAction() == is_WALKING) _characterMove = true;
		else _characterMove = false;
	}
	for (int i = 0; i < _HM.GetHeroesV().size(); i++)
	{
		if (_HM.GetHeroesV()[i]->GetCurrentAction() == is_WALKING) _characterMove = true;
		else _characterMove = false;
	}

	if (!_characterMove)
	{
		//카메라 오른쪽 회전
		if (KEYMANAGER->isOnceKeyDown('W'))
		{
			for (int i = 0; i < TILENUMY; i++)
			{
				for (int j = 0; j < TILENUMX; j++)
				{
					_battleMap->cameraTurnRight(j, i);
				}
			}

			//커서 회전
			int tempX, tempY;
			tempX = TILENUMX - 1 - _UIManager->getCursorClass()->getCurrentCursorTileY();
			tempY = _UIManager->getCursorClass()->getCurrentCursorTileX();
			_UIManager->getCursorClass()->setCursorCoordinateX((tempX - tempY)* TILEWIDTH * 0.5 + BACKBUFFERSIZEX * 0.5);
			_UIManager->getCursorClass()->setCursorCoordinateY((tempX + tempY)* TILEHEIGHT * 0.5 + (TILEHEIGHT / 2) + BACKBUFFERSIZEY/3);


			//enemy 회전
			for (int i = 0; i < _EM.GetEnemiesV().size(); i++)
			{
				int tempX, tempY;
				tempX = TILENUMY - 1 - _EM.GetEnemiesV()[i]->GetTileY();
				tempY = _EM.GetEnemiesV()[i]->GetTileX();

				_EM.SetEnemiesVTileX(i, tempX);
				_EM.SetEnemiesVTileY(i, tempY);
				_EM.SetEnemiesVCenterX(i, (_EM.GetEnemiesV()[i]->GetTileX() - _EM.GetEnemiesV()[i]->GetTileY()) * 96 + BACKBUFFERSIZEX * 0.5 + 12);
				_EM.SetEnemiesVCenterY(i, (_EM.GetEnemiesV()[i]->GetTileX() + _EM.GetEnemiesV()[i]->GetTileY()) * 48 + -20 + BACKBUFFERSIZEY / 3);

				if ((int)_EM.GetEnemiesV()[i]->GetCurrentDirection() != 0)
				{
					_EM.SetEnemiesVCurrentDirection(i, (CurrentDirectionsEnumeration)((int)_EM.GetEnemiesV()[i]->GetCurrentDirection() - 1));
				}
				else _EM.SetEnemiesVCurrentDirection(i, (CurrentDirectionsEnumeration)3);
			}

			//hero 회전
			for (int i = 0; i < _HM.GetHeroesV().size(); i++)
			{
				int tempX, tempY;
				tempY = _HM.GetHeroesV()[i]->GetTileX();
				tempX = TILENUMX - 1 - _HM.GetHeroesV()[i]->GetTileY();

				_HM.SetHeroesVTileX(i, tempX);
				_HM.SetHeroesVTileY(i, tempY);
				_HM.SetHeroesVCenterX(i, (_HM.GetHeroesV()[i]->GetTileX() - _HM.GetHeroesV()[i]->GetTileY()) * 96 + BACKBUFFERSIZEX * 0.5 + 12);
				_HM.SetHeroesVCenterY(i, (_HM.GetHeroesV()[i]->GetTileX() + _HM.GetHeroesV()[i]->GetTileY()) * 48 + -20 + BACKBUFFERSIZEY / 3);

				if ((int)_HM.GetHeroesV()[i]->GetCurrentDirection() != 0)
				{
					_HM.SetHeroesVCurrentDirection(i, (CurrentDirectionsEnumeration)((int)_HM.GetHeroesV()[i]->GetCurrentDirection() - 1));
				}
				else _HM.SetHeroesVCurrentDirection(i, (CurrentDirectionsEnumeration)3);
			}
		}

		//카메라 왼쪽 회전
		if (KEYMANAGER->isOnceKeyDown('Q'))
		{
			for (int i = 0; i < TILENUMY; i++)
			{
				for (int j = 0; j < TILENUMX; j++)
				{
					_battleMap->cameraTurnLeft(j, i);
				}
			}

			//커서 회전
			int tempX, tempY;
			tempX = _UIManager->getCursorClass()->getCurrentCursorTileY();
			tempY = TILENUMX - 1 - _UIManager->getCursorClass()->getCurrentCursorTileX();
			_UIManager->getCursorClass()->setCursorCoordinateX((tempX - tempY)* TILEWIDTH * 0.5 + BACKBUFFERSIZEX * 0.5);
			_UIManager->getCursorClass()->setCursorCoordinateY((tempX + tempY)* TILEHEIGHT * 0.5 + (TILEHEIGHT / 2) + BACKBUFFERSIZEY/3);

			//enemy 회전
			for (int i = 0; i < _EM.GetEnemiesV().size(); i++)
			{
				int tempX, tempY;
				tempX = _EM.GetEnemiesV()[i]->GetTileY();
				tempY = TILENUMX - 1 - _EM.GetEnemiesV()[i]->GetTileX();

				_EM.SetEnemiesVTileX(i, tempX);
				_EM.SetEnemiesVTileY(i, tempY);
				_EM.SetEnemiesVCenterX(i, (_EM.GetEnemiesV()[i]->GetTileX() - _EM.GetEnemiesV()[i]->GetTileY()) * 96 + BACKBUFFERSIZEX * 0.5 + 12);
				_EM.SetEnemiesVCenterY(i, (_EM.GetEnemiesV()[i]->GetTileX() + _EM.GetEnemiesV()[i]->GetTileY()) * 48 - 20 + BACKBUFFERSIZEY / 3);

				if ((int)_EM.GetEnemiesV()[i]->GetCurrentDirection() != 3)
				{
					_EM.SetEnemiesVCurrentDirection(i, (CurrentDirectionsEnumeration)((int)_EM.GetEnemiesV()[i]->GetCurrentDirection() + 1));
				}
				else _EM.SetEnemiesVCurrentDirection(i, (CurrentDirectionsEnumeration)0);
			}

			//hero 회전
			for (int i = 0; i < _HM.GetHeroesV().size(); i++)
			{
				int tempX, tempY;
				tempY = TILENUMX - 1 - _HM.GetHeroesV()[i]->GetTileX();
				tempX = _HM.GetHeroesV()[i]->GetTileY();

				_HM.SetHeroesVTileX(i, tempX);
				_HM.SetHeroesVTileY(i, tempY);
				_HM.SetHeroesVCenterX(i, (_HM.GetHeroesV()[i]->GetTileX() - _HM.GetHeroesV()[i]->GetTileY()) * 96 + BACKBUFFERSIZEX * 0.5 + 12);
				_HM.SetHeroesVCenterY(i, (_HM.GetHeroesV()[i]->GetTileX() + _HM.GetHeroesV()[i]->GetTileY()) * 48 - 20 + BACKBUFFERSIZEY / 3);

				if ((int)_HM.GetHeroesV()[i]->GetCurrentDirection() != 3)
				{
					_HM.SetHeroesVCurrentDirection(i, (CurrentDirectionsEnumeration)((int)_HM.GetHeroesV()[i]->GetCurrentDirection() + 1));
				}
				else _HM.SetHeroesVCurrentDirection(i, (CurrentDirectionsEnumeration)0);
			}

		}
	}
}