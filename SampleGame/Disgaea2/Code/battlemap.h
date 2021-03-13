#pragma once
#include "gameNode.h"
#include "tileNode.h"

class battlemap : public gameNode
{
private:

	tagTile _tiles[TILENUMX][TILENUMY];
	tagTile _temp[TILENUMX][TILENUMY];
	tagTile tempTile;
	POINT _currentTile;
	POINT _start;

	image* _tileGraass;
	image* _tileRock;
	image* _tileStones;
	image* _tileGraassTop;
	image* _tileRockTop;
	image* _tileStonesTop;
	image* _tileWater;
	image* _active;
	image* _tileDeepTile;
	image* _tileLightTile;
	image* _tileGraveTile;
	image* _startTile;

	image* _obj;

	int _cameraX, _cameraY;

	int _magicCicleAlpha;
	bool _alphaSwitch;

	bool _cameraTurn;

	bool addObj;
	bool addObj2;

	bool re;

	vector<POINT> OpenableTileV;
	vector<POINT>::iterator OpenableTileVI;

	vector<tagTile> _vOpenList;
	vector<tagTile>::iterator _viOpenList;

	vector<POINT> _vPathList;
	vector<POINT>::iterator _viPathList;

	int OpenableTileVSize;
	int PreviousOpenableTileVSize;

public:
	HRESULT init(void);
	void release(void);
	void update();
	void render(int i,int j);

	bool getCameraTurn() { return _cameraTurn; }
	void setCameraTurn(bool turn) { _cameraTurn = turn; }

	void cameraTurnRight(int i, int j);
	void cameraTurnLeft(int i, int j);

	void load(void);

	vector<POINT> GetPathsV() { return _vPathList; }
	vector<POINT>::iterator GetPathsVI() { return _viPathList; }
	vector<POINT>* GetPathsVAddress() { return &_vPathList; }
	vector<POINT>::iterator* GetPathsVIAddres() { return &_viPathList; }

	vector<POINT> addOpenableTileV(POINT currentTile, int movableDistance, bool loop);
	void SearchMovableTile(tagTile tile, POINT currentTile, bool enemy);
	void ClearMoveableTile(void);

	void SearchAttackableTile(POINT currentTile);

	float costAdd(POINT currentTile, POINT endTile);
	vector<POINT> pathFinder(POINT currentTile, POINT endTile, int movableDistance);

	void onTheTileReset(void);

	void clearAllList(void);

	battlemap();
	~battlemap();

	tagTile* getTiles(int x, int y) { return &_tiles[x][y]; }
	
	//tagTile GetTile(int indexX, int indexY) { return _tiles[indexX][indexY]; }
	//tagTile* GetTileAddress(int indexX, int indexY) { return &_tiles[indexX][indexY]; }
	//
	//tagTile GetTiles() { return **_tiles; }
	//tagTile* GetTilesAddressA() { return &**_tiles; }
};