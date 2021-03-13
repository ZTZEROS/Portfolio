#pragma once
#include "gameNode.h"
#include "tileNode.h"
//Ŀ�� ���� �̳ѹ�
enum CURSOR_DIRECTION
{
	CURSOR_STEADY,
	CURSOR_LEFT_UP,
	CURSOR_LEFT_DOWN,
	CURSOR_RIGHT_UP,
	CURSOR_RIGHT_DOWN
};


class cursor : public gameNode
{
//*******************************//
//			�̹��� ����			 //
//*******************************//
	image* _cursorImage;
	image* _cursorRectImage;
private:
	//*******************************//
	//			�̳� ����			 //
	//*******************************//
	CURSOR_DIRECTION _cursorDirection;
private:
	//*******************************//
	//			Ŀ�� ����			 //
	//*******************************//
	POINT _moveToArrivalTile; //������ Ÿ�� ��ġ
	POINT _currentCursorTile; //���� Ŀ�� Ÿ�� ��ġ
	POINT _cursorCoordinate; // Ŀ���� XY ��ǥ
	POINT _arrivalCursorCoordinate; // ������ XY ��ǥ
public:
	HRESULT init();
	void release();
	void update();
	void render(int tileAltitude);
public:
	void cursorDirection(); //Ŀ�� ���� ���⼳��
	void cursorMoving(); // Ŀ�� ����Ű�� �����̴� �Լ�
public:
	LONG getCursorCoordinateX(void) { return _cursorCoordinate.x; }
	LONG getCursorCoordinateY(void) { return _cursorCoordinate.y; }
	void setCursorCoordinateX(LONG cursorCoordinateX) { _cursorCoordinate.x = cursorCoordinateX; }
	void setCursorCoordinateY(LONG cursorCoordinateY) { _cursorCoordinate.y = cursorCoordinateY; }
	LONG getCurrentCursorTileX(void) { return _currentCursorTile.x; }
	LONG getCurrentCursorTileY(void) { return _currentCursorTile.y; }
	void setCurrentCursorTileX(LONG currentCursorTileX) { _currentCursorTile.x = currentCursorTileX; }
	void setCurrentCursorTileY(LONG currentCursorTileY) { _currentCursorTile.y = currentCursorTileY; }
	CURSOR_DIRECTION getCursorDirection(void) { return _cursorDirection; }
	void setCursorDirection(CURSOR_DIRECTION cursorDirection) { _cursorDirection = cursorDirection; }
	cursor();
	~cursor();
};

