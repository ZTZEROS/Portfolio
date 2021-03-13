#include "stdafx.h"
#include "ProgressBar.h"


ProgressBar::ProgressBar()
{
	D3DXMatrixIdentity(&world);
	D3DXMatrixIdentity(&Scale);
	D3DXMatrixIdentity(&Translation);
}


ProgressBar::~ProgressBar()
{
}

void ProgressBar::Initilize(char* front, char* behind)
{
	char path[254] = "./UI/";
	strcat_s(path, front);
	FrontBar = TextureManager::GetTexture(path, &FrontInfo);

	char path2[254] = "./UI/";
	strcat_s(path2, behind);
	BehindBar = TextureManager::GetTexture(path2, &BehindInfo);

	size.x = (float)FrontInfo.Width;
	size.y = (float)FrontInfo.Height;

}

void ProgressBar::Destory()
{
	
}

void ProgressBar::Render()
{
	//RcProgress = { (int)world._41, (int)world._42, (int)(world._41 + size.x) , (int)(world._42 + size.y) };
	RcProgress = { 0, 0, (int)size.x , (int)size.y};
	UI_Manager::GetSprite()->SetTransform(&world);
	UI_Manager::GetSprite()->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
	
	//µÚ ·»´õ
	RECT Brect = { (int)0, (int)0, (int)width, (int)size.y };
	UI_Manager::GetSprite()->Draw(FrontBar, &Brect, &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 0, 0), D3DCOLOR_ARGB(255, 255, 255, 255));

	//¾Õ ·»´õ
	UI_Manager::GetSprite()->Draw(BehindBar, &RcProgress, &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 0, 0), D3DCOLOR_ARGB(255, 255, 255, 255));
	UI_Manager::GetSprite()->End();

	
}

void ProgressBar::Update()
{
	D3DXMatrixIdentity(&world);
	world *= Translation;
}

void ProgressBar::gaugeHeight(float currentGauge, float maxGauge)
{
	height = (currentGauge / maxGauge) * size.y;
}

void ProgressBar::gaugeWidth(float currentGauge, float maxGauge)
{
	width = (currentGauge / maxGauge)*size.x;
}
