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
	//Ŀ�� �̹��� ���
	_cursorImage = IMAGEMANAGER->addImage("cursor", "UI_IMAGE/cursorImage.bmp", 125, 137, true, MAGENTA);
	//Ŀ�� ��Ʈ ���
	_cursorRectImage = IMAGEMANAGER->addImage("cursorRect", "UI_IMAGE/cursorRect.bmp", TILEWIDTH, TILEHEIGHT, true, MAGENTA);
	_currentCursorTile = PointMake(15, 15); //ó�� ����Ÿ����ġ�� (0,0)
	_moveToArrivalTile = _currentCursorTile; //���� Ÿ�� ��ġ�� ���� Ÿ�� ��ġ�� �ʱ�ȭ
	_cursorCoordinate.x = (_currentCursorTile.x - _currentCursorTile.y) * TILEWIDTH * 0.5 + BACKBUFFERSIZEX * 0.5; //Ŀ�� ���� X��ǥ��ġ�� Ŀ�� ����Ÿ�� ���� ���Ѵ�.
	_cursorCoordinate.y = (_currentCursorTile.x + _currentCursorTile.y) * TILEHEIGHT * 0.5 + (TILEHEIGHT / 2) + BACKBUFFERSIZEY/3;//Ŀ�� ���� Y��ǥ��ġ�� Ŀ�� ����Ÿ�� ���� ���Ѵ�.
	_arrivalCursorCoordinate = _cursorCoordinate; //������ ��ǥ�� ���� ��ǥ�� �ʱ�ȭ
	_cursorDirection = CURSOR_STEADY; //Ŀ�� ���� ������ ���ʾƷ��� �ʱ�ȭ (���� ���ʾƷ��� ������ ����)


	return S_OK;
}
void cursor::release()
{

}
void cursor::update()
{
	cursorDirection(); //Ŀ�� ���� ���⼳��
	cursorMoving(); // Ŀ�� ����Ű�� �����̴� �Լ�

}
void cursor::render(int tileAltitude)
{
	//Ŀ�� ��Ʈ�� �׷��ش�
	_cursorRectImage->render(getMemDC(),
		_cursorCoordinate.x - (_cursorRectImage->getWidth() / 2),
		_cursorCoordinate.y - (_cursorRectImage->getHeight() / 2) - (tileAltitude));
		
	//Ŀ���̹����� �׷��ش�
	_cursorImage->render(getMemDC(),
		_cursorCoordinate.x - (_cursorImage->getWidth() / 2),
		_cursorCoordinate.y - _cursorImage->getHeight() - (tileAltitude));

}

