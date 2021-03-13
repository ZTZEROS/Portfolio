#pragma once
#include "gameNode.h"
#include "maptool.h"
#include "battleScene.h"
#include "townmap.h"
#include "title.h"

class playGround : public gameNode
{
private:
	maptool* _maptool;

	battleScene* _battleScene;

	title* _title;
	string* sSceneName;
	townmap* _townmap;

	image* _logoImg;

	bool _shake1, _shake2;
	bool _isInStore;
	int _shakeX, _shakeY;
	int _shakeCount;

private:

public:
	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);
	playGround();
	~playGround();
};

