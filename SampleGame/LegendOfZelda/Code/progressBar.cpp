#include "stdafx.h"
#include "progressBar.h"


progressBar::progressBar()
{
}


progressBar::~progressBar()
{
}

HRESULT progressBar::init(int x, int y, int width, int height)
{
	_x = x;
	_y = y;

	_rcProgress = RectMake(x, y, width, height);

	//프로그레스 바 이미지
	_progressBarTop = IMAGEMANAGER->addImage("frontBar", "hpBarTop.bmp", x, y, width, height, true, RGB(255, 0, 255));
	_progressBarBottom = IMAGEMANAGER->addImage("backBar", "hpBarBottom.bmp", x, y, width, height, true, RGB(255, 0, 255));

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

void progressBar::release(void)
{

}

void progressBar::update(void)
{
	_rcProgress = RectMakeCenter(_x, _y,
		_progressBarBottom->getWidth(), _progressBarTop->getHeight());
}

void progressBar::render(void)
{
	/*
	IMAGEMANAGER->render("backBar", getMemDC(),
	_rcProgress.left + _progressBarBottom->getWidth() / 2,
	_y + _progressBarBottom->getHeight() / 2,
	0, 0,
	_progressBarBottom->getWidth(), _progressBarBottom->getHeight());

	IMAGEMANAGER->render("frontBar", getMemDC(),
	_rcProgress.left + _progressBarTop->getWidth() / 2,
	_y + _progressBarTop->getHeight() / 2,
	0, 0,
	_width, _progressBarTop->getHeight());
	*/

	_progressBarBottom->render(getMemDC(), _rcProgress.left, _y, 0, 0,
		_progressBarBottom->getWidth(),
		_progressBarBottom->getHeight());

	_progressBarTop->render(getMemDC(), _rcProgress.left, _y, 0, 0,
		_width,
		_progressBarTop->getHeight());

}

//프로그레스 바(게이지)
void progressBar::setGauge(float currentGauge, float maxGauge)
{
	_width = (currentGauge / maxGauge) * _progressBarBottom->getWidth();
}