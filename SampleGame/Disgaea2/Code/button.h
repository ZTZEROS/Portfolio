#pragma once
#include "gameNode.h"

typedef void(*CALLBACKFUNCTION)(void);

//��ư ���´� �̳ѹ�����
enum BUTTONSTATE
{
	BUTTONSTATE_NULL,
	BUTTONSTATE_UP,
	BUTTONSTATE_DOWN
};

class button : public gameNode
{
private:
	BUTTONSTATE _state;
	const char* _imageName;
	image* _image;
	RECT _rc;
	float _x, _y;

	POINT _btnDownFramePoint;
	POINT _btnUpFramePoint;

	CALLBACKFUNCTION _callbackFunction;

public:
	HRESULT init(const char* imageName, int x, int y,
		POINT btnUpFramePoint, POINT btnDownFramePoint,
		CALLBACKFUNCTION cbFunction);

	void release(void);
	void update(void);
	void render(void);


	button();
	~button();
};

