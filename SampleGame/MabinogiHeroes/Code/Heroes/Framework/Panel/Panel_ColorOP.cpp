#include "stdafx.h"
#include "Panel_ColorOP.h"


Panel_ColorOP::Panel_ColorOP()
{
}


Panel_ColorOP::~Panel_ColorOP()
{
}

void Panel_ColorOP::Initialize(float xSize, float ySize, D3DCOLOR color)
{
	Panel_Alpha::Initialize(xSize, ySize, color);

	texture = TextureManager::GetTexture("water.png");
}

void Panel_ColorOP::Render()
{
	GameManager::GetDevice()->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);	//색상 두개 설정하고
	GameManager::GetDevice()->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	/*
	// Modulate
	D3DTOP_MODULATE             = 4,      // multiply args together
	D3DTOP_MODULATE2X           = 5,      // multiply and  1 bit
	D3DTOP_MODULATE4X           = 6,      // multiply and  2 bits

	D3DTOP_ADD                  =  7,   // add arguments together
	D3DTOP_ADDSIGNED            =  8,   // add with -0.5 bias
	D3DTOP_ADDSIGNED2X          =  9,   // as above but left  1 bit
	D3DTOP_SUBTRACT             = 10,   // Arg1 - Arg2, with no saturation
	D3DTOP_ADDSMOOTH            = 11,   // add 2 args, subtract product
	// Arg1 + Arg2 - Arg1*Arg2
	// = Arg1 + (1-Arg1)*Arg2
	*/
	GameManager::GetDevice()->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);	//색상 두개 연산법 변경

	GameManager::GetDevice()->SetRenderState(D3DRS_LIGHTING, false);	//조명의 영향을 제거하기 위해 끔
	//GameManager::GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, false);

	GameManager::GetDevice()->SetTransform(D3DTS_WORLD, &world);
	GameManager::GetDevice()->SetMaterial(&material);
	GameManager::GetDevice()->SetTexture(0, texture);
	GameManager::GetDevice()->SetFVF(FVF_PositionColorTexture::FVF);
	GameManager::GetDevice()->DrawPrimitiveUP(
		D3DPT_TRIANGLELIST, 2, &vertex[0], sizeof(FVF_PositionColorTexture));

	//GameManager::GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, true);
	GameManager::GetDevice()->SetRenderState(D3DRS_LIGHTING, false);

	GameManager::GetDevice()->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);	//원상 복귀(이게 디폴트)

}
