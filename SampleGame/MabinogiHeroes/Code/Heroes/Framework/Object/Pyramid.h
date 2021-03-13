#pragma once
#include "../Triangle.h"

class Pyramid
{
public:
	Pyramid();
	~Pyramid();

	void Initialize(D3DXCOLOR color = D3DCOLOR_XRGB(0, 0, 0), D3DXMATRIXA16* directionMatrix = nullptr);
	void Destroy();
	void Render();

private:
	static const int TRIANGLE_COUNT = 6;
	NormalTriangle triangles[TRIANGLE_COUNT];
	D3DXMATRIXA16 world;

	D3DMATERIAL9 material;
};

