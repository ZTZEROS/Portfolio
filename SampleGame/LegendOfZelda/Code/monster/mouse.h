#pragma once
#include "monster.h"

class mouse : public monster
{
private:


public:
	void update(void);
	void move(void);

	mouse();
	~mouse();
};

