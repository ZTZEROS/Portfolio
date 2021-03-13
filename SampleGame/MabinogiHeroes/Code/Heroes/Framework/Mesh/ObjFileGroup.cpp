#include "stdafx.h"
#include "ObjFileGroup.h"


ObjFileGroup::ObjFileGroup(MaterialTexture* _materialTexture, std::vector<FVF_PositionNormalTexture>& _vertex)
	:materialTexture(_materialTexture)
{
	if (materialTexture)
	{
		materialTexture->AddReference();
	}
	vertex = std::move(_vertex);

	//if ((vertex.size() % 3) == 0)
	//{
	//	groundVertex.resize(vertex.size());
	//
	//	for (unsigned int i = 0; i < groundVertex.size(); ++i)
	//	{
	//		if ((i % 3) == 0) groundVertex[i] = vertex[i];
	//		if ((i % 3) == 1) groundVertex[i + 1] = vertex[i];
	//		if ((i % 3) == 2) groundVertex[i - 1] = vertex[i];
	//	}
	//}

	//PrimitiveCount = vertex.size() - 2;
	PrimitiveCount = vertex.size() / 3;
}


ObjFileGroup::~ObjFileGroup()
{
	SAFE_RELEASE(materialTexture);
}

void ObjFileGroup::Render()
{
	if ( materialTexture )
	{
		GameManager::GetDevice()->SetMaterial(&( materialTexture->material ));
		GameManager::GetDevice()->SetTexture(0, materialTexture->texture);
	}
	else
	{
		D3DMATERIAL9 mat;
		ZeroMemory(&mat, sizeof(D3DMATERIAL9));
		mat.Ambient = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);
		mat.Diffuse = D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f);
		mat.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		GameManager::GetDevice()->SetMaterial(&mat);
		GameManager::GetDevice()->SetTexture(0, nullptr);
	}
	GameManager::GetDevice()->SetFVF(FVF_PositionNormalTexture::FVF);
	//GameManager::GetDevice()->DrawPrimitiveUP(PrimitiveType, PrimitiveCount, &vertex[0], sizeof(FVF_PositionNormalTexture));
	//GameManager::GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, vertex.size() - 2, &vertex[0], sizeof(FVF_PositionNormalTexture));
	//if (groundVertex.size() > 0) GameManager::GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, groundVertex.size() / 3, &groundVertex[0], sizeof(FVF_PositionNormalTexture));
	GameManager::GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, vertex.size() / 3, &vertex[0], sizeof(FVF_PositionNormalTexture));
}

void ObjFileGroup::RenderGround()
{
	if (materialTexture)
	{
		GameManager::GetDevice()->SetMaterial(&(materialTexture->material));
		GameManager::GetDevice()->SetTexture(0, materialTexture->texture);
	}
	else
	{
		D3DMATERIAL9 mat;
		ZeroMemory(&mat, sizeof(D3DMATERIAL9));
		mat.Ambient = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);
		mat.Diffuse = D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f);
		mat.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		GameManager::GetDevice()->SetMaterial(&mat);
		GameManager::GetDevice()->SetTexture(0, nullptr);
	}
	GameManager::GetDevice()->SetFVF(FVF_PositionNormalTexture::FVF);
	GameManager::GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, vertex.size() / 3, &vertex[0], sizeof(FVF_PositionNormalTexture));
}