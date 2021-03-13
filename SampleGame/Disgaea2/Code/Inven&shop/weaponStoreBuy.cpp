
#include "stdafx.h"
#include "weaponStoreBuy.h"
#include "inventory.h"

weaponStoreBuy::weaponStoreBuy()
{

}

weaponStoreBuy::~weaponStoreBuy()
{

}

HRESULT weaponStoreBuy::init(void)
{	

	_info = new itemInfo;
	_info->init();

	_isRegisted = false;
	registItem();
	_totalItemNum = listedItem();
	_totalItemNum += 1;

	//커서 이미지 등록
	_currentItemNum = 0;

	_cursor = IMAGEMANAGER->findImage("cursorImage");
	_cursorRc = RectMake(_vItem[_currentItemNum].itemRc.left-15, _vItem[_currentItemNum].itemRc.top+10, 50, 50);

	//범위 랙트 초기화(vItem 0번이랑 사이즈 같게)
	_extentRc = RectMake(_vItem[0].itemRc.left, _vItem[0].itemRc.top, 300, 400);

	_zeroVectorPointX = _vItem[0].itemRc.left;
	_zeroVectorPointY = _vItem[0].itemRc.top;

	_shopImage = IMAGEMANAGER->findImage("shopimage");
	_hellslot = IMAGEMANAGER->findImage("hellslot");


	_totalMoney = 0;

	_isPushDownC = false;

	return S_OK;
}

void weaponStoreBuy::release(void)
{
}

void weaponStoreBuy::update(int money)
{
	_totalMoney = money;
	/*
	선택되는 랙트에 따라서 선택되는 아이템 인포가 바뀐다.
	구조체에 넘버를 먹여서, 현재 백터의 구조체 넘버의 정보를 따 오도록 만든다?
	*/

	if(KEYMANAGER->isOnceKeyDown(VK_DOWN))
	{
		SOUNDMANAGER->play("menuMove", 1.0f);
		//랙트 사이즈만큼 올린다.
		if( _cursorRc.bottom >= _extentRc.bottom)
		{
			//전부 1칸씪 올린다.
			for(int i = 0; i < _totalItemNum - 1; i++)
			{
				if(_currentItemNum < _totalItemNum - 2)
				{
					_vItem[i].itemRc.top -= 50;
					_vItem[i].itemRc.bottom -= 50;
				}
			}
		}
		if(_currentItemNum < _totalItemNum - 2) _currentItemNum++;
	}
	if(KEYMANAGER->isOnceKeyDown(VK_UP))
	{
		SOUNDMANAGER->play("menuMove", 1.0f);
		//랙트 사이즈만큼 올린다.
		if( _cursorRc.top < _extentRc.top + 50)
		{
			//전부 1칸씪 올린다.
			for(int i = 0; i < _totalItemNum; i++)
			{
				if(_currentItemNum > 0)
				{
					_vItem[i].itemRc.top += 50;
					_vItem[i].itemRc.bottom += 50;
				}
			}
		}
		if(_currentItemNum > 0) _currentItemNum--;
	}

	if(KEYMANAGER->isOnceKeyDown('X'))
	{
		SOUNDMANAGER->play("menuSelect", 1.0f);
		if(_inven->getFullItem() && _totalMoney >= 0)
		{
			if((_totalMoney - _vItem[_currentItemNum].itemHell) > 0) 
			{
				_inven->setItemVector(_vItem[_currentItemNum]);
				_totalMoney -= _vItem[_currentItemNum].itemHell;
				_vItem.erase(_vItem.begin() + _currentItemNum);
			}
			//더이상 넣을 수 없다는 오류 메세지 출력!
		}
		int i = 0;
		for(_viItem = _vItem.begin(); _viItem != _vItem.end(); _viItem++)
		{
			if(i >= _totalItemNum) break;
			i++;
			_viItem->itemRc = RectMake(_zeroVectorPointX, _zeroVectorPointY + ( 50 * i ), 300, 50);
		}
		_totalItemNum = i + 1;
	}

	if(_isPushDownC) // && KEYMANAGER->isOnceKeyDown('C')) 
	{
		_inven->setInvenOff(true);
		_isPushDownC = false;
	}

	if(_isRegisted)
	{
		_inven->setItemRead(true);
		//_totalItemNum = listedItem();
		_isRegisted = false;
	}

	_cursorRc = RectMake(_vItem[_currentItemNum].itemRc.left-15, _vItem[_currentItemNum].itemRc.top+10, 50, 50);
	//움직일 때마다 바뀌니까 여기다가도 한번 셋팅
	_zeroVectorPointX = _vItem[0].itemRc.left;
	_zeroVectorPointY = _vItem[0].itemRc.top - 50;

	//돈 출력
	sprintf(arrHell, "%d", _totalMoney);

	//아이템 인포를 위한 처리(함수 만들기 귀찮다)
	sprintf(_itemInfo.recoveryHP, "%d", _vItem[_currentItemNum].recoveryHP);
	sprintf(_itemInfo.recoverySP, "%d", _vItem[_currentItemNum].recoverySP);
	sprintf(_itemInfo.regulaterAttack, "%d", _vItem[_currentItemNum].regulaterAttack);
	sprintf(_itemInfo.regulaterDefence, "%d", _vItem[_currentItemNum].regulaterDefence);
	sprintf(_itemInfo.regulaterHit, "%d", _vItem[_currentItemNum].regulaterHit);
	sprintf(_itemInfo.regulaterMagic, "%d", _vItem[_currentItemNum].regulaterMagic);
	sprintf(_itemInfo.regulaterSpeed, "%d", _vItem[_currentItemNum].regulaterSpeed);
	sprintf(_itemInfo.regulaterResistance, "%d", _vItem[_currentItemNum].regulaterResistance);
	sprintf(_itemInfo.itemName, "%s", _vItem[_currentItemNum].name);
	sprintf(_itemInfo.itemIndex, "%s", _vItem[_currentItemNum].itemIndex);
	//sprintf(_itemInfo.regulaterHP, " - HP최대치: %d", _vItem[_currentItemNum].regulaterHP);
	//sprintf(_itemInfo.regulaterSP, " - SP최대치: %d", _vItem[_currentItemNum].regulaterSP);
	
	_info->update(_itemInfo);

}

