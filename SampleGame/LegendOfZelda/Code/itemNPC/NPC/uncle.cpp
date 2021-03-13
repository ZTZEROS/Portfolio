#include "stdafx.h"
#include "../../stdafx.h"
#include "uncle.h"


uncle::uncle()
{
}

uncle::~uncle()
{
}

HRESULT uncle::init(void)
{
	npc::init();
	
	//_princess = new princess;

	ptBodypos.x = 704;
	ptBodypos.y = 395;
	
	_npcFImg[STATE_WALK_DOWN].img = IMAGEMANAGER->findImage("uncleDown");
	_npcFImg[STATE_WALK_DOWN].nCount = 0;
	_npcFImg[STATE_WALK_DOWN].nEnd = 3;
	_npcFImg[STATE_WALK_DOWN].nRemainder = 4;
	_npcFImg[STATE_WALK_DOWN].nStart = 0;
	_npcFImg[STATE_WALK_DOWN].nCurrentX = 0;
	_npcFImg[STATE_WALK_DOWN].img->setX(-17);
	_npcFImg[STATE_WALK_DOWN].img->setY(0);

	_npcFImg[STATE_WALK_LEFT].img = IMAGEMANAGER->findImage("uncleLeft");
	_npcFImg[STATE_WALK_LEFT].nCount = 0;
	_npcFImg[STATE_WALK_LEFT].nEnd = 2;
	_npcFImg[STATE_WALK_LEFT].nRemainder = 4;
	_npcFImg[STATE_WALK_LEFT].nStart = 0;
	_npcFImg[STATE_WALK_LEFT].nCurrentX = 0;
	_npcFImg[STATE_WALK_LEFT].img->setX(-20);
	_npcFImg[STATE_WALK_LEFT].img->setY(-10);

	_npcFImg[STATE_WALK_RIGHT].img = IMAGEMANAGER->findImage("uncleRight");
	_npcFImg[STATE_WALK_RIGHT].nCount = 0;
	_npcFImg[STATE_WALK_RIGHT].nEnd = 2;
	_npcFImg[STATE_WALK_RIGHT].nRemainder = 4;
	_npcFImg[STATE_WALK_RIGHT].nStart = 0;
	_npcFImg[STATE_WALK_RIGHT].nCurrentX = 0;
	_npcFImg[STATE_WALK_RIGHT].img->setX(0);
	_npcFImg[STATE_WALK_RIGHT].img->setY(0);

	_npcFImg[STATE_DIE_WEAPON].img = IMAGEMANAGER->findImage("uncleWeapon");
	_npcFImg[STATE_DIE_WEAPON].nCount = 0;
	_npcFImg[STATE_DIE_WEAPON].nEnd = 1;
	_npcFImg[STATE_DIE_WEAPON].nRemainder = 0;
	_npcFImg[STATE_DIE_WEAPON].nStart = 0;
	_npcFImg[STATE_DIE_WEAPON].nCurrentX = 0;
	_npcFImg[STATE_DIE_WEAPON].img->setX(0);
	_npcFImg[STATE_DIE_WEAPON].img->setY(0);

	_npcFImg[STATE_DIED].img = IMAGEMANAGER->findImage("uncleDie");
	_npcFImg[STATE_DIED].nCount = 0;
	_npcFImg[STATE_DIED].nEnd = 1;
	_npcFImg[STATE_DIED].nRemainder = 0;
	_npcFImg[STATE_DIED].nStart = 0;
	_npcFImg[STATE_DIED].nCurrentX = 0;
	_npcFImg[STATE_DIED].img->setX(0);
	_npcFImg[STATE_DIED].img->setY(0);

	_npcFImg[STATE_SIT].img = IMAGEMANAGER->findImage("uncleSit");
	_npcFImg[STATE_SIT].nCount = 0;
	_npcFImg[STATE_SIT].nEnd = 1;
	_npcFImg[STATE_SIT].nRemainder = 0;
	_npcFImg[STATE_SIT].nStart = 0;
	_npcFImg[STATE_SIT].nCurrentX = 0;
	_npcFImg[STATE_SIT].img->setX(0);
	_npcFImg[STATE_SIT].img->setY(0);

	_npcFImg[STATE_SIT_LEFT].img = IMAGEMANAGER->findImage("uncleSitLeft");
	_npcFImg[STATE_SIT_LEFT].nCount = 0;
	_npcFImg[STATE_SIT_LEFT].nEnd = 1;
	_npcFImg[STATE_SIT_LEFT].nRemainder = 0;
	_npcFImg[STATE_SIT_LEFT].nStart = 0;
	_npcFImg[STATE_SIT_LEFT].nCurrentX = 0;
	_npcFImg[STATE_SIT_LEFT].img->setX(0);
	_npcFImg[STATE_SIT_LEFT].img->setY(0);

	_nState = STATE_SIT;

	_framecount = 0;
	rt = { 165, 600, 860, 700 };
	speed = 0;
	procedure = 0;

	_frameSpeed = 0;
	_mList.insert(make_pair("1_01", "링크야, 잠시 나갔다 오마. 걱정할 것 없다. 아침까지는 돌아오마. 집 밖으로 나오지 말거라."));

	_npcRc1 = RectMakeCenter(ptBodypos.x, ptBodypos.y, _npcFImg[_nState].img->getFrameWidth(), _npcFImg[_nState].img->getFrameHeight());

	return S_OK;
}
void uncle::release(void)
{
	npc::release();
}

