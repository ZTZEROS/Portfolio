#pragma once
#include "monster.h"

class bat : public monster
{
private:
	int _rndDirectionCount;
	int _rndNum;
	int _countNum;
	int _timecount;


public:
	HRESULT init(void);
	void release(void);
	//void update(void);
	void move(void);

	bat();
	~bat();
};