void weaponStoreBuy::render(void)
{
	IMAGEMANAGER->findImage("storebg2")->render(getIMemDC(), 0, 0);
	IMAGEMANAGER->findImage("shopimage")->render(getIMemDC(), _extentRc.left, _extentRc.top - 50);
	IMAGEMANAGER->findImage("hellslot")->render(getIMemDC(), 50, 590);
	//Rectangle(getMemDC(), _extentRc.left, _extentRc.top, _extentRc.right, _extentRc.bottom);
	HFONT font, oldfont;
	font = CreateFont(35, 0, 0, 0, 1000, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH, TEXT("맑은 고딕"));
	oldfont = (HFONT)SelectObject(getIMemDC(), font);
	SetTextColor(getIMemDC(), RGB(255, 255, 255));
	SetBkMode(getIMemDC(), TRANSPARENT);
	TextOut(getIMemDC(), 250 - (unitNum(_totalMoney) * 14), 595, arrHell, lstrlen(arrHell));
	listRender();
	SelectObject(getIMemDC(), oldfont);
	DeleteObject(font);
	IMAGEMANAGER->findImage("cursorImage")->render(getIMemDC(), _cursorRc.left, _cursorRc.top);
	_info->render();

}

void weaponStoreBuy::registItem(void)
{
	//도끼
	_weaponItem[0].name = "토르의 뿅망치";
	_weaponItem[0].img = IMAGEMANAGER->findImage("axeType");
	_weaponItem[0].itemKind = "weapon";
	_weaponItem[0].itemIndex = "내려치면 번개가 칠 지도 모른다.";
	_weaponItem[0].recoveryHP = 0;
	_weaponItem[0].recoverySP = 0;
	_weaponItem[0].regulaterAttack = 20;
	_weaponItem[0].regulaterDefence = 40;
	_weaponItem[0].regulaterSpeed = 0;
	_weaponItem[0].regulaterHit = 0;
	_weaponItem[0].regulaterHP = 0;
	_weaponItem[0].regulaterSP = 0;
	_weaponItem[0].regulaterMagic = 0;
	_weaponItem[0].regulaterResistance = 0;
	_weaponItem[0].itemHell = 5000;


	//활
	_weaponItem[1].name = "에로스의 장난감 활";
	_weaponItem[1].img = IMAGEMANAGER->findImage("bowType");
	_weaponItem[1].itemKind = "weapon";
	_weaponItem[1].itemIndex = "맞으면 뿅간다는 전설의 하트모양 화살 2개(플라스틱제, 비매품) 추가";
	_weaponItem[1].recoveryHP = 0;
	_weaponItem[1].recoverySP = 0;
	_weaponItem[1].regulaterAttack = 10;
	_weaponItem[1].regulaterDefence = 0;
	_weaponItem[1].regulaterSpeed = 0;
	_weaponItem[1].regulaterHit = 20;
	_weaponItem[1].regulaterHP = 0;
	_weaponItem[1].regulaterSP = 0;
	_weaponItem[1].regulaterMagic = 5;
	_weaponItem[1].regulaterResistance = 0;
	_weaponItem[1].itemHell = 1000;

	//총
	_weaponItem[2].name = "글록17";
	_weaponItem[2].img = IMAGEMANAGER->findImage("gunType");
	_weaponItem[2].itemKind = "weapon";
	_weaponItem[2].itemIndex = "제작자가 어릴 적 수없이 부수고 놀았던 BB탄용 권총. 최대 35발";
	_weaponItem[2].recoveryHP = 0;
	_weaponItem[2].recoverySP = 0;
	_weaponItem[2].regulaterAttack = 20;
	_weaponItem[2].regulaterDefence = 0;
	_weaponItem[2].regulaterSpeed = 12;
	_weaponItem[2].regulaterHit = 15;
	_weaponItem[2].regulaterHP = 0;
	_weaponItem[2].regulaterSP = 0;
	_weaponItem[2].regulaterMagic = 0;
	_weaponItem[2].regulaterResistance = 0;
	_weaponItem[2].itemHell = 500;

	//건틀릿
	_weaponItem[3].name = "잇포의 손장갑";
	_weaponItem[3].img = IMAGEMANAGER->findImage("guntletType");
	_weaponItem[3].itemKind = "weapon";
	_weaponItem[3].itemIndex = "미친듯이 쳐맞아도 다시 일어설 수 있다는 좀비화가 발라진 전설의 장갑";
	_weaponItem[3].recoveryHP = 0;
	_weaponItem[3].recoverySP = 0;
	_weaponItem[3].regulaterAttack = 0;
	_weaponItem[3].regulaterDefence = 900;
	_weaponItem[3].regulaterSpeed = 0;
	_weaponItem[3].regulaterHit = 0;
	_weaponItem[3].regulaterHP = 0;
	_weaponItem[3].regulaterSP = 0;
	_weaponItem[3].regulaterMagic = 0;
	_weaponItem[3].regulaterResistance = 0;
	_weaponItem[3].itemHell = 5300;

	//고기장갑
	_weaponItem[4].name = "사이타마의 손";
	_weaponItem[4].img = IMAGEMANAGER->findImage("nikuType");
	_weaponItem[4].itemKind = "weapon";
	_weaponItem[4].itemIndex = "제품명 이상의 설명이 필요한가?";
	_weaponItem[4].recoveryHP = 0;
	_weaponItem[4].recoverySP = 0;
	_weaponItem[4].regulaterAttack = 700;
	_weaponItem[4].regulaterDefence = 0;
	_weaponItem[4].regulaterSpeed = 0;
	_weaponItem[4].regulaterHit = 0;
	_weaponItem[4].regulaterHP = 0;
	_weaponItem[4].regulaterSP = 0;
	_weaponItem[4].regulaterMagic = 0;
	_weaponItem[4].regulaterResistance = 0;
	_weaponItem[4].itemHell = 10000;

	//스피어
	_weaponItem[5].name = "여의봉";
	_weaponItem[5].img = IMAGEMANAGER->findImage("spearType");
	_weaponItem[5].itemKind = "weapon";
	_weaponItem[5].itemIndex = "여의봉이라 써놓고 효자손이라 읽는다. 길이가 늘어나는 요즘세대 히트상품";
	_weaponItem[5].recoveryHP = 0;
	_weaponItem[5].recoverySP = 0;
	_weaponItem[5].regulaterAttack = 100;
	_weaponItem[5].regulaterDefence = 50;
	_weaponItem[5].regulaterSpeed = 0;
	_weaponItem[5].regulaterHit = 0;
	_weaponItem[5].regulaterHP = 0;
	_weaponItem[5].regulaterSP = 0;
	_weaponItem[5].regulaterMagic = 0;
	_weaponItem[5].regulaterResistance = 0;
	_weaponItem[5].itemHell = 1200;

	//스테프
	_weaponItem[6].name = "간달프의 스테프";
	_weaponItem[6].img = IMAGEMANAGER->findImage("steapType");
	_weaponItem[6].itemKind = "weapon";
	_weaponItem[6].itemIndex = "어떤 반지의제왕 오덕이 이쑤시개를 붙여 만들었다는 노력과 고난의 산물.";
	_weaponItem[6].recoveryHP = 0;
	_weaponItem[6].recoverySP = 0;
	_weaponItem[6].regulaterAttack = 10;
	_weaponItem[6].regulaterDefence = 5;
	_weaponItem[6].regulaterSpeed = 0;
	_weaponItem[6].regulaterHit = 0;
	_weaponItem[6].regulaterHP = 0;
	_weaponItem[6].regulaterSP = 0;
	_weaponItem[6].regulaterMagic = 50;
	_weaponItem[6].regulaterResistance = 40;
	_weaponItem[6].itemHell = 5000;

	//칼
	_weaponItem[7].name = "제다이의 광선검";
	_weaponItem[7].img = IMAGEMANAGER->findImage("swordType");
	_weaponItem[7].itemKind = "weapon";
	_weaponItem[7].itemIndex = "유원지에 가면 쉽게 구할 수 있는 초절정 LED 눈뽕 광선검(효과음기능)";
	_weaponItem[7].recoveryHP = 0;
	_weaponItem[7].recoverySP = 0;
	_weaponItem[7].regulaterAttack = 30;
	_weaponItem[7].regulaterDefence = 10;
	_weaponItem[7].regulaterSpeed = 0;
	_weaponItem[7].regulaterHit = 50;
	_weaponItem[7].regulaterHP = 0;
	_weaponItem[7].regulaterSP = 0;
	_weaponItem[7].regulaterMagic = 0;
	_weaponItem[7].regulaterResistance = 0;
	_weaponItem[7].itemHell = 1000;

}

