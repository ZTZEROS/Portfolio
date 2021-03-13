#pragma once
#include "gameNode.h"

typedef void(*CALLBACKFUNCTION)(void);

class action : public gameNode
{
private:
	image* _image;
	float _startX, _startY;
	float _endX, _endY;

	float _angle;
	float _travelRange;
	float _worldTimeCount;
	float _time;

	bool _isMoving;

	CALLBACKFUNCTION _callbackFunction;

public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);

	void moveTo(image* image, float endX, float endY, float time);
	void moveTo(image* image, float endX, float endY, float time, CALLBACKFUNCTION cbFunction);

	void moving(void);

	bool getIsMoving(void) { return _isMoving; }


	action();
	~action();
};

