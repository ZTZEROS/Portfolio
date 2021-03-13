#include "stdafx.h"
#include "HeroesDNAs.h"

HeroesDNAs::HeroesDNAs() {}
HeroesDNAs::~HeroesDNAs() {}

HRESULT HeroesDNAs::Initialize(string characterName, int tileX, int tileY)
{
	DNAs::Initialize(characterName, tileX, tileY);
	DNA.CurrentDirection = is_RIGHT_UP;
	DNA.MovableDistance += 1;


	return S_OK;
}

void HeroesDNAs::Release()
{
	DNAs::Release();
}

void HeroesDNAs::Update()
{
	DNAs::Update();
}

void HeroesDNAs::Action()
{
	DNAs::Action();
}

void HeroesDNAs::Render()
{
	DNAs::Render();
}
