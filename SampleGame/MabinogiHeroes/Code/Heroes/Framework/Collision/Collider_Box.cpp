#include "stdafx.h"
#include "Collider_Box.h"


Collider_Box::Collider_Box()
{
}


Collider_Box::~Collider_Box()
{
}

void Collider_Box::Initialize(D3DXVECTOR3& min, D3DXVECTOR3& max, D3DXMATRIXA16* _parent/* = nullptr*/)
{
	SAFE_RELEASE(aabbMesh);
	boundingBox = BoundingBox(min, max);
	D3DXVECTOR3 boxSize = boundingBox.max - boundingBox.min;
	
	D3DXCreateBox(GameManager::GetDevice(), boxSize.x, boxSize.y, boxSize.z, &aabbMesh, nullptr);
	//LPD3DXMESH tempMesh;
	//D3DXCreateBox(GameManager::GetDevice(), boxSize.x, boxSize.y, boxSize.z, &tempMesh, nullptr);
	//tempMesh->CloneMeshFVF(D3DXMESH_SYSTEMMEM, FVF_PositionNormalTexture::FVF, GameManager::GetDevice(), &aabbMesh);
	//tempMesh->Release();

	baseHeight = ( boundingBox.max.y - boundingBox.min.y ) * 0.5f;
	D3DXMatrixIdentity(&world);
	parent = _parent;

	ZeroMemory(&material, sizeof(D3DMATERIAL9));
	material.Ambient = D3DXCOLOR(0.0f, 0.8f, 0.8f, 1.0f);
	material.Diffuse = D3DXCOLOR(0.0f, 0.8f, 0.8f, 1.0f);
	material.Specular = D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f);
}

void Collider_Box::Destroy()
{
	SAFE_RELEASE(aabbMesh);
}

void Collider_Box::Update()
{
	D3DXMatrixTranslation(&world, position.x, position.y + baseHeight, position.z);
	if ( parent )
	{
		world *= ( *parent );
	}
	D3DXVec3TransformCoord(&currentBoundingBox.min, &boundingBox.min, &world);
	D3DXVec3TransformCoord(&currentBoundingBox.max, &boundingBox.max, &world);
}

void Collider_Box::Render()
{
	if ( wireMode )
	{
		GameManager::GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	}
	GameManager::GetDevice()->SetRenderState(D3DRS_LIGHTING, true);
	GameManager::GetDevice()->SetTexture(0, nullptr);
	GameManager::GetDevice()->SetMaterial(&material);
	GameManager::GetDevice()->SetTransform(D3DTS_WORLD, &world);
	if(aabbMesh != nullptr) aabbMesh->DrawSubset(0);
	if ( wireMode )
	{
		GameManager::GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}
}
