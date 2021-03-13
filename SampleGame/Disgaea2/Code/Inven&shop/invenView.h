
#pragma once
#include "../gameNode.h"
#include "inventory.h"
#include "wareHouse.h"
#include "itemList.h"

class invenView : public gameNode
{
private:
	inventory* _inven;
	wareHouse* _house;
	itemInfo* _info;

public:
	invenView();
	~invenView();
	
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);

};

