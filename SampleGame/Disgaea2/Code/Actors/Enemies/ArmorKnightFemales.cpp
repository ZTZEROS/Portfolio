#include "stdafx.h"
#include "ArmorKnightFemales.h"

ArmorKnightFemales::ArmorKnightFemales() {}
ArmorKnightFemales::~ArmorKnightFemales() {}

HRESULT ArmorKnightFemales::Initialize(string characterName, int tileX, int tileY)
{
	Enemies::Initialize(characterName, tileX, tileY);

	return S_OK;
}

void ArmorKnightFemales::Release()
{
	Enemies::Release();
}

void ArmorKnightFemales::Update()
{
	Enemies::Update();
}

void ArmorKnightFemales::Action()
{
	Enemies::Action();
}

void ArmorKnightFemales::Render()
{
	Enemies::Render();
}

ArmorKnightFemales2::ArmorKnightFemales2() {}
ArmorKnightFemales2::~ArmorKnightFemales2() {}

HRESULT ArmorKnightFemales2::Initialize(string characterName, int tileX, int tileY)
{
	Enemies::Initialize(characterName, tileX, tileY);

	return S_OK;
}

void ArmorKnightFemales2::Release()
{
	Enemies::Release();
}

void ArmorKnightFemales2::Update()
{
	Enemies::Update();
}

void ArmorKnightFemales2::Action()
{
	Enemies::Action();
}

void ArmorKnightFemales2::Render()
{
	Enemies::Render();
}