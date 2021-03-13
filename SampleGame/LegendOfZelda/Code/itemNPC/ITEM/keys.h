#pragma once
#include "items.h"
class keys : public items
{
public:
	HRESULT keys::init(int x, int y);

	keys();
	~keys();
};

class masterkeys : public items
{
public:
	HRESULT masterkeys::init(int x, int y);

	masterkeys();
	~masterkeys();
};
