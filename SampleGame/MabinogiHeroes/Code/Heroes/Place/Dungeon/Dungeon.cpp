#include "stdafx.h"
#include "Dungeon.h"

Dungeon::Dungeon() { Castle0 = nullptr; Portal = nullptr; PortalTexture = nullptr; PortalAlpha = 0.0f; PortalAlphaOn = 0; SuccubusRoom = nullptr; SuccubusRoomTexture = nullptr; }
Dungeon::~Dungeon() {}

void Dungeon::Initialize()
{
	Sharpness = 0.025f;
	HeightMap::Initialize("./Dungeon/ScaledDungeonTerrain.raw", "./Dungeon/DungeonTerrain.jpg");

	if (!Castle0)
	{
		Castle0 = new SkinnedMesh;
		Castle0->Initialize("./Dungeon/", "Castle.X");
		Castle0->SetPosition(D3DXVECTOR3(-15.0f, 0.0f, -15.0f));
		Castle0->SetScale(8.0f);
		Castle0->SetPosition(D3DXVECTOR3(40.0f, -5.0f, -50.0f));
	}

	if (!SuccubusRoom)
	{
		LPD3DXMESH succubusRoom;
		
		D3DXCreateBox(GameManager::GetDevice(), 100, 100, 100, &succubusRoom, nullptr);
		succubusRoom->CloneMeshFVF(D3DXMESH_SYSTEMMEM, FVF_PositionNormalTexture::FVF, GameManager::GetDevice(), &SuccubusRoom);
		succubusRoom->Release();

		D3DXMatrixIdentity(&SuccubusRoomScale);
		D3DXMatrixIdentity(&SuccubusRoomRotation);
		D3DXMatrixIdentity(&SuccubusRoomTranslation);
		D3DXMatrixIdentity(&SuccubusRoomWorld);
		SuccubusRoomPosition = D3DXVECTOR3(500.0f, 500.0f, 500.0f);
		
		SuccubusRoomTexture = TextureManager::GetTexture("Universe.jpg");

		ZeroMemory(&SuccubusRoomMaterial, sizeof(D3DMATERIAL9));
		SuccubusRoomMaterial.Ambient = D3DXCOLOR(1.0f, 0.5f, 0.5f, 0.5f);
		SuccubusRoomMaterial.Diffuse = D3DXCOLOR(1.0f, 0.5f, 0.5f, 0.5f);
		SuccubusRoomMaterial.Specular = D3DXCOLOR(0.8f, 0.8f, 0.8f, 0.8f);

		D3DXVECTOR3 succubusRoomVertexes[4];

		float width, depth;

		width = depth = 150.0f;

		succubusRoomVertexes[0] = D3DXVECTOR3(0.0f - width, 0.0f, 0.0f - depth);
		succubusRoomVertexes[1] = D3DXVECTOR3(0.0f + width, 0.0f, 0.0f - depth);
		succubusRoomVertexes[2] = D3DXVECTOR3(0.0f - width, 0.0f, 0.0f + depth);
		succubusRoomVertexes[3] = D3DXVECTOR3(0.0f + width, 0.0f, 0.0f + depth);

		D3DXVECTOR2 uvs[4];

		uvs[0] = D3DXVECTOR2(0, 0);
		uvs[1] = D3DXVECTOR2(1, 0);
		uvs[2] = D3DXVECTOR2(0, 1);
		uvs[3] = D3DXVECTOR2(1, 1);

		SuccubusRoomTriangles[0] = TextureNormalTriangle(succubusRoomVertexes[0], succubusRoomVertexes[1], succubusRoomVertexes[2], uvs[0], uvs[1], uvs[2], D3DXVECTOR3(0.0f, 1.0f, 0.0f));
		SuccubusRoomTriangles[1] = TextureNormalTriangle(succubusRoomVertexes[1], succubusRoomVertexes[2], succubusRoomVertexes[3], uvs[1], uvs[2], uvs[3], D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	}

	if (!Portal)
	{
		D3DXCreateCylinder(GameManager::GetDevice(), 10, 10, 20, 20, 20, &Portal, nullptr);
		D3DXMatrixIdentity(&PortalScale);
		D3DXMatrixIdentity(&PortalRotation);
		D3DXMatrixIdentity(&PortalTranslation);
		D3DXMatrixIdentity(&PortalWorld);

		//PortalTexture = TextureManager::GetTexture("Portal.bmp");

		ZeroMemory(&PortalMaterial, sizeof(D3DMATERIAL9));
		PortalMaterial.Ambient = D3DXCOLOR(0.0f, 1.0f, 1.0f, 0.3f);
		PortalMaterial.Diffuse = D3DXCOLOR(0.0f, 1.0f, 1.0f, 0.3f);
		PortalMaterial.Specular = D3DXCOLOR(0.8f, 0.8f, 0.8f, 0.8f);

		PortalPosition = D3DXVECTOR3(-55.0f, 0.0f, -25.0f);
	}

	DWORD dwShaderFlags = 0;

#if _DEBUG
	dwShaderFlags |= D3DXSHADER_DEBUG;
#endif

	if (D3DXCreateEffectFromFile(GameManager::GetDevice(), L"../ResourceData/Shader/shader.fx", nullptr, nullptr, dwShaderFlags, nullptr, &DefaultShader, nullptr)) return;
	if (D3DXCreateEffectFromFile(GameManager::GetDevice(), L"../ResourceData/Shader/EnvironmentMapping.fx", nullptr, nullptr, dwShaderFlags, nullptr, &MirrorShader, nullptr)) return;
}

void Dungeon::Destroy()
{
	SAFE_RELEASE(MirrorShader);
	SAFE_RELEASE(DefaultShader);
	SAFE_RELEASE(Portal);
	SAFE_RELEASE(SuccubusRoom);
	
	SAFE_DELETE(Castle0);

	HeightMap::Destroy();
}

void Dungeon::Update()
{
	if (CharacData::GetLethita())
	{
		D3DXVECTOR3 currentPlayerPosition;

		currentPlayerPosition = CharacData::GetLethita()->GetPosition();
		GetHeight(currentPlayerPosition.y, currentPlayerPosition.x, currentPlayerPosition.z);
		CharacData::GetLethita()->SetPosition(currentPlayerPosition);
	}

	if (Castle0)
	{
		Castle0->SetWorldLightPos(WorldLightPosition);

		//D3DXVECTOR3 currentPosition;
		//
		//currentPosition = Castle0->GetPosition();
		//GetHeight(currentPosition.y, currentPosition.x, currentPosition.z);
		//Castle0->SetPosition(currentPosition);
	}

	if (SuccubusRoom)
	{
		GetHeight(SuccubusRoomPosition.y, SuccubusRoomPosition.x, SuccubusRoomPosition.z);
		//D3DXMatrixRotationX(&SuccubusRoomRotation, D3DX_PI * 0.5f);
		D3DXMatrixTranslation(&SuccubusRoomTranslation, SuccubusRoomPosition.x, SuccubusRoomPosition.y, SuccubusRoomPosition.z);

		SuccubusRoomWorld = SuccubusRoomScale * SuccubusRoomRotation * SuccubusRoomTranslation;
	}

	if (Portal)
	{
		GetHeight(PortalPosition.y, PortalPosition.x, PortalPosition.z);
		D3DXMatrixRotationX(&PortalRotation, D3DX_PI * 0.5f);
		D3DXMatrixTranslation(&PortalTranslation, PortalPosition.x, PortalPosition.y, PortalPosition.z);

		PortalWorld = PortalScale * PortalRotation * PortalTranslation;

		if (PortalAlpha > 1.0f || PortalAlpha < 0.0f) PortalAlphaOn = !PortalAlphaOn;
		if (CharacData::GetUnEqipInfo(CH_EXP) < 3) PortalAlpha = 0.5f;
		else if(CharacData::GetUnEqipInfo(CH_EXP) > 2)	PortalAlpha += (PortalAlphaOn - 1) * 0.01f + 1 * PortalAlphaOn * 0.01f;
	
	}
}

void Dungeon::Render()
{
	GameManager::GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	GameManager::GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
	GameManager::GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	GameManager::GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	GameManager::GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	GameManager::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, 1);

	if (Portal)
	{
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
	}

	if (SuccubusRoom)
	{
		//D3DXMATRIXA16 matWorldViewProjection;
		//{
		//	D3DXMATRIXA16 matView;
		//	GameManager::GetDevice()->GetTransform(D3DTS_VIEW, &matView);
		//
		//	D3DXMATRIXA16 matProjection;
		//	GameManager::GetDevice()->GetTransform(D3DTS_PROJECTION, &matProjection);
		//
		//	D3DXMatrixMultiply(&matWorldViewProjection, &matView, &matProjection);
		//}
		//
		//D3DXVECTOR4 worldLightPosition = { 500.0f - 57.5f, 500.0f + 115.0f, 500.0f - 57.5f, 1.0f };
		//D3DXVECTOR4 worldCameraPosition = GameManager::GetCamera()->GetEyePosition();
		//
		//MirrorShader->SetMatrix("gWorldMatrix", &SuccubusRoomWorld);
		//MirrorShader->SetMatrix("gWorldViewProjectionMatrix", &matWorldViewProjection);
		//
		//MirrorShader->SetVector("gWorldLightPosition", &worldLightPosition);
		//MirrorShader->SetVector("gWorldCameraPosition", &worldCameraPosition);
		//
		////MirrorShader->SetVector("gLightColor", &gLightColor);
		//
		//MirrorShader->SetTexture("EnvironmentMap_Tex", SuccubusRoomTexture);
		//
		//UINT numPasses = 0;
		//MirrorShader->Begin(&numPasses, NULL);
		//{
		//	for (UINT i = 0; i < numPasses; ++i)
		//	{
		//		MirrorShader->BeginPass(i);
		//		{
		//			GameManager::GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 4, &SuccubusRoomTriangles[0], sizeof(FVF_PositionNormalTexture));
		//		}
		//		MirrorShader->EndPass();
		//	}
		//}
		//MirrorShader->End();

		GameManager::GetDevice()->SetRenderState(D3DRS_LIGHTING, 1);
		GameManager::GetDevice()->SetFVF(FVF_PositionNormalTexture::FVF);
		GameManager::GetDevice()->SetMaterial(&SuccubusRoomMaterial);
		GameManager::GetDevice()->SetTexture(0, SuccubusRoomTexture);
		GameManager::GetDevice()->SetTransform(D3DTS_WORLD, &SuccubusRoomWorld);
		GameManager::GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		//SuccubusRoom->DrawSubset(0);
		GameManager::GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 4, &SuccubusRoomTriangles[0], sizeof(FVF_PositionNormalTexture));
	}

	GameManager::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, 0);
	GameManager::GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void Dungeon::UpdateAndRender()
{
	HeightMap::Render();

	if (Castle0) Castle0->UpdateAndRender();
}

void Dungeon::RenderShadow()
{
	if (Castle0)
	{
		Castle0->SetLightViewProjection(LightViewProjection);
		Castle0->RenderShadow(Castle0->GetRootFrame());
	}
}

void Dungeon::MonsterUpdate(MonsterBase * monster)
{
	D3DXVECTOR3 currentPlayerPosition;

	currentPlayerPosition = monster->GetPosition();
	GetHeight(currentPlayerPosition.y, currentPlayerPosition.x, currentPlayerPosition.z);
	monster->SetPosition(currentPlayerPosition);
}
