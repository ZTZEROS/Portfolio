#pragma once
#include "objects.h"

class chestBox : public objects
{
private:

public:
	HRESULT init(int x, int y);
	void release();
	void update();
	void render();
public:

	chestBox();
	~chestBox();
};

