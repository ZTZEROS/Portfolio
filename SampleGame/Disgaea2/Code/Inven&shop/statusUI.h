
#pragma once
#include "../gameNode.h"
#include "../image.h"

class statusUI : public gameNode 
{
private:
	image* _img;
	COLORREF _color;


public:
	statusUI();
	~statusUI();

	HRESULT init(void);
	virtual void release(void);
	void update(void);
	void render(void);



};
