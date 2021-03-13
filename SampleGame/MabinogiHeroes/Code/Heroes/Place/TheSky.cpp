#include "stdafx.h"
#include "TheSky.h"

TheSky::TheSky() { SkySphere = nullptr; SkyTexture = nullptr; }
TheSky::~TheSky() {}

void TheSky::Initialize(unsigned int radius, unsigned int slices, unsigned int stacks)
{
	Reset();

	D3DXMatrixIdentity(&Scale);
	D3DXMatrixIdentity(&Rotation);
	D3DXMatrixIdentity(&Translation);
	D3DXMatrixIdentity(&World);

	Position = { 0.0f, 0.0f, 0.0f, 0.0f };

	LPD3DXMESH skySphere, cloud;
	D3DXCreateSphere(GameManager::GetDevice(), (float)radius, slices, stacks, &skySphere, nullptr);
	D3DXCreateSphere(GameManager::GetDevice(), (float)radius - 1, slices, stacks, &cloud, nullptr);

	ZeroMemory(&Material, sizeof(D3DMATERIAL9));
	Material.Ambient = D3DXCOLOR(0.8f, 1.0f, 1.0f, 0.75f);
	Material.Diffuse = D3DXCOLOR(0.8f, 1.0f, 1.0f, 0.75f);
	Material.Specular = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);

	//SkyTexture = TextureManager::GetTexture("TheSky.bmp");
	SkyTexture = TextureManager::GetTexture("Cloud.bmp");
	CloudTexture = TextureManager::GetTexture("Cloud.bmp");

	skySphere->CloneMeshFVF(D3DXMESH_SYSTEMMEM, FVF_PositionNormalTexture::FVF, GameManager::GetDevice(), &SkySphere);
	skySphere->Release();
	
	cloud->CloneMeshFVF(D3DXMESH_SYSTEMMEM, FVF_PositionNormalTexture::FVF, GameManager::GetDevice(), &CloudSphere);
	cloud->Release();


	FVF_PositionNormalTexture* pVerts;
	
	if (SUCCEEDED(SkySphere->LockVertexBuffer(0, (LPVOID*)&pVerts)))
	{
		// get vertex count
		int numVerts = SkySphere->GetNumVertices();
		
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
		SkySphere->UnlockVertexBuffer();
	}

	if (SUCCEEDED(CloudSphere->LockVertexBuffer(0, (LPVOID*)&pVerts)))
	{
		// get vertex count
		int numVerts = CloudSphere->GetNumVertices();

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
		CloudSphere->UnlockVertexBuffer();
	}
}

void TheSky::Destroy()
{
	SAFE_RELEASE(CloudSphere);
	SAFE_RELEASE(SkySphere);
	CloudTexture = nullptr;
	SkyTexture = nullptr;
}

void TheSky::Reset()
{
}

void TheSky::Update()
{
	D3DXMatrixScaling(&Scale, 1.0f, 1.0f, 1.0f);
	D3DXMatrixRotationX(&Rotation, D3DX_PI * 0.5);
	D3DXMatrixTranslation(&Translation, Position.x, Position.y, Position.z);

	World = Scale * Rotation * Translation;
}

void TheSky::Render()
{
	GameManager::GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	
	GameManager::GetDevice()->SetRenderState(D3DRS_LIGHTING, 1);
	GameManager::GetDevice()->SetFVF(SkySphere->GetFVF());
	GameManager::GetDevice()->SetMaterial(&Material);
	GameManager::GetDevice()->SetTexture(0, SkyTexture);
	GameManager::GetDevice()->SetTransform(D3DTS_WORLD, &World);
	GameManager::GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	SkySphere->DrawSubset(0);

	GameManager::GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}
void TheSky::RenderShadow()
{
	GameManager::GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	
	GameManager::GetDevice()->SetTexture(0, CloudTexture);
	CloudSphere->DrawSubset(0);
	
	GameManager::GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void TheSky::Shader()
{

}
