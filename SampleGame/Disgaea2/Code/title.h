#pragma once
#include "gameNode.h"

class title : public gameNode
{

private:
	image* title_bg;
	image* title_ban1_0;
	image* title_ban1_1;
	image* title_ban2_0;
	image* title_ban2_1;

	int iSelected;
	bool menuStart;

public:
	title();
	~title();

	HRESULT init(void);
	void update(void);
	void release(void);
	void render(void);

};

