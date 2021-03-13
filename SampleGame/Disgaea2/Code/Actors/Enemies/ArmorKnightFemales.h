#pragma once
#include "Enemies.h"

class ArmorKnightFemales : public Enemies
{
public:
	virtual HRESULT Initialize(string characterName, int tileX, int tileY);
	virtual void Release();
	virtual void Update();
	virtual void Render();

	virtual void Action();

	ArmorKnightFemales();
	~ArmorKnightFemales();
};

class ArmorKnightFemales2 : public Enemies
{
public:
	virtual HRESULT Initialize(string characterName, int tileX, int tileY);
	virtual void Release();
	virtual void Update();
	virtual void Render();

	virtual void Action();

	ArmorKnightFemales2();
	~ArmorKnightFemales2();
};