void cursor::cursorDirection()
{
	//���� Ÿ����ġ�� ���� ��ǥ���� �����ش�
	_currentCursorTile.x = (_cursorCoordinate.y - BACKBUFFERSIZEY/3 + (_cursorCoordinate.x - BACKBUFFERSIZEX * 0.5) * 0.5) / TILEHEIGHT;//���� X��ǥ�� ���� Ÿ����ġ�� ���ϴ� ����
	_currentCursorTile.y = (_cursorCoordinate.y - BACKBUFFERSIZEY/3 - (_cursorCoordinate.x - BACKBUFFERSIZEX * 0.5) * 0.5) / TILEHEIGHT;//���� Y��ǥ�� ���� Ÿ����ġ�� ���ϴ� ����
	if (_moveToArrivalTile.x == _currentCursorTile.x &&
		_moveToArrivalTile.y == _currentCursorTile.y &&
		_arrivalCursorCoordinate.x == _cursorCoordinate.x &&
		_arrivalCursorCoordinate.y == _cursorCoordinate.y) _cursorDirection = CURSOR_STEADY;
    // vk_up ���� ��
	if (KEYMANAGER->isStayKeyDown(VK_UP))
	{
		//���࿡ ����Ÿ��X �� 0���� ũ�ٸ�
		if (_currentCursorTile.x > 0)
		{
			//���� Ŀ�� ������ �������� �ƴ϶��
			if (_cursorDirection != CURSOR_LEFT_UP)
			{
				//Ŀ���� XY��ǥ�� ���� Ŀ��Ÿ����ġ���� �����ش�.
				_cursorCoordinate.x = (_currentCursorTile.x - _currentCursorTile.y) * TILEWIDTH * 0.5 + BACKBUFFERSIZEX * 0.5;    //���� XŸ����ġ�� ���� ��ǥ�� ���ϴ� ����
				_cursorCoordinate.y = (_currentCursorTile.x + _currentCursorTile.y) * TILEHEIGHT * 0.5 + (TILEHEIGHT / 2) + BACKBUFFERSIZEY/3; //���� YŸ����ġ�� ���� ��ǥ�� ���ϴ� ����
			}
			_cursorDirection = CURSOR_LEFT_UP;	// Ŀ�� ������ ���� ���� ���ش� 
			_moveToArrivalTile.y = _currentCursorTile.y;     // ������Ÿ��Y ��ġ�� ���� Ÿ��Y ��ġ�� ����
			_moveToArrivalTile.x = _currentCursorTile.x - 1; //������ Ÿ��X ��ġ�� ���� Ÿ��X ��ġ���� ���̳ʽ� 1ĭ
		}
	}
	// vk_left  ���ʾƷ�
	if (KEYMANAGER->isStayKeyDown(VK_LEFT))
	{
		//���� ����YŸ���� 19���� �۴ٸ�
		if (_currentCursorTile.y < 19)
		{
			//���� Ŀ�� ������ ���ʾƷ��� �ƴ϶��
			if (_cursorDirection != CURSOR_LEFT_DOWN)
			{
				//Ŀ���� XY��ǥ�� ���� Ŀ��Ÿ����ġ���� �����ش�.
				_cursorCoordinate.x = (_currentCursorTile.x - _currentCursorTile.y) * TILEWIDTH * 0.5 + BACKBUFFERSIZEX * 0.5;		//���� XŸ����ġ�� ���� ��ǥ�� ���ϴ� ����
				_cursorCoordinate.y = (_currentCursorTile.x + _currentCursorTile.y) * TILEHEIGHT * 0.5 + (TILEHEIGHT / 2) + BACKBUFFERSIZEY/3;	//���� YŸ����ġ�� ���� ��ǥ�� ���ϴ� ����
			}
			_cursorDirection = CURSOR_LEFT_DOWN;			 //Ŀ�� ���� �Ʒ� ���� ����
			_moveToArrivalTile.x = _currentCursorTile.x;	 // ������Ÿ��X ��ġ�� ���� Ÿ��X ��ġ�� ����
			_moveToArrivalTile.y = _currentCursorTile.y + 1; //������ Ÿ��Y ��ġ�� ���� Ÿ��Y ��ġ���� �÷��� 1ĭ
		}
	}
	// vk_right ������ ��
	if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
	{
		//���� ����YŸ���� 0���� ũ�ٸ�
		if (_currentCursorTile.y > 0)
		{
			//���� Ŀ�� ������ ���������� �ƴ϶��
			if (_cursorDirection != CURSOR_RIGHT_UP)
			{
				//Ŀ���� XY��ǥ�� ���� Ŀ��Ÿ����ġ���� �����ش�.
				_cursorCoordinate.x = (_currentCursorTile.x - _currentCursorTile.y) * TILEWIDTH * 0.5 + BACKBUFFERSIZEX * 0.5;		//���� XŸ����ġ�� ���� ��ǥ�� ���ϴ� ����
				_cursorCoordinate.y = (_currentCursorTile.x + _currentCursorTile.y) * TILEHEIGHT * 0.5 + (TILEHEIGHT / 2) + BACKBUFFERSIZEY/3;	//���� YŸ����ġ�� ���� ��ǥ�� ���ϴ� ����
			}
			_cursorDirection = CURSOR_RIGHT_UP;				//Ŀ�� �������� ���� ����
			_moveToArrivalTile.x = _currentCursorTile.x;	// ������Ÿ��X ��ġ�� ���� Ÿ��X ��ġ�� ����
			_moveToArrivalTile.y = _currentCursorTile.y - 1;//������ Ÿ��Y ��ġ�� ���� Ÿ��Y ��ġ���� ���̳ʽ� 1ĭ
		}
	}

	// vk_down ������ �Ʒ�
	if (KEYMANAGER->isStayKeyDown(VK_DOWN))
	{
		//���� ����YŸ���� 19���� �۴ٸ�
		if (_currentCursorTile.x < 19)
		{
			//���� Ŀ�� ������ �����ʾƷ��� �ƴ϶��
			if (_cursorDirection != CURSOR_RIGHT_DOWN)
			{
				//Ŀ���� XY��ǥ�� ���� Ŀ��Ÿ����ġ���� �����ش�.
				_cursorCoordinate.x = (_currentCursorTile.x - _currentCursorTile.y) * TILEWIDTH * 0.5 + BACKBUFFERSIZEX * 0.5;		//���� XŸ����ġ�� ���� ��ǥ�� ���ϴ� ����
				_cursorCoordinate.y = (_currentCursorTile.x + _currentCursorTile.y) * TILEHEIGHT * 0.5 + (TILEHEIGHT / 2) + BACKBUFFERSIZEY/3;	//���� YŸ����ġ�� ���� ��ǥ�� ���ϴ� ����
			}
			_cursorDirection = CURSOR_RIGHT_DOWN;			//Ŀ�� ������ �Ʒ� ���⼳��
			_moveToArrivalTile.y = _currentCursorTile.y;	// ������Ÿ��Y ��ġ�� ���� Ÿ��Y ��ġ�� ����
			_moveToArrivalTile.x = _currentCursorTile.x + 1;//������ Ÿ��X ��ġ�� ���� Ÿ��X ��ġ���� �÷��� 1ĭ
		}
	}

	//������ Ÿ�Ϸκ��� ������ ��ǥ�� ���Ѵ�.
	_arrivalCursorCoordinate.x = (_moveToArrivalTile.x - _moveToArrivalTile.y) * TILEWIDTH * 0.5 + BACKBUFFERSIZEX * 0.5;		//������X��ǥ�� ������Ÿ�Ͽ��� ���ϴ°���
	_arrivalCursorCoordinate.y = (_moveToArrivalTile.x + _moveToArrivalTile.y) * TILEHEIGHT * 0.5 + (TILEHEIGHT / 2) + BACKBUFFERSIZEY/3;	//������Y��ǥ�� ������Ÿ�Ͽ��� ���ϴ°���
}
void cursor::cursorMoving()
{
	//��ǥ ��ǥ������ ���� ��ǥ��ġ�� ����
	//��ǥ Ÿ�Ϲ�ȣ�� ���� Ÿ�Ϲ�ȣ�� ���ٸ���
	// ����~~~~
	if (_moveToArrivalTile.x == _currentCursorTile.x &&
		_moveToArrivalTile.y == _currentCursorTile.y &&
		_arrivalCursorCoordinate.x == _cursorCoordinate.x &&
		_arrivalCursorCoordinate.y == _cursorCoordinate.y) return;
	//���⿡ ���� Ŀ�� ������ ����ġ��
	switch (_cursorDirection)
	{
	case CURSOR_LEFT_UP:
		_cursorCoordinate.x -= TILEWIDTH / 32;  //���� Ŀ�� X��ǥ���� Ÿ�ϰ���ũ�⿡�� 32�� �������� ���ش�
		_cursorCoordinate.y -= TILEHEIGHT / 32; //���� Ŀ�� Y��ǥ���� Ÿ�ϼ���ũ�⿡�� 32�� �������� ���ش�.
		break;
	case CURSOR_LEFT_DOWN:
		_cursorCoordinate.x -= TILEWIDTH / 32; //���� Ŀ�� X��ǥ���� Ÿ�ϰ���ũ�⿡�� 32�� �������� ���ش�
		_cursorCoordinate.y += TILEHEIGHT / 32;//���� Ŀ�� Y��ǥ���� Ÿ�ϼ���ũ�⿡�� 32�� �������� �����ش�.
		break;
	case CURSOR_RIGHT_UP:
		_cursorCoordinate.x += TILEWIDTH / 32;  //���� Ŀ�� X��ǥ���� Ÿ�ϰ���ũ�⿡�� 32�� �������� �����ش�
		_cursorCoordinate.y -= TILEHEIGHT / 32;	//���� Ŀ�� Y��ǥ���� Ÿ�ϼ���ũ�⿡�� 32�� �������� ���ش�
		break;
	case CURSOR_RIGHT_DOWN:
		_cursorCoordinate.x += TILEWIDTH / 32;  //���� Ŀ�� X��ǥ���� Ÿ�ϰ���ũ�⿡�� 32�� �������� �����ش�
		_cursorCoordinate.y += TILEHEIGHT / 32;	//���� Ŀ�� Y��ǥ���� Ÿ�ϼ���ũ�⿡�� 32�� �������� �����ش�
		break;

	}
}