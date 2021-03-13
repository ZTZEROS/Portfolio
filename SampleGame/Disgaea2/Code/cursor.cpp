#include "stdafx.h"
#include "cursor.h"


cursor::cursor()
{
}


cursor::~cursor()
{
}

HRESULT cursor::init()
{
	//커서 이미지 등록
	_cursorImage = IMAGEMANAGER->addImage("cursor", "UI_IMAGE/cursorImage.bmp", 125, 137, true, MAGENTA);
	//커서 렉트 등록
	_cursorRectImage = IMAGEMANAGER->addImage("cursorRect", "UI_IMAGE/cursorRect.bmp", TILEWIDTH, TILEHEIGHT, true, MAGENTA);
	_currentCursorTile = PointMake(15, 15); //처음 현재타일위치는 (0,0)
	_moveToArrivalTile = _currentCursorTile; //목적 타일 위치는 현재 타일 위치로 초기화
	_cursorCoordinate.x = (_currentCursorTile.x - _currentCursorTile.y) * TILEWIDTH * 0.5 + BACKBUFFERSIZEX * 0.5; //커서 현재 X좌표위치는 커서 현재타일 에서 구한다.
	_cursorCoordinate.y = (_currentCursorTile.x + _currentCursorTile.y) * TILEHEIGHT * 0.5 + (TILEHEIGHT / 2) + BACKBUFFERSIZEY/3;//커서 현재 Y좌표위치는 커서 현재타일 에서 구한다.
	_arrivalCursorCoordinate = _cursorCoordinate; //목적지 좌표는 현재 좌표로 초기화
	_cursorDirection = CURSOR_STEADY; //커서 현재 방향은 왼쪽아래로 초기화 (딱히 왼쪽아래일 이유는 없음)


	return S_OK;
}
void cursor::release()
{

}
void cursor::update()
{
	cursorDirection(); //커서 가는 방향설정
	cursorMoving(); // 커서 방향키로 움직이는 함수

}
void cursor::render(int tileAltitude)
{
	//커서 렉트를 그려준다
	_cursorRectImage->render(getMemDC(),
		_cursorCoordinate.x - (_cursorRectImage->getWidth() / 2),
		_cursorCoordinate.y - (_cursorRectImage->getHeight() / 2) - (tileAltitude));
		
	//커서이미지를 그려준다
	_cursorImage->render(getMemDC(),
		_cursorCoordinate.x - (_cursorImage->getWidth() / 2),
		_cursorCoordinate.y - _cursorImage->getHeight() - (tileAltitude));

}

