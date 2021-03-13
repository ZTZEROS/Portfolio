#include "stdafx.h"
#include "battlemap.h"


battlemap::battlemap()
{
}


battlemap::~battlemap()
{
}

HRESULT battlemap::init(void)
{
	_tileRock = IMAGEMANAGER->addImage("돌타일", "image/maptool/돌타일.bmp", TILEWIDTH + 5, TILEHEIGHT * 2, true, RGB(255, 0, 255));
	_tileGraass = IMAGEMANAGER->addImage("풀타일", "image/maptool/풀타일.bmp", TILEWIDTH + 5, TILEHEIGHT * 2, true, RGB(255, 0, 255));
	_tileStones = IMAGEMANAGER->addImage("자갈타일", "image/maptool/자갈타일.bmp", TILEWIDTH + 5, TILEHEIGHT * 2, true, RGB(255, 0, 255));
	_tileRockTop = IMAGEMANAGER->addImage("돌타일상단", "image/maptool/돌타일상단.bmp", TILEWIDTH + 5, TILEHEIGHT * 2, true, RGB(255, 0, 255));
	_tileGraassTop = IMAGEMANAGER->addImage("풀타일상단", "image/maptool/풀타일상단.bmp", TILEWIDTH + 5, TILEHEIGHT * 2, true, RGB(255, 0, 255));
	_tileStonesTop = IMAGEMANAGER->addImage("자갈타일상단", "image/maptool/자갈타일상단.bmp", TILEWIDTH + 5, TILEHEIGHT * 2, true, RGB(255, 0, 255));
	_tileWater = IMAGEMANAGER->addImage("물타일", "image/maptool/물타일.bmp", TILEWIDTH, TILEHEIGHT, true, RGB(255, 0, 255), true);
	_active = IMAGEMANAGER->addImage("액티브", "image/maptool/moveable.bmp", TILEWIDTH, TILEHEIGHT, true, RGB(255, 0, 255), true);

	_tileDeepTile = IMAGEMANAGER->addImage("마을타일1", "image/maptool/tile_town1.bmp", TILEWIDTH + 5, TILEHEIGHT * 2, true, RGB(255, 0, 255));
	_tileLightTile = IMAGEMANAGER->addImage("마을타일2", "image/maptool/tile_town2.bmp", TILEWIDTH + 5, TILEHEIGHT * 2, true, RGB(255, 0, 255));
	_tileGraveTile = IMAGEMANAGER->addImage("마을타일3", "image/maptool/tile_town3.bmp", TILEWIDTH + 5, TILEHEIGHT * 2, true, RGB(255, 0, 255));
	_startTile = IMAGEMANAGER->addImage("시작타일", "image/maptool/startTile.bmp", TILEWIDTH + 5, TILEHEIGHT, true, RGB(255, 0, 255));

	IMAGEMANAGER->addImage("마법진", "image/maptool/마법진.bmp", TILEWIDTH-40, TILEHEIGHT, true, MAGENTA, true);

	_obj = IMAGEMANAGER->addFrameImage("object2", "image/maptool/오브젝트.bmp", TILEWIDTH * 2, TILEHEIGHT * 6, 2, 3, true, RGB(255, 0, 255), false);

	_cameraX = _cameraY = 0;

	_cameraTurn = false;

	_magicCicleAlpha = 150;
	_alphaSwitch = false;

	load();

	PreviousOpenableTileVSize = OpenableTileVSize = 0;

	re = false;

	return S_OK;
}

void battlemap::release(void)
{

}

void battlemap::update()
{
	if (_magicCicleAlpha > 200)_alphaSwitch = false;
	else if (_magicCicleAlpha < 100)_alphaSwitch = true;


	if (_alphaSwitch)_magicCicleAlpha+=5;
	else _magicCicleAlpha-=5;

	if (_cameraTurn)
	{
		for (int i = 0; i < TILENUMY; i++)
		{
			for (int j = 0; j < TILENUMX; j++)
			{
				_tiles[j][i] = _temp[j][i];
				_tiles[j][i].X = j;
				_tiles[j][i].Y = i;
			}
		}

		_cameraTurn = false;
	}
}

