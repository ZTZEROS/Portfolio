#include "stdafx.h"
#include "Flonnes.h"

Flonnes::Flonnes() {}
Flonnes::~Flonnes() {}

HRESULT Flonnes::Initialize(string characterName, int tileX, int tileY)
{
	//Heroes::Initialize(characterName,  tileX, tileY);
	Heroes::Initialize("Flonne", tileX, tileY);

	return S_OK;
}

void Flonnes::Release()
{
	Heroes::Release();
}

void Flonnes::Update()
{
	Heroes::Update();
}

void Flonnes::Render()
{
	Heroes::Render();
}