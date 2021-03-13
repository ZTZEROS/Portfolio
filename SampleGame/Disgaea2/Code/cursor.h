#pragma once
#include "gameNode.h"
#include "tileNode.h"
//커서 방향 이넘문
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
//			이미지 변수			 //
//*******************************//
	image* _cursorImage;
	image* _cursorRectImage;
private:
	//*******************************//
	//			이넘 변수			 //
	//*******************************//
	CURSOR_DIRECTION _cursorDirection;
private:
	//*******************************//
	//			커서 변수			 //
	//*******************************//
	POINT _moveToArrivalTile; //목적지 타일 위치
	POINT _currentCursorTile; //현재 커서 타일 위치
	POINT _cursorCoordinate; // 커서의 XY 좌표
	POINT _arrivalCursorCoordinate; // 목적지 XY 좌표
public:
	HRESULT init();
	void release();
	void update();
	void render(int tileAltitude);
public:
	void cursorDirection(); //커서 가는 방향설정
	void cursorMoving(); // 커서 방향키로 움직이는 함수
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

