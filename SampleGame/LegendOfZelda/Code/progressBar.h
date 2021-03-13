#pragma once
#include "gameNode.h"

class progressBar : public gameNode
{
private:
	RECT _rcProgress;
	int _x, _y;
	float _width;

	image* _progressBarTop;
	image* _progressBarBottom;

public:
	HRESULT init(int x, int y, int width, int height);
	HRESULT init(char* btnUpImageKeyName, char* btnDownImageKeyName,
		int x, int y, int width, int height);

	void release(void);
	void update(void);
	void render(void);

	//프로그레스 바(게이지)
	void setGauge(float currentGauge, float maxGauge);

	//프로그레스 바 좌표 셋팅 함수
	void setX(int x) { _x = x; }
	void setY(int y) { _y = y; }

	RECT getRect(void) { return _rcProgress; }

	progressBar();
	~progressBar();
};

