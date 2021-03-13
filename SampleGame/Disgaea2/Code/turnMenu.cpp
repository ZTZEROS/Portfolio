#include "stdafx.h"
#include "turnMenu.h"


turnMenu::turnMenu()
	:_turnMenus(TURN_NONE), 
	 _cursorPositionY(0),
	_charChoicePositionY(0),
	_menusInTurnMenu(RENDER_TURNMENU),
	_numberOfCharacter(0),
	_handCursorPlace(0)
{
}


turnMenu::~turnMenu()
{
}

HRESULT turnMenu::init()
{
	_turnMenuCursorImage = new image;
	_turnMenuCursorImage->init("UI_IMAGE/handCursor.bmp", 50, 50, false, true, MAGENTA);
	return S_OK;
}
void turnMenu::release()
{
	SAFE_DELETE(_turnMenuCursorImage);
}
void turnMenu::update(int numberOfCharacters, int x, int y)
{
	switch (_menusInTurnMenu)
	{
	case RENDER_TURNMENU:
		//아랫키 눌럿을때 메뉴 밑으로~~~
		if (KEYMANAGER->isOnceKeyDown(VK_DOWN) && _turnMenus != SETTING)
		{
			_turnMenus += 1;
			SOUNDMANAGER->play("menuMove", 1.0f);
		}
		//윗키 눌럿을때 메뉴 위로~~~
		if (KEYMANAGER->isOnceKeyDown(VK_UP) && _turnMenus != EXECUTE)
		{
			_turnMenus -= 1;
			SOUNDMANAGER->play("menuMove", 1.0f);
		}
		//턴메뉴에 따른 손가락 커서 Y축수치 바꿔주고~~~
		switch (_turnMenus)
		{
		case EXECUTE:
			_cursorPositionY = 0;
			break;
		case END_TURN:
			_cursorPositionY = 50;
			break;
		case CHARACTER_LIST:
			_cursorPositionY = 100;
			break;
		case BONUS:
			_cursorPositionY = 150;
			break;
		case HELP:
			_cursorPositionY = 200;
			break;
		case GIVEUP:
			_cursorPositionY = 250;
			break;
		case SETTING:
			_cursorPositionY = 300;
			break;
		}
		break;
	case RENDER_CHARACTER_LIST:
		//벡터 비워주고~
		_vCharacterList.clear();
		//캐릭터 수 0으로 초기화
		_numberOfCharacter = 0;
		//캐릭터 수 받아와주고~
		_numberOfCharacter = numberOfCharacters;
		//캐릭터 수만큼 구조체 만들어주고~~
		for (int i = 0; i < _numberOfCharacter; i++)
		{
			//구조체 변수 선언
			tagCharacterInList characterInList;
			//구조체 청소~
			ZeroMemory(&characterInList, sizeof(tagCharacterInList));
			//렉트 만들어주고~
			characterInList.rc = RectMakeCenter(
				x + 125,
				y + 90 + (i * 55),
				IMAGEMANAGER->findImage("charChoice")->getWidth() - 30,
				55);
			// 사용될지모르겠으나 만드는순서대로 번호 매겨주고~
			characterInList.numbering = i;
			//벡터에 담아보자~~~~
			_vCharacterList.push_back(characterInList);
		}
		//아랫키 눌럿고 캐릭터수보다 더 밑으로 내려가지않게~~~
		if (KEYMANAGER->isOnceKeyDown(VK_DOWN) && _handCursorPlace < _vCharacterList.size() -1)
		{
			_charChoicePositionY += 55;
			_handCursorPlace++;
			SOUNDMANAGER->play("menuMove", 1.0f);
		}
		//윗키 눌럿고 첫번째 캐릭터보다 손가락 커서가 더 위로 가지않게~~ 
		if (KEYMANAGER->isOnceKeyDown(VK_UP) && _handCursorPlace > 0)
		{
			_charChoicePositionY -= 55;
			_handCursorPlace--;
			SOUNDMANAGER->play("menuMove", 1.0f);
		}

		break;
	}
	
}
void turnMenu::render(int x, int y)
{
	switch (_menusInTurnMenu)
	{
		//턴메뉴 렌더~~~
	case RENDER_TURNMENU:
		IMAGEMANAGER->findImage("turnMenu")->render(getMemDC(), x, y);
		_turnMenuCursorImage->render(getMemDC(), x - (_turnMenuCursorImage->getWidth()), y + (70) + _cursorPositionY);
		break;
		//캐릭터 리스트 렌더~~
	case RENDER_CHARACTER_LIST:
		IMAGEMANAGER->findImage("charChoice")->render(getMemDC(), x, y);
		_turnMenuCursorImage->render(getMemDC(), x - (_turnMenuCursorImage->getWidth()), y + (70) + _charChoicePositionY);

		break;
	}

}