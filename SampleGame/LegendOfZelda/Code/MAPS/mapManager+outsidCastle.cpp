#include "stdafx.h"
#include "../stdafx.h"
#include "mapManager.h"
#include "../link/link.h"

void mapManager::houseInit(void)
{
	_houseMap = new insideHouse;
	_houseMap->init();
	for (int i = 0; i < 3; i++)
	{
		_jar = new jar;
		_jar->init(191, 313 + i * 64);
		_vJar.push_back(_jar);
	}
	_chest = new chestBox;
	_chest->init(800, 670);

	//npc 삼촌, 공주
	_uncle = new uncle;
	_uncle->init();
	_princess = new princess;
	_princess->init();
}

void mapManager::houseRender(void)
{
	_houseMap->render();
	for (int i = 0; i < _vJar.size(); i++)
	{
		_vJar[i]->render();
	}
	_chest->render();

	//npc 삼촌, 공주 렌더
	_uncle->render();
	_princess->render();
}

void mapManager::houseItemUpdate(void)
{
	//**********************************************//
	//					보물상자						//
	//**********************************************//
	_chest->update(); //상자 클래스 업데이트 넣어주고~
	if (_linkAndMap->getItemOpen())//링크가 박스를 건드렷니?
	{
		if (_chest->getObjectState() == UNTOUCHED) _IM->CreateLamp(_chest->getObjectCoordinateX(), _chest->getObjectCoordinateY());
		_chest->setObjectState(TOUCHED);
	}
	//**********************************************//
	//						항아리					//
	//**********************************************//
	for (int i = 0; i < _vJar.size(); i++)
	{
		_vJar[i]->update();
	}
	for (int i = 0; i < _vJar.size(); i++)//링크가 항아릴 들엇니?
	{
		if (_vJar[i]->getIsLift())
		{
			if (_vJar[i]->getObjectState() == UNTOUCHED) _IM->CreateLifeHeart(_vJar[i]->getObjectCoordinateX(), _vJar[i]->getObjectCoordinateY());
			_vJar[i]->setObjectState(TOUCHED);
		}
	}
	for (int i = 0; i < _vJar.size(); i++)
	{
		if (_vJar[i]->getisFire())
		{
			for (int j = _vJar[i]->getThrowObjectY() - 5; j < _vJar[i]->getThrowObjectX(); j++)
			{
				COLORREF color = GetPixel(IMAGEMANAGER->findImage("집안픽셀")->getMemDC(), _vJar[i]->getThrowObjectX(), j + _mapPixelLoopX);
				if (color != RGB(255, 255, 255))		// 보물상자
				{
					_vJar[i]->objectDestroy();
					break;
				}
			}
		}

	}
	//**********************************************//
	//						NPC						//
	//**********************************************//
	_uncle->update();

	if (_princess->getprincessState() == STATE_NONE && _uncle->getuncleState() == STATE_SIT)
	{
		_uncle->setUncleState(STATE_SIT_LEFT);
	}
	_princess->update();

	//**********************************************//
	//				ITEM collision					//
	//**********************************************//
	for (int i = 0; i < _IM->Get_vItem().size(); i++)
	{
		RECT Temp;

		if (IntersectRect(&Temp, &(_IM->Get_vItem()[i]->GetVirtualItem()), &(_linkAndMap->getBodyRect())))
		{
			if (_IM->Get_vItem()[i]->GetItemType() == UIITEM)
			{
				_linkAndMap->setCurrentHp(_linkAndMap->getCurrentHp() + 2);
				_IM->equipItem(i);

				if (_linkAndMap->getCurrentHp() > _linkAndMap->getMaxHp()) _linkAndMap->setCurrentHp(_linkAndMap->getMaxHp());
				break;
			}

			if (_IM->Get_vItem()[i]->GetItemType() == GOLDITEM)
			{
				_IM->equipItem(i);
				break;
			}
		}
	}
}

