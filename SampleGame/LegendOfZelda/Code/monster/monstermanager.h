#pragma once
#include "../gameNode.h"
#include "greensoldier.h"
#include "bluesoldier.h"
#include "boss.h"
#include "mouse.h"
#include "snake.h"
#include "spider.h"
#include "bat.h"
#include "../link/link.h"
#include <vector>

class link;

class monstermanager : public gameNode
{
private:
	typedef vector<monster*> vMonster;
	typedef vector<monster*>::iterator viMonster;

private:
	vMonster _vGreensoldier;
	viMonster _viGreensoldier;
	vMonster _vBluesoldier;
	viMonster _viBluesoldier;
	vMonster _vBoss;
	viMonster _viBoss;
	vMonster _vMouse;
	viMonster _viMouse;
	vMonster _vSnake;
	viMonster _viSnake;
	vMonster _vSpider;
	viMonster _viSpider;
	vMonster _vBat;
	viMonster _viBat;

	int _whereIsLink;

	RECT Temp;

	link* _link;

public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);
	
	//배치
	void setgreensoldier(int pattern);
	void setbluesoldier(int pattern);
	void setboss(int pattern);
	void setmouse(int pattern);
	void setsnake(int pattern);
	void setspider(int pattern);
	void setbat(int pattern);
	
	//공격
	void hitgreensoldier(void);
	void hitbluesoldier(void);
	void hitboss(void);
	void hitmouse(void);
	void hitsnake(void);
	void hitspider(void);
	void hitbat(void);
	
	void removeMonster(int arrNum);

	int GetWhereIsLink() { return _whereIsLink; }

	
	vector<monster*> getVGreensoldier(void) { return _vGreensoldier; }
	vector<monster*>::iterator getVIGreensoldier(void) { return _viGreensoldier; }
	vector<monster*> getVBluesoldier(void) { return _vBluesoldier; }
	vector<monster*>::iterator getVIBluesoldier(void) { return _viBluesoldier; }
	vector<monster*> getVBoss(void) { return _vBoss; }
	vector<monster*>::iterator getVIBoss(void) { return _viBoss; }
	vector<monster*> getVMouse(void) { return _vMouse; }
	vector<monster*>::iterator getVIMouse(void) { return _viMouse; }
	vector<monster*> getVSnake(void) { return _vSnake; }
	vector<monster*>::iterator getVISnake(void) { return _viSnake; }
	vector<monster*> getVSpider(void) { return _vSpider; }
	vector<monster*>::iterator getVISpider(void) { return _viSpider; }
	vector<monster*> getVBat(void) { return _vBat; }
	vector<monster*>::iterator getVIBat(void) { return _viBat; }


	void setlinkMemoryAddressLink(link* link) { _link = link; }
	void setWhereIsLink(int whereIsLink) { _whereIsLink = whereIsLink; }
	void EraseGreensoldier(int arrayNumber) { _vGreensoldier.erase(_vGreensoldier.begin()+arrayNumber); }

	monstermanager();
	~monstermanager();
};