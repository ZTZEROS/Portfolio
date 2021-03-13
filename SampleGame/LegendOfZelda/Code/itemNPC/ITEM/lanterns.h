#pragma once
#include "items.h"
class lanterns : public items
{
public:
	HRESULT lanterns::init(int x, int y);

	lanterns();
	~lanterns();
};

