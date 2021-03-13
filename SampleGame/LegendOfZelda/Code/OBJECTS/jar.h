#pragma once
#include "objects.h"
class jar :public objects
{
private:

public:
	HRESULT init(int x, int y);
	void release();
	void update();
	void render();
public:

	jar();
	~jar();
};

