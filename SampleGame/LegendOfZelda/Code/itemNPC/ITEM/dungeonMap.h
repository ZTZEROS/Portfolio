#pragma once
#include "items.h"

class dungeonMap : public items
{
public:
	HRESULT dungeonMap::init(int x, int y);

	dungeonMap();
	~dungeonMap();
};

