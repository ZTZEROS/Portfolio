#pragma once
#include "singletonBase.h"

class explosionManager :public singletonBase<explosionManager>
{
private:
	RECT _rc;
	HDC _hdc;

public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);

	void setExplosion(HDC hdc, int x, int y, int width, int height);

	explosionManager();
	~explosionManager();
};

