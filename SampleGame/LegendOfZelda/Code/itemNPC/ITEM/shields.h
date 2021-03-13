#pragma once
#include "items.h"

class shields : public items
{
public:
	HRESULT shields::init(int x, int y);

	shields();
	~shields();
};

