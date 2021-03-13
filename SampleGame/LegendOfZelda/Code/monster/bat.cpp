#include "stdafx.h"
#include "../stdafx.h"
#include "bat.h"

bat::bat()
{
}


bat::~bat()
{
}


HRESULT bat::init(void)
{
	_rndDirectionCount = RND->getFromIntTo(0, 4);
	_rndNum = _timecount = 0;

	return S_OK;
}

void bat::release(void)
{

}

//업데이트 때문에 프레임이 안돌아감
//void greensoldier::update(void)
//{
//	move();
//}

void bat::move(void)
{
	_moveCount++;

	//direction2();

	/*
	if (_image != IMAGEMANAGER->findImage("greenattacksoldier"))
	{
	switch (_pattern)
	{
	case 0:
	{
	if (_moveCount < 50)
	{
	_image->setX(_image->getX() - 3);
	_image->setY(_image->getY() + 5);

	_rc.left -= 3;
	_rc.right -= 3;
	_rc.top += 5;
	_rc.bottom += 5;
	}

	else
	{
	_image->setX(_image->getX() + 7);

	_rc.left += 7;
	_rc.right += 7;
	}
	break;
	}
	///////////////
	case 1:
	{
	if (_moveCount < 100)
	{
	_image->setX(_image->getX() - 3);
	_image->setY(_image->getY() - 3);

	_rc.left -= 3;
	_rc.right -= 3;
	_rc.top -= 3;
	_rc.bottom -= 3;
	}

	else
	{
	_image->setX(_image->getX() + 7);

	_rc.left += 7;
	_rc.right += 7;
	}
	break;
	}

	case 2:
	{
	if (_moveCount < 150)
	{

	_image->setX(_image->getX() - 7);

	_rc.left -= 7;
	_rc.right -= 7;
	}
	else
	{
	_image->setX(_image->getX() - 7);

	_rc.left -= 7;
	_rc.right -= 7;
	}
	break;
	}

	default:
	break;
	}

	}
	*/

}