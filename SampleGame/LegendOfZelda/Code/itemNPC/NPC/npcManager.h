#pragma once
#include "../../gameNode.h"
#include "uncle.h"
#include "princess.h"
#include "soldier.h"
#include "priest.h"

class npcManager : public gameNode
{
private:

public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);

	void setUncle(void);
	void setPrincess(void);
	void setSoldier(void);
	void setPriest(void);

	npcManager();
	~npcManager();
};