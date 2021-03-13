#include "stdafx.h"
#include "EnemiesDNAs.h"

EnemiesDNAs::EnemiesDNAs() {}
EnemiesDNAs::~EnemiesDNAs() {}

HRESULT EnemiesDNAs::Initialize(string characterName, int tileX, int tileY)
{
	DNAs::Initialize(characterName, tileX, tileY);
	DNA.CurrentDirection = is_LEFT_DOWN;
	DNA.MaxHP = DNA.CurrentHP = 200;

	return S_OK;
}

void EnemiesDNAs::Release()
{
	DNAs::Release();
}

void EnemiesDNAs::Update()
{
	DNAs::Update();
}

void EnemiesDNAs::Render()
{
	DNAs::Render();
}


void EnemiesDNAs::Action()
{
	DNAs::Action();
}

void EnemiesDNAs::FundamentalAI()
{

}

void EnemiesDNAs::GeneticAI()
{

}

void EnemiesDNAs::SelectDestination()
{
	
}