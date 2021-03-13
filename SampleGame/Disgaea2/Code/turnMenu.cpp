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
		//�Ʒ�Ű �������� �޴� ������~~~
		if (KEYMANAGER->isOnceKeyDown(VK_DOWN) && _turnMenus != SETTING)
		{
			_turnMenus += 1;
			SOUNDMANAGER->play("menuMove", 1.0f);
		}
		//��Ű �������� �޴� ����~~~
		if (KEYMANAGER->isOnceKeyDown(VK_UP) && _turnMenus != EXECUTE)
		{
			_turnMenus -= 1;
			SOUNDMANAGER->play("menuMove", 1.0f);
		}
		//�ϸ޴��� ���� �հ��� Ŀ�� Y���ġ �ٲ��ְ�~~~
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
		//���� ����ְ�~
		_vCharacterList.clear();
		//ĳ���� �� 0���� �ʱ�ȭ
		_numberOfCharacter = 0;
		//ĳ���� �� �޾ƿ��ְ�~
		_numberOfCharacter = numberOfCharacters;
		//ĳ���� ����ŭ ����ü ������ְ�~~
		for (int i = 0; i < _numberOfCharacter; i++)
		{
			//����ü ���� ����
			tagCharacterInList characterInList;
			//����ü û��~
			ZeroMemory(&characterInList, sizeof(tagCharacterInList));
			//��Ʈ ������ְ�~
			characterInList.rc = RectMakeCenter(
				x + 125,
				y + 90 + (i * 55),
				IMAGEMANAGER->findImage("charChoice")->getWidth() - 30,
				55);
			// �������𸣰����� ����¼������ ��ȣ �Ű��ְ�~
			characterInList.numbering = i;
			//���Ϳ� ��ƺ���~~~~
			_vCharacterList.push_back(characterInList);
		}
		//�Ʒ�Ű ������ ĳ���ͼ����� �� ������ ���������ʰ�~~~
		if (KEYMANAGER->isOnceKeyDown(VK_DOWN) && _handCursorPlace < _vCharacterList.size() -1)
		{
			_charChoicePositionY += 55;
			_handCursorPlace++;
			SOUNDMANAGER->play("menuMove", 1.0f);
		}
		//��Ű ������ ù��° ĳ���ͺ��� �հ��� Ŀ���� �� ���� �����ʰ�~~ 
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
		//�ϸ޴� ����~~~
	case RENDER_TURNMENU:
		IMAGEMANAGER->findImage("turnMenu")->render(getMemDC(), x, y);
		_turnMenuCursorImage->render(getMemDC(), x - (_turnMenuCursorImage->getWidth()), y + (70) + _cursorPositionY);
		break;
		//ĳ���� ����Ʈ ����~~
	case RENDER_CHARACTER_LIST:
		IMAGEMANAGER->findImage("charChoice")->render(getMemDC(), x, y);
		_turnMenuCursorImage->render(getMemDC(), x - (_turnMenuCursorImage->getWidth()), y + (70) + _charChoicePositionY);

		break;
	}

}