void mapManager::houseLoop(void)
{
	_mapPixelLoopX = _houseMap->getLoopX(); //링크한테 넘겨주는 픽셀업댓!
	_mapPixelLoopY = _houseMap->getLoopY(); //링크한테 넘겨주는 픽셀 업댓!

	_IM->SetLoopX(_houseMap->getLoopX()); //To change X-coordinate value of items on field, Update LoopX value of ItemManager (영어를 생활 속에서 쓰고자 하는 몸부림)
	_IM->SetLoopY(_houseMap->getLoopY()); //To change Y-coordinate value of items on field, Update LoopY value of ItemManager 

	// 2016. 01. 06 홍근영 수정 //

	if (!_linkAndMap->getPixelCollision() && (_linkAndMap->getLinkState() == MOVE || _linkAndMap->getLinkState() == LIFTPOTMOVE || _linkAndMap->getLinkState() == WEAPONMOVE)
		&& _linkAndMap->getPushDown()) // 조건 수정

	{
		if (_linkCenterY >= _rcHorizontal.bottom && _houseMap->getLoopY() < (IMAGEMANAGER->findImage("집")->getHeight() - WINSIZEY))
		{
			_linkAndMap->setCameraOnY(false);		// 수정 함.
			_houseMap->setLoopY(_houseMap->getLoopY() + MAPLOOPSPEED);
			_houseMap->setCenterInterDoorY(_houseMap->getCenterInterDoorY() - MAPLOOPSPEED);
			_chest->setObjectCoordinateY(_chest->getObjectCoordinateY() - MAPLOOPSPEED);
			for (int i = 0; i < _vJar.size(); i++)
			{
				_vJar[i]->setObjectCoordinateY(_vJar[i]->getObjectCoordinateY() - MAPLOOPSPEED);
				_vJar[i]->setThrowObjectY(_vJar[i]->getThrowObjectY() - MAPLOOPSPEED);
			}
		}
		else _linkAndMap->setCameraOnY(true);		// 추가 함.
	}

	if (!_linkAndMap->getPixelCollision() && (_linkAndMap->getLinkState() == MOVE || _linkAndMap->getLinkState() == LIFTPOTMOVE || _linkAndMap->getLinkState() == WEAPONMOVE)
		&& _linkAndMap->getPushUp())		// 조건 수정.

	{

		if (_linkCenterY <= _rcHorizontal.top && _houseMap->getLoopY() > 0)
		{
			for (int i = _linkAndMap->getBodyRect().left; i < _linkAndMap->getBodyRect().right; ++i)
			{
				COLORREF color = GetPixel(IMAGEMANAGER->findImage("집안픽셀")->getMemDC(), i, _linkAndMap->getY() - 5 + _houseMap->getLoopY());

				if (RGB(252, 123, 123) == color)
				{
					_linkAndMap->setCameraOnY(true);
					break;
				}
				else if (RGB(145, 117, 234) == color)
				{
					_linkAndMap->setCameraOnY(true);
					break;
				}
				else
				{
					_linkAndMap->setCameraOnY(false);		// 수정 함.
					_houseMap->setLoopY(_houseMap->getLoopY() - MAPLOOPSPEED);
					_houseMap->setCenterInterDoorY(_houseMap->getCenterInterDoorY() + MAPLOOPSPEED);
					_chest->setObjectCoordinateY(_chest->getObjectCoordinateY() + MAPLOOPSPEED);
					for (int i = 0; i < _vJar.size(); i++)
					{
						_vJar[i]->setObjectCoordinateY(_vJar[i]->getObjectCoordinateY() + MAPLOOPSPEED);
						_vJar[i]->setThrowObjectY(_vJar[i]->getThrowObjectY() + MAPLOOPSPEED);
					}
					break;
				}
			}
		}
		else _linkAndMap->setCameraOnY(true);
	}

}

void mapManager::outsideHouseInit(void)
{
	_outsideHouseMap = new outsideHouse;
	_outsideHouseMap->init();
	//***********************************************//
	//					풀세팅~~~					 //
	//***********************************************//

	//npc 병사 셋팅
	_soldier1 = new soldier;
	_soldier1->init(-150, 800, STATE_RIGHT);
	_soldier2 = new soldier;
	_soldier2->init(-150, 900, STATE_RIGHT);
	_soldier3 = new soldier;
	_soldier3->init(620, 1250, STATE_UP);
	_soldier4 = new soldier;
	_soldier4->init(720, 1250, STATE_UP);
	_soldier5 = new soldier;
	_soldier5->init(1700, 800, STATE_LEFT);
	_soldier6 = new soldier;
	_soldier6->init(1700, 900, STATE_LEFT);
	_soldier7 = new soldier;
	_soldier7->init(-230, -1400, STATE_DOWN);
	_soldier8 = new soldier;
	_soldier8->init(-2200, -2880, STATE_RIGHT);
	_soldier9 = new soldier;
	_soldier9->init(-2200, -2760, STATE_RIGHT);

	rt = { 165, 600, 860, 700 };
}

