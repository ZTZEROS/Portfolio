
#pragma once
#include "../gameNode.h"
#include "itemList.h"

class itemInfo : public gameNode
{
private:
	strItemInfo _sinfo;
	image* _itemInfoImage;
	image* _textEx;

public:
	itemInfo();
	~itemInfo();

	virtual HRESULT init(void);
	virtual void update(strItemInfo sinfo);
	virtual void render(void);

};
