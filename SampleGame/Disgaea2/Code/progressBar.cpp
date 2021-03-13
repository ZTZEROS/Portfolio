#include "stdafx.h"
#include "progressBar.h"


progressBar::progressBar()
{
}


progressBar::~progressBar()
{
}

HRESULT progressBar::playerInit(char* currentGaugeImageName, char* maxGaugeImageName, int x, int y)
{
	_x = x;
	_y = y;
	
	_currentGaugeImageName = currentGaugeImageName;
	_maxGaugeImageName = maxGaugeImageName;
	_rcProgress = RectMakeCenter(x, y, PROGRESSBARWIDTH, PROGRESSBARHEIGHT);

	//프로그레스 바 이미지
	_progressBarTop = IMAGEMANAGER->addImage(currentGaugeImageName, "UI_IMAGE/playerProgressBar.bmp", x, y, PROGRESSBARWIDTH, PROGRESSBARHEIGHT, true, RGB(255, 0, 255));
	_progressBarBottom = IMAGEMANAGER->addImage(maxGaugeImageName, "UI_IMAGE/progressBarFrame.bmp", x, y, PROGRESSBARWIDTH, PROGRESSBARHEIGHT, true, RGB(255, 0, 255));

	_width = _progressBarTop->getWidth();

	return S_OK;
}

HRESULT progressBar::enemyInit(char* currentGaugeImageName, char* maxGaugeImageName, int x, int y)
{
	_x = x;
	_y = y;

	_rcProgress = RectMake(x, y, PROGRESSBARWIDTH, PROGRESSBARHEIGHT);

	//프로그레스 바 이미지
	_progressBarTop = IMAGEMANAGER->addImage(currentGaugeImageName, "UI_IMAGE/enemyProgressBar.bmp", x, y, PROGRESSBARWIDTH, PROGRESSBARHEIGHT, true, RGB(255, 0, 255));
	_progressBarBottom = IMAGEMANAGER->addImage(maxGaugeImageName, "UI_IMAGE/progressBarFrame.bmp", x, y, PROGRESSBARWIDTH, PROGRESSBARHEIGHT, true, RGB(255, 0, 255));

	_width = _progressBarTop->getWidth();

	return S_OK;
}

HRESULT progressBar::init(char* btnUpImageKeyName, char* btnDownImageKeyName,
	int x, int y, int width, int height)
{
	_x = x;
	_y = y;

	_rcProgress = RectMakeCenter(x, y, width, height);

	char strUpName[128];
	char strDownName[128];

	ZeroMemory(strUpName, sizeof(strUpName));
	ZeroMemory(strDownName, sizeof(strDownName));

	sprintf(strUpName, "%s.bmp", btnUpImageKeyName);
	sprintf(strDownName, "%s.bmp", btnDownImageKeyName);

	_progressBarBottom = IMAGEMANAGER->addImage(btnDownImageKeyName, strDownName, x, y,
		width, height, true, RGB(255, 0, 255));
	_progressBarTop = IMAGEMANAGER->addImage(btnUpImageKeyName, strUpName, x, y,
		width, height, true, RGB(255, 0, 255));

	_width = _progressBarBottom->getWidth();

	return S_OK;
}

HRESULT progressBar::overviewPanelLevelInit(char* currentGaugeImageName, char* maxGaugeImageName, int x, int y)
{
	_x = x;
	_y = y;
	_currentGaugeImageName = currentGaugeImageName;
	_maxGaugeImageName = maxGaugeImageName;
	_rcProgress = RectMakeCenter(x, y, OVERVIEWPROGRESSWIDTH, OVERVIEWPROGRESSHEIGHT);

	//프로그레스 바 이미지
	_progressBarTop = IMAGEMANAGER->addImage(currentGaugeImageName, "UI_IMAGE/overviewLvlProgressBar.bmp", x, y, OVERVIEWPROGRESSWIDTH, OVERVIEWPROGRESSHEIGHT, true, RGB(255, 0, 255));
	_progressBarBottom = IMAGEMANAGER->addImage(maxGaugeImageName, "UI_IMAGE/panelGaugeFrame.bmp", x, y, OVERVIEWPROGRESSWIDTH, OVERVIEWPROGRESSHEIGHT, true, RGB(255, 0, 255));

	_width = _progressBarTop->getWidth();

	return S_OK;
}