void mapManager::outsideHouseItemUpdate(void)
{
	_raining->update(); //비온다~~~~~~~~
	//**********************************************//
	//						잔디들					//
	//**********************************************//
	for (int i = 0; i < _vGrass.size(); i++)
	{
		_vGrass[i]->update();
	}
	for (int i = 0; i < _vGrass.size(); i++)
	{
		if (_vGrass[i]->getIsLift())
		{
			_vGrass[i]->setObjectState(TOUCHED);
		}
	}
	//npc 병사 업데이트
	speed++;
	_soldier1->update();
	_soldier2->update();
	_soldier3->update();
	_soldier4->update();
	_soldier5->update();
	_soldier6->update();
	_soldier7->update();
	_soldier8->update();
	_soldier9->update();

	TimerVariable++;
	_princess->update();

	if (TimerVariable >= 10000 && !(_linkAndMap->getUIManager()->GetTalking()))// && _storyline == BEFOREHOLE)
	{
		_princess->SetprincessState(STATE_ECO2);
		TimerVariable = 0;
	}

}
void mapManager::outsideHouseRender(void)
{
	_outsideHouseMap->render();
	for (int i = 0; i < _vGrass.size(); i++)
	{
		_vGrass[i]->render();
	}
	//npc 병사 렌더
	_soldier1->render();
	_soldier2->render();
	_soldier3->render();
	_soldier4->render();
	_soldier5->render();
	_soldier6->render();
	_soldier7->render();
	_soldier8->render();
	_soldier9->render();

	_princess->render();

	_raining->render();

	//procedure = 0;

	//if (KEYMANAGER->isOnceKeyDown('Z'))
	//{
	//	procedure+=1;
	//	textframe = 0;
	//}

	if (IntersectRect(&temp, &_linkAndMap->getObjectRect(), &_soldier1->getnpcRc()))
	{
		speed++;
		_soldier1->SetTalking(1);
		if (textframe >= strlen(_soldier1->getmList().find("downleft_01")->second.c_str()) && KEYMANAGER->isOnceKeyDown('X'))
		{
			_soldier1->SetTalking(0);
			textframe = 0;
		}

		if (_linkAndMap->getUIManager()->GetCurrentMenu()) DrawText(getMemDC(), (_soldier1->getmList().find("downleft_01")->second.c_str()), textframe, &rt, DT_LEFT | DT_WORDBREAK);
		if (speed >= 5 && textframe < strlen(_soldier1->getmList().find("downleft_01")->second.c_str()))
		{
			textframe += 2;
			speed = 0;
		}
	}
	if (IntersectRect(&temp, &_linkAndMap->getObjectRect(), &_soldier2->getnpcRc()))
	{
		speed++;
		_soldier2->SetTalking(1);
		if (textframe >= strlen(_soldier1->getmList().find("downleft_02")->second.c_str()) && KEYMANAGER->isOnceKeyDown('X'))
		{
			_soldier2->SetTalking(0);
			textframe = 0;
		}

		if (_linkAndMap->getUIManager()->GetCurrentMenu() == 0) DrawText(getMemDC(), _soldier2->getmList().find("downleft_02")->second.c_str(), textframe, &rt, DT_LEFT | DT_WORDBREAK);
		if (speed >= 5 && textframe < strlen(_soldier2->getmList().find("downleft_02")->second.c_str()))
		{
			textframe += 2;
			speed = 0;
		}
	}
	if (IntersectRect(&temp, &_linkAndMap->getObjectRect(), &_soldier3->getnpcRc()))
	{
		speed++;
		_soldier3->SetTalking(1);
		if (textframe >= strlen(_soldier1->getmList().find("down_01")->second.c_str()) && KEYMANAGER->isOnceKeyDown('X'))
		{
			_soldier3->SetTalking(0);
			textframe = 0;
		}

		if (_linkAndMap->getUIManager()->GetCurrentMenu() == 0) DrawText(getMemDC(), _soldier3->getmList().find("down_01")->second.c_str(), textframe, &rt, DT_LEFT | DT_WORDBREAK);
		if (speed >= 5 && textframe < strlen(_soldier3->getmList().find("down_01")->second.c_str()))
		{
			textframe += 2;
			speed = 0;
		}
	}
	if (IntersectRect(&temp, &_linkAndMap->getObjectRect(), &_soldier4->getnpcRc()))
	{
		speed++;
		_soldier4->SetTalking(1);
		if (textframe >= strlen(_soldier1->getmList().find("down_02")->second.c_str()) && KEYMANAGER->isOnceKeyDown('X'))
		{
			_soldier4->SetTalking(0);
			textframe = 0;
		}

		if (_linkAndMap->getUIManager()->GetCurrentMenu() == 0) DrawText(getMemDC(), _soldier4->getmList().find("down_02")->second.c_str(), textframe, &rt, DT_LEFT | DT_WORDBREAK);
		if (speed >= 5 && textframe < strlen(_soldier4->getmList().find("down_02")->second.c_str()))
		{
			textframe += 2;
			speed = 0;
		}
	}
	if (IntersectRect(&temp, &_linkAndMap->getObjectRect(), &_soldier5->getnpcRc()))
	{
		speed++;
		_soldier5->SetTalking(1);
		if (textframe >= strlen(_soldier1->getmList().find("right_01")->second.c_str()) && KEYMANAGER->isOnceKeyDown('X'))
		{
			_soldier5->SetTalking(0);
			textframe = 0;
		}

		if (_linkAndMap->getUIManager()->GetCurrentMenu() == 0) DrawText(getMemDC(), _soldier5->getmList().find("right_01")->second.c_str(), textframe, &rt, DT_LEFT | DT_WORDBREAK);
		if (speed >= 5 && textframe < strlen(_soldier5->getmList().find("right_01")->second.c_str()))
		{
			textframe += 2;
			speed = 0;
		}
	}
	if (IntersectRect(&temp, &_linkAndMap->getObjectRect(), &_soldier6->getnpcRc()))
	{
		speed++;
		_soldier6->SetTalking(1);
		if (textframe >= strlen(_soldier1->getmList().find("right_02")->second.c_str()) && KEYMANAGER->isOnceKeyDown('X'))
		{
			_soldier6->SetTalking(0);
			textframe = 0;
		}

		if (_linkAndMap->getUIManager()->GetCurrentMenu() == 0) DrawText(getMemDC(), _soldier6->getmList().find("right_02")->second.c_str(), textframe, &rt, DT_LEFT | DT_WORDBREAK);
		if (speed >= 5 && textframe < strlen(_soldier6->getmList().find("right_02")->second.c_str()))
		{
			textframe += 2;
			speed = 0;
		}
	}
	if (IntersectRect(&temp, &_linkAndMap->getObjectRect(), &_soldier7->getnpcRc()))
	{
		speed++;
		_soldier7->SetTalking(1);
		if (textframe >= strlen(_soldier1->getmList().find("castle_door")->second.c_str()) && KEYMANAGER->isOnceKeyDown('X'))
		{
			_soldier7->SetTalking(0);
			textframe = 0;
		}

		if (_linkAndMap->getUIManager()->GetCurrentMenu() == 0) DrawText(getMemDC(), _soldier7->getmList().find("castle_door")->second.c_str(), textframe, &rt, DT_LEFT | DT_WORDBREAK);
		if (speed >= 5 && textframe < strlen(_soldier7->getmList().find("castle_door")->second.c_str()))
		{
			textframe += 2;
			speed = 0;
		}
	}
	if (IntersectRect(&temp, &_linkAndMap->getObjectRect(), &_soldier8->getnpcRc()))
	{
		speed++;
		_soldier8->SetTalking(1);
		if (textframe >= strlen(_soldier1->getmList().find("castle_left_01")->second.c_str()) && KEYMANAGER->isOnceKeyDown('X'))
		{
			_soldier8->SetTalking(0);
			textframe = 0;
		}

		if (_linkAndMap->getUIManager()->GetCurrentMenu() == 0) DrawText(getMemDC(), _soldier8->getmList().find("castle_left_01")->second.c_str(), textframe, &rt, DT_LEFT | DT_WORDBREAK);
		if (speed >= 5 && textframe < strlen(_soldier8->getmList().find("castle_left_01")->second.c_str()))
		{
			textframe += 2;
			speed = 0;
		}
	}
	if (IntersectRect(&temp, &_linkAndMap->getObjectRect(), &_soldier9->getnpcRc()))
	{
		speed++;
		_soldier9->SetTalking(1);
		if (textframe >= strlen(_soldier1->getmList().find("castle_left_02")->second.c_str()) && KEYMANAGER->isOnceKeyDown('X'))
		{
			_soldier9->SetTalking(0);
			textframe = 0;
		}

		if (_linkAndMap->getUIManager()->GetCurrentMenu() == 0) DrawText(getMemDC(), _soldier9->getmList().find("castle_left_02")->second.c_str(), textframe, &rt, DT_LEFT | DT_WORDBREAK);
		if (speed >= 5 && textframe < strlen(_soldier9->getmList().find("castle_left_02")->second.c_str()))
		{
			textframe += 2;
			speed = 0;
		}
	}

}
void mapManager::outsideHouseLoop(void)
{
	switch (_outsideArea)
	{
	case HOUSEAREA:
		_mapLoopLimitUp = 4000;
		_mapLoopLimitDown = (IMAGEMANAGER->findImage("집밖")->getHeight() - WINSIZEY);
		break;
	case CASTLEAREA:
		_mapLoopLimitUp = 0;
		_mapLoopLimitDown = 3230;
		break;
	}

	_mapPixelLoopX = _outsideHouseMap->getLoopX(); //링크한테 넘겨주는 픽셀업댓!
	_mapPixelLoopY = _outsideHouseMap->getLoopY(); //링크한테 넘겨주는 픽셀 업댓!
	//위아래로 루프~~
	if (!_linkAndMap->getPixelCollision() && (_linkAndMap->getLinkState() == MOVE || _linkAndMap->getLinkState() == LIFTPOTMOVE || _linkAndMap->getLinkState() == WEAPONMOVE || _linkAndMap->getLinkState() == LIFTGRASSMOVE)
		&& _linkAndMap->getPushDown()) // 조건 수정
	{
		if (_slidingMap == MAPSLIDING)
		{
			if (_outsideHouseMap->getLoopY() < 4000)
			{

				_outsideHouseMap->setLoopY(_outsideHouseMap->getLoopY() + MAPLOOPSPEED);
				_linkAndMap->setY(_linkAndMap->getY() - MAPLOOPSPEED + 0.5f);
				_outsideHouseMap->setCenterInterDoorY(_outsideHouseMap->getCenterInterDoorY() - MAPLOOPSPEED);
				_outsideHouseMap->setLoopRectY(_outsideHouseMap->getLoopRectY() - MAPLOOPSPEED);
				_outsideHouseMap->setExteriorGateY(_outsideHouseMap->getExteriorGateY() - MAPLOOPSPEED);
				for (int i = 0; i < _vGrass.size(); i++)
				{
					_vGrass[i]->setObjectCoordinateY(_vGrass[i]->getObjectCoordinateY() - MAPLOOPSPEED);
					_vGrass[i]->setThrowObjectY(_vGrass[i]->getThrowObjectY() - MAPLOOPSPEED);
				}
			}
			else _slidingMap = MAPSTEADY, _outsideArea = HOUSEAREA;
		}
		if (_linkCenterY >= _rcHorizontal.bottom && _outsideHouseMap->getLoopY() < _mapLoopLimitDown)
		{
			for (int i = _linkAndMap->getBodyRect().left; i < _linkAndMap->getBodyRect().right; ++i)
			{
				COLORREF color = GetPixel(IMAGEMANAGER->findImage("집밖픽셀")->getMemDC(), i + _outsideHouseMap->getLoopX(), _linkAndMap->getBodyRect().bottom + 5 + _outsideHouseMap->getLoopY());

				if (RGB(252, 123, 123) == color)
				{
					_linkAndMap->setCameraOnY(true);
					break;
				}
				else if (RGB(145, 117, 234) == color)
				{
					_linkAndMap->setCameraOnY(true);
					break;
				}
				else
				{
					_linkAndMap->setCameraOnY(false);		// 수정 함.
					_outsideHouseMap->setLoopY(_outsideHouseMap->getLoopY() + MAPLOOPSPEED);
					_outsideHouseMap->setCenterInterDoorY(_outsideHouseMap->getCenterInterDoorY() - MAPLOOPSPEED);
					_outsideHouseMap->setLoopRectY(_outsideHouseMap->getLoopRectY() - MAPLOOPSPEED);
					_outsideHouseMap->setExteriorGateY(_outsideHouseMap->getExteriorGateY() - MAPLOOPSPEED);
					for (int i = 0; i < _vGrass.size(); i++)
					{
						_vGrass[i]->setObjectCoordinateY(_vGrass[i]->getObjectCoordinateY() - MAPLOOPSPEED);
						_vGrass[i]->setThrowObjectY(_vGrass[i]->getThrowObjectY() - MAPLOOPSPEED);
					}
					//병사
					_soldier1->setNPCCoordinateY(_soldier1->getNPCCoordinateY() - MAPLOOPSPEED);
					_soldier2->setNPCCoordinateY(_soldier2->getNPCCoordinateY() - MAPLOOPSPEED);
					_soldier3->setNPCCoordinateY(_soldier3->getNPCCoordinateY() - MAPLOOPSPEED);
					_soldier4->setNPCCoordinateY(_soldier4->getNPCCoordinateY() - MAPLOOPSPEED);
					_soldier5->setNPCCoordinateY(_soldier5->getNPCCoordinateY() - MAPLOOPSPEED);
					_soldier6->setNPCCoordinateY(_soldier6->getNPCCoordinateY() - MAPLOOPSPEED);
					_soldier7->setNPCCoordinateY(_soldier7->getNPCCoordinateY() - MAPLOOPSPEED);
					_soldier8->setNPCCoordinateY(_soldier8->getNPCCoordinateY() - MAPLOOPSPEED);
					_soldier9->setNPCCoordinateY(_soldier9->getNPCCoordinateY() - MAPLOOPSPEED);
					break;
				}
			}
		}
		else _linkAndMap->setCameraOnY(true);		// 추가 함.
	}
	if (!_linkAndMap->getPixelCollision() && (_linkAndMap->getLinkState() == MOVE || _linkAndMap->getLinkState() == LIFTPOTMOVE || _linkAndMap->getLinkState() == WEAPONMOVE || _linkAndMap->getLinkState() == LIFTGRASSMOVE)
		&& _linkAndMap->getPushUp())		// 조건 수정.
	{
		if (_slidingMap == MAPSLIDING)
		{
			if (_outsideHouseMap->getLoopY() > 4000 - WINSIZEY)
			{
				_outsideHouseMap->setLoopY(_outsideHouseMap->getLoopY() - MAPLOOPSPEED);
				_linkAndMap->setY(_linkAndMap->getY() + MAPLOOPSPEED - 0.5f);
				_outsideHouseMap->setCenterInterDoorY(_outsideHouseMap->getCenterInterDoorY() + MAPLOOPSPEED);
				_outsideHouseMap->setLoopRectY(_outsideHouseMap->getLoopRectY() + MAPLOOPSPEED);
				_outsideHouseMap->setExteriorGateY(_outsideHouseMap->getExteriorGateY() + MAPLOOPSPEED);
				for (int i = 0; i < _vGrass.size(); i++)
				{
					_vGrass[i]->setObjectCoordinateY(_vGrass[i]->getObjectCoordinateY() + MAPLOOPSPEED);
					_vGrass[i]->setThrowObjectY(_vGrass[i]->getThrowObjectY() + MAPLOOPSPEED);
				}
			}
			else _slidingMap = MAPSTEADY, _outsideArea = CASTLEAREA;
		}
		if (_linkCenterY <= _rcHorizontal.top && _outsideHouseMap->getLoopY() > _mapLoopLimitUp)
		{
			for (int i = _linkAndMap->getBodyRect().left; i < _linkAndMap->getBodyRect().right; ++i)
			{
				COLORREF color = GetPixel(IMAGEMANAGER->findImage("집밖픽셀")->getMemDC(), i + _outsideHouseMap->getLoopX(), _linkAndMap->getY() - 10 + _outsideHouseMap->getLoopY());

				if (RGB(252, 123, 123) == color)
				{
					_linkAndMap->setCameraOnY(true);
					break;
				}
				else if (RGB(145, 117, 234) == color)
				{
					_linkAndMap->setCameraOnY(true);
					break;
				}
				else
				{
					_linkAndMap->setCameraOnY(false);		// 수정 함.
					_outsideHouseMap->setLoopY(_outsideHouseMap->getLoopY() - MAPLOOPSPEED);
					_outsideHouseMap->setCenterInterDoorY(_outsideHouseMap->getCenterInterDoorY() + MAPLOOPSPEED);
					_outsideHouseMap->setLoopRectY(_outsideHouseMap->getLoopRectY() + MAPLOOPSPEED);
					_outsideHouseMap->setExteriorGateY(_outsideHouseMap->getExteriorGateY() + MAPLOOPSPEED);
					for (int i = 0; i < _vGrass.size(); i++)
					{
						_vGrass[i]->setObjectCoordinateY(_vGrass[i]->getObjectCoordinateY() + MAPLOOPSPEED);
						_vGrass[i]->setThrowObjectY(_vGrass[i]->getThrowObjectY() + MAPLOOPSPEED);
					}

					//병사
					_soldier1->setNPCCoordinateY(_soldier1->getNPCCoordinateY() + MAPLOOPSPEED);
					_soldier2->setNPCCoordinateY(_soldier2->getNPCCoordinateY() + MAPLOOPSPEED);
					_soldier3->setNPCCoordinateY(_soldier3->getNPCCoordinateY() + MAPLOOPSPEED);
					_soldier4->setNPCCoordinateY(_soldier4->getNPCCoordinateY() + MAPLOOPSPEED);
					_soldier5->setNPCCoordinateY(_soldier5->getNPCCoordinateY() + MAPLOOPSPEED);
					_soldier6->setNPCCoordinateY(_soldier6->getNPCCoordinateY() + MAPLOOPSPEED);
					_soldier7->setNPCCoordinateY(_soldier7->getNPCCoordinateY() + MAPLOOPSPEED);
					_soldier8->setNPCCoordinateY(_soldier8->getNPCCoordinateY() + MAPLOOPSPEED);
					_soldier9->setNPCCoordinateY(_soldier9->getNPCCoordinateY() + MAPLOOPSPEED);
					break;
				}
			}

		}
		else _linkAndMap->setCameraOnY(true);		// 추가
	}
	//왼쪽오른쪽으로로 루프~~
	if (!_linkAndMap->getPixelCollision() && (_linkAndMap->getLinkState() == MOVE || _linkAndMap->getLinkState() == LIFTPOTMOVE || _linkAndMap->getLinkState() == WEAPONMOVE || _linkAndMap->getLinkState() == LIFTGRASSMOVE)
		&& _linkAndMap->getPushRight()) // 조건 수정
	{
		if (_linkCenterX >= _rcVertical.right && _outsideHouseMap->getLoopX() < (IMAGEMANAGER->findImage("집밖")->getWidth() - WINSIZEX))
		{
			for (int i = _linkAndMap->getY(); i < _linkAndMap->getBodyRect().bottom; ++i)
			{
				COLORREF color = GetPixel(IMAGEMANAGER->findImage("집밖픽셀")->getMemDC(), _linkAndMap->getBodyRect().right + _outsideHouseMap->getLoopX() + 5, i + _outsideHouseMap->getLoopY());

				if (RGB(252, 123, 123) == color)
				{
					_linkAndMap->setCameraOnX(true);
					break;
				}
				else if (RGB(145, 117, 234) == color)
				{
					_linkAndMap->setCameraOnX(true);
					break;
				}
				else
				{
					_linkAndMap->setCameraOnX(false);		// 수정 함.
					_outsideHouseMap->setLoopX(_outsideHouseMap->getLoopX() + MAPLOOPSPEED);
					_outsideHouseMap->setCenterInterDoorX(_outsideHouseMap->getCenterInterDoorX() - MAPLOOPSPEED);
					_outsideHouseMap->setLoopRectX(_outsideHouseMap->getLoopRectX() - MAPLOOPSPEED);
					_outsideHouseMap->setExteriorGateX(_outsideHouseMap->getExteriorGateX() - MAPLOOPSPEED);
					for (int i = 0; i < _vGrass.size(); i++)
					{
						_vGrass[i]->setObjectCoordinateX(_vGrass[i]->getObjectCoordinateX() - MAPLOOPSPEED);
						_vGrass[i]->setThrowObjectX(_vGrass[i]->getThrowObjectX() - MAPLOOPSPEED);
					}
					//병사
					_soldier1->setNPCCoordinateX(_soldier1->getNPCCoordinateX() - MAPLOOPSPEED);
					_soldier2->setNPCCoordinateX(_soldier2->getNPCCoordinateX() - MAPLOOPSPEED);
					_soldier3->setNPCCoordinateX(_soldier3->getNPCCoordinateX() - MAPLOOPSPEED);
					_soldier4->setNPCCoordinateX(_soldier4->getNPCCoordinateX() - MAPLOOPSPEED);
					_soldier5->setNPCCoordinateX(_soldier5->getNPCCoordinateX() - MAPLOOPSPEED);
					_soldier6->setNPCCoordinateX(_soldier6->getNPCCoordinateX() - MAPLOOPSPEED);
					_soldier7->setNPCCoordinateX(_soldier7->getNPCCoordinateX() - MAPLOOPSPEED);
					_soldier8->setNPCCoordinateX(_soldier8->getNPCCoordinateX() - MAPLOOPSPEED);
					_soldier9->setNPCCoordinateX(_soldier9->getNPCCoordinateX() - MAPLOOPSPEED);
					break;
				}
			}


		}
		else _linkAndMap->setCameraOnX(true);		// 추가 함.
	}
	if (!_linkAndMap->getPixelCollision() && (_linkAndMap->getLinkState() == MOVE || _linkAndMap->getLinkState() == LIFTPOTMOVE || _linkAndMap->getLinkState() == WEAPONMOVE || _linkAndMap->getLinkState() == LIFTGRASSMOVE)
		&& _linkAndMap->getPushLeft())		// 조건 수정.
	{
		if (_linkCenterX <= _rcVertical.left && _outsideHouseMap->getLoopX() > 0)
		{
			for (int i = _linkAndMap->getY(); i < _linkAndMap->getBodyRect().bottom; ++i)
			{
				COLORREF color = GetPixel(IMAGEMANAGER->findImage("집밖픽셀")->getMemDC(), _linkAndMap->getBodyRect().left + _outsideHouseMap->getLoopX(), i + _outsideHouseMap->getLoopY());

				if (RGB(252, 123, 123) == color)
				{
					_linkAndMap->setCameraOnX(true);
					break;
				}
				else if (RGB(145, 117, 234) == color)
				{
					_linkAndMap->setCameraOnX(true);
					break;
				}
				else
				{
					_linkAndMap->setCameraOnX(false);		// 수정 함.
					_outsideHouseMap->setLoopX(_outsideHouseMap->getLoopX() - MAPLOOPSPEED);
					_outsideHouseMap->setCenterInterDoorX(_outsideHouseMap->getCenterInterDoorX() + MAPLOOPSPEED);
					_outsideHouseMap->setLoopRectX(_outsideHouseMap->getLoopRectX() + MAPLOOPSPEED);
					_outsideHouseMap->setExteriorGateX(_outsideHouseMap->getExteriorGateX() + MAPLOOPSPEED);
					for (int i = 0; i < _vGrass.size(); i++)
					{
						_vGrass[i]->setObjectCoordinateX(_vGrass[i]->getObjectCoordinateX() + MAPLOOPSPEED);
						_vGrass[i]->setThrowObjectX(_vGrass[i]->getThrowObjectX() + MAPLOOPSPEED);
					}
					_soldier1->setNPCCoordinateX(_soldier1->getNPCCoordinateX() + MAPLOOPSPEED);
					_soldier2->setNPCCoordinateX(_soldier2->getNPCCoordinateX() + MAPLOOPSPEED);
					_soldier3->setNPCCoordinateX(_soldier3->getNPCCoordinateX() + MAPLOOPSPEED);
					_soldier4->setNPCCoordinateX(_soldier4->getNPCCoordinateX() + MAPLOOPSPEED);
					_soldier5->setNPCCoordinateX(_soldier5->getNPCCoordinateX() + MAPLOOPSPEED);
					_soldier6->setNPCCoordinateX(_soldier6->getNPCCoordinateX() + MAPLOOPSPEED);
					_soldier7->setNPCCoordinateX(_soldier7->getNPCCoordinateX() + MAPLOOPSPEED);
					_soldier8->setNPCCoordinateX(_soldier8->getNPCCoordinateX() + MAPLOOPSPEED);
					_soldier9->setNPCCoordinateX(_soldier9->getNPCCoordinateX() + MAPLOOPSPEED);
					break;
				}
			}

		}
		else _linkAndMap->setCameraOnX(true);		// 추가
	}
}