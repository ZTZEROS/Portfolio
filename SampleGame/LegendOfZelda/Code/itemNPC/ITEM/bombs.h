#pragma once
#include "items.h"

class bombs : public items
{
private:

public:
	HRESULT bombs::init(int x, int y);

	bombs();
	~bombs();
};

