#include "stdafx.h"
#include "Enemies.h"

Enemies::Enemies() {}
Enemies::~Enemies() {}

HRESULT Enemies::Initialize(string characterName, int tileX, int tileY)
{
	EnemiesDNAs::Initialize(characterName, tileX, tileY);
	
	return S_OK;
}

void Enemies::Release()
{
	EnemiesDNAs::Release();
}

void Enemies::Update()
{
	EnemiesDNAs::Update();
}

void Enemies::Action()
{
	EnemiesDNAs::Action();
}

void Enemies::Render()
{
	EnemiesDNAs::Render();
}
