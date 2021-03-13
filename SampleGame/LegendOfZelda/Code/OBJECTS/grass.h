#pragma once
#include "objects.h"


class grass : public objects
{
private:
	
public:
	HRESULT init(int x, int y);
	void release();
	void update();
	void render();
public:

	grass();
	~grass();
};

