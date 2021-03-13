#pragma once
#include "Character.h"
#include "HeightMap.h"

class Character_HeightMapMove :
	public Character
{
public:
	Character_HeightMapMove();
	virtual ~Character_HeightMapMove();

	virtual void Update() override;

	inline void SetHeightMap(HeightMap* _h){ heightMap = _h; }

protected:
	HeightMap* heightMap = nullptr;

};

