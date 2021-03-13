#pragma once
#include "../DNAs/HeroesDNAs.h"

struct HeroesStructure
{

};

class Heroes : public HeroesDNAs
{
protected:
	HeroesStructure Hero;

public:
	virtual HRESULT Initialize(string characterName, int tileX, int tileY);
	virtual void Release();
	virtual void Update();
	virtual void Render();

	virtual void Action();

	HeroesStructure GetHero() { return Hero; }

	void SetHero(HeroesStructure hero) { Hero = hero; }

	Heroes();
	~Heroes();
};

