#pragma once
#include "../ITEM/items.h"

class armours : public items
{
public:
	HRESULT armours::init(int x, int y);

	armours();
	~armours();
};