void cursor::cursorDirection()
{
	//현재 타일위치를 현재 좌표에서 구해준다
	_currentCursorTile.x = (_cursorCoordinate.y - BACKBUFFERSIZEY/3 + (_cursorCoordinate.x - BACKBUFFERSIZEX * 0.5) * 0.5) / TILEHEIGHT;//현재 X좌표로 현재 타일위치를 구하는 공식
	_currentCursorTile.y = (_cursorCoordinate.y - BACKBUFFERSIZEY/3 - (_cursorCoordinate.x - BACKBUFFERSIZEX * 0.5) * 0.5) / TILEHEIGHT;//현재 Y좌표로 현재 타일위치를 구하는 공식
	if (_moveToArrivalTile.x == _currentCursorTile.x &&
		_moveToArrivalTile.y == _currentCursorTile.y &&
		_arrivalCursorCoordinate.x == _cursorCoordinate.x &&
		_arrivalCursorCoordinate.y == _cursorCoordinate.y) _cursorDirection = CURSOR_STEADY;
    // vk_up 왼쪽 윗
	if (KEYMANAGER->isStayKeyDown(VK_UP))
	{
		//만약에 현재타일X 가 0보다 크다면
		if (_currentCursorTile.x > 0)
		{
			//만약 커서 방향이 왼쪽위가 아니라면
			if (_cursorDirection != CURSOR_LEFT_UP)
			{
				//커서의 XY좌표를 현재 커서타일위치에서 구해준다.
				_cursorCoordinate.x = (_currentCursorTile.x - _currentCursorTile.y) * TILEWIDTH * 0.5 + BACKBUFFERSIZEX * 0.5;    //현재 X타일위치로 현재 좌표를 구하는 공식
				_cursorCoordinate.y = (_currentCursorTile.x + _currentCursorTile.y) * TILEHEIGHT * 0.5 + (TILEHEIGHT / 2) + BACKBUFFERSIZEY/3; //현재 Y타일위치로 현재 좌표를 구하는 공식
			}
			_cursorDirection = CURSOR_LEFT_UP;	// 커서 방향을 왼쪽 위로 해준다 
			_moveToArrivalTile.y = _currentCursorTile.y;     // 목적지타일Y 위치는 현재 타일Y 위치로 보정
			_moveToArrivalTile.x = _currentCursorTile.x - 1; //목적지 타일X 위치는 현재 타일X 위치에서 마이너스 1칸
		}
	}
	// vk_left  왼쪽아래
	if (KEYMANAGER->isStayKeyDown(VK_LEFT))
	{
		//만약 현재Y타일이 19보다 작다면
		if (_currentCursorTile.y < 19)
		{
			//만약 커서 방향이 왼쪽아래가 아니라면
			if (_cursorDirection != CURSOR_LEFT_DOWN)
			{
				//커서의 XY좌표를 현재 커서타일위치에서 구해준다.
				_cursorCoordinate.x = (_currentCursorTile.x - _currentCursorTile.y) * TILEWIDTH * 0.5 + BACKBUFFERSIZEX * 0.5;		//현재 X타일위치로 현재 좌표를 구하는 공식
				_cursorCoordinate.y = (_currentCursorTile.x + _currentCursorTile.y) * TILEHEIGHT * 0.5 + (TILEHEIGHT / 2) + BACKBUFFERSIZEY/3;	//현재 Y타일위치로 현재 좌표를 구하는 공식
			}
			_cursorDirection = CURSOR_LEFT_DOWN;			 //커서 왼쪽 아래 방향 설정
			_moveToArrivalTile.x = _currentCursorTile.x;	 // 목적지타일X 위치는 현재 타일X 위치로 보정
			_moveToArrivalTile.y = _currentCursorTile.y + 1; //목적지 타일Y 위치는 현재 타일Y 위치에서 플러스 1칸
		}
	}
	// vk_right 오른쪽 위
	if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
	{
		//만약 현재Y타일이 0보다 크다면
		if (_currentCursorTile.y > 0)
		{
			//만약 커서 방향이 오른쪽위가 아니라면
			if (_cursorDirection != CURSOR_RIGHT_UP)
			{
				//커서의 XY좌표를 현재 커서타일위치에서 구해준다.
				_cursorCoordinate.x = (_currentCursorTile.x - _currentCursorTile.y) * TILEWIDTH * 0.5 + BACKBUFFERSIZEX * 0.5;		//현재 X타일위치로 현재 좌표를 구하는 공식
				_cursorCoordinate.y = (_currentCursorTile.x + _currentCursorTile.y) * TILEHEIGHT * 0.5 + (TILEHEIGHT / 2) + BACKBUFFERSIZEY/3;	//현재 Y타일위치로 현재 좌표를 구하는 공식
			}
			_cursorDirection = CURSOR_RIGHT_UP;				//커서 오른쪽위 방향 설정
			_moveToArrivalTile.x = _currentCursorTile.x;	// 목적지타일X 위치는 현재 타일X 위치로 보정
			_moveToArrivalTile.y = _currentCursorTile.y - 1;//목적지 타일Y 위치는 현재 타일Y 위치에서 마이너스 1칸
		}
	}

	// vk_down 오른쪽 아래
	if (KEYMANAGER->isStayKeyDown(VK_DOWN))
	{
		//만약 현재Y타일이 19보다 작다면
		if (_currentCursorTile.x < 19)
		{
			//만약 커서 방향이 오른쪽아래가 아니라면
			if (_cursorDirection != CURSOR_RIGHT_DOWN)
			{
				//커서의 XY좌표를 현재 커서타일위치에서 구해준다.
				_cursorCoordinate.x = (_currentCursorTile.x - _currentCursorTile.y) * TILEWIDTH * 0.5 + BACKBUFFERSIZEX * 0.5;		//현재 X타일위치로 현재 좌표를 구하는 공식
				_cursorCoordinate.y = (_currentCursorTile.x + _currentCursorTile.y) * TILEHEIGHT * 0.5 + (TILEHEIGHT / 2) + BACKBUFFERSIZEY/3;	//현재 Y타일위치로 현재 좌표를 구하는 공식
			}
			_cursorDirection = CURSOR_RIGHT_DOWN;			//커서 오른쪽 아래 방향설정
			_moveToArrivalTile.y = _currentCursorTile.y;	// 목적지타일Y 위치는 현재 타일Y 위치로 보정
			_moveToArrivalTile.x = _currentCursorTile.x + 1;//목적지 타일X 위치는 현재 타일X 위치에서 플러스 1칸
		}
	}

	//목적지 타일로부터 목적지 좌표를 구한다.
	_arrivalCursorCoordinate.x = (_moveToArrivalTile.x - _moveToArrivalTile.y) * TILEWIDTH * 0.5 + BACKBUFFERSIZEX * 0.5;		//목적지X좌표는 목적지타일에서 구하는공식
	_arrivalCursorCoordinate.y = (_moveToArrivalTile.x + _moveToArrivalTile.y) * TILEHEIGHT * 0.5 + (TILEHEIGHT / 2) + BACKBUFFERSIZEY/3;	//목적지Y좌표는 목적지타일에서 구하는공식
}
void cursor::cursorMoving()
{
	//목표 좌표지점과 현재 좌표위치가 같고
	//목표 타일번호와 현재 타일번호가 같다면은
	// 리턴~~~~
	if (_moveToArrivalTile.x == _currentCursorTile.x &&
		_moveToArrivalTile.y == _currentCursorTile.y &&
		_arrivalCursorCoordinate.x == _cursorCoordinate.x &&
		_arrivalCursorCoordinate.y == _cursorCoordinate.y) return;
	//방향에 따른 커서 움직임 스위치문
	switch (_cursorDirection)
	{
	case CURSOR_LEFT_UP:
		_cursorCoordinate.x -= TILEWIDTH / 32;  //현재 커서 X좌표에서 타일가로크기에서 32를 나눈값을 빼준다
		_cursorCoordinate.y -= TILEHEIGHT / 32; //현재 커서 Y좌표에서 타일세로크기에서 32를 나눈값을 빼준다.
		break;
	case CURSOR_LEFT_DOWN:
		_cursorCoordinate.x -= TILEWIDTH / 32; //현재 커서 X좌표에서 타일가로크기에서 32를 나눈값을 빼준다
		_cursorCoordinate.y += TILEHEIGHT / 32;//현재 커서 Y좌표에서 타일세로크기에서 32를 나눈값을 더해준다.
		break;
	case CURSOR_RIGHT_UP:
		_cursorCoordinate.x += TILEWIDTH / 32;  //현재 커서 X좌표에서 타일가로크기에서 32를 나눈값을 더해준다
		_cursorCoordinate.y -= TILEHEIGHT / 32;	//현재 커서 Y좌표에서 타일세로크기에서 32를 나눈값을 빼준다
		break;
	case CURSOR_RIGHT_DOWN:
		_cursorCoordinate.x += TILEWIDTH / 32;  //현재 커서 X좌표에서 타일가로크기에서 32를 나눈값을 더해준다
		_cursorCoordinate.y += TILEHEIGHT / 32;	//현재 커서 Y좌표에서 타일세로크기에서 32를 나눈값을 더해준다
		break;

	}
}