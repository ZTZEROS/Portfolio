#include "stdafx.h"
#include "Barricade.h"


Barricade::Barricade()
{
}


Barricade::~Barricade()
{
}

void Barricade::Initialize(D3DXVECTOR3& pos, float radius /*= 1.0f*/)
{
	position = pos;
	localPivot = D3DXVECTOR3(0, 0, 0);

	bs.radius = radius;
	bs.center = position + localPivot;

	D3DXCreateSphere(GameManager::GetDevice(), radius, 16, 8, &barricadeMesh, nullptr);
	D3DXCreateSphere(GameManager::GetDevice(), bs.radius, 16, 8, &bsMesh, nullptr);

	D3DXMatrixIdentity(&world);

	D3DXMATRIXA16 local;
	D3DXMatrixTranslation(&local, localPivot.x, localPivot.y, localPivot.z);
	world *= local;

	D3DXMATRIXA16 trans;
	D3DXMatrixTranslation(&trans, position.x, position.y, position.z);
	world *= trans;

	D3DXMatrixIdentity(&boundingWorld);
	D3DXMATRIXA16 boundingTrans;
	D3DXMatrixTranslation(&boundingTrans, position.x, position.y, position.z);
	boundingWorld *= boundingTrans;


	ZeroMemory(&baseMaterial, sizeof(D3DMATERIAL9));
	baseMaterial.Ambient = D3DXCOLOR(0.0f, 0.8f, 0.0f, 1.0f);
	baseMaterial.Diffuse = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
}

void Barricade::Destroy()
{
	SAFE_RELEASE(barricadeMesh);
	SAFE_RELEASE(bsMesh);
}

void Barricade::Render()
{
	if (barricadeMesh)
	{
		GameManager::GetDevice()->SetRenderState(D3DRS_LIGHTING, true);
		GameManager::GetDevice()->SetMaterial(&baseMaterial);
		GameManager::GetDevice()->SetTexture(0, nullptr);
		GameManager::GetDevice()->SetTransform(D3DTS_WORLD, &world);
		barricadeMesh->DrawSubset(0);
	}
	if (bsMesh)
	{
		GameManager::GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		GameManager::GetDevice()->SetRenderState(D3DRS_LIGHTING, false);
		GameManager::GetDevice()->SetTexture(0, nullptr);
		GameManager::GetDevice()->SetTransform(D3DTS_WORLD, &boundingWorld);
		bsMesh->DrawSubset(0);
		GameManager::GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}
}
