#include "stdafx.h"
#include "Panel_AlphaMaterial.h"


Panel_AlphaMaterial::Panel_AlphaMaterial()
{
}


Panel_AlphaMaterial::~Panel_AlphaMaterial()
{
}

void Panel_AlphaMaterial::Initialize(float xSize, float ySize, char* fileName)
{
	Panel_Texture::Initialize(xSize, ySize, fileName);

	material.Diffuse = D3DXCOLOR(0.8f, 0.8f, 0.8f, 0.5f);
}

void Panel_AlphaMaterial::Update()
{
	Panel_Texture::Update();
	//방향키로 투명값 조절
}

void Panel_AlphaMaterial::Render()
{
	GameManager::GetDevice()->SetTextureStageState(
		0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
	GameManager::GetDevice()->SetTextureStageState(
		0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	GameManager::GetDevice()->SetRenderState(
		D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	GameManager::GetDevice()->SetRenderState(
		D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	GameManager::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	//GameManager::GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, false);

	GameManager::GetDevice()->SetTransform(D3DTS_WORLD, &world);
	GameManager::GetDevice()->SetMaterial(&material);
	GameManager::GetDevice()->SetTexture(0, texture);
	GameManager::GetDevice()->SetFVF(FVF_PositionTexture::FVF);
	GameManager::GetDevice()->DrawPrimitiveUP(
		D3DPT_TRIANGLELIST, 2, &vertex[0], sizeof(FVF_PositionTexture));


	GameManager::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, false);

}