#pragma once
#include "../gameNode.h"
#include "../animation.h"

enum ANIMATIONPROGRESS
{
	STANDBY,
	START,
	COMPLETE
};

class blackCircle :public gameNode
{
private:
	image* _blackCircleImage;
	animation* _blackCircleAni;
	ANIMATIONPROGRESS _aniProgress;
	bool _animationOn;
private:
	
public:
	HRESULT init();
	void startAni(bool reverse);
	void release(void);
	void update(void);
	void render(int x, int y);
public:
	bool getAnimationOn(void) { return _animationOn; }
	ANIMATIONPROGRESS getAniProgress(void) { return _aniProgress; }
	image* getblackImage(void) { return _blackCircleImage; }
	blackCircle();
	~blackCircle();
};