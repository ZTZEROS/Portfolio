#pragma once
#include "gameNode.h"
#include "itemNPC\NPC\npc.h"

struct tagPlayer
{
	RECT rc;
	int x, y;
	int count;
};
class testScene :public gameNode
{
private:
	npc* _npc;
	tagPlayer _fakePlayer;

public:
	HRESULT init(void);
	void update(void);
	void release(void);
	void render(void);

	testScene();
	~testScene();
};

