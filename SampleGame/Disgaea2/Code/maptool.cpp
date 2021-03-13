#include "stdafx.h"
#include "maptool.h"


maptool::maptool()
{
}


maptool::~maptool()
{
}

HRESULT maptool::init(void)
{
	_tileRock = IMAGEMANAGER->addImage("돌타일맵", "image/maptool/돌타일.bmp", MAPTILEWIDTH+5, MAPTILEHEIGHT * 2, true, RGB(255, 0, 255));
	_tileGraass = IMAGEMANAGER->addImage("풀타일맵", "image/maptool/풀타일.bmp", MAPTILEWIDTH + 5, MAPTILEHEIGHT * 2, true, RGB(255, 0, 255));
	_tileStones = IMAGEMANAGER->addImage("자갈타일맵", "image/maptool/자갈타일.bmp", MAPTILEWIDTH + 5, MAPTILEHEIGHT * 2, true, RGB(255, 0, 255));
	_tileRockTop = IMAGEMANAGER->addImage("돌타일맵상단", "image/maptool/돌타일상단.bmp", MAPTILEWIDTH + 5, MAPTILEHEIGHT * 2, true, RGB(255, 0, 255));
	_tileGraassTop = IMAGEMANAGER->addImage("풀타일맵상단", "image/maptool/풀타일상단.bmp", MAPTILEWIDTH + 5, MAPTILEHEIGHT * 2, true, RGB(255, 0, 255));
	_tileStonesTop = IMAGEMANAGER->addImage("자갈타일맵상단", "image/maptool/자갈타일상단.bmp", MAPTILEWIDTH + 5, MAPTILEHEIGHT * 2, true, RGB(255, 0, 255));
	_tileWater = IMAGEMANAGER->addImage("물타일멥", "image/maptool/물타일.bmp", MAPTILEWIDTH , MAPTILEHEIGHT, true, RGB(255, 0, 255),true);
	_tileDeepTile = IMAGEMANAGER->addImage("마을타일맵1", "image/maptool/tile_town1.bmp", MAPTILEWIDTH + 5, MAPTILEHEIGHT * 2, true, RGB(255, 0, 255));
	_tileLightTile = IMAGEMANAGER->addImage("마을타일맵2", "image/maptool/tile_town2.bmp", MAPTILEWIDTH + 5, MAPTILEHEIGHT * 2, true, RGB(255, 0, 255));
	_tileGraveTile= IMAGEMANAGER->addImage("마을타일맵3", "image/maptool/tile_town3.bmp", MAPTILEWIDTH + 5, MAPTILEHEIGHT * 2, true, RGB(255, 0, 255));
	_startTile = IMAGEMANAGER->addImage("시작타일맵", "image/maptool/startTile.bmp", MAPTILEWIDTH + 5, MAPTILEHEIGHT, true, RGB(255, 0, 255));

	IMAGEMANAGER->addFrameImage("leftButton", "image/maptool/leftmove.bmp", 100, 100,2,1, true, RGB(255, 0, 255), false);
	IMAGEMANAGER->addFrameImage("rightButton", "image/maptool/rightmove.bmp", 100, 100, 2, 1, true, RGB(255, 0, 255), false);
	IMAGEMANAGER->addFrameImage("upButton", "image/maptool/upmove.bmp", 100, 100, 1, 2, true, RGB(255, 0, 255), false);
	IMAGEMANAGER->addFrameImage("downButton", "image/maptool/downmove.bmp", 100, 100, 1, 2, true, RGB(255, 0, 255), false);

	IMAGEMANAGER->addFrameImage("eraseButton", "image/maptool/erase.bmp", 100, 100, 1, 2, true, RGB(255, 0, 255), false);
	IMAGEMANAGER->addFrameImage("saveButton", "image/maptool/save.bmp", 100, 100, 1, 2, true, RGB(255, 0, 255), false);
	IMAGEMANAGER->addFrameImage("loadButton", "image/maptool/load.bmp", 100, 100, 1, 2, true, RGB(255, 0, 255), false);

	_obj = IMAGEMANAGER->addFrameImage("object", "image/maptool/오브젝트.bmp", MAPTILEWIDTH*2, MAPTILEHEIGHT*6, 2, 3, true, RGB(255, 0, 255), false);
	
	IMAGEMANAGER->addImage("altitudeButton", "image/maptool/높이조절.bmp", 100, 50,true, RGB(255, 0, 255), false);

	IMAGEMANAGER->addImage("maptoolbg", "image/maptool/bg.bmp", WINSIZEX, WINSIZEY,true,RGB(255,0,255),false);
	IMAGEMANAGER->addImage("maptoolbgMenu", "image/maptool/bgMenu.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255), false);

	IMAGEMANAGER->findImage("leftButton")->setFrameX(0);
	IMAGEMANAGER->findImage("upButton")->setFrameY(0);
	IMAGEMANAGER->findImage("rightButton")->setFrameX(1);
	IMAGEMANAGER->findImage("downButton")->setFrameY(1);

	_gride = true;
	_erase = false;

	_cameraX = _cameraY = 0 ;

	_nowMapX = _nowMapY = 0;

	_nowBrush = 0;
	_nowAltitude = 0;

	_cameraUp = RectMake(WINSIZEX / 3, 10, 100, 50);
	_cameraDown = RectMake(WINSIZEX / 3, WINSIZEY- WINSIZEY/4.5, 100, 50);
	_cameraLeft = RectMake(10, WINSIZEY/3, 50, 100);
	_cameraRight = RectMake(WINSIZEX/2 +230, WINSIZEY / 3, 50, 100);

	_mapArea = RectMake( 69, 73, WINSIZEX * 0.5 + 152, WINSIZEY - 253 );

	_altitudeDown = RectMake(WINSIZEX / 2 + 320, WINSIZEY / 3-25, 35, 50);
	_altitudeUp = RectMake(WINSIZEX / 2 + 385, WINSIZEY / 3 - 25, 35, 50);

	_eraseButton = RectMake(WINSIZEX / 2 + 320, WINSIZEY / 3 +80, 100, 50);

	_sample = RectMake(850, 100, MAPTILEWIDTH, MAPTILEHEIGHT * 2);

	_save = RectMake(WINSIZEX / 8, WINSIZEY - WINSIZEY / 8, 100,50);
	_load = RectMake(WINSIZEX / 4, WINSIZEY - WINSIZEY / 8, 100,50);

	for (int i = 0; i < TILENUMY; i++)
	{
		for (int j = 0; j < TILENUMX; j++)
		{
			_tiles[j][i].terrain = TR_NONE;
			_tiles[j][i].obj = OBJ_NONE;
			_tiles[j][i].onTile = ON_NONE;
			_tiles[j][i].activeTile = false;
			_tiles[j][i].altitude = 0;
			_tiles[j][i].X = j;
			_tiles[j][i].Y = i;
			_tiles[j][i].CostFromStart = 0;
			_tiles[j][i].CostToGoal = 0;
			_tiles[j][i].TotalCost = 0;
		}
	}

	return S_OK;
}

void maptool::release(void)
{

}

void maptool::update(void)
{

	_nowMapX = (_ptMouse.y + _cameraY + (_ptMouse.x + _cameraX - WINSIZEX * 0.5) * 0.5) / MAPTILEHEIGHT;
	_nowMapY = (_ptMouse.y + _cameraY - (_ptMouse.x + _cameraX - WINSIZEX * 0.5) * 0.5) / MAPTILEHEIGHT;

	if (KEYMANAGER->isStayKeyDown(MK_LBUTTON))
	{
		if (PtInRect(&_cameraUp, _ptMouse) && _cameraY > -200)
		{
			_cameraY -= 5;
			IMAGEMANAGER->findImage("upButton")->setFrameY(1);
		}
		else if (PtInRect(&_cameraDown, _ptMouse) && _cameraY < 200)
		{
			_cameraY += 5;
			IMAGEMANAGER->findImage("downButton")->setFrameY(0);
		}
		else if (PtInRect(&_cameraLeft, _ptMouse) && _cameraX > -300)
		{
			_cameraX -= 5;
			IMAGEMANAGER->findImage("leftButton")->setFrameX(1);
		}
		else if (PtInRect(&_cameraRight, _ptMouse) && _cameraX < 500)
		{
			_cameraX += 5;
			IMAGEMANAGER->findImage("rightButton")->setFrameX(0);
		}

		if (!_erase && _nowMapX >= 0 && _nowMapY >= 0 && _nowMapX <= 19 && _nowMapY <= 19 && PtInRect(&_mapArea,_ptMouse))
		{
			if (_nowBrush == 0)_tiles[_nowMapX][_nowMapY].terrain = TR_GRASS;
			else if (_nowBrush == 1)_tiles[_nowMapX][_nowMapY].terrain = TR_ROCK;
			else if (_nowBrush == 2)_tiles[_nowMapX][_nowMapY].terrain = TR_STONES;
			else if (_nowBrush == 3)_tiles[_nowMapX][_nowMapY].terrain = TR_WATER;
			else if (_nowBrush == 4)_tiles[_nowMapX][_nowMapY].terrain = TR_DEEPTILE;
			else if (_nowBrush == 5)_tiles[_nowMapX][_nowMapY].terrain = TR_LIGHTTILE;
			else if (_nowBrush == 6)_tiles[_nowMapX][_nowMapY].terrain = TR_GRAVELTILE;
			else if (_nowBrush == 7)_tiles[_nowMapX][_nowMapY].obj = OBJ_PUPLE_FLOWER;
			else if (_nowBrush == 8)_tiles[_nowMapX][_nowMapY].obj = OBJ_WHITE_FLOWER;
			else if (_nowBrush == 9)_tiles[_nowMapX][_nowMapY].obj = OBJ_WEEDS;
			else if (_nowBrush == 10)_tiles[_nowMapX][_nowMapY].obj = OBJ_PEBBLE;
			else if (_nowBrush == 11)_tiles[_nowMapX][_nowMapY].obj = OBJ_YELLOW_FLOWER;
			else if (_nowBrush == 12)
			{
				for (int i = 0; i < TILENUMY; i++)
				{
					for (int j = 0; j < TILENUMX; j++)
					{
						if (_tiles[j][i].obj == OBJ_STARTPOTAL)_tiles[j][i].obj = OBJ_NONE;
					}
				}

				_tiles[_nowMapX][_nowMapY].obj = OBJ_STARTPOTAL;
			}
		
			if(_nowBrush<7)_tiles[_nowMapX][_nowMapY].altitude = _nowAltitude;
		}
		else if (_erase && _nowMapX >= 0 && _nowMapY >= 0 && _nowMapX <= 19 && _nowMapY <= 19 && PtInRect(&_mapArea, _ptMouse))
		{
			_tiles[_nowMapX][_nowMapY].terrain = TR_NONE;
			_tiles[_nowMapX][_nowMapY].obj = OBJ_NONE;
			_tiles[_nowMapX][_nowMapY].altitude = 0;
		}

	}
	
	if (KEYMANAGER->isOnceKeyUp(MK_LBUTTON))
	{
		IMAGEMANAGER->findImage("leftButton")->setFrameX(0);
		IMAGEMANAGER->findImage("upButton")->setFrameY(0);
		IMAGEMANAGER->findImage("rightButton")->setFrameX(1);
		IMAGEMANAGER->findImage("downButton")->setFrameY(1);
		IMAGEMANAGER->findImage("saveButton")->setFrameY(0);
		IMAGEMANAGER->findImage("loadButton")->setFrameY(0);
	}

	if (KEYMANAGER->isOnceKeyDown(MK_LBUTTON))
	{
		if (PtInRect(&_sample, _ptMouse))
		{
			if (_nowBrush <12)_nowBrush ++;
			else _nowBrush = 0;
		}
		if (PtInRect(&_eraseButton, _ptMouse))
		{
			if (_erase)_erase=false;
			else _erase = true;
		}
		if (PtInRect(&_altitudeDown, _ptMouse) && _nowAltitude>0)_nowAltitude--;
		if (PtInRect(&_altitudeUp, _ptMouse) && _nowAltitude<20)_nowAltitude++;

		if (PtInRect(&_save, _ptMouse))
		{
			IMAGEMANAGER->findImage("saveButton")->setFrameY(1);
			save();
		}

		if (PtInRect(&_load, _ptMouse))
		{
			IMAGEMANAGER->findImage("loadButton")->setFrameY(1);
			load();
		}
	}

	if (KEYMANAGER->isOnceKeyDown(VK_F1))
	{
		if (_gride)_gride = false;
		else _gride = true;
	}
	if (KEYMANAGER->isOnceKeyDown(VK_F2))
	{
		SCENEMANAGER->changeScene("titleScene");
	}
}

void maptool::render(void)
{
	IMAGEMANAGER->render("maptoolbg", getMemDC());

	HPEN pen1 = CreatePen(PS_INSIDEFRAME, 2, RGB(200, 100, 240));
	HPEN pen2 = CreatePen(PS_INSIDEFRAME, 2, RGB(150, 20, 20));

	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 20; j++)
		{

			if (_gride)
			{
				char str[128];
				sprintf(str, "%d ,%d", j, i);
				SetTextColor(getMemDC(), RGB(60, 150, 70));
				TextOut(getMemDC(), (j - i) * MAPTILEWIDTH * 0.5 + WINSIZEX * 0.5 - 10 - _cameraX, (j + i) * MAPTILEHEIGHT * 0.5 - 18 - _cameraY, str, strlen(str));

				SelectObject(getMemDC(), pen1);

				LineMake(getMemDC(), (j - i) * MAPTILEWIDTH * 0.5 + WINSIZEX * 0.5 - _cameraX, (j + i) * MAPTILEHEIGHT * 0.5 - _cameraY,
					(j - i) * MAPTILEWIDTH * 0.5 - MAPTILEWIDTH * 0.5 + WINSIZEX * 0.5 - _cameraX, (j + i) * MAPTILEHEIGHT * 0.5 + MAPTILEHEIGHT * 0.5 - _cameraY);
				LineMake(getMemDC(), (j - i) * MAPTILEWIDTH * 0.5 + WINSIZEX * 0.5 - _cameraX, (j + i) * MAPTILEHEIGHT * 0.5 - _cameraY,
					(j - i) * MAPTILEWIDTH * 0.5 + MAPTILEWIDTH * 0.5 + WINSIZEX * 0.5 - _cameraX, (j + i) * MAPTILEHEIGHT * 0.5 + MAPTILEHEIGHT * 0.5 - _cameraY);
				LineMake(getMemDC(), (j - i) * MAPTILEWIDTH * 0.5 + WINSIZEX * 0.5 - _cameraX, (j + i) * MAPTILEHEIGHT * 0.5 + MAPTILEHEIGHT - _cameraY,
					(j - i) * MAPTILEWIDTH * 0.5 - MAPTILEWIDTH * 0.5 + WINSIZEX * 0.5 - _cameraX, (j + i) * MAPTILEHEIGHT * 0.5 + MAPTILEHEIGHT * 0.5 - _cameraY);
				LineMake(getMemDC(), (j - i) * MAPTILEWIDTH * 0.5 + WINSIZEX * 0.5 - _cameraX, (j + i) * MAPTILEHEIGHT * 0.5 + MAPTILEHEIGHT - _cameraY,
					(j - i) * MAPTILEWIDTH * 0.5 + MAPTILEWIDTH * 0.5 + WINSIZEX * 0.5 - _cameraX, (j + i) * MAPTILEHEIGHT * 0.5 + MAPTILEHEIGHT * 0.5 - _cameraY);
			}
		
			if ((j - i) * MAPTILEWIDTH * 0.5  + MAPTILEWIDTH * 0.5 + WINSIZEX * 0.5 - _cameraX >_mapArea.left && (j + i) * MAPTILEHEIGHT * 0.5  - _cameraY < _mapArea.bottom
				&& (j - i) * MAPTILEWIDTH * 0.5 + MAPTILEWIDTH * 0.5 - WINSIZEX * 0.5 - _cameraX <_mapArea.right && (j + i) * MAPTILEHEIGHT * 0.5 + MAPTILEHEIGHT - _cameraY > _mapArea.top)
			{
				if (_tiles[j][i].terrain == TR_GRASS)_tileGraass->render(getMemDC(), (j - i) * MAPTILEWIDTH * 0.5 + WINSIZEX * 0.5 - (MAPTILEWIDTH * 0.5) - _cameraX, (j + i) * MAPTILEHEIGHT * 0.5 - _cameraY);
				else if (_tiles[j][i].terrain == TR_STONES)_tileStones->render(getMemDC(), (j - i) * MAPTILEWIDTH * 0.5 + WINSIZEX * 0.5 - (MAPTILEWIDTH * 0.5) - _cameraX, (j + i) * MAPTILEHEIGHT * 0.5 - _cameraY);
				else if (_tiles[j][i].terrain == TR_ROCK)_tileRock->render(getMemDC(), (j - i) * MAPTILEWIDTH * 0.5 + WINSIZEX * 0.5 - (MAPTILEWIDTH * 0.5) - _cameraX, (j + i) * MAPTILEHEIGHT * 0.5 - _cameraY);
				else if (_tiles[j][i].terrain == TR_WATER)_tileWater->alphaRender(getMemDC(), (j - i) * MAPTILEWIDTH * 0.5 + WINSIZEX * 0.5 - (MAPTILEWIDTH * 0.5) - _cameraX, (j + i) * MAPTILEHEIGHT * 0.5 - _cameraY + MAPTILEHEIGHT / 3 - _tiles[j][i].altitude*MAPTILEHEIGHT / 2.1, 150);
				else if (_tiles[j][i].terrain == TR_DEEPTILE)_tileDeepTile->render(getMemDC(), (j - i) * MAPTILEWIDTH * 0.5 + WINSIZEX * 0.5 - (MAPTILEWIDTH * 0.5) - _cameraX, (j + i) * MAPTILEHEIGHT * 0.5 - _cameraY);
				else if (_tiles[j][i].terrain == TR_LIGHTTILE)_tileLightTile->render(getMemDC(), (j - i) * MAPTILEWIDTH * 0.5 + WINSIZEX * 0.5 - (MAPTILEWIDTH * 0.5) - _cameraX, (j + i) * MAPTILEHEIGHT * 0.5 - _cameraY);
				else if (_tiles[j][i].terrain == TR_GRAVELTILE)_tileGraveTile->render(getMemDC(), (j - i) * MAPTILEWIDTH * 0.5 + WINSIZEX * 0.5 - (MAPTILEWIDTH * 0.5) - _cameraX, (j + i) * MAPTILEHEIGHT * 0.5 - _cameraY);

				for (int k = 1; k < _tiles[j][i].altitude+1; k++)
				{
					if (_tiles[j][i].terrain == TR_GRASS)_tileGraassTop->render(getMemDC(), (j - i) * MAPTILEWIDTH * 0.5 + WINSIZEX * 0.5 - (MAPTILEWIDTH * 0.5) - _cameraX, (j + i) * MAPTILEHEIGHT * 0.5 - _cameraY - MAPTILEHEIGHT / 2.1 *k);
					else if (_tiles[j][i].terrain == TR_STONES)_tileStonesTop->render(getMemDC(), (j - i) * MAPTILEWIDTH * 0.5 + WINSIZEX * 0.5 - (MAPTILEWIDTH * 0.5) - _cameraX, (j + i) * MAPTILEHEIGHT * 0.5 - _cameraY - MAPTILEHEIGHT / 2.1 * k);
					else if (_tiles[j][i].terrain == TR_ROCK)_tileRockTop->render(getMemDC(), (j - i) * MAPTILEWIDTH * 0.5 + WINSIZEX * 0.5 - (MAPTILEWIDTH * 0.5) - _cameraX, (j + i) * MAPTILEHEIGHT * 0.5 - _cameraY - MAPTILEHEIGHT / 2.1 * k);
					else if (_tiles[j][i].terrain == TR_DEEPTILE)_tileDeepTile->render(getMemDC(), (j - i) * MAPTILEWIDTH * 0.5 + WINSIZEX * 0.5 - (MAPTILEWIDTH * 0.5) - _cameraX, (j + i) * MAPTILEHEIGHT * 0.5 - _cameraY - MAPTILEHEIGHT / 2.1 * k);
					else if (_tiles[j][i].terrain == TR_LIGHTTILE)_tileLightTile->render(getMemDC(), (j - i) * MAPTILEWIDTH * 0.5 + WINSIZEX * 0.5 - (MAPTILEWIDTH * 0.5) - _cameraX, (j + i) * MAPTILEHEIGHT * 0.5 - _cameraY - MAPTILEHEIGHT / 2.1 * k);
					else if (_tiles[j][i].terrain == TR_GRAVELTILE)_tileGraveTile->render(getMemDC(), (j - i) * MAPTILEWIDTH * 0.5 + WINSIZEX * 0.5 - (MAPTILEWIDTH * 0.5) - _cameraX, (j + i) * MAPTILEHEIGHT * 0.5 - _cameraY - MAPTILEHEIGHT / 2.1 * k);
				}

				if (_tiles[j][i].obj == OBJ_PUPLE_FLOWER)_obj->frameRender(getMemDC(), (j - i) * MAPTILEWIDTH * 0.5 + WINSIZEX * 0.5 - (MAPTILEWIDTH * 0.5) - _cameraX, (j + i) * MAPTILEHEIGHT * 0.5 - _cameraY - _tiles[j][i].altitude*MAPTILEHEIGHT / 2.1,0,0);
				else if (_tiles[j][i].obj == OBJ_WHITE_FLOWER)_obj->frameRender(getMemDC(), (j - i) * MAPTILEWIDTH * 0.5 + WINSIZEX * 0.5 - (MAPTILEWIDTH * 0.5) - _cameraX, (j + i) * MAPTILEHEIGHT * 0.5 - _cameraY - _tiles[j][i].altitude*MAPTILEHEIGHT / 2.1,1,0);
				else if (_tiles[j][i].obj == OBJ_WEEDS)_obj->frameRender(getMemDC(), (j - i) * MAPTILEWIDTH * 0.5 + WINSIZEX * 0.5 - (MAPTILEWIDTH * 0.5) - _cameraX, (j + i) * MAPTILEHEIGHT * 0.5 - _cameraY - _tiles[j][i].altitude*MAPTILEHEIGHT / 2.1-20,0,1);
				else if (_tiles[j][i].obj == OBJ_PEBBLE)_obj->frameRender(getMemDC(), (j - i) * MAPTILEWIDTH * 0.5 + WINSIZEX * 0.5 - (MAPTILEWIDTH * 0.5) - _cameraX, (j + i) * MAPTILEHEIGHT * 0.5 - _cameraY - _tiles[j][i].altitude*MAPTILEHEIGHT / 2.1 - 20,1,1);
				else if (_tiles[j][i].obj == OBJ_YELLOW_FLOWER)_obj->frameRender(getMemDC(), (j - i) * MAPTILEWIDTH * 0.5 + WINSIZEX * 0.5 - (MAPTILEWIDTH * 0.5) - _cameraX, (j + i) * MAPTILEHEIGHT * 0.5 - _cameraY - _tiles[j][i].altitude*MAPTILEHEIGHT / 2.1 - 18,0,2);
				else if (_tiles[j][i].obj == OBJ_STARTPOTAL)_startTile->render(getMemDC(), (j - i) * MAPTILEWIDTH * 0.5 + WINSIZEX * 0.5 - (MAPTILEWIDTH * 0.5) - _cameraX, (j + i) * MAPTILEHEIGHT * 0.5 - _cameraY - _tiles[j][i].altitude*MAPTILEHEIGHT / 2.1);

			}
		}
	}

				SelectObject(getMemDC(), pen2);

				LineMake(getMemDC(), (_nowMapX - _nowMapY) * MAPTILEWIDTH * 0.5 + WINSIZEX * 0.5 - _cameraX, (_nowMapX + _nowMapY) * MAPTILEHEIGHT * 0.5 - _cameraY - _tiles[_nowMapX][_nowMapY].altitude*MAPTILEHEIGHT / 2.1,
					(_nowMapX - _nowMapY) * MAPTILEWIDTH * 0.5 - MAPTILEWIDTH * 0.5 + WINSIZEX * 0.5 - _cameraX, (_nowMapX + _nowMapY) * MAPTILEHEIGHT * 0.5 + MAPTILEHEIGHT * 0.5 - _cameraY - _tiles[_nowMapX][_nowMapY].altitude*MAPTILEHEIGHT / 2.1);
				LineMake(getMemDC(), (_nowMapX - _nowMapY) * MAPTILEWIDTH * 0.5 + WINSIZEX * 0.5 - _cameraX, (_nowMapX + _nowMapY) * MAPTILEHEIGHT * 0.5 - _cameraY - _tiles[_nowMapX][_nowMapY].altitude*MAPTILEHEIGHT / 2.1,
					(_nowMapX - _nowMapY) * MAPTILEWIDTH * 0.5 + MAPTILEWIDTH * 0.5 + WINSIZEX * 0.5 - _cameraX, (_nowMapX + _nowMapY) * MAPTILEHEIGHT * 0.5 + MAPTILEHEIGHT * 0.5 - _cameraY - _tiles[_nowMapX][_nowMapY].altitude*MAPTILEHEIGHT / 2.1);
				LineMake(getMemDC(), (_nowMapX - _nowMapY) * MAPTILEWIDTH * 0.5 + WINSIZEX * 0.5 - _cameraX, (_nowMapX + _nowMapY) * MAPTILEHEIGHT * 0.5 + MAPTILEHEIGHT - _cameraY - _tiles[_nowMapX][_nowMapY].altitude*MAPTILEHEIGHT / 2.1,
					(_nowMapX - _nowMapY) * MAPTILEWIDTH * 0.5 - MAPTILEWIDTH * 0.5 + WINSIZEX * 0.5 - _cameraX, (_nowMapX + _nowMapY) * MAPTILEHEIGHT * 0.5 + MAPTILEHEIGHT * 0.5 - _cameraY - _tiles[_nowMapX][_nowMapY].altitude*MAPTILEHEIGHT / 2.1);
				LineMake(getMemDC(), (_nowMapX - _nowMapY) * MAPTILEWIDTH * 0.5 + WINSIZEX * 0.5 - _cameraX, (_nowMapX + _nowMapY) * MAPTILEHEIGHT * 0.5 + MAPTILEHEIGHT - _cameraY - _tiles[_nowMapX][_nowMapY].altitude*MAPTILEHEIGHT / 2.1,
					(_nowMapX - _nowMapY) * MAPTILEWIDTH * 0.5 + MAPTILEWIDTH * 0.5 + WINSIZEX * 0.5 - _cameraX, (_nowMapX + _nowMapY) * MAPTILEHEIGHT * 0.5 + MAPTILEHEIGHT * 0.5 - _cameraY - _tiles[_nowMapX][_nowMapY].altitude*MAPTILEHEIGHT /2.1);
	
	IMAGEMANAGER->render("maptoolbgMenu", getMemDC());

	IMAGEMANAGER->frameRender("leftButton", getMemDC(), _cameraLeft.left, _cameraLeft.top);
	IMAGEMANAGER->frameRender("rightButton", getMemDC(), _cameraRight.left, _cameraRight.top);
	IMAGEMANAGER->frameRender("upButton", getMemDC(), _cameraUp.left, _cameraUp.top);
	IMAGEMANAGER->frameRender("downButton", getMemDC(), _cameraDown.left, _cameraDown.top);

	IMAGEMANAGER->frameRender("saveButton", getMemDC(), _save.left, _save.top);
	IMAGEMANAGER->frameRender("loadButton", getMemDC(), _load.left, _load.top);

	IMAGEMANAGER->frameRender("eraseButton", getMemDC(), _eraseButton.left, _eraseButton.top,0,(int)_erase);
	
	IMAGEMANAGER->render("altitudeButton", getMemDC(), _altitudeDown.left, _altitudeDown.top);
	SetTextColor(getMemDC(), RGB(200, 200, 200));
	char str[128];
	sprintf(str, "%d", _nowAltitude);
	
	if(_nowAltitude<10)TextOut(getMemDC(), _altitudeDown.right+10, _altitudeDown.top+15, str, strlen(str));
	else TextOut(getMemDC(), _altitudeDown.right + 7, _altitudeDown.top + 15, str, strlen(str));


	switch (_nowBrush)
	{
		case 0:
			_tileGraass->render(getMemDC(), 850, 100);
		break;

		case 1:
			_tileRock->render(getMemDC(), 850, 100);
		break;

		case 2:
			_tileStones->render(getMemDC(), 850, 100);
		break;
	
		case 3:
			_tileWater->render(getMemDC(), 850, 100);
		break;

		case 4://
			_tileDeepTile->render(getMemDC(), 850, 100);
			break;

		case 5://
			_tileLightTile->render(getMemDC(), 850, 100);
			break;

		case 6://
			_tileGraveTile->render(getMemDC(), 850, 100);
			break;
		case 7:
			_obj->frameRender( getMemDC(), 850, 100,0,0);
		break;

		case 8:
			_obj->frameRender( getMemDC(), 850, 100,1,0);
		break;

		case 9:
			_obj->frameRender( getMemDC(), 850, 100,0,1);
		break;

		case 10:
			_obj->frameRender( getMemDC(), 850, 100,1,1);
		break;

		case 11:
			_obj->frameRender( getMemDC(), 850, 100,0,2);
		break;

		case 12:
			_startTile->render(getMemDC(), 850, 100);
		break;
	}

	DeleteObject(pen1);
	DeleteObject(pen2);
}


void maptool::save(void)
{
	HANDLE file;
	DWORD write;

	file = CreateFile("tileMap.map", GENERIC_WRITE, 0, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	WriteFile(file, _tiles, sizeof(tagTile) * TILENUMX * TILENUMY, &write, NULL);
	
	CloseHandle(file);
}

void maptool::load(void)
{
	HANDLE file;
	DWORD read;

	file = CreateFile("townmap.map", GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	ReadFile(file, _tiles, sizeof(tagTile) * TILENUMX * TILENUMY, &read, NULL);
	
	CloseHandle(file);
}