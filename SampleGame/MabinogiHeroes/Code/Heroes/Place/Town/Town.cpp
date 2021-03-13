#include "stdafx.h"
#include "Town.h"

Town::Town()
{ 
	Inn0 = nullptr; Fountain = nullptr; FountainWater = nullptr; Portal = nullptr; PortalTexture = nullptr; PortalAlpha = 0.0f; PortalAlphaOn = 0; DefaultShader = nullptr;
	ZeroMemory(&Barrels, sizeof(SkinnedMesh*) * BARREL_COUNT);
	ZeroMemory(&Trees, sizeof(SkinnedMesh*) * TREE_COUNT);
}
Town::~Town() {}

void Town::Initialize()
{
	HeightMap::Initialize("./Town/TownTerrain.raw", "./Town/TownTerrain.jpg");

	if (!Inn0)
	{
		Inn0 = new SkinnedMesh;
		Inn0->Initialize("./Town/", "WoodenHouse.X");
		Inn0->SetPosition(D3DXVECTOR3(-15.0f, 0.0f, -15.0f));
	}

	if (!Barrels[0])
	{
		for (unsigned int i = 0; i < BARREL_COUNT; ++i)
		{
			SAFE_DELETE(Barrels[i]);
			
			Barrels[i] = new SkinnedMesh;
		}
		Barrels[0]->Initialize("./Town/", "Barrel0.X");
		Barrels[1]->Initialize("./Town/", "Barrel1.X");
		Barrels[2]->Initialize("./Town/", "Barrel2.X");
		
		Barrels[0]->SetScale(5.0f);
		Barrels[1]->SetScale(5.0f);
		Barrels[2]->SetScale(5.0f);

		Barrels[0]->SetPosition(D3DXVECTOR3(-45.0f, 0.0f, -40.0f));
		Barrels[1]->SetPosition(D3DXVECTOR3(-50.0f, 0.0f, -40.0f));
		Barrels[2]->SetPosition(D3DXVECTOR3(-45.0f, 0.0f, -45.0f));
	}

	if (!Trees[0])
	{
		for (unsigned int i = 0; i < TREE_COUNT; ++i)
		{
			SAFE_DELETE(Trees[i]);

			Trees[i] = new SkinnedMesh;
		}
		
		Trees[0]->Initialize("./Town/", "Tree0.X");
		Trees[1]->Initialize("./Town/", "Tree1.X");
		Trees[2]->Initialize("./Town/", "Tree2.X");
		Trees[3]->Initialize("./Town/", "Tree3.X");
		Trees[4]->Initialize("./Town/", "Tree4.X");
		Trees[5]->Initialize("./Town/", "Tree5.X");
		Trees[6]->Initialize("./Town/", "Tree6.X");
		Trees[7]->Initialize("./Town/", "Tree7.X");
		Trees[8]->Initialize("./Town/", "Tree8.X");
		Trees[9]->Initialize("./Town/", "Tree9.X");
		
		Trees[0]->SetScale(5.0f);
		Trees[1]->SetScale(5.0f);
		Trees[2]->SetScale(5.0f);
		Trees[3]->SetScale(5.0f);
		Trees[4]->SetScale(5.0f);
		Trees[5]->SetScale(5.0f);
		Trees[6]->SetScale(5.0f);
		Trees[7]->SetScale(5.0f);
		Trees[8]->SetScale(5.0f);
		Trees[9]->SetScale(5.0f);
				
		Trees[0]->SetPosition(D3DXVECTOR3(-37.0f, 0.0f, -10.0f));
		Trees[1]->SetPosition(D3DXVECTOR3(-35.0f, 0.0f, -8.0f));
		Trees[2]->SetPosition(D3DXVECTOR3(-35.0f, 0.0f, -10.0f));
		Trees[3]->SetPosition(D3DXVECTOR3(-37.0f, 0.0f, -8.0f));
		Trees[4]->SetPosition(D3DXVECTOR3(-39.0f, 0.0f, -10.0f));
		
		Trees[5]->SetPosition(D3DXVECTOR3(17.0f, 0.0f, -5.0f));
		Trees[6]->SetPosition(D3DXVECTOR3(13.0f, 0.0f, -10.0f));
		Trees[7]->SetPosition(D3DXVECTOR3(12.0f, 0.0f, -12.0f));
		Trees[8]->SetPosition(D3DXVECTOR3(19.0f, 0.0f, -10.0f));
		Trees[9]->SetPosition(D3DXVECTOR3(17.0f, 0.0f, -7.0f));
	}

	if (!Fountain)
	{
		Fountain = new SkinnedMesh;
		Fountain->Initialize("./Town/", "Fountain.X");
		Fountain->SetPosition(D3DXVECTOR3(-20.0f, 0.0f, 20.0f));
	}

	if (!FountainWater)
	{
		//FountainWater = new Particle_Water(D3DXVECTOR3(-20,10,20), 300);
		//FountainWater->Initialize("Waterdrop.bmp");
	}

	if (!Portal)
	{
		D3DXCreateCylinder(GameManager::GetDevice(), 10, 10, 20, 20, 20, &Portal, nullptr);
		
		//LPD3DXMESH portal;
		//D3DXCreateCylinder(GameManager::GetDevice(), 10, 10, 20, 20, 20, &portal, nullptr);
		//portal->CloneMeshFVF(D3DXMESH_SYSTEMMEM, FVF_PositionTexture::FVF, GameManager::GetDevice(), &Portal);
		//portal->Release();
		
		//PortalTexture = TextureManager::GetTexture("Portal.bmp");
		
		//FVF_PositionTexture* pVerts;
		//
		//if (SUCCEEDED(Portal->LockVertexBuffer(0, (LPVOID*)&pVerts)))
		//{
		//	// get vertex count
		//	int numVerts = Portal->GetNumVertices();
		//
		//	// loop through the vertices
		//	for (int i = 0; i < numVerts; i++)
		//	{
		//		pVerts->tex.x = pVerts->pos.x / float(numVerts);
		//		pVerts->tex.y = pVerts->pos.y / float(numVerts);
		//
		//		// go to next vertex
		//		pVerts++;
		//	}
		//
		//	// unlock the vertex buffer
		//	Portal->UnlockVertexBuffer();
		//}

		D3DXMatrixIdentity(&PortalScale);
		D3DXMatrixIdentity(&PortalRotation);
		D3DXMatrixIdentity(&PortalTranslation);
		D3DXMatrixIdentity(&PortalWorld);

		ZeroMemory(&PortalMaterial, sizeof(D3DMATERIAL9));
		PortalMaterial.Ambient = D3DXCOLOR(0.0f, 1.0f, 1.0f, 0.3f);
		PortalMaterial.Diffuse = D3DXCOLOR(0.0f, 1.0f, 1.0f, 0.3f);
		PortalMaterial.Specular = D3DXCOLOR(0.8f, 0.8f, 0.8f, 0.8f);

		PortalPosition = { -100.0f, 0.0f, 0.0f };
	}

	DWORD dwShaderFlags = 0;

#if _DEBUG
	dwShaderFlags |= D3DXSHADER_DEBUG;
#endif

	// ½¦ÀÌ´õ ·Îµù
	if (D3DXCreateEffectFromFile(GameManager::GetDevice(), L"../ResourceData/Shader/shader.fx", nullptr, nullptr, dwShaderFlags, nullptr, &DefaultShader, nullptr)) return;
}

