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
	//ĳ���� ����ŭ ����ü ������ְ�~~
	for (int i = 0; i < _numberOfHeroes; i++)
	{
		//����ü ���� ����
		tagHeroInList heroInList;
		//����ü û��~
		ZeroMemory(&heroInList, sizeof(tagHeroInList));
		//��Ʈ ������ְ�~
		heroInList.rc = RectMakeCenter(
			x + 125,
			y + 60 + (i * 55),
			IMAGEMANAGER->findImage("charChoice")->getWidth() - 30,
			40);
		// �������𸣰����� ����¼������ ��ȣ �Ű��ְ�~
		heroInList.numbering = i;

		//���Ϳ� ��ƺ���~~~~
		_vHeroList.push_back(heroInList);
	}
	return S_OK;
}
void HeroList::release()
{

}
void HeroList::update()
{
	
	//�Ʒ�Ű ������ ĳ���ͼ����� �� ������ ���������ʰ�~~~
	if (KEYMANAGER->isOnceKeyDown(VK_DOWN) && _cursorPlace < _vHeroList.size() - 1)
	{
		_cursorPositionY += 50;
		_cursorPlace++;
		_rcHand.top += 50;
		_rcHand.bottom += 50;
	}
	//��Ű ������ ù��° ĳ���ͺ��� �հ��� Ŀ���� �� ���� �����ʰ�~~ 
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