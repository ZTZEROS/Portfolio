#pragma once
#include "../DNAs/EnemiesDNAs.h"

struct EnemiesStructure
{

};

class Enemies : public EnemiesDNAs
{
protected:
	EnemiesStructure Enemy;

public:
	virtual HRESULT Initialize(string characterName, int centerX, int centerY);
	virtual void Release();
	virtual void Update();
	virtual void Render();
	
	virtual void Action();

	EnemiesStructure GetEnemy() { return Enemy; }

	void SetEnemy(EnemiesStructure enemy) { Enemy = enemy; }

	Enemies();
	~Enemies();
};

