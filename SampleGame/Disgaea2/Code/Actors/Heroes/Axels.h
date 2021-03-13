#pragma once
#include "Heroes.h"

class Axels : public Heroes
{
public:
	virtual HRESULT Initialize(string characterName, int tileX, int tileY);
	virtual void Release();
	virtual void Update();
	virtual void Render();

	virtual void Action();

	Axels();
	~Axels();
};

