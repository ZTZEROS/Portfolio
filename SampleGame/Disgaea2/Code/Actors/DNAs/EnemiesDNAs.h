#pragma once
#include "DNAs.h"

struct EnemiesDNAsStructure
{
	int DistanceToHeroes;
};

class EnemiesDNAs : public DNAs
{
protected:
	EnemiesDNAsStructure EnemiesDNA;
	//Consideration[100];

public:
	virtual HRESULT Initialize(string characterName, int tileX, int tileY);
	virtual void Release();
	virtual void Update();
	virtual void Render();

	virtual void Action();

	virtual void FundamentalAI();
	virtual void GeneticAI();
	virtual void SelectDestination();
	
	EnemiesDNAsStructure GetEnemiesDNA() { return EnemiesDNA; }

	void SetEnemiesDNA(EnemiesDNAsStructure enemyDNA) { EnemiesDNA = enemyDNA; }

	EnemiesDNAs();
	~EnemiesDNAs();
};

