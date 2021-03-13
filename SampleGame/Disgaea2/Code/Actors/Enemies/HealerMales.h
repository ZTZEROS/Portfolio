#pragma once
#include "Enemies.h"

class HealerMales :	public Enemies
{
public:
	virtual HRESULT Initialize(string characterName, int tileX, int tileY);
	virtual void Release();
	virtual void Update();
	virtual void Render();

	virtual void Action();

	HealerMales();
	~HealerMales();
};

