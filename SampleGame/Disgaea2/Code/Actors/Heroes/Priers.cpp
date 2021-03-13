#include "stdafx.h"
#include "Priers.h"

Priers::Priers() {}
Priers::~Priers() {}

HRESULT Priers::Initialize(string characterName, int tileX, int tileY)
{
	//Heroes::Initialize(characterName, tileX, tileY);
	Heroes::Initialize("Prier", tileX, tileY);

	return S_OK;
}

void Priers::Release()
{
	Heroes::Release();
}

void Priers::Update()
{
	Heroes::Update();
}

void Priers::Action()
{
	Heroes::Action();
}

void Priers::Render()
{
	Heroes::Render();
}