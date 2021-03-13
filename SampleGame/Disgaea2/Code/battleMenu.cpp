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
	//이미지 등록해주고~~~~
	_battleMenuImage = IMAGEMANAGER->addImage(
			"배틀메뉴","UI_IMAGE/battleMenu.bmp",
		245, 375, true,MAGENTA);
	_battleMenuCursorImage = IMAGEMANAGER->addImage(
		"배틀커서", "UI_IMAGE/handCursor.bmp", 50, 50,
		true, MAGENTA);

	return S_OK;
}
void battleMenu::release(void)
{

}
void battleMenu::update()
{
	//아랫키 눌럿을때 메뉴 밑으로~~~
	if (KEYMANAGER->isOnceKeyDown(VK_DOWN) && _menus != EQUIPMENT)
	{
		_menus += 1;
		SOUNDMANAGER->play("menuMove", 1.0f);
	}
	//윗키 눌럿을때 메뉴 위로~~~
	if (KEYMANAGER->isOnceKeyUp(VK_UP) && _menus != MOVE)
	{
		_menus -= 1;
		SOUNDMANAGER->play("menuMove", 1.0f);
	}
	//메뉴가 바뀔때 손가락 커서 모양 Y축 위치 업데이트~ 
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
	//배틀메뉴 이미지 렌더링~
	_battleMenuImage->render(getMemDC(), x, y);
	//배틀매뉴 커서 이미지 렌더링
	_battleMenuCursorImage->render(getMemDC(), x 
		- _battleMenuCursorImage->getWidth() , y + _handPositionY);

}