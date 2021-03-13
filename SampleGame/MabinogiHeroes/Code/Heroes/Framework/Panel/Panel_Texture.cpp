#include "stdafx.h"
#include "Panel_Texture.h"


Panel_Texture::Panel_Texture()
{
}


Panel_Texture::~Panel_Texture()
{
}

void Panel_Texture::Initialize(float xSize, float ySize, char* fileName)
{
	float halfX = xSize * 0.5f;
	vertex.resize(6);
	vertex[0].pos = D3DXVECTOR3(-halfX, 0, 0);
	vertex[0].tex = D3DXVECTOR2(0, 1);

	vertex[1].pos = D3DXVECTOR3(-halfX, ySize, 0);
	vertex[1].tex = D3DXVECTOR2(0, 0);

	vertex[2].pos = D3DXVECTOR3(halfX, ySize, 0);
	vertex[2].tex = D3DXVECTOR2(1, 0);

	vertex[3].pos = D3DXVECTOR3(-halfX, 0, 0);
	vertex[3].tex = D3DXVECTOR2(0, 1);

	vertex[4].pos = D3DXVECTOR3(halfX, ySize, 0);
	vertex[4].tex = D3DXVECTOR2(1, 0);

	vertex[5].pos = D3DXVECTOR3(halfX, 0, 0);
	vertex[5].tex = D3DXVECTOR2(1, 1);

	texture = TextureManager::GetTexture(fileName);
	D3DXMatrixIdentity(&world);
	ZeroMemory(&material, sizeof(D3DMATERIAL9));
	material.Ambient = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	material.Diffuse = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	material.Specular = D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f);
}

void Panel_Texture::Destroy()
{
	texture = nullptr;
}

void Panel_Texture::Update()
{
	D3DXMATRIXA16 rotation;
	D3DXMatrixRotationY(&rotation, rotationY);
	D3DXMATRIXA16 translation;
	D3DXMatrixTranslation(&translation, position.x, position.y, position.z);

	world = rotation * translation;
}

void Panel_Texture::Render()
{
	GameManager::GetDevice()->SetTransform(D3DTS_WORLD, &world);
	GameManager::GetDevice()->SetMaterial(&material);
	GameManager::GetDevice()->SetTexture(0, texture);
	GameManager::GetDevice()->SetFVF(FVF_PositionTexture::FVF);
	GameManager::GetDevice()->DrawPrimitiveUP(
		D3DPT_TRIANGLELIST, 2, &vertex[0], sizeof(FVF_PositionTexture));

}
