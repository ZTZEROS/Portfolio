#pragma once
#include "../gameNode.h"
#include "../UIstat.h"
#include "statinven.h"

class equipmentView : public gameNode
{
private:
	UIstat* _us;
	statinven* _inven;

public:
	equipmentView();
	~equipmentView();

	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);

};
