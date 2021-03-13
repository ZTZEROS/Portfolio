#include "stdafx.h"
#include "title.h"


title::title()
{
}


title::~title()
{
}

HRESULT title::init(void)
{
	title_bg = new image;
	title_ban1_0 = new image;
	title_ban1_1 = new image;
	title_ban2_0 = new image;
	title_ban2_1 = new image;

	IMAGEMANAGER->addImage("title_bg", "UI_IMAGE/title/bg3.bmp", 1024, 768, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("title_ban1_0", "UI_IMAGE/title/ban1_0.bmp", 200, 70, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("title_ban1_1", "UI_IMAGE/title/ban1_1.bmp", 448, 135, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("title_ban2_0", "UI_IMAGE/title/ban2_0.bmp", 200, 70, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("title_ban2_1", "UI_IMAGE/title/ban2_1.bmp", 448, 135, true, RGB(255, 0, 255));


	title_bg = IMAGEMANAGER->findImage("title_bg");
	title_ban1_0 = IMAGEMANAGER->findImage("title_ban1_0");
	title_ban1_1 = IMAGEMANAGER->findImage("title_ban1_1");
	title_ban2_0 = IMAGEMANAGER->findImage("title_ban2_0");
	title_ban2_1 = IMAGEMANAGER->findImage("title_ban2_1");

	iSelected = 0;

	
	STREAMMANAGER->startStream("opening.wmv");
	SOUNDMANAGER->play("titleBgm", 0.7f);

	menuStart = false;

	return S_OK;
}

void title::update(void)
{

	if (STREAMMANAGER->getisplay())
	{
		STREAMMANAGER->closePlayEndStream();
	}

	if (KEYMANAGER->isOnceKeyDown(VK_UP) || KEYMANAGER->isOnceKeyDown(VK_DOWN))
	{
		iSelected++;
		if (iSelected >= 2)
		{
			iSelected = 0;
		}
		SOUNDMANAGER->play("menuMove", 1.0f);
	}
	if (KEYMANAGER->isOnceKeyDown('X'))
	{
		SOUNDMANAGER->play("menuSelect", 1.0f);

		if (STREAMMANAGER->getisplay())
		{
			STREAMMANAGER->closeStream();
		}
		else
		{
			if (iSelected == 1)
			{
				SCENEMANAGER->changeScene("mapTool");
				SOUNDMANAGER->stop("titleBgm");
			}
			if (iSelected == 0)
			{
				SCENEMANAGER->changeScene("townScene");
				SOUNDMANAGER->play("townBgm", 0.5f);
				SOUNDMANAGER->stop("titleBgm");
			}
		}
	}
}

void title::release(void)
{

}

void title::render(void)
{
	if (!STREAMMANAGER->getisplay())
	{
		title_bg->render(getMemDC(), 0, 0);

		if (iSelected == 0)
		{
			title_ban2_0->render(getMemDC(), 559, 669);
			title_ban1_1->render(getMemDC(), 445, 533);

		}
		else
		{
			title_ban1_0->render(getMemDC(), 559, 580);
			title_ban2_1->render(getMemDC(), 446, 620);
		}
	}
}
