#include "stdafx.h"
#include "Adells.h"

Adells::Adells() {}
Adells::~Adells() {}

HRESULT Adells::Initialize(string characterName, int tileX, int tileY)
{
	//Heroes::Initialize(characterName, tileX, tileY);
	Heroes::Initialize("Adell", tileX, tileY);

	return S_OK;
}

void Adells::Release()
{
	Heroes::Release();
}

void Adells::Update()
{
	Heroes::Update();
}

void Adells::Action()
{
	Heroes::Action();
}

void Adells::Render()
{
	Heroes::Render();
}