void battlemap::render(int j, int i)
{

	if (_tiles[j][i].terrain == TR_GRASS)_tileGraass->render(getMemDC(), (j - i) * TILEWIDTH * 0.5 + BACKBUFFERSIZEX * 0.5 - (TILEWIDTH * 0.5), (j + i) * TILEHEIGHT * 0.5 + BACKBUFFERSIZEY/3);
	else if (_tiles[j][i].terrain == TR_STONES)_tileStones->render(getMemDC(), (j - i) * TILEWIDTH * 0.5 + BACKBUFFERSIZEX * 0.5 - (TILEWIDTH * 0.5), (j + i) * TILEHEIGHT * 0.5 + BACKBUFFERSIZEY/3);
	else if (_tiles[j][i].terrain == TR_ROCK)_tileRock->render(getMemDC(), (j - i) * TILEWIDTH * 0.5 + BACKBUFFERSIZEX * 0.5 - (TILEWIDTH * 0.5), (j + i) * TILEHEIGHT * 0.5 + BACKBUFFERSIZEY/3);
	else if (_tiles[j][i].terrain == TR_WATER)_tileWater->alphaRender(getMemDC(), (j - i) * TILEWIDTH * 0.5 + BACKBUFFERSIZEX * 0.5 - (TILEWIDTH * 0.5), (j + i) * TILEHEIGHT * 0.5 + TILEHEIGHT / 3 - _tiles[j][i].altitude*TILEHEIGHT / 2.1 + BACKBUFFERSIZEY/3, 150);
	else if (_tiles[j][i].terrain == TR_DEEPTILE)_tileDeepTile->render(getMemDC(), (j - i) * TILEWIDTH * 0.5 + BACKBUFFERSIZEX * 0.5 - (TILEWIDTH * 0.5) , (j + i) * TILEHEIGHT * 0.5 + BACKBUFFERSIZEY/3);
	else if (_tiles[j][i].terrain == TR_LIGHTTILE)_tileLightTile->render(getMemDC(), (j - i) * TILEWIDTH * 0.5 + BACKBUFFERSIZEX * 0.5 - (TILEWIDTH * 0.5) , (j + i) * TILEHEIGHT * 0.5 + BACKBUFFERSIZEY/3);
	else if (_tiles[j][i].terrain == TR_GRAVELTILE)_tileGraveTile->render(getMemDC(), (j - i) * TILEWIDTH * 0.5 + BACKBUFFERSIZEX * 0.5 - (TILEWIDTH * 0.5) , (j + i) * TILEHEIGHT * 0.5 + BACKBUFFERSIZEY/3);

	if (_tiles[j][i].altitude > 0)
	{
		for (int k = 1; k < _tiles[j][i].altitude + 1; k++)
		{
			if (_tiles[j][i].terrain == TR_GRASS)_tileGraassTop->render(getMemDC(), (j - i) * TILEWIDTH * 0.5 + BACKBUFFERSIZEX * 0.5 - (TILEWIDTH * 0.5), (j + i) * TILEHEIGHT * 0.5 - TILEHEIGHT / 2.1 *k + BACKBUFFERSIZEY/3);
			else if (_tiles[j][i].terrain == TR_STONES)_tileStonesTop->render(getMemDC(), (j - i) * TILEWIDTH * 0.5 + BACKBUFFERSIZEX * 0.5 - (TILEWIDTH * 0.5), (j + i) * TILEHEIGHT * 0.5 - TILEHEIGHT / 2.1 * k + BACKBUFFERSIZEY/3);
			else if (_tiles[j][i].terrain == TR_ROCK)_tileRockTop->render(getMemDC(), (j - i) * TILEWIDTH * 0.5 + BACKBUFFERSIZEX * 0.5 - (TILEWIDTH * 0.5), (j + i) * TILEHEIGHT * 0.5 - TILEHEIGHT / 2.1 * k + BACKBUFFERSIZEY/3);
			else if (_tiles[j][i].terrain == TR_DEEPTILE)_tileDeepTile->render(getMemDC(), (j - i) * TILEWIDTH * 0.5 + BACKBUFFERSIZEX * 0.5 - (TILEWIDTH * 0.5), (j + i) * TILEHEIGHT * 0.5 - TILEHEIGHT / 2.1 *k + BACKBUFFERSIZEY/3);
			else if (_tiles[j][i].terrain == TR_LIGHTTILE)_tileLightTile->render(getMemDC(), (j - i) * TILEWIDTH * 0.5 + BACKBUFFERSIZEX * 0.5 - (TILEWIDTH * 0.5), (j + i) * TILEHEIGHT * 0.5 - TILEHEIGHT / 2.1 * k + BACKBUFFERSIZEY/3);
			else if (_tiles[j][i].terrain == TR_GRAVELTILE)_tileGraveTile->render(getMemDC(), (j - i) * TILEWIDTH * 0.5 + BACKBUFFERSIZEX * 0.5 - (TILEWIDTH * 0.5), (j + i) * TILEHEIGHT * 0.5 - TILEHEIGHT / 2.1 * k + BACKBUFFERSIZEY/3);
		}
	}

	if (_tiles[j][i].obj == OBJ_PUPLE_FLOWER)_obj->frameRender(getMemDC(), (j - i) * TILEWIDTH * 0.5 + BACKBUFFERSIZEX * 0.5 - (TILEWIDTH * 0.5), (j + i) * TILEHEIGHT * 0.5 - _tiles[j][i].altitude*TILEHEIGHT / 2.1 + BACKBUFFERSIZEY/3, 0, 0);
	else if (_tiles[j][i].obj == OBJ_WHITE_FLOWER)_obj->frameRender(getMemDC(), (j - i) * TILEWIDTH * 0.5 + BACKBUFFERSIZEX * 0.5 - (TILEWIDTH * 0.5), (j + i) * TILEHEIGHT * 0.5 - _tiles[j][i].altitude*TILEHEIGHT / 2.1 + BACKBUFFERSIZEY/3, 1, 0);
	else if (_tiles[j][i].obj == OBJ_WEEDS)_obj->frameRender(getMemDC(), (j - i) * TILEWIDTH * 0.5 + BACKBUFFERSIZEX * 0.5 - (TILEWIDTH * 0.5), (j + i) * TILEHEIGHT * 0.5 - _tiles[j][i].altitude*TILEHEIGHT / 2.1 - 57 + BACKBUFFERSIZEY/3, 0, 1);
	else if (_tiles[j][i].obj == OBJ_PEBBLE)_obj->frameRender(getMemDC(), (j - i) * TILEWIDTH * 0.5 + BACKBUFFERSIZEX * 0.5 - (TILEWIDTH * 0.5), (j + i) * TILEHEIGHT * 0.5 - _tiles[j][i].altitude*TILEHEIGHT / 2.1 - 50 + BACKBUFFERSIZEY/3, 1, 1);
	else if (_tiles[j][i].obj == OBJ_YELLOW_FLOWER)_obj->frameRender(getMemDC(), (j - i) * TILEWIDTH * 0.5 + BACKBUFFERSIZEX * 0.5 - (TILEWIDTH * 0.5), (j + i) * TILEHEIGHT * 0.5 - _tiles[j][i].altitude*TILEHEIGHT / 2.1 - 50 + BACKBUFFERSIZEY/3, 0, 2);
	else if (_tiles[j][i].obj == OBJ_STARTPOTAL)
	{
		_startTile->render(getMemDC(), (j - i) * TILEWIDTH * 0.5 + BACKBUFFERSIZEX * 0.5 - (TILEWIDTH * 0.5), (j + i) * TILEHEIGHT * 0.5  - _tiles[j][i].altitude*TILEHEIGHT / 2.1 + BACKBUFFERSIZEY/3);
		IMAGEMANAGER->alphaRender("마법진", getMemDC(), (j - i) * TILEWIDTH * 0.5 + BACKBUFFERSIZEX * 0.5 - (TILEWIDTH * 0.5)+20, (j + i) * TILEHEIGHT * 0.5 - _tiles[j][i].altitude*TILEHEIGHT / 2.1 + BACKBUFFERSIZEY/3, _magicCicleAlpha);
	}
	if (_tiles[j][i].activeTile)_active->alphaRender(getMemDC(), (j - i) * TILEWIDTH * 0.5 + BACKBUFFERSIZEX * 0.5 - (TILEWIDTH * 0.5), (j + i) * TILEHEIGHT * 0.5 - _tiles[j][i].altitude*TILEHEIGHT / 2.1 + BACKBUFFERSIZEY/3, _magicCicleAlpha);
}
void battlemap::cameraTurnRight(int i, int j)
{
	_temp[i][j] = _tiles[j][(TILENUMX - 1) - i];
	_cameraTurn = true;
}

