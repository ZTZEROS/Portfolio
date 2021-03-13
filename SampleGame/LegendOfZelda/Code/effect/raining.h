#pragma once
#include "../gameNode.h"
#include "../animation.h"
class raining : public gameNode
{
private:
	image* _rainingImage;
	image* _alphaImage;
	animation* _aniRain;
	BYTE _alphaVal;
	int _alphaCount;
public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);
	raining();
	~raining();
};