//아이템 능력치 보정용 함수
tagItem weaponStoreBuy::itemOptionDefine(tagItem ti)
{
	ti.itemClass = (itemRarity)RND->getFromIntTo(1, 5); // 1~5까지의 등급을 렌덤으로 먹인다.
		
	//올려줄 능치 개수
	int loop = RND->getFromIntTo(0, 2); //몇 개 올려줄 건지. 루프용

	//아이템 가격을 정하기 위한 기본 변수값(전체 능력치를 더 한 값.)
	int oldStateNum = ti.recoveryHP + ti.recoverySP + ti.regulaterAttack + ti.regulaterDefence + ti.regulaterHit + ti.regulaterHP +
		ti.regulaterSP + ti.regulaterMagic + ti.regulaterResistance + ti.regulaterSpeed;

	//아이템 등급에 따른 능치 보정
	if(loop > 0)
	{
		for(int i = 0; i <= loop; i++)
		{
			_is = (itemState)RND->getFromIntTo(0, 10);
			switch(_is)
			{
				case STATE_RECOVERY_HP:
					if(ti.itemKind == "consumeItemHP") ti.recoveryHP = ti.recoveryHP + ti.itemClass*(RND->getFromIntTo(1, 5) - i);
					break;
				case STATE_RECOVERY_SP:
					if(ti.itemKind == "consumeItemSP") ti.recoverySP = ti.recoverySP + ti.itemClass*(RND->getFromIntTo(1, 5) - i);
					break;
				case STATE_REGULATER_HP:
					if(ti.itemKind != "consumeItemHP" && ti.itemKind != "consumeItemSP") ti.regulaterHP = ti.regulaterHP + ti.itemClass*(RND->getFromIntTo(1, 5) - i);
					break;
				case STATE_REGULATER_SP:
					if(ti.itemKind != "consumeItemHP" && ti.itemKind != "consumeItemSP") ti.regulaterSP = ti.regulaterSP + ti.itemClass*(RND->getFromIntTo(1, 5) - i);
					break;
				case STATE_REGULATER_ATK:
					if(ti.itemKind != "consumeItemHP" && ti.itemKind != "consumeItemSP") ti.regulaterAttack = ti.regulaterAttack + ti.itemClass*(RND->getFromIntTo(1, 5) - i);
					break;
				case STATE_REGULATER_DEF:
					if(ti.itemKind != "consumeItemHP" && ti.itemKind != "consumeItemSP") ti.regulaterDefence = ti.regulaterDefence + ti.itemClass*(RND->getFromIntTo(1, 5) - i);
					break;
				case STATE_REGULATER_HIT:
					if(ti.itemKind != "consumeItemHP" && ti.itemKind != "consumeItemSP") ti.regulaterHit = ti.regulaterHit + ti.itemClass*(RND->getFromIntTo(1, 5) - i);
					break;
				case STATE_REGULATER_SPEED:
					if(ti.itemKind != "consumeItemHP" && ti.itemKind != "consumeItemSP") ti.regulaterSpeed = ti.regulaterSpeed + ti.itemClass*(RND->getFromIntTo(1, 5) - i);
					break;
				case STATE_REGULATER_MAGIC:
					if(ti.itemKind != "consumeItemHP" && ti.itemKind != "consumeItemSP") ti.regulaterMagic = ti.regulaterMagic + ti.itemClass*(RND->getFromIntTo(1, 5) - i);
					break;
				case STATE_REGULATER_RES:
					if(ti.itemKind != "consumeItemHP" && ti.itemKind != "consumeItemSP") ti.regulaterResistance = ti.regulaterResistance + ti.itemClass*(RND->getFromIntTo(1, 5) - i);
					break;
				default:
					break;
			}
		}
	}
	//보정 후 스테이터스 값
	int nowStateNum = ti.recoveryHP + ti.recoverySP + ti.regulaterAttack + ti.regulaterDefence + ti.regulaterHit + ti.regulaterHP +
		ti.regulaterSP + ti.regulaterMagic + ti.regulaterResistance + ti.regulaterSpeed;
	
	//가격을 정합니다.(기본 금액+ ((보정 후 스테이터스 수치 - 보정 전 스테이터스 수치) / 2))
	ti.itemHell = ti.itemHell + ((nowStateNum - oldStateNum) / 2);

	wsprintf(ti.str, "                 HL           %d", ti.itemHell);
	
	return ti;
}

