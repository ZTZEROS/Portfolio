#pragma once
#include "npc.h"

class soldier : public npc
{
private:

	image _image;
	int _framecount;	//������ ī��Ʈ
	int _remainder;		//������ �ӵ�
	RECT _solRc;
	int _frameSpeed;
	int _soldierstate;

	int speed;

	int procedure;
	RECT rt;	//��ȭâ

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

