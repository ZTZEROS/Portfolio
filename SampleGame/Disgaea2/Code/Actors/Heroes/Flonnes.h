#pragma once
#include "Heroes.h"

class Flonnes : public Heroes
{
private:

public:
	virtual HRESULT Initialize(string characterName, int tileX, int tileY);
	virtual void Release();
	virtual void Update();
	virtual void Render();

	DNAsStructure GetDNA() { return DNA; }

	Flonnes();
	~Flonnes();
};

