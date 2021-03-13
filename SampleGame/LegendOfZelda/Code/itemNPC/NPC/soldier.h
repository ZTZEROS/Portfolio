#pragma once
#include "npc.h"

class soldier : public npc
{
private:

	image _image;
	int _framecount;	//프레임 카운트
	int _remainder;		//프레임 속도
	RECT _solRc;
	int _frameSpeed;
	int _soldierstate;

	int speed;

	int procedure;
	RECT rt;	//대화창

public:
	HRESULT init(int x, int y, NPCSTATE nState);
	void release(void);
	void update(void);
	void render(void);

	void framePlay();

	void move(void);
	void chat(void);

	soldier();
	~soldier();
};

