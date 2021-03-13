#include "stdafx.h"
#include "battleMenu.h"


battleMenu::battleMenu()
	:_menus(NONE), _handPositionY(0)
{
}


battleMenu::~battleMenu()
{
}

HRESULT battleMenu::init(void)
{
	//�̹��� ������ְ�~~~~
	_battleMenuImage = IMAGEMANAGER->addImage(
			"��Ʋ�޴�","UI_IMAGE/battleMenu.bmp",
		245, 375, true,MAGENTA);
	_battleMenuCursorImage = IMAGEMANAGER->addImage(
		"��ƲĿ��", "UI_IMAGE/handCursor.bmp", 50, 50,
		true, MAGENTA);

	return S_OK;
}
void battleMenu::release(void)
{

}
void battleMenu::update()
{
	//�Ʒ�Ű �������� �޴� ������~~~
	if (KEYMANAGER->isOnceKeyDown(VK_DOWN) && _menus != EQUIPMENT)
	{
		_menus += 1;
		SOUNDMANAGER->play("menuMove", 1.0f);
	}
	//��Ű �������� �޴� ����~~~
	if (KEYMANAGER->isOnceKeyUp(VK_UP) && _menus != MOVE)
	{
		_menus -= 1;
		SOUNDMANAGER->play("menuMove", 1.0f);
	}
	//�޴��� �ٲ� �հ��� Ŀ�� ��� Y�� ��ġ ������Ʈ~ 
	switch (_menus)
	{
	case MOVE:
		_handPositionY = 0;
		break;
	case ATTACK:
		_handPositionY = 55;
		break;
	case DEFEND:
		_handPositionY = 110;
		break;
	case ITEM:
		_handPositionY = 165;
		break;
	case EQUIPMENT:
		_handPositionY = 220;
		break;
	}
}
void battleMenu::render(int x, int y)
{
	//��Ʋ�޴� �̹��� ������~
	_battleMenuImage->render(getMemDC(), x, y);
	//��Ʋ�Ŵ� Ŀ�� �̹��� ������
	_battleMenuCursorImage->render(getMemDC(), x 
		- _battleMenuCursorImage->getWidth() , y + _handPositionY);

}