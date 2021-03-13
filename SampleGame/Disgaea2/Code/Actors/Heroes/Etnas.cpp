#include "stdafx.h"
#include "Etnas.h"

Etnas::Etnas() {}
Etnas::~Etnas() {}

HRESULT Etnas::Initialize(string characterName, int tileX, int tileY)
{
	//Heroes::Initialize(characterName, tileX, tileY);
	Heroes::Initialize("Etna", tileX, tileY);

	return S_OK;
}

void Etnas::Release()
{
	Heroes::Release();
}

void Etnas::Update()
{
	Heroes::Update();
}

void Etnas::Render()
{
	Heroes::Render();
}