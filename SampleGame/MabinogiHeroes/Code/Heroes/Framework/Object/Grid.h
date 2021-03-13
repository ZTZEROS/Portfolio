#pragma once
#include "../FVF.h"
#include "Pyramid.h"

class Grid
{
public:
	Grid();
	~Grid();

	void Initialize(int lineCount = 30, float interval = 1.0f);
	void Destroy();
	void Render();

	const std::vector<D3DXVECTOR3>& GetGroundVertex() { return groundVertex; }

	inline D3DXMATRIXA16 GetWorld() { return world; }
	inline void SetShowGround(bool show) { showGround = show; }
	inline void SetShowArrow(bool show) { showArrow = show; }

private:
	std::vector<FVF_PositionColor>	vertices;
	std::vector<D3DXVECTOR3> groundVertex;

	static const int axisCount = 3;
	Pyramid* axisArrow[axisCount];

	D3DMATERIAL9 groundMaterial;
	D3DXMATRIXA16 world;

	bool showGround = false;
	bool showArrow = false;
};

