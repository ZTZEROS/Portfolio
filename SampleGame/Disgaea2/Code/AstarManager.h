#pragma once
#include "gameNode.h"
#include "tileNode.h"


class AstarManager : public gameNode
{
private:
	vector<tagTile*> TotalMapTileV;
	vector<tagTile*>::iterator TotalMapTileVI;

	vector<tagTile*> OpenedTileV;
	vector<tagTile*>::iterator OpenedTileVI;

	vector<tagTile*> ClosedTileV;
	vector<tagTile*>::iterator ClosedTileVI;

	tagTile* StartTile;
	tagTile* EndTile;
	tagTile* CurrentTile;

	int Count;
	bool Start;

public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);

	void SetTiles(void);

	vector<tagTile*> AddOpenedTile(tagTile* currentTile);
	//vector<tagTile*> OpenTile(tagTile* currentTile);

	void PathFinder(tagTile* currentTile);
	
	

	AstarManager();
	~AstarManager();
};