void Town::Destroy()
{
	SAFE_RELEASE(DefaultShader);
	SAFE_RELEASE(Portal);

	SAFE_DELETE(Inn0);

	for (unsigned int i = 0; i < BARREL_COUNT; ++i)
	{
		SAFE_DELETE(Barrels[i]);
	}

	for (unsigned int i = 0; i < TREE_COUNT; ++i)
	{
		SAFE_DELETE(Trees[i]);
	}

	SAFE_DELETE(FountainWater);
	SAFE_DELETE(Fountain);
	HeightMap::Destroy();
}

void Town::Update()
{
	if (CharacData::GetLethita())
	{
		D3DXVECTOR3 currentPlayerPosition;

		currentPlayerPosition = CharacData::GetLethita()->GetPosition();
		GetHeight(currentPlayerPosition.y, currentPlayerPosition.x, currentPlayerPosition.z);
		CharacData::GetLethita()->SetPosition(currentPlayerPosition);
	}

	if (Inn0)
	{
		Inn0->SetWorldLightPos(WorldLightPosition);

		D3DXVECTOR3 currentPosition;

		currentPosition = Inn0->GetPosition();
		GetHeight(currentPosition.y, currentPosition.x, currentPosition.z);
		Inn0->SetPosition(currentPosition);
	}


	if (Barrels[0])
	{
		for (unsigned int i = 0; i < BARREL_COUNT; ++i)
		{
			Barrels[i]->SetWorldLightPos(WorldLightPosition);

			D3DXVECTOR3 currentPosition;

			currentPosition = Barrels[i]->GetPosition();
			GetHeight(currentPosition.y, currentPosition.x, currentPosition.z);
			Barrels[i]->SetPosition(currentPosition);
		}
	}

	if (Trees[0])
	{
		for (unsigned int i = 0; i < TREE_COUNT; ++i)
		{
			Trees[i]->SetWorldLightPos(WorldLightPosition);

			D3DXVECTOR3 currentPosition;

			currentPosition = Trees[i]->GetPosition();
			GetHeight(currentPosition.y, currentPosition.x, currentPosition.z);
			Trees[i]->SetPosition(currentPosition);
		}
	}

	if (Fountain)
	{
		Fountain->SetWorldLightPos(WorldLightPosition);

		D3DXVECTOR3 currentPosition;

		currentPosition = Fountain->GetPosition();
		GetHeight(currentPosition.y, currentPosition.x, currentPosition.z);
		Fountain->SetPosition(currentPosition);
	}

	if (FountainWater) FountainWater->Update();

	if (Portal)
	{
		GetHeight(PortalPosition.y, PortalPosition.x, PortalPosition.z);
		D3DXMatrixRotationX(&PortalRotation, D3DX_PI * 0.5f);
		D3DXMatrixTranslation(&PortalTranslation, PortalPosition.x, PortalPosition.y, PortalPosition.z);

		PortalWorld = PortalScale * PortalRotation * PortalTranslation;

		if (PortalAlpha > 1.0f || PortalAlpha < 0.0f) PortalAlphaOn = !PortalAlphaOn;

		PortalAlpha += (PortalAlphaOn - 1) * 0.01f + 1 * PortalAlphaOn * 0.01f;
	}
}

