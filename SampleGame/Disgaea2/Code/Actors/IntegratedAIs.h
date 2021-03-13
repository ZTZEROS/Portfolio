#pragma once
#include "EnemiesManagers.h"

class EnemiesManagers;

struct ActorsStructure
{
	int MaxHP, MaxSP;
	int CurrentHP, CurrentSP;
	int Attack, Defence;
	int Hit, Speed;
	int Magic, Resistance;
	POINT Position;
};

struct StatusesStructure
{
	ActorsStructure OfEnemies;
	ActorsStructure OfHeroes;
};

class IntegratedAIs //Integrated Artificial Intelligences
{
private:
	StatusesStructure Sum;
	StatusesStructure Average;
	
	POINT CenterOfEnemies;
	POINT CenterOfHeroes;
	
	bool Advantage;
	//Consideration[1000];

	EnemiesManagers* EM;

public:

	virtual void Initialize();
	virtual void Release();
	virtual void Update();
	virtual void Render();

	void FundamentalAI();
	void GeneticAI();
	void SelectDestination();

	POINT GetCenterOfEnemies() { return CenterOfEnemies; }
	POINT GetCenterOfHeroes() { return CenterOfHeroes; }

	void SetEMAddress(EnemiesManagers* em) { EM = em; }

	IntegratedAIs();
	~IntegratedAIs();
};

void IntegratedFundamentalAI();
void IntegratedGeneticAI();
void SelectDestination();