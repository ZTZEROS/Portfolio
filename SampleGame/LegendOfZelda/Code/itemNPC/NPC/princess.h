#pragma once
#include "npc.h"

class princess : public npc
{
private:
	image _image;
	int _framecount;	//프레임 카운트
	int _remainder;		//프레임 속도
	RECT _prinRc;
	
	bool pause;
	int speed;

	int procedure;
	RECT rt;

	int princessState;
public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);

	void framePlay();

	void move(void);
	void chat(void);

	bool getpause() { return pause; }
	int getprincessState() { return _nState; }
	void SetprincessState(NPCSTATE princessst) { _nState = princessst; }
	princess();
	~princess();
};
