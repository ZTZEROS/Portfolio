#include "stdafx.h"
#include "../stdafx.h"
#include "monstermanager.h"

monstermanager::monstermanager()
{
}


monstermanager::~monstermanager()
{
}

HRESULT monstermanager::init(void)
{
	setgreensoldier(1.0f);
	setbat(1.0f);

	return S_OK;
}

void monstermanager::release(void)
{

}

void monstermanager::update(void)
{
	

	for (_viGreensoldier = _vGreensoldier.begin(); _viGreensoldier != _vGreensoldier.end(); ++_viGreensoldier)
	{
		(*_viGreensoldier)->update();
	}

	//for (_viBluesoldier = _vBluesoldier.begin(); _viBluesoldier != _vBluesoldier.end(); ++_viBluesoldier)
	//{
	//	(*_viBluesoldier)->update();
	//}
	//for (_viBoss = _vBoss.begin(); _viBoss != _vBoss.end(); ++_viBoss)
	//{
	//	(*_viBoss)->update();
	//}
	//for (_viMouse = _vMouse.begin(); _viMouse != _vMouse.end(); ++_viMouse)
	//{
	//	(*_viMouse)->update();
	//}
	//for (_viSnake = _vSnake.begin(); _viSnake != _vSnake.end(); ++_viSnake)
	//{
	//	(*_viSnake)->update();
	//}
	//for (_viSpider = _vSpider.begin(); _viSpider != _vSpider.end(); ++_viSpider)
	//{
	//	(*_viSpider)->update();
	//}
	
	for (_viBat = _vBat.begin(); _viBat != _vBat.end(); ++_viBat)
	{
		(*_viBat)->update();
	}
	for (_viBoss = _vBoss.begin(); _viBoss != _vBoss.end(); ++_viBoss)
	{
		(*_viBoss)->update();
	}
	
	//hitgreensoldier();
	//hitbluesoldier();
	//hitboss();
	//hitmouse();
	//hitsnake();
	//hitspider();
	//hitbat();
}

void monstermanager::render(void)
{
	for (_viGreensoldier = _vGreensoldier.begin(); _viGreensoldier != _vGreensoldier.end(); ++_viGreensoldier)
	{
		(*_viGreensoldier)->render();
	}

	//for (_viBluesoldier = _vBluesoldier.begin(); _viBluesoldier != _vBluesoldier.end(); ++_viBluesoldier)
	//{
	//	(*_viBluesoldier)->render();
	//}
	//for (_viBoss = _vBoss.begin(); _viBoss != _vBoss.end(); ++_viBoss)
	//{
	//	(*_viBoss)->render();
	//}
	//for (_viMouse = _vMouse.begin(); _viMouse != _vMouse.end(); ++_viMouse)
	//{
	//	(*_viMouse)->render();
	//}
	//for (_viSnake = _vSnake.begin(); _viSnake != _vSnake.end(); ++_viSnake)
	//{
	//	(*_viSnake)->render();
	//}
	//for (_viSpider = _vSpider.begin(); _viSpider != _vSpider.end(); ++_viSpider)
	//{
	//	(*_viSpider)->render();
	//}
	for (_viBat = _vBat.begin(); _viBat != _vBat.end(); ++_viBat)
	{
		(*_viBat)->render();
	}
	for (_viBoss = _vBoss.begin(); _viBoss != _vBoss.end(); ++_viBoss)
	{
		(*_viBoss)->render();
	}
}

void monstermanager::setgreensoldier(int pattern)
{	
	for (int i = 0; i < 3; i++)
	{
		for (int j = 3; j < 5; j++)
		{
			monster* greensoldiermon;
			greensoldiermon = new greensoldier;
			greensoldiermon->init("greensoldierattackdescend", PointMake(500 + 80 * j, 200 + 80 * i), 1.0f, pattern);
			_vGreensoldier.push_back(greensoldiermon);
		}
	}
}

void monstermanager::hitgreensoldier(void)
{

}


