#pragma once
#include "npc.h"
#include "../../link/link.h"

class uncle : public npc
{
private:
	
	image _image;
	int _framecount;	//������ ī��Ʈ
	int _remainder;		//������ �ӵ�
	RECT _uncRc;
	int _frameSpeed;
	int _unclestate;

	int speed;

	int procedure;
	RECT rt;	//��ȭâ

public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);

	void framePlay();

	void move(void);
	void chat(void);

	int getuncleState() { return _nState; }
	void setUncleState(NPCSTATE unclestate) { _nState = unclestate; }

	uncle();
	~uncle();
};
