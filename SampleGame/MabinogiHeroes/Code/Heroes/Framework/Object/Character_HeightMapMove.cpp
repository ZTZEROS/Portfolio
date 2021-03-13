#include "stdafx.h"
#include "Character_HeightMapMove.h"


Character_HeightMapMove::Character_HeightMapMove()
{
}


Character_HeightMapMove::~Character_HeightMapMove()
{
}

void Character_HeightMapMove::Update()
{
	Character::Update();
	if (heightMap)
	{
		heightMap->GetHeight(position.y, position.x, position.z);
	}
}