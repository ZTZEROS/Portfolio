#include "stdafx.h"
#include "WindBlade.h"


WindBlade::WindBlade()
{
	memset(&WindBladeMaterial, 1, sizeof(D3DMATERIAL9));
}


WindBlade::~WindBlade()
{
}

void WindBlade::init()
{

}

void WindBlade::release()
{
	for (auto iter = vertices.begin(); iter != vertices.end();)
	{
		iter = vertices.erase(iter);
	}
}

void WindBlade::addVPosition(D3DXVECTOR3 _start, D3DXVECTOR3 _end)
{
	D3DCOLOR color = D3DCOLOR_RGBA(0, 128, 128, 255);
	D3DXVECTOR3 temp;
	temp = _start;
	temp.y = CharacData::GetLethita()->GetPosition().y + 3.5f;

	vertices.push_back(FVF_PositionColor(temp, color));
	
	/*temp = (_end + _start) / 2;
	temp.y = 3.5f;

	vertices.push_back(FVF_PositionColor(temp, color));*/

	temp = _end;
	temp.y = CharacData::GetLethita()->GetPosition().y + 3.5f;

	vertices.push_back(FVF_PositionColor(temp, color));
}

void WindBlade::render(D3DXMATRIXA16& world)
{
	GameManager::GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
	GameManager::GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	GameManager::GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	GameManager::GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	GameManager::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, 1);

	D3DXMATRIXA16 tempWorld;

	for (unsigned int i = 0; i < vertices.size(); ++i)
	{
		D3DCOLOR color = vertices[i].color;
		
		if (color < 0x10000000) continue;

		color -= 0x0f000000;
		vertices[i].color = color;
	}

	D3DXMatrixIdentity(&tempWorld);
	//단순 표시용
//	GameManager::GetDevice()->SetRenderState(D3DRS_LIGHTING, false);
	GameManager::GetDevice()->SetFVF(FVF_PositionColor::FVF);
	

	GameManager::GetDevice()->SetMaterial(&WindBladeMaterial);
	GameManager::GetDevice()->SetTransform(D3DTS_WORLD, &tempWorld);
	GameManager::GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	if(vertices.size() > 0)	GameManager::GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, vertices.size()-3, &vertices[0], sizeof(FVF_PositionColor));
	
	GameManager::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, 0);
	GameManager::GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}
