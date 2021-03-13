#include "stdafx.h"
#include "../stdafx.h"
#include "mapManager.h"
#include "../link/link.h"


void mapManager::exitAndEnterMap(void)
{
	switch (_exitAndEnter)
	{
	//****************************************//
	//				다른맵으로 입장~			  //
	//****************************************//
	case ENTER:

		switch (_blackCircle->getAniProgress())
		{
		case STANDBY:
			
			switch (_whereIsLink)
			{
			case INSIDE_HOUSE_AREA:
				_blackCircle->startAni(ENTER);
				break;
			case OUTSIDE_HOUSE_AREA:
				_blackCircle->startAni(ENTER);
				break;
			}
			break;
		case COMPLETE:_exitAndEnter = EXIT;
			for (int i = 0; i < _IM->Get_vItem().size(); i++)
			{
				_IM->equipItem(i);
			}
			break;
		}

		break;
		//****************************************//
		//				다른맵으로 나감~			  //
		//****************************************//
	case EXIT:

		switch (_blackCircle->getAniProgress())
		{
		case STANDBY:
			RECT temp;
			switch (_whereIsLink)
			{
			case INSIDE_HOUSE_AREA:
				if (IntersectRect(&temp, _houseMap->getInterDoor(), &_linkArea))
				{
					_blackCircle->startAni(EXIT);
				}
				break;
			case OUTSIDE_HOUSE_AREA:
				if (IntersectRect(&temp, _outsideHouseMap->getInterDoor(), &_linkArea))
				{
					_blackCircle->startAni(EXIT);
				}
				break;
			}
			break;
		case COMPLETE:
			for (int i = 0; i < _IM->Get_vItem().size(); i++)
			{
				_IM->equipItem(i);
			}
			_exitAndEnter = ENTER;
			switch (_whereIsLink)
			{
			case INSIDE_HOUSE_AREA:
				_whereIsLink = OUTSIDE_HOUSE_AREA;
				_raining->init();
				_linkAndMap->setX(510);
				_linkAndMap->setY(400);
				_vJar.clear();
				//풀 리세팅~
				for (int i = 0; i < 2; i++)
				{
					for (int j = 0; j < 2; j++)
					{
						_grass = new grass;
						_grass->init(668 + j * 65, 410 + i * 65);
						_vGrass.push_back(_grass);
					}
				}
				for (int i = 0; i < 2; i++)
				{
					for (int j = 0; j < 3; j++)
					{
						_grass = new grass;
						_grass->init(220 + j * 65, 410 + i * 65);
						_vGrass.push_back(_grass);
					}
				}
				for (int i = 0; i < 2; i++)
				{
					_grass = new grass;
					_grass->init(412 + i * 64, -420 - i * 65);
					_vGrass.push_back(_grass);
				}
				for (int i = 0; i < 2; i++)
				{
					_grass = new grass;
					_grass->init(860 + i * 128, -483 + i * 63);
					_vGrass.push_back(_grass);
				}
				break;
			case OUTSIDE_HOUSE_AREA:
				_whereIsLink = INSIDE_HOUSE_AREA;
				_linkAndMap->setX(510);
				_linkAndMap->setY(610);
				_vGrass.clear();
				//항아리 리세팅~
				for (int i = 0; i < 3; i++)
				{
					_jar = new jar;
					_jar->init(191, 183 + i * 64);
					_vJar.push_back(_jar);
				}
				break;
			case CASTLE_BASEMENT:
				break;
			case CASTLE_1F:
				break;
			case CASTLE_2F:
				break;
			case CASTLE_B1F:
				break;
			case STAIRS_TO_PRINCESS:
				break;
			case BOSS_ROOM:
				break;
			}
			break;
		}

		break;
	}

	
}