void Town::Render()
{
	if (FountainWater) FountainWater->Render();
	
	if (Portal)
	{
		GameManager::GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		GameManager::GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
		GameManager::GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

		GameManager::GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		GameManager::GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		GameManager::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, 1);

		D3DXMATRIXA16 portalWorld;

		D3DXMatrixScaling(&portalWorld, 0.75f, 0.75f, 0.75f);

		portalWorld = portalWorld * PortalRotation * PortalTranslation;

		GameManager::GetDevice()->SetRenderState(D3DRS_LIGHTING, 1);
		GameManager::GetDevice()->SetFVF(Portal->GetFVF());
		GameManager::GetDevice()->SetMaterial(&PortalMaterial);
		GameManager::GetDevice()->SetTexture(0, PortalTexture);
		GameManager::GetDevice()->SetTransform(D3DTS_WORLD, &portalWorld);
		GameManager::GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

		Portal->DrawSubset(0);

		D3DXMATRIXA16 view, projection;
		GameManager::GetDevice()->GetTransform(D3DTS_VIEW, &view);
		GameManager::GetDevice()->GetTransform(D3DTS_PROJECTION, &projection);
		D3DXMATRIXA16 rotation;
		
		D3DXMatrixRotationX(&rotation, D3DXToRadian(45.0f));
		
		DefaultShader->SetMatrix("world", &PortalWorld);
		DefaultShader->SetMatrix("view", &view);
		DefaultShader->SetMatrix("projection", &projection);
		DefaultShader->SetMatrix("rotation", &rotation);
		DefaultShader->SetFloat("alpha", PortalAlpha);

		//GameManager::GetDevice()->SetTransform(D3DTS_WORLD, &PortalWorld);
		//GameManager::GetDevice()->SetTexture(0, PortalTexture);
		//GameManager::GetDevice()->SetFVF(FVF_PositionTexture::FVF);
		
		//DefaultShader->SetTexture("m_PortalDiffuse", PortalTexture);
		DefaultShader->SetTexture("m_TextureDiffuse", PortalTexture);
		
		D3DXHANDLE technique = DefaultShader->GetTechniqueByName("Default_DirectX_Effect");
		DefaultShader->SetTechnique(technique);
		
		UINT numPasses = 0;
		DefaultShader->Begin(&numPasses, NULL);
		for (int i = 0; i < numPasses; ++i)
		{
			DefaultShader->BeginPass(i);
			
			Portal->DrawSubset(0);
		
			DefaultShader->EndPass();
		
		}
		DefaultShader->End();
		
		GameManager::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, 0);
		GameManager::GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}
}

void Town::UpdateAndRender()
{
	HeightMap::Render();
	
	if (Inn0) Inn0->UpdateAndRender();
	
	if (Barrels[0])
	{
		for (unsigned int i = 0; i < BARREL_COUNT; ++i)
		{
			Barrels[i]->UpdateAndRender();
		}
	}

	if (Trees[0])
	{
		for (unsigned int i = 0; i < TREE_COUNT; ++i)
		{
			Trees[i]->UpdateAndRender();
		}
	}

	if (Fountain) Fountain->UpdateAndRender();

	//if (FountainWater) FountainWater->Render();
}

void Town::RenderShadow()
{
	if (Inn0)
	{
		Inn0->SetLightViewProjection(LightViewProjection);
		Inn0->RenderShadow(Inn0->GetRootFrame());
	}

	if (Barrels[0])
	{
		for (unsigned int i = 0; i < BARREL_COUNT; ++i)
		{
			Barrels[i]->SetLightViewProjection(LightViewProjection);
			Barrels[i]->RenderShadow(Barrels[i]->GetRootFrame());
		}
	}

	if (Trees[0])
	{
		for (unsigned int i = 0; i < TREE_COUNT; ++i)
		{
			Trees[i]->SetLightViewProjection(LightViewProjection);
			Trees[i]->RenderShadow(Trees[i]->GetRootFrame());
		}
	}

	if (Fountain)
	{
		Fountain->SetLightViewProjection(LightViewProjection);
		Fountain->RenderShadow(Fountain->GetRootFrame());
	}

	//if (FountainWater) FountainWater->Render();
}

void Town::RenderWater()
{
	//if (FountainWater) FountainWater->Render();
}