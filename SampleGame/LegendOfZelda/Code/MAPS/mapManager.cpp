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
	//			�ʷ����뷺Ʈ2��			  //
	//************************************//
	_rcHorizontal = RectMakeCenter(WINSIZEX / 2, WINSIZEY / 2, WINSIZEX, 5);
	_rcVertical = RectMakeCenter(WINSIZEX / 2, WINSIZEY / 2, 5, WINSIZEY);
	//************************************//
	//			���� Ŭ���� �̴�			  //
	//************************************//
	houseInit(); //���� ������Ʈ �� �׸� �̴�~
	outsideHouseInit(); //���ٱ� ������Ʈ �� �׸� �̴�~
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
	//				�ʷ�����				  //
	//************************************//
	_linkArea = RectMakeCenter(_linkAndMap->getShadowX(), _linkAndMap->getShadowY(), 60, 20); //�ʿ��� ����� ��ũ��Ʈ~
	_linkCenterX = _linkArea.left + (_linkArea.right - _linkArea.left) / 2; //�ʿ��� ����ִ� ��ũ ����X
	_linkCenterY = _linkArea.top + (_linkArea.bottom - _linkArea.top) / 2; // �ʿ��� ����ִ� ��ũ ����Y
	//************************************//
	//				�ʽ���ġ				  //
	//************************************//
	switch (_whereIsLink)
	{
	case INSIDE_HOUSE_AREA:
		_houseMap->update(); //insideHouse class ����
		houseLoop(); //���� �� ����~����~������Ʈ��~
		houseItemUpdate(); // ���ȿ����� ���� ������Ʈ~
		break;
	case OUTSIDE_HOUSE_AREA:
		_outsideHouseMap->update(); //outsideHouse class Update
		outsideHouseLoop(); //���� �� ��������������Ʈ��~
		outsideHouseItemUpdate(); //���� ���� ������Ʈ~
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
	//				�����ۿ�				  //
	//************************************//
	_IM->update();
	_linkAndMap->getUIManager()->SetTalking(_princess->GetTalking() + _uncle->GetTalking()
		+ _soldier1->GetTalking() + _soldier2->GetTalking() + _soldier3->GetTalking() + _soldier4->GetTalking() + _soldier5->GetTalking() + _soldier6->GetTalking() + _soldier7->GetTalking() + _soldier8->GetTalking() + _soldier9->GetTalking());
	
	//************************************//
	//				���Ϳ�				  //
	//************************************//
	_MM->update();

	//��ũ->���� �浹ó��
	for (int i = 0; i < _MM->getVGreensoldier().size(); i++)
	{
		if (IntersectRect(&Collision, &(_linkAndMap->getBodyRect()), &(_MM->getVGreensoldier()[i]->GetVirtualBody()))) //�������� RECT�� �ٲ��ָ� ��
		{
			_MM->EraseGreensoldier(i);
			break;
		}
	}

	//************************************//
	//				����������Ʈ			  //
	//************************************//
	exitAndEnterMap(); //�� �Լ��� ������ �׷���~~~~~~~~~~~~
	
	_blackCircle->update(); //������ �ִϸ��̼� ������Ʈ~~
	if (_blackCircle->getAniProgress() == COMPLETE&& _exitAndEnter == EXIT) _changeMap = CHANGING;
	else if (_blackCircle->getAniProgress() == STANDBY&& _exitAndEnter == ENTER) _changeMap = CHANGING;
	else _changeMap = COMPLETECHANGE;
	
}

void mapManager::render(void)
{
	//************************************//
	//				�ʽ���ġ				  //
	//************************************//
	
	//������ ��������
	if ((_linkAndMap->getUIManager()->GetCurrentMenu()) != 0) _princess->SetCurrentMenu(1);
	else _princess->SetCurrentMenu(0);

	if ((_linkAndMap->getUIManager()->GetCurrentMenu()) != 0) _uncle->SetCurrentMenu(1);
	else _uncle->SetCurrentMenu(0);
	//������ ��������

	switch (_whereIsLink)
	{
	case INSIDE_HOUSE_AREA:
		switch (_changeMap)
		{
		case COMPLETECHANGE:
			houseRender(); // �Ͽ콺��������
			_IM->render(); // Item Rendering
						   //z-order�� ���� �׾Ƹ� ���� ������~~
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
	//				��Ʈ �׷��ֱ�			  //
	//************************************//
	Rectangle(getMemDC(), _rcHorizontal.left, _rcHorizontal.top, _rcHorizontal.right, _rcHorizontal.bottom);
	Rectangle(getMemDC(), _rcVertical.left, _rcVertical.top, _rcVertical.right, _rcVertical.bottom);
	Rectangle(getMemDC(), _linkArea.left, _linkArea.top, _linkArea.right, _linkArea.bottom);
	//************************************//
	//				�����ۿ�				  //
	//************************************//
	_IM->render();

	//************************************//
	//				���Ϳ�				  //
	//************************************//
	
	_MM->render();
	//************************************//
	//				����������Ʈ			  //
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