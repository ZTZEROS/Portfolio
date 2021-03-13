#pragma once
#include "DNAs.h"

struct HeroesDNAsStructure
{

};

class HeroesDNAs : public DNAs
{
protected:
	HeroesDNAsStructure HeroesDNA;

public:
	virtual HRESULT Initialize(string characterName, int tileX, int tileY);
	virtual void Release();
	virtual void Update();
	virtual void Render();

	virtual void Action();

	HeroesDNAsStructure GetHeroesDNA() { return HeroesDNA; }

	void SetHeroesDNA(HeroesDNAsStructure heroDNA) { HeroesDNA = heroDNA; }


	HeroesDNAs();
	~HeroesDNAs();
};

