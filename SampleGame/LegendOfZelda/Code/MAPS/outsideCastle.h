#pragma once
#include "maps.h"

class outsideCastle : public maps
{
private:

public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);

	outsideCastle();
	~outsideCastle();
};