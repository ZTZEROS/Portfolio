#include "stdafx.h"
#include "Axels.h"

Axels::Axels() {}
Axels::~Axels() {}

HRESULT Axels::Initialize(string characterName, int tileX, int tileY)
{
	//Heroes::Initialize(characterName, tileX, tileY);
	Heroes::Initialize("Axel", tileX, tileY);

	return S_OK;
}

void Axels::Release()
{
	Heroes::Release();
}

void Axels::Update()
{
	Heroes::Update();
}

void Axels::Action()
{
	Heroes::Action();
}

void Axels::Render()
{
	Heroes::Render();
}