HRESULT progressBar::overviewPanelHPInit(char* currentGaugeImageName, char* maxGaugeImageName, int x, int y)
{
	_x = x;
	_y = y;
	_currentGaugeImageName = currentGaugeImageName;
	_maxGaugeImageName = maxGaugeImageName;
	_rcProgress = RectMakeCenter(x, y, OVERVIEWPROGRESSWIDTH, OVERVIEWPROGRESSHEIGHT);

	//프로그레스 바 이미지
	_progressBarTop = IMAGEMANAGER->addImage(currentGaugeImageName, "UI_IMAGE/playerProgressBar.bmp", x, y, OVERVIEWPROGRESSWIDTH, OVERVIEWPROGRESSHEIGHT, true, RGB(255, 0, 255));
	_progressBarBottom = IMAGEMANAGER->addImage(maxGaugeImageName, "UI_IMAGE/panelGaugeFrame.bmp", x, y, OVERVIEWPROGRESSWIDTH, OVERVIEWPROGRESSHEIGHT, true, RGB(255, 0, 255));

	_width = _progressBarTop->getWidth();

	return S_OK;
}

HRESULT progressBar::overviewPanelSPInit(char* currentGaugeImageName, char* maxGaugeImageName, int x, int y)
{
	_x = x;
	_y = y;
	_currentGaugeImageName = currentGaugeImageName;
	_maxGaugeImageName = maxGaugeImageName;
	_rcProgress = RectMakeCenter(x, y, OVERVIEWPROGRESSWIDTH, OVERVIEWPROGRESSHEIGHT);

	//프로그레스 바 이미지
	_progressBarTop = IMAGEMANAGER->addImage(currentGaugeImageName, "UI_IMAGE/playerProgressBar.bmp", x, y, OVERVIEWPROGRESSWIDTH, OVERVIEWPROGRESSHEIGHT, true, RGB(255, 0, 255));
	_progressBarBottom = IMAGEMANAGER->addImage(maxGaugeImageName, "UI_IMAGE/panelGaugeFrame.bmp", x, y, OVERVIEWPROGRESSWIDTH, OVERVIEWPROGRESSHEIGHT, true, RGB(255, 0, 255));

	_width = _progressBarTop->getWidth();

	return S_OK;
}

void progressBar::release(void)
{

}

void progressBar::update(int x, int y, float currentGauge, float maxGauge)
{
	_width = (currentGauge / maxGauge) * _progressBarBottom->getWidth();
	_rcProgress = RectMakeCenter(
		x, 
		y,
		_progressBarBottom->getWidth(), 
		_progressBarTop->getHeight());
}

void progressBar::render(void)
{
	/*
	IMAGEMANAGER->render(_maxGaugeImageName, getMemDC(),
	_rcProgress.left,
	_rcProgress.top,
	0, 0,
	_progressBarBottom->getWidth(), _progressBarBottom->getHeight());

	IMAGEMANAGER->render(_currentGaugeImageName, getMemDC(),
	_rcProgress.left,
	_rcProgress.top ,
	0, 0,
	_width, _progressBarTop->getHeight());
	*/
	
	_progressBarBottom->render(getMemDC(), _rcProgress.left, _rcProgress.top, 0, 0,
		_progressBarBottom->getWidth(),
		_progressBarBottom->getHeight());

	_progressBarTop->render(getMemDC(), _rcProgress.left, _rcProgress.top, 0, 0,
		_width,
		_progressBarTop->getHeight());
	
}

//프로그레스 바(게이지)
void progressBar::setGauge(float currentGauge, float maxGauge)
{
	_width = (currentGauge / maxGauge) * _progressBarBottom->getWidth();
}