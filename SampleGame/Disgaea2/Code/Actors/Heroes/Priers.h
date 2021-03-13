#pragma once
#include "Heroes.h"

class Priers : public Heroes
{
public:
	virtual HRESULT Initialize(string characterName, int tileX, int tileY);
	virtual void Release();
	virtual void Update();
	virtual void Render();

	virtual void Action();

	Priers();
	~Priers();
};

