#include "stdafx.h"
#include "../stdafx.h"
#include "spider.h"

spider::spider()
{
}


spider::~spider()
{
}

//void spider::update(void)
//{
//	move();
//}

void spider::move(void)
{
	_moveCount++;

	//if (_image != (*IMAGEMANAGER->findImage("spider_move")))
	//{
	//	switch (_pattern)
	//	{
	//	case 1:
	//	
	//		if (_moveCount < 50)
	//		{
	//			_image.setX(_image->getX() + 3);
	//			_image->setY(_image->getY() - 5);
	//
	//			_rc.left += 3;
	//			_rc.right += 3;
	//			_rc.top -= 5;
	//			_rc.bottom -= 5;
	//		}
	//		else if (_moveCount < 70)
	//		{
	//			_image->setX(_image->getX() + 3);
	//			_image->setY(_image->getY() + 5);
	//
	//			_rc.left += 3;
	//			_rc.right += 3;
	//			_rc.top += 5;
	//			_rc.bottom += 5;
	//		}
	//
	//		else if (_moveCount < 120)
	//		{
	//			_image->setX(_image->getX() - 1);
	//			_image->setY(_image->getY() - 1);
	//
	//			_rc.left -= 1;
	//			_rc.right -= 1;
	//			_rc.top -= 1;
	//			_rc.bottom -= 1;
	//		}
	//		else if (_moveCount < 180)
	//		{
	//			_image->setX(_image->getX() - 1);
	//			_image->setY(_image->getY() + 1);
	//
	//			_rc.left -= 1;
	//			_rc.right -= 1;
	//			_rc.top += 1;
	//			_rc.bottom += 1;
	//		}
	//		else if (_moveCount < 350)
	//		{
	//			_image->setX(_image->getX() - 1);
	//
	//			_rc.left -= 1;
	//			_rc.right -= 1;
	//		}
	//
	//		else
	//		{
	//			_image->setX(_image->getX() + 1);
	//
	//			_rc.left += 1;
	//			_rc.right += 1;
	//		}
	//		break;
	//	
	//
	//
	//	default:
	//		break;
	//	}
	//
	//}
}