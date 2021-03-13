#pragma once
#include "gameNode.h"

#define PROGRESSBARWIDTH 100
#define PROGRESSBARHEIGHT 10

#define OVERVIEWPROGRESSWIDTH 220
#define OVERVIEWPROGRESSHEIGHT 15

class progressBar : public gameNode
{
private:
	RECT _rcProgress;
	int _x, _y;
	float _width;
	char* _currentGaugeImageName;
	char* _maxGaugeImageName;
	image* _progressBarTop;
	image* _progressBarBottom;
	
public:
	HRESULT playerInit(char* currentGaugeImageName, char* maxGaugeImageName,int x, int y);
	HRESULT enemyInit(char* currentGaugeImageName, char* maxGaugeImageName, int x, int y);
	HRESULT overviewPanelLevelInit(char* currentGaugeImageName, char* maxGaugeImageName, int x, int y);
	HRESULT overviewPanelHPInit(char* currentGaugeImageName, char* maxGaugeImageName, int x, int y);
	HRESULT overviewPanelSPInit(char* currentGaugeImageName, char* maxGaugeImageName, int x, int y);
	HRESULT init(char* btnUpImageKeyName, char* btnDownImageKeyName,
		int x, int y, int width, int height);

	void release(void);
	void update(int x, int y, float currentGauge, float maxGauge);
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

