#include "stdafx.h"
#include "HeroList.h"


HeroList::HeroList()
	:_cursorPlace(0),
	_cursorPositionY(0),
	_numberOfHeroes(0)
{
}


HeroList::~HeroList()
{
}

HRESULT HeroList::init(int numberOfHeroes, int x, int y)
{
	_heroListCursor = new image;
	_heroListCursor->init("UI_IMAGE/handCursor.bmp", 50, 50, false, true, MAGENTA);
	_rcHand = RectMakeCenter(x, y+55, 80, 20);
	_numberOfHeroes = numberOfHeroes;
	//캐릭터 수만큼 구조체 만들어주고~~
	for (int i = 0; i < _numberOfHeroes; i++)
	{
		//구조체 변수 선언
		tagHeroInList heroInList;
		//구조체 청소~
		ZeroMemory(&heroInList, sizeof(tagHeroInList));
		//렉트 만들어주고~
		heroInList.rc = RectMakeCenter(
			x + 125,
			y + 60 + (i * 55),
			IMAGEMANAGER->findImage("charChoice")->getWidth() - 30,
			40);
		// 사용될지모르겠으나 만드는순서대로 번호 매겨주고~
		heroInList.numbering = i;

		//벡터에 담아보자~~~~
		_vHeroList.push_back(heroInList);
	}
	return S_OK;
}
void HeroList::release()
{

}
void HeroList::update()
{
	
	//아랫키 눌럿고 캐릭터수보다 더 밑으로 내려가지않게~~~
	if (KEYMANAGER->isOnceKeyDown(VK_DOWN) && _cursorPlace < _vHeroList.size() - 1)
	{
		_cursorPositionY += 50;
		_cursorPlace++;
		_rcHand.top += 50;
		_rcHand.bottom += 50;
	}
	//윗키 눌럿고 첫번째 캐릭터보다 손가락 커서가 더 위로 가지않게~~ 
	if (KEYMANAGER->isOnceKeyDown(VK_UP) && _cursorPlace > 0)
	{
		_cursorPositionY -= 50;
		_cursorPlace--;
		_rcHand.top -= 50;
		_rcHand.bottom -= 50;
	}
	for (int i = 0; i < _vHeroList.size(); i++)
	{
		RECT temp;
		if (IntersectRect(&temp, &_rcHand, &_vHeroList[i].rc))
		{
			_selectedOne = _vHeroList[i].numbering;
		}
	}
}
void HeroList::render(int x, int y)
{ 
	IMAGEMANAGER->findImage("heroList")->render(getMemDC(), x, y);
	_heroListCursor->render(getMemDC(), x - (_heroListCursor->getWidth()), y + (25) + _cursorPositionY);
	
	
}

void HeroList:: removeList(int index, int x, int y)
{
	for (int i = 0; i < _vHeroList.size(); i++)
	{
		if (_vHeroList[i].numbering == index)
		{
			_vHeroList.erase(_vHeroList.begin() + i);
		}
	}

	_numberOfHeroes--;
	_vHeroList[_selectedOne].Out = true;
	if (_vHeroList.size() == 2)
	{
		for (int i = 0; i < 2; i++)
		{
			_vHeroList[i].rc = RectMakeCenter(x + 125, y + 60 + i * 55,
				IMAGEMANAGER->findImage("charChoice")->getWidth() - 30,
				55);
		}
	}
	if (_vHeroList.size() == 1)
	{
		for (int i = 0; i < 1; i++)
		{
			_vHeroList[i].rc = RectMakeCenter(x + 125, y + 60,
				IMAGEMANAGER->findImage("charChoice")->getWidth() - 30,
				55);
		}
	}
}