void battlemap::cameraTurnLeft(int i, int j)
{
	_temp[j][(TILENUMX - 1) - i] = _tiles[i][j];
	_cameraTurn = true;
}

void battlemap::load(void)
{
	HANDLE file;
	DWORD read;

	file = CreateFile("tileMap.map", GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	ReadFile(file, _tiles, sizeof(tagTile) * TILENUMX * TILENUMY, &read, NULL);

	CloseHandle(file);
}


vector<POINT> battlemap::addOpenableTileV(POINT currentTile, int movableDistance, bool loop)
{
	tagTile tile;

	if (loop)_start = currentTile;

	bool enemy;

	if (_tiles[_start.x][_start.y].onTile == ON_ENEMY) enemy = true;
	else if (_tiles[_start.x][_start.y].onTile == ON_HERO) enemy = false;

	if (currentTile.y > 0)
	{
		tile = _tiles[currentTile.x][currentTile.y - 1];
		SearchMovableTile(tile, currentTile, enemy);
	}

	if (currentTile.x > 0)
	{
		tile = _tiles[currentTile.x - 1][currentTile.y];
		SearchMovableTile(tile, currentTile, enemy);
	}

	if (currentTile.x < 19)
	{
		tile = _tiles[currentTile.x + 1][currentTile.y];
		SearchMovableTile(tile, currentTile, enemy);
	}

	if (currentTile.y < 19)
	{
		tile = _tiles[currentTile.x][currentTile.y + 1];
		SearchMovableTile(tile, currentTile, enemy);
	}

	if (loop)
	{
		for (int i = 1; i < movableDistance; i++)
		{
			OpenableTileVSize = OpenableTileV.size();

			for (int j = PreviousOpenableTileVSize; j < OpenableTileVSize; j++)
			{
				addOpenableTileV(OpenableTileV[j], movableDistance, false);
			}

			PreviousOpenableTileVSize = OpenableTileVSize;
		}

		PreviousOpenableTileVSize = OpenableTileVSize = 0;
	}

	return OpenableTileV;
}

void battlemap::SearchMovableTile(tagTile tile ,POINT currentTile, bool enemy)
{
	int i;
	i = abs(tile.altitude - _tiles[currentTile.x][currentTile.y].altitude);
	if (enemy)
	{
		if (tile.onTile !=ON_HERO && i < 2 && (!tile.activeTile) && (tile.terrain != TR_WATER) && (tile.X != _start.x || tile.Y != _start.y))
		{

			POINT i;
			i.x = tile.X;
			i.y = tile.Y;

			_tiles[tile.X][tile.Y].activeTile = true;
			OpenableTileV.push_back(i);
		}
	}
	else
	{
		if (tile.onTile != ON_ENEMY && i < 2 && (!tile.activeTile) && (tile.terrain != TR_WATER) && (tile.X != _start.x || tile.Y != _start.y))
		{

			POINT i;
			i.x = tile.X;
			i.y = tile.Y;

			_tiles[tile.X][tile.Y].activeTile = true;
			OpenableTileV.push_back(i);
		}
	}
}

void battlemap::ClearMoveableTile(void)
{
	for (int i = 0; i < TILENUMY; i++)
	{
		for (int j = 0; j < TILENUMX; j++)
		{
			_tiles[j][i].activeTile = false;
		}
	}
	OpenableTileV.clear();
}


void battlemap::SearchAttackableTile(POINT currentTile)
{
	
	if (currentTile.y > 0)
	{
		_tiles[currentTile.x][currentTile.y - 1].activeTile = true;
	}

	if (currentTile.x > 0)
	{
		_tiles[currentTile.x - 1][currentTile.y].activeTile = true;
	}

	if (currentTile.x < 19)
	{
		_tiles[currentTile.x + 1][currentTile.y].activeTile = true;
		
	}

	if (currentTile.y < 19)
	{
		_tiles[currentTile.x][currentTile.y + 1].activeTile = true;
	}
}


float battlemap::costAdd(POINT currentTile, POINT endTile)
{
	float tempCost[4];

	tempCost[0] = 50000;
	tempCost[1] = 50000;
	tempCost[2] = 50000;
	tempCost[3] = 50000;

	float returnCost;

	if (currentTile.y > 0)
	{
		if (_tiles[currentTile.x][currentTile.y - 1].activeTile
			&& abs(_tiles[currentTile.x][currentTile.y - 1].altitude - _tiles[currentTile.x][currentTile.y].altitude)<2)tempCost[0] = getDistance(((endTile.x - endTile.y) * TILEWIDTH * 0.5), ((endTile.x + endTile.y) * TILEHEIGHT * 0.5), ((currentTile.x - (currentTile.y - 1)) * TILEWIDTH * 0.5), ((currentTile.x + (currentTile.y - 1)) * TILEHEIGHT * 0.5));
	}

	if (currentTile.x > 0)
	{
		if (_tiles[currentTile.x - 1][currentTile.y].activeTile
			&& abs(_tiles[currentTile.x-1][currentTile.y].altitude - _tiles[currentTile.x][currentTile.y].altitude)<2)tempCost[1] = getDistance(((endTile.x - endTile.y) * TILEWIDTH * 0.5), ((endTile.x + endTile.y) * TILEHEIGHT * 0.5), (((currentTile.x - 1) - currentTile.y) * TILEWIDTH * 0.5), (((currentTile.x - 1) + currentTile.y) * TILEHEIGHT * 0.5));

	}

	if (currentTile.x < 19)
	{
		if (_tiles[currentTile.x + 1][currentTile.y].activeTile
			&& abs(_tiles[currentTile.x + 1][currentTile.y].altitude - _tiles[currentTile.x][currentTile.y].altitude)<2)tempCost[2] = getDistance(((endTile.x - endTile.y) * TILEWIDTH * 0.5), ((endTile.x + endTile.y) * TILEHEIGHT * 0.5), (((currentTile.x + 1) - currentTile.y) * TILEWIDTH * 0.5), (((currentTile.x + 1) + currentTile.y) * TILEHEIGHT * 0.5));
	}

	if (currentTile.y < 19)
	{
		if (_tiles[currentTile.x][currentTile.y + 1].activeTile
			&& abs(_tiles[currentTile.x][currentTile.y+1].altitude - _tiles[currentTile.x][currentTile.y].altitude)<2)tempCost[3] = getDistance(((endTile.x - endTile.y) * TILEWIDTH * 0.5), ((endTile.x + endTile.y) * TILEHEIGHT * 0.5), ((currentTile.x - (currentTile.y + 1)) * TILEWIDTH * 0.5), ((currentTile.x + (currentTile.y + 1)) * TILEHEIGHT * 0.5));
	}

	returnCost = tempCost[0];

	for (int j = 1; j < 4; j++)
	{
		if (tempCost[j] < returnCost)returnCost = tempCost[j];
	}

	return returnCost;
}

vector<POINT> battlemap::pathFinder(POINT currentTile, POINT endTile, int movableDistance)
{
	POINT startTileNum;
	tagTile tempTile;
	startTileNum = currentTile;
	POINT parentTileNum = PointMake(-1,-1);
	int tileLeft = 0;
	while (1)
	{
		for (int i = 0; i < movableDistance; i++)
		{
			if (startTileNum.y > 0) 
			{
				if (_tiles[startTileNum.x][startTileNum.y - 1].activeTile && abs(_tiles[startTileNum.x][startTileNum.y - 1].altitude - _tiles[startTileNum.x][startTileNum.y].altitude)<2
					&& (startTileNum.x != parentTileNum.x || startTileNum.y-1 != parentTileNum.y))
				{
					_tiles[startTileNum.x][startTileNum.y - 1].CostToGoal =
						getDistance(((endTile.x - endTile.y) * TILEWIDTH * 0.5), ((endTile.x + endTile.y) * TILEHEIGHT * 0.5), ((startTileNum.x - (startTileNum.y - 1)) * TILEWIDTH * 0.5), ((startTileNum.x + (startTileNum.y - 1)) * TILEHEIGHT * 0.5))
						 +costAdd(PointMake(startTileNum.x, startTileNum.y - 1), endTile);;

					_vOpenList.push_back(_tiles[startTileNum.x][startTileNum.y - 1]);
				}
			}

			if (startTileNum.x > 0)
			{
				if (_tiles[startTileNum.x - 1][startTileNum.y].activeTile && abs(_tiles[startTileNum.x-1][startTileNum.y].altitude - _tiles[startTileNum.x][startTileNum.y].altitude)<2
					&& (startTileNum.x-1 != parentTileNum.x || startTileNum.y != parentTileNum.y))
				{
					_tiles[startTileNum.x - 1][startTileNum.y].CostToGoal =
						getDistance(((endTile.x - endTile.y) * TILEWIDTH * 0.5), ((endTile.x + endTile.y) * TILEHEIGHT * 0.5), (((startTileNum.x - 1) - startTileNum.y) * TILEWIDTH * 0.5), (((startTileNum.x - 1) + startTileNum.y) * TILEHEIGHT * 0.5))
						 +costAdd(PointMake(startTileNum.x - 1, startTileNum.y), endTile);
					_vOpenList.push_back(_tiles[startTileNum.x - 1][startTileNum.y]);
				}
			}

			if (startTileNum.x < 19)
			{
				if (_tiles[startTileNum.x + 1][startTileNum.y].activeTile && abs(_tiles[startTileNum.x+1][startTileNum.y].altitude - _tiles[startTileNum.x][startTileNum.y].altitude)<2
					&& (startTileNum.x +1!= parentTileNum.x || startTileNum.y != parentTileNum.y))
				{
					_tiles[startTileNum.x + 1][startTileNum.y].CostToGoal =
						getDistance(((endTile.x - endTile.y) * TILEWIDTH * 0.5), ((endTile.x + endTile.y) * TILEHEIGHT * 0.5), (((startTileNum.x + 1) - startTileNum.y) * TILEWIDTH * 0.5), (((startTileNum.x + 1) + startTileNum.y) * TILEHEIGHT * 0.5))
						 +costAdd(PointMake(startTileNum.x + 1, startTileNum.y), endTile);
					_vOpenList.push_back(_tiles[startTileNum.x + 1][startTileNum.y]);
				}
			}

			if (startTileNum.y < 19)
			{
				if (_tiles[startTileNum.x][startTileNum.y + 1].activeTile&& abs(_tiles[startTileNum.x][startTileNum.y + 1].altitude - _tiles[startTileNum.x][startTileNum.y].altitude)<2
					&& (startTileNum.x != parentTileNum.x || startTileNum.y + 1 != parentTileNum.y))
				{
					_tiles[startTileNum.x][startTileNum.y + 1].CostToGoal =
						getDistance(((endTile.x - endTile.y) * TILEWIDTH * 0.5), ((endTile.x + endTile.y) * TILEHEIGHT * 0.5), ((startTileNum.x - (startTileNum.y + 1)) * TILEWIDTH * 0.5), ((startTileNum.x + (startTileNum.y + 1)) * TILEHEIGHT * 0.5))
						 +costAdd(PointMake(startTileNum.x, startTileNum.y + 1), endTile);
					_vOpenList.push_back(_tiles[startTileNum.x][startTileNum.y + 1]);
				}
			}

			if(_vOpenList.size()>0)tempTile = _vOpenList[0];
			else if (_vOpenList.size() == 0)_tiles[startTileNum.x][startTileNum.y].activeTile = false;
	
			for (int j = 1; j < _vOpenList.size(); j++)
			{
				if (tempTile.CostToGoal > _vOpenList[j].CostToGoal)
				{
					tempTile = _vOpenList[j];
				}
			}

			_vPathList.push_back(PointMake(tempTile.X, tempTile.Y));
			_vOpenList.clear();

			parentTileNum = startTileNum;
			startTileNum = PointMake(tempTile.X, tempTile.Y);

			if (tempTile.X == endTile.x && tempTile.Y == endTile.y)break;
		}

		bool Dbreak=false;

		if (_vPathList[_vPathList.size() - 1].x != endTile.x ||  _vPathList[_vPathList.size() - 1].y != endTile.y)
		{
			for (int i = 0; i < _vPathList.size()-1; i++)
			{
				for (int j = i + 1; j < _vPathList.size(); j++)
				{
					if (_vPathList[i].x == _vPathList[j].x && _vPathList[i].y == _vPathList[j].y)
					{
						_tiles[_vPathList[i+1].x][_vPathList[i+1].y].activeTile = false;
						startTileNum = currentTile;
						
						Dbreak = true;
						break;
					}
				}
				if (Dbreak)break;
			}
			_vPathList.clear();
		}
		else if (_vPathList[_vPathList.size() - 1].x == endTile.x &&  _vPathList[_vPathList.size() - 1].y == endTile.y)break;
	}

	return _vPathList;
}

void battlemap::clearAllList(void)
{
	_vPathList.clear();
}

void battlemap::onTheTileReset(void)
{
	for (int i=0; i < TILENUMX; i++)
	{
		for (int j=0; j < TILENUMY; j++)
		{
			_tiles[j][i].onTile = ON_NONE;
		}
	}
}