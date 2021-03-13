#pragma once
#include "items.h"

class swords : public items
{
public:
	HRESULT swords::init(int x, int y);

	swords();
	~swords();
};

