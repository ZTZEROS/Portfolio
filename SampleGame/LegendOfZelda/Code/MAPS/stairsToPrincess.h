#pragma once
#include "maps.h"

class stairsToPrincess : public maps
{
private:

public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);

	stairsToPrincess();
	~stairsToPrincess();
};