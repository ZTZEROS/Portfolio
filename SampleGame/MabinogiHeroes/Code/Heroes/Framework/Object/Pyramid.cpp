#include "stdafx.h"
#include "Pyramid.h"


Pyramid::Pyramid()
{
}


Pyramid::~Pyramid()
{
	Destroy();
}

void Pyramid::Initialize(D3DXCOLOR color /*= D3DCOLOR_XRGB(0, 0, 0)*/, D3DXMATRIXA16* directionMatrix /*= nullptr*/)
{
	ZeroMemory(&material, sizeof(D3DMATERIAL9));
	material.Ambient = color;
	material.Diffuse = color;
	material.Specular = color;

	const int VERTEX_COUNT = 5;
	D3DXVECTOR3 vertex[VERTEX_COUNT];
	vertex[0] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[1] = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
	vertex[2] = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);
	vertex[3] = D3DXVECTOR3(1.0f, -1.0f, 1.0f);
	vertex[4] = D3DXVECTOR3(1.0f, -1.0f, -1.0f);

	D3DXVECTOR3 dir1, dir2, normal;

	dir1 = vertex[1] - vertex[0];
	dir2 = vertex[2] - vertex[0];
	D3DXVec3Cross(&normal, &dir1, &dir2);
	triangles[0] = NormalTriangle(vertex[0], vertex[1], vertex[2], normal);

	dir1 = vertex[2] - vertex[0];
	dir2 = vertex[3] - vertex[0];
	D3DXVec3Cross(&normal, &dir1, &dir2);
	triangles[1] = NormalTriangle(vertex[0], vertex[2], vertex[3], normal);

	dir1 = vertex[3] - vertex[0];
	dir2 = vertex[4] - vertex[0];
	D3DXVec3Cross(&normal, &dir1, &dir2);
	triangles[2] = NormalTriangle(vertex[0], vertex[3], vertex[4], normal);

	dir1 = vertex[4] - vertex[0];
	dir2 = vertex[1] - vertex[0];
	D3DXVec3Cross(&normal, &dir1, &dir2);
	triangles[3] = NormalTriangle(vertex[0], vertex[4], vertex[1], normal);

	dir1 = vertex[4] - vertex[1];
	dir2 = vertex[4] - vertex[1];
	D3DXVec3Cross(&normal, &dir1, &dir2);
	triangles[4] = NormalTriangle(vertex[1], vertex[4], vertex[3], normal);

	dir1 = vertex[3] - vertex[1];
	dir2 = vertex[2] - vertex[1];
	D3DXVec3Cross(&normal, &dir1, &dir2);
	triangles[5] = NormalTriangle(vertex[1], vertex[3], vertex[2], normal);

	if (directionMatrix)
	{
		world = (*directionMatrix);
	}
	else
	{
		D3DXMatrixIdentity(&world);
	}

}

void Pyramid::Destroy()
{

}

void Pyramid::Render()
{
	GameManager::GetDevice()->SetTransform(D3DTS_WORLD, &world);
	GameManager::GetDevice()->SetMaterial(&material);
	GameManager::GetDevice()->SetFVF(FVF_PositionNormal::FVF);
	GameManager::GetDevice()->DrawPrimitiveUP(
		D3DPT_TRIANGLELIST,
		TRIANGLE_COUNT,
		&triangles[0],
		sizeof(FVF_PositionNormal));
}