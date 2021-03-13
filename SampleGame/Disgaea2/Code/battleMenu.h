#pragma once
#include "gameNode.h"

enum MENUS
{
	NONE,
	MOVE,
	ATTACK,
	DEFEND,
	ITEM,
	EQUIPMENT
};

class battleMenu :public gameNode
{
private:
	//*******************************//
	//			�̹��� ����			 //
	//*******************************//
	image* _battleMenuImage;
	image* _battleMenuCursorImage;
private:
	int _menus;
	int _handPositionY;
public:
	HRESULT init(void);
	void release(void);
	void update();
	void render(int x, int y );
public:
	//*******************************//
	//			���� ����				 //
	//*******************************//
	int getMenus(void) { return _menus; }
	void setMenus(int menus) { _menus = menus; }
	battleMenu();
	~battleMenu();
};

