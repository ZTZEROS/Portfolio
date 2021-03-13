#include "stdafx.h"
#include "../../stdafx.h"
#include "soldier.h"


soldier::soldier()
{
}

soldier::~soldier()
{
}

HRESULT soldier::init(int x, int y, NPCSTATE nState)
{
	npc::init();

	//_princess = new princess;

	ptBodypos.x = x;
	ptBodypos.y = y;

	_npcFImg[STATE_UP].img = IMAGEMANAGER->findImage("soldierUp");
	_npcFImg[STATE_UP].nCount = 0;
	_npcFImg[STATE_UP].nEnd = 1;
	_npcFImg[STATE_UP].nRemainder = 0;
	_npcFImg[STATE_UP].nStart = 0;
	_npcFImg[STATE_UP].nCurrentX = 0;
	_npcFImg[STATE_UP].img->setX(0);
	_npcFImg[STATE_UP].img->setY(0);

	_npcFImg[STATE_DOWN].img = IMAGEMANAGER->findImage("soldierDown");
	_npcFImg[STATE_DOWN].nCount = 0;
	_npcFImg[STATE_DOWN].nEnd = 1;
	_npcFImg[STATE_DOWN].nRemainder = 0;
	_npcFImg[STATE_DOWN].nStart = 0;
	_npcFImg[STATE_DOWN].nCurrentX = 0;
	_npcFImg[STATE_DOWN].img->setX(0);
	_npcFImg[STATE_DOWN].img->setY(0);

	_npcFImg[STATE_LEFT].img = IMAGEMANAGER->findImage("soldierLeft");
	_npcFImg[STATE_LEFT].nCount = 0;
	_npcFImg[STATE_LEFT].nEnd = 1;
	_npcFImg[STATE_LEFT].nRemainder = 0;
	_npcFImg[STATE_LEFT].nStart = 0;
	_npcFImg[STATE_LEFT].nCurrentX = 0;
	_npcFImg[STATE_LEFT].img->setX(0);
	_npcFImg[STATE_LEFT].img->setY(0);

	_npcFImg[STATE_RIGHT].img = IMAGEMANAGER->findImage("soldierRight");
	_npcFImg[STATE_RIGHT].nCount = 0;
	_npcFImg[STATE_RIGHT].nEnd = 1;
	_npcFImg[STATE_RIGHT].nRemainder = 0;
	_npcFImg[STATE_RIGHT].nStart = 0;
	_npcFImg[STATE_RIGHT].nCurrentX = 0;
	_npcFImg[STATE_RIGHT].img->setX(0);
	_npcFImg[STATE_RIGHT].img->setY(0);

	_nState = nState;

	_framecount = 0;
	rt = { 165, 600, 860, 700 };
	speed = 0;
	procedure = 0;

	_frameSpeed = 0;
	_mList.insert(make_pair("downleft_01", "������ �̷��� ���� �ð��� ���ϰ� �ٴϴ°���? �� �ڶ� ������ ������ �ֵ��� ��!"));
	_mList.insert(make_pair("downleft_02", "������ ǥ�������״� �� ���� ���� ���ư����� ��. (S��ư���� ���� ���� �ִ�.)"));
	_mList.insert(make_pair("down_01", "�� �ϳ� �� �� ���� ���屺, ��ŭ �� �� �ֳ�? (X��ư���� ������ �� �� �ִ�.)"));
	_mList.insert(make_pair("down_02", "�װ� Į ���� ���� ���� ��������! (Į�� ���� �� Z��ư���� �ֵθ� �� �ִ�.)"));
	_mList.insert(make_pair("right_01", "���� ǥ�ð� �� �������̴�."));
	_mList.insert(make_pair("right_02", "���ڿ��� ���������� ���� �� �ִ�.(X��ư���� ���ڸ� ����.)"));
	_mList.insert(make_pair("castle_door", "�̺�, �� ������ �� �ڰ��� ����! ������ ���ư��� ���̳� ��!"));
	_mList.insert(make_pair("castle_left_01", "������ �̰��� ���ε��� �ѷ��� �ִ�. �ʸ� ȥ���� ���� ���� ������, �� �˰Ŷ�� �ϴ´�."));
	_mList.insert(make_pair("castle_left_02", " ������ �� ���� �ð��� ������ �ϰ� �ִ°ų�. � ������ ���ư�!"));

	_npcRc1 = RectMakeCenter(ptBodypos.x, ptBodypos.y, _npcFImg[_nState].img->getFrameWidth(), _npcFImg[_nState].img->getFrameHeight());

	return S_OK;
}
void soldier::release(void)
{
	npc::release();
}

void soldier::update(void)
{
	npc::update();
	_frameSpeed++;

	move();
	chat();

	speed++;
	if (_nState == STATE_UP || STATE_DOWN || STATE_LEFT || STATE_RIGHT)
	{
		framePlay();
	}
	_npcRc1 = RectMakeCenter(ptBodypos.x, ptBodypos.y, _npcFImg[_nState].img->getFrameWidth(), _npcFImg[_nState].img->getFrameHeight());
}

void soldier::render(void)
{
	npc::render();
	Rectangle(getMemDC(), _npcRc1.left, _npcRc1.top, _npcRc1.right, _npcRc1.bottom);
	_npcFImg[_nState].img->frameRender(getMemDC(), _npcFImg[_nState].img->getX() + _npcRc1.left, _npcFImg[_nState].img->getY() + _npcRc1.top);
	//HFONT normal = CreateFont(25, 0, 0, 0, 600, 0, 0, 0, HANGUL_CHARSET, 0, 0, 0, 0, TEXT("HY����L"));
	//SetTextColor(getMemDC(), RGB(255, 255, 255));
	//SelectObject(getMemDC(), normal);
	chat();
}



void soldier::move(void)
{
	switch (_nState)
	{
	case STATE_UP: 
		break;
	case STATE_DOWN:
		break;
	case STATE_LEFT:
		break;
	case STATE_RIGHT:
		break;
	}
}
void soldier::chat()
{
	switch (_nState)
	{
	case STATE_UP:
		break;
	case STATE_DOWN:
		break;
	case STATE_LEFT:
		break;
	case STATE_RIGHT:
		break;
	}
}

void soldier::framePlay()
{
	npc::framePlay();

	_npcFImg[_nState].nCount++;
	_npcFImg[_nState].img->setFrameX(_npcFImg[_nState].nCurrentX);
	if (_npcFImg[_nState].nCount % 2 && _frameSpeed >= 7)
	{
		_npcFImg[_nState].nCurrentX++;
		if (_npcFImg[_nState].nCurrentX == _npcFImg[_nState].nEnd)
		{
			_npcFImg[_nState].nCurrentX = 0;
			_frameSpeed = 0;
		}
	}
}