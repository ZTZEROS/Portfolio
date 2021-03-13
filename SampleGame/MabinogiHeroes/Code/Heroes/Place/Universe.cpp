#include "stdafx.h"
#include "Universe.h"

Universe::Universe() { UniverseSphere = nullptr; UniverseTexture = nullptr; /*TextureMappingShader = nullptr;*/ }
Universe::~Universe() {}

void Universe::Initialize(unsigned int radius, unsigned int slices, unsigned int stacks)
{
	Reset();

	D3DXMatrixIdentity(&Scale);
	D3DXMatrixIdentity(&Rotation);
	D3DXMatrixIdentity(&Translation);
	D3DXMatrixIdentity(&World);

	Position = { 500.0f, 500.0f, 500.0f, 0.0f };

	LPD3DXMESH universeSphere;
	D3DXCreateSphere(GameManager::GetDevice(), (float)radius, slices, stacks, &universeSphere, nullptr);

	ZeroMemory(&Material, sizeof(D3DMATERIAL9));
	Material.Ambient = D3DXCOLOR(0.8f, 1.0f, 1.0f, 0.75f);
	Material.Diffuse = D3DXCOLOR(0.8f, 1.0f, 1.0f, 0.75f);
	Material.Specular = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);

	UniverseTexture = TextureManager::GetTexture("Universe.jpg");

	universeSphere->CloneMeshFVF(D3DXMESH_SYSTEMMEM, FVF_PositionNormalTexture::FVF, GameManager::GetDevice(), &UniverseSphere);
	universeSphere->Release();

	/*DWORD dwShaderFlags = 0;

#if _DEBUG
	dwShaderFlags |= D3DXSHADER_DEBUG;
#endif

	if (D3DXCreateEffectFromFile(GameManager::GetDevice(), L"../ResourceData/Shader/TextureMapping.fx", nullptr, nullptr, dwShaderFlags, nullptr, &TextureMappingShader, nullptr)) return;

	if (FAILED(D3DXLoadMeshFromX(L"../ResourceData/Xfile/Dungeon/Sphere.x", D3DXMESH_SYSTEMMEM, GameManager::GetDevice(), NULL, NULL, NULL, NULL, &UniverseSphere))) return;
*/
	FVF_PositionNormalTexture* pVerts;
	
	if (SUCCEEDED(UniverseSphere->LockVertexBuffer(0, (LPVOID*)&pVerts)))
	{
		// get vertex count
		int numVerts = UniverseSphere->GetNumVertices();
	
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
			pVerts->tex.y = asinf(pVerts->normal.z) / D3DX_PI + 0.5f;
	
			// go to next vertex
			pVerts++;
		}
	
		// unlock the vertex buffer
		UniverseSphere->UnlockVertexBuffer();
	}
}

void Universe::Destroy()
{
	//SAFE_RELEASE(TextureMappingShader);
	SAFE_RELEASE(UniverseSphere);
	UniverseTexture = nullptr;
}

void Universe::Reset()
{
}

void Universe::Update()
{
	D3DXMatrixScaling(&Scale, 1.0f, 1.0f, 1.0f);
	//D3DXMatrixRotationX(&Rotation, D3DX_PI * 0.5);
	D3DXMatrixTranslation(&Translation, Position.x, Position.y, Position.z);

	World = Scale * Rotation * Translation;
}

void Universe::Render()
{
	GameManager::GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	GameManager::GetDevice()->SetRenderState(D3DRS_LIGHTING, 1);
	GameManager::GetDevice()->SetFVF(UniverseSphere->GetFVF());
	GameManager::GetDevice()->SetMaterial(&Material);
	GameManager::GetDevice()->SetTexture(0, UniverseTexture);
	GameManager::GetDevice()->SetTransform(D3DTS_WORLD, &World);
	GameManager::GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	UniverseSphere->DrawSubset(0);

	GameManager::GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}
void Universe::RenderShadow()
{
	GameManager::GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	GameManager::GetDevice()->SetTexture(0, UniverseTexture);
	UniverseSphere->DrawSubset(0);

	GameManager::GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//void Universe::Shader()
//{
//	GameManager::GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
//
//	// 뷰 행렬을 만든다.
//	D3DXMATRIXA16 matView;
//	GameManager::GetDevice()->GetTransform(D3DTS_VIEW, &matView);
//
//	// 투영행렬을 만든다.
//	D3DXMATRIXA16 matProjection;
//	GameManager::GetDevice()->GetTransform(D3DTS_PROJECTION, &matProjection);
//
//	D3DXMATRIXA16 matWorld;
//	float scale = 5.0f;
//
//	D3DXMatrixScaling(&matWorld, scale, scale, scale);
//
//	matWorld = matWorld * World;
//
//	// 쉐이더 전역변수들을 설정
//	TextureMappingShader->SetMatrix("gWorldMatrix", &matWorld);
//	TextureMappingShader->SetMatrix("gViewMatrix", &matView);
//	TextureMappingShader->SetMatrix("gProjectionMatrix", &matProjection);
//
//	TextureMappingShader->SetTexture("DiffuseMap_Tex", UniverseTexture);
//
//	// 쉐이더를 시작한다.
//	UINT numPasses = 0;
//	TextureMappingShader->Begin(&numPasses, NULL);
//	{
//		for (UINT i = 0; i < numPasses; ++i)
//		{
//			TextureMappingShader->BeginPass(i);
//			{
//				// 구체를 그린다.
//				UniverseSphere->DrawSubset(0);
//			}
//			TextureMappingShader->EndPass();
//		}
//	}
//	TextureMappingShader->End();
//
//	GameManager::GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
//}
