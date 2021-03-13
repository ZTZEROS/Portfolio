#include "stdafx.h"
#include "HealerMales.h"

HealerMales::HealerMales() {}
HealerMales::~HealerMales() {}

HRESULT HealerMales::Initialize(string characterName, int tileX, int tileY)
{
	Enemies::Initialize(characterName, tileX, tileY);

	return S_OK;
}

void HealerMales::Release()
{
	Enemies::Release();
}

void HealerMales::Update()
{
	Enemies::Update();
}

void HealerMales::Action()
{
	Enemies::Action();
}

void HealerMales::Render()
{
	Enemies::Render();
}