//여기서 아이템 보정된 값을 진열
int weaponStoreBuy::listedItem(void)
{
	//먼저 등록된 아이템부터 오른차순으로 벡터에 로드된다.
	//-->반복문을 돌려서 1번 몇개 2번 몇개 식으로 로드한다.

	int totalListedItemNum = 0;


	//방어구 먼저
	for(int i = 0; i < 8; i++)
	{
		for(int j = 0; j < RND->getFromIntTo(0, 8); j++)
		{
			totalListedItemNum++;
			_weaponItem[i].itemRc = RectMake(50, 90 + ( 50 * totalListedItemNum ), 300, 50);
			_vItem.push_back(itemOptionDefine(_weaponItem[i]));
		}
	}

	return totalListedItemNum;

}

void weaponStoreBuy::listRender(void)
{
	//리스트를 일정 간격으로 출력한다.

	for(_viItem = _vItem.begin(); _viItem != _vItem.end(); ++_viItem)
	{
		if(_viItem->itemRc.top >= _extentRc.top && _viItem->itemRc.bottom <= _extentRc.bottom)
		{
			//Rectangle(getMemDC(), _viItem->itemRc.left, _viItem->itemRc.top, _viItem->itemRc.right, _viItem->itemRc.bottom);
			//_viItem->img->render(getMemDC(), _viItem->itemRc.left+30, _viItem->itemRc.top);
			TextOut(getIMemDC(), _viItem->itemRc.left+90, _viItem->itemRc.top, _viItem->name, lstrlen(_viItem->name));
			TextOut(getIMemDC(), _viItem->itemRc.left+230, _viItem->itemRc.top, _viItem->str, lstrlen(_viItem->str));
		}
	}
}

