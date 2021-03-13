#pragma once
#include "gameNode.h"
#include "tileNode.h"

#define MAPTILEWIDTH 64			//타일 가로길이
#define MAPTILEHEIGHT 32			//타일 세로길이

class maptool : public gameNode
{
private:
	tagTile _tiles[TILENUMX][TILENUMY];

	image* _tileGraass;
	image* _tileRock;
	image* _tileStones;
	image* _tileGraassTop;
	image* _tileRockTop;
	image* _tileStonesTop;
	image* _tileWater;
	image* _tileDeepTile;
	image* _tileLightTile;
	image* _tileGraveTile;
	image* _startTile;

	image* _obj;

	int _cameraX, _cameraY;
	int _nowMapX, _nowMapY;
	int _nowBrush;
	int _nowAltitude;

	bool _gride;
	bool _erase;

	RECT _cameraUp, _cameraDown, _cameraLeft, _cameraRight;
	RECT _tileSampleMoveLeft, _tileSampleMoveRight;
	RECT _mapArea;
	RECT _sample;
	RECT _altitudeDown, _altitudeUp;
	RECT _eraseButton;
	RECT _save, _load;

public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);

	void save(void);
	void load(void);

	tagTile* getTiles() { return *_tiles; }

	maptool();
	~maptool();
};

