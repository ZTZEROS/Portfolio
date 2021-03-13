#pragma once
#include "npc.h"
enum PRIESTSTATE
{
	//STATE_UP,
	//STATE_DOWN,
	//STATE_LEFT,
	//STATE_RIGHT,
	//STATE_END
};

class priest : public npc
{
private:
	PRIESTSTATE _pState;
	image* _imgpriest;
	int _framecount;
	RECT _priRc;
	float _x;
	float _y;
public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);

	void move(void);
	void chat(void);

	priest();
	~priest();
};