void monstermanager::setbluesoldier(int pattern)
{
	for (int i = 0; i < 1; i++)
	{
		for (int j = 0; j < 1; j++)
		{
			monster* bluesoldiermon;
			bluesoldiermon = new bluesoldier;
			bluesoldiermon->init("bluesoldierattackdescend", PointMake(150, 400), 1.0f, pattern);
			_vBluesoldier.push_back(bluesoldiermon);
		}
	}

}

void monstermanager::hitbluesoldier(void)
{

}

void monstermanager::setboss(int pattern)
{
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			monster* bossmon;
			bossmon = new boss;
			bossmon->init("ironmacewalkright", PointMake(150 + 80 * j, 400 + 80 * i), 1.0f, pattern);
			_vBoss.push_back(bossmon);
		}
	}
	for (int i = 0; i < 1; i++)
	{
		for (int j = 0; j < 1; j++)
		{
			monster* iron;
			iron = new boss;
			iron->init("iron", PointMake(150 + 80 * j, 400 + 80 * i), 1.0f, pattern);
			_vBoss.push_back(iron);
		}
	}
}

void monstermanager::hitboss(void)
{

}

void monstermanager::setmouse(int pattern)
{
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			monster* mousemon;
			mousemon = new mouse;
			mousemon->init("mousedescendstop", PointMake(150, 400), 1.0f, pattern);
			_vMouse.push_back(mousemon);
		}
	}

}

void monstermanager::hitmouse(void)
{

}

void monstermanager::setsnake(int pattern)
{
	for (int i =5; i < 6; i++)
	{
		for (int j = 5; j < 6; j++)
		{
			monster* snakemon;
			snakemon = new snake;
			snakemon->init("snakewalkright", PointMake(150, 400), 1.0f, pattern);
			_vSnake.push_back(snakemon);
		}
	}

}

void monstermanager::hitsnake(void)
{

}

void monstermanager::setspider(int pattern)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			monster* spidermon;
			spidermon = new spider;
			spidermon->init("spider", PointMake(150 + 80 * i, 400 + 80 * j), 1.0f, pattern);
			_vSpider.push_back(spidermon);
		}
	}

}

void monstermanager::hitspider(void)
{

}

void monstermanager::setbat(int pattern)
{
	for (int i = 0; i < 1; i++)
	{
		for (int j = 0; j < 1; j++)
		{
			monster* batmon;
			batmon = new bat;
			batmon->init("batmove", PointMake(150, 400), 1.0f, pattern);
			_vBat.push_back(batmon);
		}
	}

}

void monstermanager::hitbat(void)
{

}
//
//void monstermanager::minionBulletFire(void)
//{
//	for (_viMouse = _vMouse.begin(); _viMouse != _vMouse.end(); ++_viMouse)
//	{
//		if ((*_viMouse)->bulletCountFire())
//		{
//			RECT rc = (*_viMouse)->getRect();
//			_bullet->fire(rc.left + (rc.right - rc.left) / 2,
//				rc.bottom + (rc.top - rc.bottom) / 2 + 30,
//				-(PI / 2), 3.0f);
//		}
//	}
//
//}


void monstermanager::removeMonster(int arrNum)
{
	SAFE_DELETE(_vGreensoldier[arrNum]);
	_vGreensoldier.erase(_vGreensoldier.begin() + arrNum);
	SAFE_DELETE(_vBluesoldier[arrNum]);
	_vBluesoldier.erase(_vBluesoldier.begin() + arrNum);
	SAFE_DELETE(_vBoss[arrNum]);
	_vBoss.erase(_vBoss.begin() + arrNum);
	SAFE_DELETE(_vMouse[arrNum]);
	_vMouse.erase(_vMouse.begin() + arrNum);
	SAFE_DELETE(_vSnake[arrNum]);
	_vSnake.erase(_vSnake.begin() + arrNum);
	SAFE_DELETE(_vSpider[arrNum]);
	_vSpider.erase(_vSpider.begin() + arrNum);
	SAFE_DELETE(_vBat[arrNum]);
	_vBat.erase(_vBat.begin() + arrNum);
}