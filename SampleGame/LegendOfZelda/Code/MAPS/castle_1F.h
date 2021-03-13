#pragma once
#include "maps.h"

class castle_1F : public maps
{
private:

public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);

	castle_1F();
	~castle_1F();
};