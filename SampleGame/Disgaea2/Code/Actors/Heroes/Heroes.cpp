#include "stdafx.h"
#include "Heroes.h"

Heroes::Heroes() {}
Heroes::~Heroes() {}

HRESULT Heroes::Initialize(string characterName, int tileX, int tileY)
{
	HeroesDNAs::Initialize(characterName, tileX, tileY);

	return S_OK;
}

void Heroes::Release()
{
	HeroesDNAs::Release();
}

void Heroes::Update()
{
	HeroesDNAs::Update();
}

void Heroes::Action()
{
	HeroesDNAs::Action();
}

void Heroes::Render()
{
	HeroesDNAs::Render();
}