void uncle::update(void)
{
	npc::update();
	_frameSpeed++;

	move();
	//chat();
	speed++;
	if (_nState == STATE_WALK_DOWN || STATE_WALK_LEFT || STATE_WALK_RIGHT || STATE_DIE_WEAPON || STATE_DIED || STATE_SIT || STATE_SIT_LEFT)
	{
		framePlay();
	}
}

void uncle::render(void)
{
	npc::render();
	Rectangle(getMemDC(), _npcRc1.left, _npcRc1.top, _npcRc1.right, _npcRc1.bottom);
	_npcFImg[_nState].img->frameRender(getMemDC(), _npcFImg[_nState].img->getX() + _npcRc1.left, _npcFImg[_nState].img->getY() + _npcRc1.top);
	//HFONT normal = CreateFont(25, 0, 0, 0, 600, 0, 0, 0, HANGUL_CHARSET, 0, 0, 0, 0, TEXT("HY엽서L"));
	//SetTextColor(getMemDC(), RGB(255, 255, 255));
	//SelectObject(getMemDC(), normal);
	if (CurrentMenu == 0) chat();
}



void uncle::move(void)
{
	switch (_nState)
	{
	case STATE_SIT:
		break;
	case STATE_SIT_LEFT:
		break;
	case STATE_WALK_DOWN:
		if (_npcRc1.top <= WINSIZEY)
		{
			_npcRc1.top += 5.0;
			_npcRc1.bottom += 5.0;
		}
		break;
	case STATE_WALK_LEFT:
		if (_npcRc1.right - 40 >= WINSIZEX / 2)
		{ 
			_npcRc1.left -=5.0;
			_npcRc1.right -= 5.0;
		}
		if (_npcRc1.right - 40 <= WINSIZEX / 2)
		{
			_nState = STATE_WALK_DOWN;
		}
		break;
	case STATE_DIE_WEAPON:
		break;
	case STATE_DIED:
		break;
	}
}
void uncle::chat()
{
	switch (_nState)
	{
	case STATE_SIT:
		break;
	case STATE_SIT_LEFT:
		if (KEYMANAGER->isOnceKeyDown('X'))
		{
			procedure++;
			textframe = 0;// strlen(_mIter->second.c_str());
		}

		if (procedure == 0)
		{
			Talking = 1;
			_mIter = _mList.find("1_01");	
			DrawText(getMemDC(), _mIter->second.c_str(), textframe, &rt, DT_LEFT | DT_WORDBREAK);
		}
		
		if (procedure == 1)
		{	
			Talking = 0;
			_nState = STATE_WALK_LEFT;
		}
		
		if (speed >= 5 && textframe < strlen(_mIter->second.c_str()))
		{
			textframe += 2;
			speed = 0;
		}
		break;
	case STATE_WALK_DOWN:
		break;
	case STATE_WALK_LEFT:
		break;
	case STATE_DIE_WEAPON:
		break;
	case STATE_DIED:
		break;
	}
}

void uncle::framePlay()
{
	npc::framePlay();

	_npcFImg[_nState].nCount++;
	_npcFImg[_nState].img->setFrameX(_npcFImg[_nState].nCurrentX);

	if (_npcFImg[_nState].nCount % 2 && _frameSpeed >= 10)
	{
		_npcFImg[_nState].nCurrentX++;
		if (_npcFImg[_nState].nCurrentX > _npcFImg[_nState].nEnd)
		{
			_npcFImg[_nState].nCurrentX = 0;
			_frameSpeed = 0;
		}
	}
}