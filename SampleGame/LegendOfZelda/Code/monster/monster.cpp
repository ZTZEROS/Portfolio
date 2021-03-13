#include "stdafx.h"
#include "../stdafx.h"
#include "monster.h"

monster::monster()
{
}


monster::~monster()
{
}

HRESULT monster::init(void)
{
	return S_OK;
}

HRESULT monster::init(const char* imageName, POINT position, float HP, int pattern)
{
	_currentFrameX = _currentFrameY = 0;
	_count = 0;
	_currentHP = _maxHP = 100.0f;
	_centerX = position.x;
	_centerY = position.y;
	_image = (*IMAGEMANAGER->findImage(imageName));
	
	VirtualBody = RectMakeCenter(_centerX, _centerY, _image.getFrameWidth()-20, _image.getFrameHeight()-10);

	_sword = RectMakeCenter(_centerX, _centerY, _image.getFrameWidth(), _image.getFrameHeight());

	_x = position.x;
	_y = position.y;
	
	
	_rcview = false;
	return S_OK;
}

void monster::release(void)
{

}

void monster::update(void)
{
	_count++;

	if (_count % 16 == 0)
	{
		_image.setFrameX(_image.getFrameX() + 1);
		_currentFrameX++;
		if (_currentFrameX >= _image.getMaxFrameX() + 1) _currentFrameX = 0;
	}

	
	if (KEYMANAGER->isOnceKeyDown('T'))
	{
		if (!_rcview)_rcview = 1;
		else _rcview = 0;
	}

	direction();

	VirtualBody = RectMakeCenter(_centerX, _centerY, _image.getFrameWidth()-20, _image.getFrameHeight()-10);
}

void monster::render(void)
{
	Rectangle(getMemDC(), VirtualBody.left, VirtualBody.top, VirtualBody.right, VirtualBody.bottom);
	
	_image.frameRender(getMemDC(), VirtualBody.left, VirtualBody.top, _currentFrameX, _currentFrameY);

}

void monster::direction(void)
{
	switch (_mondirection)
	{
	case MONUP:
		_image = (*IMAGEMANAGER->findImage("greensoldierattackclimb"));
		if (VirtualBody.top >= 0) _centerY -= ENEMYSPEED;
		break;
	case MONDOWN:
		_image = (*IMAGEMANAGER->findImage("greensoldierattackdescend"));
		if (VirtualBody.bottom <= WINSIZEY) _centerY += ENEMYSPEED;
		break;
	case MONLEFT:
		_image = (*IMAGEMANAGER->findImage("greensoldierattackleft"));
		if (VirtualBody.left >= 0) _centerX -= ENEMYSPEED;
		break;
	case MONRIGHT:
		_image = (*IMAGEMANAGER->findImage("greensoldierattackright"));
		if(VirtualBody.right <= WINSIZEX) _centerX += ENEMYSPEED;
		break;
	}
	

	if (RND->getInt(100) > 97) _mondirection = RND->getInt(4);
}