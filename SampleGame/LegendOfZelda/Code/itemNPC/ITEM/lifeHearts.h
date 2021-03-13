#pragma once
#include "items.h"

class lifeHearts : public items
{
public:
	HRESULT lifeHearts::init(int x, int y);

	lifeHearts();
	~lifeHearts();
};

class halflifeHearts : public items
{
public:
	HRESULT halflifeHearts::init(int x, int y);

	halflifeHearts();
	~halflifeHearts();
};
