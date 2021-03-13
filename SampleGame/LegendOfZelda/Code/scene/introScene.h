#pragma once
#include "../gameNode.h"
class introScene : public gameNode
{
private:
	
public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);



	introScene();
	~introScene();
};

