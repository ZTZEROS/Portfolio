#pragma once
#include "maps.h"

class bossRoom :public maps
{
private:

public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);
	

	bossRoom();
	~bossRoom();
};