#pragma once
#include "monster.h"

class boss : public monster
{
private:
	int _rndDirectionCount;
	int _rndNum;
	int _countNum;
	int _timecount;

public:
	HRESULT init(void);
	void release(void);
	//virtual void update(void);
	virtual void move(void);

	boss();
	~boss();
};
