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
	_mList.insert(make_pair("downleft_01", "꼬마야 이렇게 늦은 시간에 뭐하고 다니는거지? 다 자랄 때까지 집에나 있도록 해!"));
	_mList.insert(make_pair("downleft_02", "지도에 표시해줄테니 길 잃지 말고 돌아가도록 해. (S버튼으로 맵을 볼수 있다.)"));
	_mList.insert(make_pair("down_01", "힘 꽤나 쓸 것 같이 생겼군, 얼만큼 들 수 있나? (X버튼으로 물건을 들 수 있다.)"));
	_mList.insert(make_pair("down_02", "네가 칼 잡을 날이 올지 내기하지! (칼이 있을 때 Z버튼으로 휘두를 수 있다.)"));
	_mList.insert(make_pair("right_01", "맵의 표시가 네 목적지이다."));
	_mList.insert(make_pair("right_02", "상자에서 여러가지를 얻을 수 있다.(X버튼으로 상자를 연다.)"));
	_mList.insert(make_pair("castle_door", "이봐, 넌 성으로 들어갈 자격이 없어! 집으로 돌아가서 잠이나 자!"));
	_mList.insert(make_pair("castle_left_01", "꼬마야 이곳은 군인들이 둘러서 있다. 너를 혼쭐을 내줄 수도 있지만, 잘 알거라고 믿는다."));
	_mList.insert(make_pair("castle_left_02", " 꼬마야 이 늦은 시간에 무엇을 하고 있는거냐. 어서 집으로 돌아가!"));

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
	//HFONT normal = CreateFont(25, 0, 0, 0, 600, 0, 0, 0, HANGUL_CHARSET, 0, 0, 0, 0, TEXT("HY엽서L"));
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