#include "stdafx.h"
#include "Panel_Alpha.h"


Panel_Alpha::Panel_Alpha()
{
}


Panel_Alpha::~Panel_Alpha()
{
}

void Panel_Alpha::Initialize(float xSize, float ySize, D3DCOLOR color)
{
	float halfX = xSize * 0.5f;
	vertex.resize(6);
	vertex[0].pos = D3DXVECTOR3(-halfX, 0, 0);
	vertex[0].color = color;
	vertex[0].tex = D3DXVECTOR2(0, 1);

	vertex[1].pos = D3DXVECTOR3(-halfX, ySize, 0);
	vertex[1].color = color;
	vertex[1].tex = D3DXVECTOR2(0, 0);

	vertex[2].pos = D3DXVECTOR3(halfX, ySize, 0);
	vertex[2].color = color;
	vertex[2].tex = D3DXVECTOR2(1, 0);

	vertex[3].pos = D3DXVECTOR3(-halfX, 0, 0);
	vertex[3].color = color;
	vertex[3].tex = D3DXVECTOR2(0, 1);

	vertex[4].pos = D3DXVECTOR3(halfX, ySize, 0);
	vertex[4].color = color;
	vertex[4].tex = D3DXVECTOR2(1, 0);

	vertex[5].pos = D3DXVECTOR3(halfX, 0, 0);
	vertex[5].color = color;
	vertex[5].tex = D3DXVECTOR2(1, 1);

	D3DXMatrixIdentity(&world);
	ZeroMemory(&material, sizeof(D3DMATERIAL9));
	material.Ambient = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	material.Diffuse = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	material.Specular = D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f);
}

void Panel_Alpha::Destroy()
{
	texture = nullptr;
}

void Panel_Alpha::Update()
{
	D3DXMATRIXA16 rotation;
	D3DXMatrixRotationY(&rotation, rotationY);
	D3DXMATRIXA16 translation;
	D3DXMatrixTranslation(&translation, position.x, position.y, position.z);

	world = rotation * translation;
}

void Panel_Alpha::Render()
{
	GameManager::GetDevice()->SetRenderState(D3DRS_LIGHTING, false);

	// D3DBLEND_ZERO : (0, 0, 0, 0)
	// D3DBLEND_ONE : (1, 1, 1, 1)
	// D3DBLEND_SRCCOLOR : (rs, gs, bs, as)
	// D3DBLEND_INVSRCCOLOR : (1-rs, 1-gs, 1-bs, 1-as)
	// D3DBLEND_SRCALPHA : (as, as, as, as) (Source Blend 기본값)
	// D3DBLEND_INVSRCALPHA : (1-as, 1-as, 1-as, 1-as) (Destination Blend 기본값)
	// D3DBLEND_DESTALPHA : (ad, ad, ad, ad)
	// D3DBLEND_INVDESTALPHA : (1-ad, 1-ad, 1-ad, 1-ad)
	// D3DBLEND_DESTCOLOR : (rd, gd, bd, ad)
	// D3DBLEND_INVDESTCCOLOR : (1-rd, 1-gd, 1-bd, 1-ad)
	// D3DBLEND_SRCALPHASAT : (f, f, f, 1); f=min(as, 1-ad)
	GameManager::GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		//src는 알파 블랜드 켜고 그리는거
	GameManager::GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	//desc는 그 전에 그려져 있던 것

	// D3DBLENDOP_ADD (기본값)	: src + dest
	// D3DBLENDOP_SUBTRACT		: src - dest
	// D3DBLENDOP_REVSUBTRACT	: dest - src
	// D3DBLENDOP_MIN			: MIN(src, dest)
	// D3DBLENDOP_MAX			: MAX(src, dest)
	GameManager::GetDevice()->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	GameManager::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	GameManager::GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, false);
	
	GameManager::GetDevice()->SetTransform(D3DTS_WORLD, &world);
	GameManager::GetDevice()->SetMaterial(&material);
	GameManager::GetDevice()->SetTexture(0, texture);
	GameManager::GetDevice()->SetFVF(FVF_PositionColorTexture::FVF);
	GameManager::GetDevice()->DrawPrimitiveUP(
		D3DPT_TRIANGLELIST, 2, &vertex[0], sizeof(FVF_PositionColorTexture));

	GameManager::GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, true);
	GameManager::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	GameManager::GetDevice()->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	GameManager::GetDevice()->SetRenderState(D3DRS_LIGHTING, true);
}
