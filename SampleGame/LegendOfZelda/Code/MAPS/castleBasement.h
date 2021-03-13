#pragma once
#include "maps.h"

class castleBasement : public maps
{
private:

public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);

	castleBasement();
	~castleBasement();
};