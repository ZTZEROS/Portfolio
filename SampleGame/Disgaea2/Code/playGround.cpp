#include "stdafx.h"
#include "playGround.h"


playGround::playGround()
{
}


playGround::~playGround()
{
}


HRESULT playGround::init(void)
{
	gameNode::init(true);

	//==========================//
	//		Character Images	//
	//==========================//

	IMAGEMANAGER->addFrameImage("Prier_is_IDLE", "image/character/Prier/Prier_is_IDLE.bmp", 600, 640, 6, 4, 1, MAGENTA, 0);
	IMAGEMANAGER->addFrameImage("Prier_is_WALKING", "image/character/Prier/Prier_is_WALKING.bmp", 600, 640, 6, 4, 1, MAGENTA, 0);
	IMAGEMANAGER->addFrameImage("Prier_is_ATTACKING", "image/character/Prier/Prier_is_ATTACKING.bmp", 560, 640, 4, 4, 1, MAGENTA, 0);
	IMAGEMANAGER->addFrameImage("Prier_is_BEINGATTACKED", "image/character/Prier/Prier_is_BEINGATTACKED.bmp", 100, 640, 1, 4, 1, MAGENTA, 0);
	IMAGEMANAGER->addImage("PrierFace", "image/character/Prier/PrierFace.bmp", 133, 133, 1, MAGENTA, 0);
	IMAGEMANAGER->addImage("PrierFace2", "image/character/Prier/PrierFace2.bmp", 128, 544, 1, MAGENTA, 0);

	//IMAGEMANAGER->addFrameImage("Flonne_is_IDLE", "image/character/Flonne/Flonne_is_IDLE.bmp", 600, 640, 6, 4 1, MAGENTA, 0);
	//IMAGEMANAGER->addFrameImage("Flonne_is_WALKING", "image/character/Flonne/Flonne_is_WALKING.bmp", 600, 640, 6, 4 1, MAGENTA, 0);
	//IMAGEMANAGER->addFrameImage("Flonne_is_ATTACKING", "image/character/Flonne/Flonne_is_ATTACKING.bmp", 560, 640, 4, 4, 1, MAGENTA, 0);
	//IMAGEMANAGER->addFrameImage("Flonne_is_BEINGATTACKED", "image/character/Flonne/Flonne_is_BEINGATTACKED.bmp", 100, 640, 1, 4, 1, MAGENTA, 0);
	//IMAGEMANAGER->addImage("FlonneFace", "image/character/Flonne/FlonneFace.bmp", 133, 133, 1, MAGENTA, 0);
	//IMAGEMANAGER->addImage("FlonneFace2", "image/character/Flonne/FlonneFace2.bmp", 128, 544, 1, MAGENTA, 0);

	//IMAGEMANAGER->addFrameImage("Etna_is_IDLE", "image/character/Etna/Etna_is_IDLE.bmp", 600, 640, 6, 4 1, MAGENTA, 0);
	//IMAGEMANAGER->addFrameImage("Etna_is_WALKING", "image/character/Etna/Etna_is_WALKING.bmp", 600, 640, 6, 4 1, MAGENTA, 0);
	//IMAGEMANAGER->addFrameImage("Etna_is_ATTACKING", "image/character/Etna/Etna_is_ATTACKING.bmp", 560, 640, 4, 4, 1, MAGENTA, 0);
	//IMAGEMANAGER->addFrameImage("Etna_is_BEINGATTACKED", "image/character/Etna/Etna_is_BEINGATTACKEDv", 100, 640, 1, 4, 1, MAGENTA, 0);
	//IMAGEMANAGER->addImage("EtnaFace", "image/character/Etna/EtnaFace.bmp", 133, 133, 1, MAGENTA, 0);
	//IMAGEMANAGER->addImage("EtnaFace2", "image/character/Etna/EtnaFace2.bmp", 128, 544, 1, MAGENTA, 0);

	IMAGEMANAGER->addFrameImage("Adell_is_IDLE", "image/character/Adell/Adell_is_IDLE.bmp", 600, 640, 6, 4, 1, MAGENTA, 0);
	IMAGEMANAGER->addFrameImage("Adell_is_WALKING", "image/character/Adell/Adell_is_WALKING.bmp", 522, 624, 6, 4, 1, MAGENTA, 0);
	IMAGEMANAGER->addFrameImage("Adell_is_ATTACKING", "image/character/Adell/Adell_is_ATTACKING.bmp", 480, 632, 4, 4, 1, MAGENTA, 0);
	IMAGEMANAGER->addFrameImage("Adell_is_BEINGATTACKED", "image/character/Adell/Adell_is_BEINGATTACKED.bmp", 95, 668, 1, 4, 1, MAGENTA, 0);
	IMAGEMANAGER->addImage("AdellFace", "image/character/Adell/AdellFace.bmp", 133, 133, 1, MAGENTA, 0);
	IMAGEMANAGER->addImage("AdellFace2", "image/character/Adell/AdellFace2.bmp", 128, 544, 1, MAGENTA, 0);

	IMAGEMANAGER->addFrameImage("ArmorKnightFemale_is_IDLE", "image/character/ArmorKnightFemale/ArmorKnightFemale_is_IDLE.bmp", 600, 640, 6, 4, 1, MAGENTA, 0);
	IMAGEMANAGER->addFrameImage("ArmorKnightFemale_is_WALKING", "image/character/ArmorKnightFemale/ArmorKnightFemale_is_WALKING.bmp", 600, 640, 6, 4, 1, MAGENTA, 0);
	IMAGEMANAGER->addFrameImage("ArmorKnightFemale_is_ATTACKING", "image/character/ArmorKnightFemale/ArmorKnightFemale_is_ATTACKING.bmp", 600, 640, 4, 4, 1, MAGENTA, 0);
	IMAGEMANAGER->addFrameImage("ArmorKnightFemale_is_BEINGATTACKED", "image/character/ArmorKnightFemale/ArmorKnightFemale_is_BEINGATTACKED.bmp", 100, 640, 1, 4, 1, MAGENTA, 0);
	IMAGEMANAGER->addImage("ArmorKnightFemaleFace", "image/character/ArmorKnightFemale/ArmorKnightFemaleFace.bmp", 133, 133, 1, MAGENTA, 0);
	IMAGEMANAGER->addImage("ArmorKnightFemaleFace2", "image/character/ArmorKnightFemale/ArmorKnightFemaleFace2.bmp", 128, 544, 1, MAGENTA, 0);
	
	IMAGEMANAGER->addFrameImage("Axel_is_IDLE", "image/character/Axel/Axel_is_IDLE.bmp", 774, 644, 6, 4, 1, MAGENTA, 0);
	IMAGEMANAGER->addFrameImage("Axel_is_WALKING", "image/character/Axel/Axel_is_WALKING.bmp", 630, 720, 6, 4, 1, MAGENTA, 0);
	IMAGEMANAGER->addFrameImage("Axel_is_ATTACKING", "image/character/Axel/Axel_is_ATTACKING.bmp", 488, 628, 4, 4, 1, MAGENTA, 0);
	IMAGEMANAGER->addFrameImage("Axel_is_BEINGATTACKED", "image/character/Axel/Axel_is_BEINGATTACKED.bmp", 95, 668, 1, 4, 1, MAGENTA, 0);
	IMAGEMANAGER->addImage("AxelFace", "image/character/Axel/AxelFace.bmp", 133, 133, 1, MAGENTA, 0);
	IMAGEMANAGER->addImage("AxelFace2", "image/character/Axel/AxelFace2.bmp", 128, 544, 1, MAGENTA, 0);
	
	IMAGEMANAGER->addFrameImage("Fenrich_is_IDLE", "image/character/Fenrich/Fenrich_is_IDLE.bmp", 840, 568, 6, 4, 1, MAGENTA, 0);
	IMAGEMANAGER->addFrameImage("Fenrich_is_WALKING", "image/character/Fenrich/Fenrich_is_WALKING.bmp", 576, 608, 6, 4, 1, MAGENTA, 0);
	IMAGEMANAGER->addFrameImage("Fenrich_is_ATTACKING", "image/character/Fenrich/Fenrich_is_ATTACKING.bmp", 540, 632, 4, 4, 1, MAGENTA, 0);
	IMAGEMANAGER->addFrameImage("Fenrich_is_BEINGATTACKED", "image/character/Fenrich/Fenrich_is_BEINGATTACKED.bmp", 97, 556, 1, 4, 1, MAGENTA, 0);
	IMAGEMANAGER->addImage("FenrichFace", "image/character/Fenrich/FenrichFace.bmp", 133, 133, 1, MAGENTA, 0);
	IMAGEMANAGER->addImage("FenrichFace2", "image/character/Fenrich/FenrichFace2.bmp", 128, 544, 1, MAGENTA, 0);
	
	IMAGEMANAGER->addFrameImage("HealerMale_is_IDLE", "image/character/HealerMale/HealerMale_is_IDLE.bmp", 600, 640, 6, 4, 1, MAGENTA, 0);
	IMAGEMANAGER->addFrameImage("HealerMale_is_WALKING", "image/character/HealerMale/HealerMale_is_WALKING.bmp", 600, 640, 6, 4, 1, MAGENTA, 0);
	IMAGEMANAGER->addFrameImage("HealerMale_is_ATTACKING", "image/character/HealerMale/HealerMale_is_ATTACKING.bmp", 600, 640, 4, 4, 1, MAGENTA, 0);
	IMAGEMANAGER->addFrameImage("HealerMale_is_BEINGATTACKED", "image/character/HealerMale/HealerMale_is_BEINGATTACKED.bmp", 116, 584, 1, 4, 1, MAGENTA, 0);
	IMAGEMANAGER->addImage("HealerMaleFace", "image/character/HealerMale/HealerMaleFace.bmp", 133, 133, 1, MAGENTA, 0);
	IMAGEMANAGER->addImage("HealerMaleFace2", "image/character/HealerMale/HealerMaleFace2.bmp", 128, 544, 1, MAGENTA, 0);

	IMAGEMANAGER->addImage("shadow", "image/character/shadow.bmp", 150, 50, true, MAGENTA, true);


	//============================================================//
	//              ItemImage				     //
	//==========================================================//
	
	//아이템 아이콘 등록
	//방어구
	IMAGEMANAGER->addImage("necklessType", "image/itemImage/pendentImage.bmp", 48, 48, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("armorType", "image/itemImage/armorImage.bmp", 48, 48, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("shoseType", "image/itemImage/shoseImage.bmp", 48, 48, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("ringType", "image/itemImage/ringImage.bmp", 48, 48, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("necklessType", "image/itemImage/pendentImage.bmp", 48, 48, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("chuingType", "image/itemImage/chugumImage.bmp", 48, 48, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("gumType", "image/itemImage/gumImage.bmp", 48, 48, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("candyType", "image/itemImage/candyImage.bmp", 48, 48, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("canType", "image/itemImage/drinkImage.bmp", 48, 48, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("mersleType", "image/itemImage/mersleImage.bmp", 48, 48, true, RGB(255, 0, 255));

	//아이템 아이콘 등록
	//무기
	IMAGEMANAGER->addImage("axeType", "image/itemImage/axeImage.bmp", 48, 48, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("bowType", "image/itemImage/bowImage.bmp", 48, 48, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("gunType", "image/itemImage/gunImage.bmp", 48, 48, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("guntletType", "image/itemImage/guntletImage.bmp", 48, 48, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("nikuType", "image/itemImage/nikuImage.bmp", 48, 48, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("spearType", "image/itemImage/spearImage.bmp", 48, 48, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("steapType", "image/itemImage/steapImage.bmp", 48, 48, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("swordType", "image/itemImage/swordImage.bmp", 48, 48, true, RGB(255, 0, 255));

	//아무것도 없을 때 이미지 등록
	IMAGEMANAGER->addImage("backitem", "image/itemImage/candyImage.bmp", 0, 0, false, RGB(255, 0, 255));

	//커서 이미지 등록
	IMAGEMANAGER->addImage("cursorImage", "UI_IMAGE/handCursor.bmp", 30, 30, true, RGB(255, 0, 255));

	//롤 이미지 등록
	IMAGEMANAGER->addImage("progressbarimage", "UI_IMAGE/exCursor.bmp", 15, 50, true, RGB(255, 0, 255));

	IMAGEMANAGER->addImage("shopimage", "UI_IMAGE/shop.bmp", 600, 450, false, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("hellslot", "UI_IMAGE/hellslot.bmp", 300, 50, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("itemInfoImage", "UI_IMAGE/iteminfo.bmp", 350, 200, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("itemex", "UI_IMAGE/itemEx.bmp", 940, 50, true, RGB(255, 0, 255));
	
	//배경이미지 등록
	IMAGEMANAGER->addImage("storebg1", "image/storebg.bmp", 1024, 768, false, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("storebg2", "image/storebg2.bmp", 1024, 768, false, RGB(255, 0, 255));

	//=========================================================//
	//              sound									   //
	//=========================================================//

	SOUNDMANAGER->addSound("titleBgm", "sound/bgm/title.mp3", true, true);
	SOUNDMANAGER->addSound("battleBgm", "sound/bgm/battle.mp3", true, true);
	SOUNDMANAGER->addSound("townBgm", "sound/bgm/store.mp3", true, true);

	SOUNDMANAGER->addSound("menuMove", "sound/effect/menuMove.mp3", false, false);
	SOUNDMANAGER->addSound("menuSelect", "sound/effect/menuSelect.mp3", false, false);
	SOUNDMANAGER->addSound("menucancel", "sound/effect/menucancel.mp3", false, false);

	_shake1, _shake2 = false;
	_isInStore = false;
	_shakeX, _shakeY = 0;
	_shakeCount = 0;

	_townmap = new townmap;
	_townmap->init();

	//==========================//
	//		SceneInit			//
	//==========================//

	SCENEMANAGER->addScene("mapTool", new maptool);
	SCENEMANAGER->addScene("battle", new battleScene);
	SCENEMANAGER->addScene("titleScene", new title);
	SCENEMANAGER->addScene("townScene", _townmap);
	SCENEMANAGER->changeScene("titleScene");


	_logoImg = IMAGEMANAGER->addImage("logo","image/logo.bmp", 150, 88, true,MAGENTA);

	return S_OK;
}


void playGround::release(void)
{
	gameNode::release();

}


void playGround::update(void)
{
	gameNode::update( );



	_isInStore = _townmap->getOpenStore();


	KEYANIMANAGER->update();
	SCENEMANAGER->update();
	SOUNDMANAGER->update();

	//카메라 줌인, 줌아웃 연산
	if (CAMERA->getZoomIn())
	{	
		if (CAMERA->getMaxValue() <= 200)
		{
			CAMERA->setWheelValue(CAMERA->getWheelValue() + 5);
			CAMERA->setMaxValue(CAMERA->getMaxValue() + 5);
		}
		else if (CAMERA->getMaxValue() > 200)
		{
			CAMERA->setMaxValue(0);
			CAMERA->setZoomIn(false);
		}
	}

	if (CAMERA->getZoomOut())
	{	
		if (CAMERA->getMaxValue() <= 200)
		{
			CAMERA->setWheelValue(CAMERA->getWheelValue() - 5);
			CAMERA->setMaxValue(CAMERA->getMaxValue() + 5);
		}
		else if (CAMERA->getMaxValue() > 200)
		{
			CAMERA->setMaxValue(0);
			CAMERA->setZoomOut(false);
		}
	}
	

	if (KEYMANAGER->isOnceKeyDown('A')) CAMERA->setShake(true, 50);

	//카메라 진동 연산
	if (!CAMERA->getShake())
	{
		_shakeX = _shakeY = 0;
		_shake1 = _shake2 = false;
		_shakeCount = 0;
	}
	else _shakeCount++;

	if (CAMERA->getShakeTime() == _shakeCount) CAMERA->setShake(false);

	if (CAMERA->getShake() && _shakeX <= 0)
	{
		_shake1 = true;
		_shake2 = false;
	}

	if (CAMERA->getShake() && _shakeX >= 5)
	{
		_shake2 = true;
		_shake1 = false;
	}

	if (_shake1)
	{
		_shakeX++;
		_shakeY--;
	}
	if (_shake2)
	{
		_shakeX--;
		_shakeY++;
	}

}


//그려주는 것은 여기에!
void playGround::render(void)
{
	if (!STREAMMANAGER->getisplay())
	{
		if(_isInStore)
		{
			PatBlt(getIMemDC(), 0, 0, WINSIZEX, WINSIZEY, BLACKNESS);
		}
		else
		{
			if (CAMERA->getWheelValue() >= 0)PatBlt(getMemDC(), CAMERA->getCameraX(), CAMERA->getCameraY(), WINSIZEX, WINSIZEY, BLACKNESS);
			else if (CAMERA->getWheelValue() < 0)PatBlt(getMemDC(), CAMERA->getCameraX() + CAMERA->getWheelValue() * 2, CAMERA->getCameraY() + CAMERA->getWheelValue() * 2, WINSIZEX - CAMERA->getWheelValue() * 4, WINSIZEY - CAMERA->getWheelValue() * 4, BLACKNESS);
		}		
		//****************************************************//

		SCENEMANAGER->render();

		if (CAMERA->getWheelValue() > 0)
		{
			SetStretchBltMode(getMemDC(), COLORONCOLOR);
			StretchBlt(getMemDC(), CAMERA->getCameraX() - CAMERA->getWheelValue() / 2, CAMERA->getCameraY() - CAMERA->getWheelValue() / 2, WINSIZEX + CAMERA->getWheelValue(), WINSIZEY + CAMERA->getWheelValue(),
				getMemDC(), CAMERA->getCameraX(), CAMERA->getCameraY(), WINSIZEX, WINSIZEY, SRCCOPY);
		}

		else if (CAMERA->getWheelValue() < 0)
		{
			SetStretchBltMode(getMemDC(), COLORONCOLOR);
			StretchBlt(getMemDC(), CAMERA->getCameraX(), CAMERA->getCameraY(), WINSIZEX, WINSIZEY,
				getMemDC(), CAMERA->getCameraX() + CAMERA->getWheelValue() / 2, CAMERA->getCameraY() + CAMERA->getWheelValue() / 2, WINSIZEX - CAMERA->getWheelValue(), WINSIZEY - CAMERA->getWheelValue(), SRCCOPY);
		}
	
		//****************************************************//
		SetTextColor(getMemDC(), MAGENTA);

		_logoImg->render(getMemDC(), CAMERA->getCameraX()+WINSIZEX-150, CAMERA->getCameraY());

		TIMEMANAGER->render(getMemDC(), CAMERA->getCameraX(), CAMERA->getCameraY());
		//백버퍼에 옮겨 그려준다
		if(_isInStore)
		{
			this->getInvenBackBuffer()->render(getHDC(), 0, 0);
		}

		else
		{
			this->getBackBuffer()->render(getHDC(), 0, 0, CAMERA->getCameraX() + _shakeX, CAMERA->getCameraY() + _shakeY,
				WINSIZEX, WINSIZEY);
		}
	}
}

