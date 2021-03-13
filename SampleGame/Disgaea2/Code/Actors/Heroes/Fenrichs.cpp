#include "stdafx.h"
#include "Fenrichs.h"

Fenrichs::Fenrichs() {}
Fenrichs::~Fenrichs() {}

HRESULT Fenrichs::Initialize(string characterName, int tileX, int tileY)
{
	//Heroes::Initialize(characterName, tileX, tileY);
	Heroes::Initialize("Fenrich", tileX, tileY);

	return S_OK;
}

void Fenrichs::Release()
{
	Heroes::Release();
}

void Fenrichs::Update()
{
	Heroes::Update();
}

void Fenrichs::Action()
{
	Heroes::Action();
}

void Fenrichs::Render()
{
	Heroes::Render();
}