#include "stdafx.h"
#include "../stdafx.h"
#include "mapManager.h"
#include "../link/link.h"

mapManager::mapManager()
	:_whereIsLink(INSIDE_HOUSE_AREA), _playerMove(true),
	_changeMap(COMPLETECHANGE), _exitAndEnter(EXIT),_slidingMap(MAPSTEADY)
	, _outsideArea(HOUSEAREA)
{
}

mapManager::~mapManager()
{

}

HRESULT mapManager::init(void)
{
	//************************************//
	//			맵루프용렉트2개			  //
	//************************************//
	_rcHorizontal = RectMakeCenter(WINSIZEX / 2, WINSIZEY / 2, WINSIZEX, 5);
	_rcVertical = RectMakeCenter(WINSIZEX / 2, WINSIZEY / 2, 5, WINSIZEY);
	//************************************//
	//			각종 클래스 이닛			  //
	//************************************//
	houseInit(); //집안 오브젝트 및 그림 이닛~
	outsideHouseInit(); //집바깥 오브젝트 및 그림 이닛~
	_blackCircle = new blackCircle;
	_blackCircle->init();
	_raining = new raining;
	_IM = new itemManager;
	_IM->init();

	_MM = new monstermanager;
	_MM->init();

	_mapPixelLoopX;
	return S_OK;
}
void mapManager::release(void)
{

}

void mapManager::update(void)
{
	mapSliding();
	//************************************//
	//				맵루프용				  //
	//************************************//
	_linkArea = RectMakeCenter(_linkAndMap->getShadowX(), _linkAndMap->getShadowY(), 60, 20); //맵에서 만드는 링크렉트~
	_linkCenterX = _linkArea.left + (_linkArea.right - _linkArea.left) / 2; //맵에서 잡아주는 링크 센터X
	_linkCenterY = _linkArea.top + (_linkArea.bottom - _linkArea.top) / 2; // 맵에서 잡아주는 링크 센터Y
	//************************************//
	//				맵스위치				  //
	//************************************//
	switch (_whereIsLink)
	{
	case INSIDE_HOUSE_AREA:
		_houseMap->update(); //insideHouse class 업댓
		houseLoop(); //집안 맵 루프~루프~업데이트용~
		houseItemUpdate(); // 집안에서의 각종 업데이트~
		break;
	case OUTSIDE_HOUSE_AREA:
		_outsideHouseMap->update(); //outsideHouse class Update
		outsideHouseLoop(); //집밖 맵 루프루프업데이트용~
		outsideHouseItemUpdate(); //집밖 각종 업데이트~
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


	//************************************//
	//				아이템용				  //
	//************************************//
	_IM->update();
	_linkAndMap->getUIManager()->SetTalking(_princess->GetTalking() + _uncle->GetTalking()
		+ _soldier1->GetTalking() + _soldier2->GetTalking() + _soldier3->GetTalking() + _soldier4->GetTalking() + _soldier5->GetTalking() + _soldier6->GetTalking() + _soldier7->GetTalking() + _soldier8->GetTalking() + _soldier9->GetTalking());
	
	//************************************//
	//				몬스터용				  //
	//************************************//
	_MM->update();

	//링크->몬스터 충돌처리
	for (int i = 0; i < _MM->getVGreensoldier().size(); i++)
	{
		if (IntersectRect(&Collision, &(_linkAndMap->getBodyRect()), &(_MM->getVGreensoldier()[i]->GetVirtualBody()))) //공격판정 RECT로 바꿔주면 끝
		{
			_MM->EraseGreensoldier(i);
			break;
		}
	}

	//************************************//
	//				검은원이팩트			  //
	//************************************//
	exitAndEnterMap(); //이 함수가 검은원 그려줌~~~~~~~~~~~~
	
	_blackCircle->update(); //검은원 애니매이션 업데이트~~
	if (_blackCircle->getAniProgress() == COMPLETE&& _exitAndEnter == EXIT) _changeMap = CHANGING;
	else if (_blackCircle->getAniProgress() == STANDBY&& _exitAndEnter == ENTER) _changeMap = CHANGING;
	else _changeMap = COMPLETECHANGE;
	
}

void mapManager::render(void)
{
	//************************************//
	//				맵스위치				  //
	//************************************//
	
	//눈물의 렌더연산
	if ((_linkAndMap->getUIManager()->GetCurrentMenu()) != 0) _princess->SetCurrentMenu(1);
	else _princess->SetCurrentMenu(0);

	if ((_linkAndMap->getUIManager()->GetCurrentMenu()) != 0) _uncle->SetCurrentMenu(1);
	else _uncle->SetCurrentMenu(0);
	//눈물의 렌더연산

	switch (_whereIsLink)
	{
	case INSIDE_HOUSE_AREA:
		switch (_changeMap)
		{
		case COMPLETECHANGE:
			houseRender(); // 하우스집렌더링
			_IM->render(); // Item Rendering
						   //z-order를 위한 항아리 나중 렌더링~~
			for (int i = 0; i < _vJar.size(); i++)
			{
				_vJar[i]->render();
			}
			break;
		case CHANGING:
			IMAGEMANAGER->findImage("black")->render(getMemDC());
			break;
		}
		break;
	case OUTSIDE_HOUSE_AREA:
		switch (_changeMap)
		{
		case COMPLETECHANGE:
			outsideHouseRender();
			break;
		case CHANGING:
			IMAGEMANAGER->findImage("black")->render(getMemDC());
			break;
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
	//************************************//
	//				렉트 그려주기			  //
	//************************************//
	Rectangle(getMemDC(), _rcHorizontal.left, _rcHorizontal.top, _rcHorizontal.right, _rcHorizontal.bottom);
	Rectangle(getMemDC(), _rcVertical.left, _rcVertical.top, _rcVertical.right, _rcVertical.bottom);
	Rectangle(getMemDC(), _linkArea.left, _linkArea.top, _linkArea.right, _linkArea.bottom);
	//************************************//
	//				아이템용				  //
	//************************************//
	_IM->render();

	//************************************//
	//				몬스터용				  //
	//************************************//
	
	_MM->render();
	//************************************//
	//				검은원이팩트			  //
	//************************************//
	_blackCircle->render(_linkCenterX - (_blackCircle->getblackImage()->getFrameWidth() / 2),
		_linkCenterY - (_blackCircle->getblackImage()->getFrameHeight() / 2));
	char str[120];
	sprintf(str, "%d", _linkCenterY);
	TextOut(getMemDC(), 200, 200, str, strlen(str));
}

void mapManager::mapSliding(void)
{
	RECT temp1;
	if (IntersectRect(&temp1, &_linkArea, &_outsideHouseMap->getLoopRect()))
	{
		_slidingMap = MAPSLIDING;
	}
}