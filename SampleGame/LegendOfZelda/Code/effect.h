#pragma once
#include "gameNode.h"

//속도를 위한 전방선언
class animation;

class effect : public gameNode
{
protected:
	int			_x;					//이펙트 좌표위치 left
	int			_y;					//이펙트 좌표위치 top
		
	image*		_effectImage;		//이펙트 이미지
	animation*	_effectAnimation;	//이펙트 애니메이션
	BOOL		_isRunning;			//이펙트 활성화 여부
	float		_elapsedTime;		//이펙트 경과시간


public:
	effect();
	virtual ~effect();

	virtual HRESULT init(image* effectImage, int frameW, int frameH, int fps, float elapsedTime);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	void startEffect(int x, int y);
	virtual void killEffect(void);

	BOOL getIsRunning(void) { return _isRunning; }
};

