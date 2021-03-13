#pragma once
#include "maps.h"

class castle_B1F : public maps
{
private:

public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);

	castle_B1F();
	~castle_B1F();
};