#include "stdafx.h"
#include "PathNode.h"


PathNode::PathNode()
{
}


PathNode::~PathNode()
{
}

void PathNode::Initialize(D3DXVECTOR3& pos, float radius/* = 0.5f*/)
{
	position = pos;

	D3DXCreateSphere(GameManager::GetDevice(), radius, 16, 8, &nodeMesh, nullptr);

	D3DXMatrixIdentity(&world);

	D3DXMATRIXA16 local;
	D3DXMatrixTranslation(&local, localPivot.x, localPivot.y, localPivot.z);
	world *= local;

	D3DXMATRIXA16 trans;
	D3DXMatrixTranslation(&trans, position.x, position.y, position.z);
	world *= trans;

	ZeroMemory(&baseMaterial, sizeof(D3DMATERIAL9));
	baseMaterial.Ambient = D3DXCOLOR(0.8f, 0.0f, 0.0f, 1.0f);
	baseMaterial.Diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

}

void PathNode::Destroy()
{
	SAFE_RELEASE(nodeMesh);
}

void PathNode::Render()
{
	if (nodeMesh)
	{
		GameManager::GetDevice()->SetRenderState(D3DRS_LIGHTING, true);
		GameManager::GetDevice()->SetMaterial(&baseMaterial);
		GameManager::GetDevice()->SetTexture(0,nullptr);
		GameManager::GetDevice()->SetTransform(D3DTS_WORLD, &world);
		nodeMesh->DrawSubset(0);
	}
}
