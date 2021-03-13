#include "stdafx.h"
#include "Collider_Sphere.h"


Collider_Sphere::Collider_Sphere()
{
}


Collider_Sphere::~Collider_Sphere()
{
}

void Collider_Sphere::Initialize(D3DXVECTOR3& center, float radius, D3DXMATRIXA16* _parent /*= nullptr*/)
{
	SAFE_RELEASE(sphereMesh);
	boundingSphere.center = center;
	boundingSphere.radius = radius;
	D3DXCreateSphere(GameManager::GetDevice(), boundingSphere.radius, 16, 8, &sphereMesh, nullptr);

	D3DXMatrixIdentity(&world);
	parent = _parent;

	ZeroMemory(&material, sizeof(D3DMATERIAL9));
	material.Ambient = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	material.Diffuse = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	material.Specular = D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f);
}

void Collider_Sphere::Destroy()
{
	SAFE_RELEASE(sphereMesh);
}

void Collider_Sphere::Update()
{
	D3DXMatrixTranslation(&world, boundingSphere.center.x, boundingSphere.center.y, boundingSphere.center.z);
	if ( parent )
	{
		world *= ( *parent );
	}
}

void Collider_Sphere::Render()
{
	if ( wireMode )
	{
		GameManager::GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	}
	GameManager::GetDevice()->SetRenderState(D3DRS_LIGHTING, true);
	GameManager::GetDevice()->SetTexture(0, nullptr);
	GameManager::GetDevice()->SetMaterial(&material);
	GameManager::GetDevice()->SetTransform(D3DTS_WORLD, &world);
	if (sphereMesh != nullptr) sphereMesh->DrawSubset(0);
	if ( wireMode )
	{
		GameManager::GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}

}
