#include "stdafx.h"
#include "TheSun.h"

TheSun::TheSun() { SunSphere = nullptr; SunTexture = nullptr; }
TheSun::~TheSun() {}

void TheSun::Initialize(float radius, unsigned int slices, unsigned int stacks)
{
	Reset();

	D3DXMatrixIdentity(&Scale);
	D3DXMatrixIdentity(&Rotation);
	D3DXMatrixIdentity(&Translation);
	D3DXMatrixIdentity(&World);

	Position = { -57.5f, 115.0f, -57.5f, 1.0f };

	LPD3DXMESH sunSphere;

	D3DXCreateSphere(GameManager::GetDevice(), (float)radius, slices, stacks, &sunSphere, nullptr);
	sunSphere->CloneMeshFVF(D3DXMESH_SYSTEMMEM, FVF_PositionNormalTexture::FVF, GameManager::GetDevice(), &SunSphere);
	sunSphere->Release();

	//D3DXCreateCylinder(GameManager::GetDevice(), radius, radius, 1.0f, slices, stacks, &SunSphere, nullptr);

	ZeroMemory(&Material, sizeof(D3DMATERIAL9));
	Material.Ambient = D3DXCOLOR(0.9f, 1.0f, 1.0f, 1.0f);
	Material.Diffuse = D3DXCOLOR(0.9f, 1.0f, 1.0f, 1.0f);
	Material.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	ZeroMemory(&Light, sizeof(D3DLIGHT9));
	Light.Type = D3DLIGHTTYPE::D3DLIGHT_SPOT;
	Light.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Light.Specular = D3DXCOLOR(0.8f, 0.8f, 0.8f, 0.8f);
	
	D3DXVECTOR3 direction(1.0f, -2.0f, 1.0f);
	D3DXVec3Normalize(&direction, &direction);
	Light.Direction = direction;
	Light.Position = D3DXVECTOR3(Position.x, Position.y, Position.z);
	Light.Range = 1000.0f;
	Light.Theta = D3DX_PI * 0.5f;
	Light.Phi = D3DX_PI;
	
	GameManager::GetDevice()->SetLight(1, &Light);
	//GameManager::GetDevice()->LightEnable(0, 0);
	GameManager::GetDevice()->LightEnable(1, 1);

	SunTexture = TextureManager::GetTexture("TheSun.bmp");

	FVF_PositionNormalTexture* pVerts;

	if (SUCCEEDED(SunSphere->LockVertexBuffer(0, (LPVOID*)&pVerts)))
	{
		// get vertex count
		int numVerts = SunSphere->GetNumVertices();
	
		// loop through the vertices
		for (int i = 0; i < numVerts; i++)
		{
			//D3DXVECTOR3 v;
			//v = pVerts->pos;
			//D3DXVec3Normalize(&v, &v);
	
			// calculate texture coordinates
			//pVerts->tex.x = asinf(v.x) / D3DX_PI + 0.5f;
			//pVerts->tex.y = asinf(v.y) / D3DX_PI + 0.5f;
			pVerts->tex.x = asinf(pVerts->normal.x) / D3DX_PI + 0.5f;
			pVerts->tex.y = asinf(pVerts->normal.y) / D3DX_PI + 0.5f;
	
			// go to next vertex
			pVerts++;
		}
	
		// unlock the vertex buffer
		SunSphere->UnlockVertexBuffer();
	}
}

void TheSun::Destroy()
{
	SAFE_RELEASE(SunSphere);
}

void TheSun::Reset()
{
}

void TheSun::Update()
{
	D3DXMATRIXA16 rotationX, rotationY, rotationZ;

	D3DXMatrixScaling(&Scale, 1.0f, 1.0f, 1.0f);
	D3DXMatrixRotationX(&rotationX, D3DX_PI * 0.25f);
	D3DXMatrixRotationY(&rotationY, D3DX_PI * 0.25f);
	D3DXMatrixTranslation(&Translation, Position.x, Position.y, Position.z);

	Rotation = rotationX * rotationY;

	World = Scale * Rotation * Translation;
}

void TheSun::Render()
{
	GameManager::GetDevice()->SetRenderState(D3DRS_LIGHTING, 1);
	GameManager::GetDevice()->SetFVF(SunSphere->GetFVF());
	GameManager::GetDevice()->SetMaterial(&Material);
	GameManager::GetDevice()->SetTexture(0, SunTexture);
	GameManager::GetDevice()->SetTransform(D3DTS_WORLD, &World);
	GameManager::GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	SunSphere->DrawSubset(0);
}

void TheSun::Shader()
{
}
