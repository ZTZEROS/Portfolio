#include "stdafx.h"
#include "DungeonScene.h"

DungeonScene::DungeonScene() { Grid0 = nullptr; TheSun0 = nullptr; TheSky0 = nullptr; Universe0 = nullptr; Dungeon0 = nullptr; for (int i = 0; i < MonstMax; ++i) hellLady[i] = nullptr; }
DungeonScene::~DungeonScene() {}

void DungeonScene::Initialize()
{
	Reset();
	UI_Mnager::SetUsing(1);

	if (!Grid0)
	{
		Grid0 = new Grid;
		Grid0->Initialize();
	}
	
	if (!Universe0)
	{
		Universe0 = new Universe;
		Universe0->Initialize();
	}

	if (!TheSky0)
	{
		TheSky0 = new TheSky;
		TheSky0->Initialize();
	}

	if (!TheSun0)
	{
		TheSun0 = new TheSun;
		TheSun0->Initialize();
	}

	if (!Dungeon0)
	{
		Dungeon0 = new Dungeon;
		Dungeon0->Initialize();
	}

	for (int i = 0; i < MonstMax; ++i)
	{
		if (hellLady[i] == nullptr)
		{
			hellLady[i] = new HellLady();
			hellLady[i]->InitXFile();
			hellLady[i]->SetPosition(D3DXVECTOR3(0, 66.0f, 26.0f + 20*i));
			hellLady[i]->SetOriginPosition(D3DXVECTOR3(0, 10.0f, 26.0f + 20 * i));
			hellLady[i]->SetAnimationIndex(0);
		}
	}
	if (!succubus)
	{
		succubus = new Succubus();
		succubus->InitXFile();
		succubus->SetPosition(D3DXVECTOR3(500.0, 500.0f, 500.0f));
		succubus->SetOriginPosition(D3DXVECTOR3(0, 10.0f, 26.0f));
		succubus->SetAnimationIndex(0);
		succubus->SetIsLive(false);
	}

	
	GameManager::GetCamera()->SetLookTarget(CharacData::GetLethita()->GetPositionAddress());
	CharacData::GetLethita()->SetPosition(D3DXVECTOR3(66.0f, 66.0f, 66.0f));
	CharacData::GetLethita()->SetAnimationIndex(0);
	const int shadowMapSize = 4096;
	if (FAILED(GameManager::GetDevice()->CreateTexture(shadowMapSize, shadowMapSize, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F, D3DPOOL_DEFAULT, &ShadowRenderTarget, NULL))) return;
	if (FAILED(GameManager::GetDevice()->CreateDepthStencilSurface(shadowMapSize, shadowMapSize, D3DFMT_D24X8, D3DMULTISAMPLE_NONE, 0, 1, &ShadowDepthStencil, NULL))) return;

	DWORD dwShaderFlags = 0;

#if _DEBUG
	dwShaderFlags |= D3DXSHADER_DEBUG;
#endif

	// 쉐이더 로딩
	if (D3DXCreateEffectFromFile(GameManager::GetDevice(), L"../ResourceData/Shader/CreateShadow.fx", nullptr, nullptr, dwShaderFlags, nullptr, &CreateShadowShader, nullptr)) return;
	if (D3DXCreateEffectFromFile(GameManager::GetDevice(), L"../ResourceData/Shader/ApplyShadow.fx", nullptr, nullptr, dwShaderFlags, nullptr, &ApplyShadowShader, nullptr)) return;
	if (D3DXCreateEffectFromFile(GameManager::GetDevice(), L"../ResourceData/Shader/uvAnimation.fx", nullptr, nullptr, dwShaderFlags, nullptr, &uvAnimationShader, nullptr)) return;
	
	isReading = true;
}

void DungeonScene::Destroy()
{
	if (threadHandle)
	{
		CloseHandle(threadHandle);
		threadHandle = nullptr;
	}
	
	SAFE_DELETE(succubus);
	
	for (int i = 0; i < MonstMax; ++i)
	{
		SAFE_DELETE(hellLady[i]);
	}
	
	SAFE_RELEASE(uvAnimationShader);
	SAFE_RELEASE(ApplyShadowShader);
	SAFE_RELEASE(CreateShadowShader);

	SAFE_RELEASE(ShadowDepthStencil);
	SAFE_RELEASE(ShadowRenderTarget);

	SAFE_DELETE(Dungeon0);
	SAFE_DELETE(TheSky0);
	SAFE_DELETE(TheSun0);
	SAFE_DELETE(Universe0);
	SAFE_DELETE(Grid0);
}

void DungeonScene::Reset()
{
}

void DungeonScene::Update()
{
	DieCheck();
	if (TheSky0) TheSky0->Update();

	if (TheSun0)
	{
		TheSun0->Update();

		D3DXVECTOR4 positionOfTheSun0 = TheSun0->GetPosition();

		TheSun0->SetPosition(positionOfTheSun0);
		Dungeon0->SetWorldLightPos(-TheSun0->GetPosition());
		CharacData::GetLethita()->SetWorldLightPos(-TheSun0->GetPosition());
		for (int i = 0; i < MonstMax; ++i)
		{
			if (hellLady[i]) hellLady[i]->SetWorldLightPos(-TheSun0->GetPosition());
		}
		if (succubus) succubus->SetWorldLightPos(-TheSun0->GetPosition());
	}

	if (Universe0) Universe0->Update();

	if (Dungeon0)
	{
		Dungeon0->Update();
		if (succubus) Dungeon0->MonsterUpdate(succubus);
		for (int i = 0; i < MonstMax; ++i)
		{
			if (hellLady[i])
			{
				Dungeon0->MonsterUpdate(hellLady[i]);
			}
		}
	}
	
	D3DXVECTOR3 distance = CharacData::GetLethita()->GetPosition() - Dungeon0->GetPortalPosition();
	
	if ((distance.x * distance.x + distance.y * distance.y + distance.z * distance.z) < 100)
	{
		CharacData::GetLethita()->SetPosition(D3DXVECTOR3(500.0f, 500.0f, 550.0f));
		succubus->SetIsLive(true);
		CharacData::SetInfomation(CH_EXP, 1);

		SoundManager::Get().StopAllSound();
		SoundManager::Get().play("bgm_ep8_succubus", 0.5f);
		SoundManager::Get().SetSoundTime("bgm_ep8_succubus", 30);
	}

	if (CharacData::GetUnEqipInfo(CH_EXP) > 10)
	{
		SceneManager::Get().ChangeScene(SceneType::SCENE_TOWN);
	}
}

void DungeonScene::Render()
{
	if (Universe0) Universe0->Render();

	RenderShadow();
	
	if (Grid0) Grid0->Render();
	if (TheSky0) TheSky0->Render();
	if (TheSun0) TheSun0->Render(); //TheSun0->Shader();
	
	if (Dungeon0) Dungeon0->Render();
	
	for (int i = 0; i < MonstMax; ++i)
	{
		if (hellLady[i]) hellLady[i]->UpdateAndRender();
	}
	if (succubus) succubus->UpdateAndRender();

	CharacData::GetLethita()->UpdateAndRender();

	if (CharacData::GetUnEqipInfo(CH_EXP) == 3)
	{
		RECT fpsRectangle = { 1200, 600, 1201, 601 };
		char fpsText[256] = "서큐버스의 은신처로 향하는 문이 열렸습니다.";
		UI_Manager::GetFont()->DrawTextA(NULL, fpsText, strlen(fpsText), &fpsRectangle, DT_TOP | DT_RIGHT | DT_NOCLIP, D3DCOLOR_XRGB(255, 255, 255));
	}
}

void DungeonScene::RenderShadow()
{
	GameManager::GetDevice()->SetSamplerState(0, D3DSAMP_BORDERCOLOR, 0xffffffff);

	GameManager::GetDevice()->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	GameManager::GetDevice()->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
	GameManager::GetDevice()->SetSamplerState(1, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	GameManager::GetDevice()->SetSamplerState(1, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

	D3DXVECTOR4 WorldLightPosition = TheSun0->GetPosition();

	D3DXMATRIXA16 matWorld;

	// 광원-뷰 행렬을 만든다.
	D3DXMATRIXA16 matLightView;
	{
		D3DXVECTOR3 vEyePt(WorldLightPosition.x, WorldLightPosition.y, WorldLightPosition.z);
		D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
		D3DXMatrixLookAtLH(&matLightView, &vEyePt, &vLookatPt, &vUpVec);
	}

	// 광원-투영 행렬을 만든다.
	D3DXMATRIXA16 matLightProjection;
	D3DXMatrixPerspectiveFovLH(&matLightProjection, D3DX_PI / 2.0f, 1, 1, 3000);

	//Set LightViewProjection of the Objects to Render Shadow
	D3DXMATRIXA16 matLightViewProjection;
	matLightViewProjection = matLightView * matLightProjection;

	Dungeon0->SetLightViewProjection(matLightViewProjection);
	CharacData::GetLethita()->SetLightViewProjection(matLightViewProjection);
	for (int i = 0; i < MonstMax; ++i)
	{
		if (hellLady[i]) hellLady[i]->SetLightViewProjection(matLightViewProjection);
	}
	if (succubus) succubus->SetLightViewProjection(matLightViewProjection);

	// 뷰/투영행렬을 만든다.
	D3DXMATRIXA16 matViewProjection;
	{
		// 뷰 행렬을 만든다.
		D3DXMATRIXA16 matView;
		GameManager::GetDevice()->GetTransform(D3DTS_VIEW, &matView);

		// 투영행렬을 만든다.
		D3DXMATRIXA16 matProjection;
		GameManager::GetDevice()->GetTransform(D3DTS_PROJECTION, &matProjection);

		D3DXMatrixMultiply(&matViewProjection, &matView, &matProjection);
	}

	// 현재 하드웨어 벡버퍼와 깊이버퍼
	LPDIRECT3DSURFACE9 pHWBackBuffer = nullptr;
	LPDIRECT3DSURFACE9 pHWDepthStencilBuffer = nullptr;
	GameManager::GetDevice()->GetRenderTarget(0, &pHWBackBuffer);
	GameManager::GetDevice()->GetDepthStencilSurface(&pHWDepthStencilBuffer);

	//////////////////////////////
	// 1. 그림자 만들기
	//////////////////////////////

	// 그림자 맵의 렌더타깃과 깊이버퍼를 사용한다.
	LPDIRECT3DSURFACE9 pShadowSurface = nullptr;
	if (SUCCEEDED(ShadowRenderTarget->GetSurfaceLevel(0, &pShadowSurface)))
	{
		GameManager::GetDevice()->SetRenderTarget(0, pShadowSurface);
		pShadowSurface->Release();
		pShadowSurface = nullptr;
	}
	GameManager::GetDevice()->SetDepthStencilSurface(ShadowDepthStencil);

	// 저번 프레임에 그렸던 그림자 정보를 지움
	GameManager::GetDevice()->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), 0xffffffff, 1.0f, 0);

	// 그림자 만들기 쉐이더 전역변수들을 설정
	CreateShadowShader->SetMatrix("gWorldMatrix", &matWorld);
	CreateShadowShader->SetMatrix("gLightViewMatrix", &matLightView);
	CreateShadowShader->SetMatrix("gLightProjectionMatrix", &matLightProjection);

	// 그림자 만들기 쉐이더를 시작
	{
		UINT numPasses = 0;
		CreateShadowShader->Begin(&numPasses, NULL);
		{
			for (UINT i = 0; i < numPasses; ++i)
			{
				CreateShadowShader->BeginPass(i);
				{
					CreateShadowShader->SetMatrix("gWorldMatrix", &Dungeon0->GetWorld());
					CreateShadowShader->CommitChanges();
					Dungeon0->RenderShadow();

					CreateShadowShader->SetMatrix("gWorldMatrix", &(CharacData::GetLethita()->GetWorld()));
					CreateShadowShader->CommitChanges();
					CharacData::GetLethita()->RenderShadow();

					for (int i = 0; i < MonstMax; ++i)
					{
						if (hellLady[i])
						{
							CreateShadowShader->SetMatrix("gWorldMatrix", &(hellLady[i]->GetWorld()));
							CreateShadowShader->CommitChanges();
							hellLady[i]->RenderShadow();
						}
					}
					if (succubus)
					{
						CreateShadowShader->SetMatrix("gWorldMatrix", &(succubus->GetWorld()));
						CreateShadowShader->CommitChanges();
						succubus->RenderShadow();
					}
				}
				CreateShadowShader->EndPass();
			}
		}
		CreateShadowShader->End();
	}


	//////////////////////////////
	// 2. 그림자 입히기
	//////////////////////////////

	// 하드웨어 백버퍼/깊이버퍼를 사용한다.
	GameManager::GetDevice()->SetRenderTarget(0, pHWBackBuffer);
	GameManager::GetDevice()->SetDepthStencilSurface(pHWDepthStencilBuffer);

	pHWBackBuffer->Release();
	pHWBackBuffer = nullptr;
	pHWDepthStencilBuffer->Release();
	pHWDepthStencilBuffer = nullptr;

	// 그림자 입히기 쉐이더 전역변수들을 설정
	ApplyShadowShader->SetMatrix("gWorldMatrix", &matWorld);
	ApplyShadowShader->SetMatrix("gViewProjectionMatrix", &matViewProjection);
	ApplyShadowShader->SetMatrix("gLightViewMatrix", &matLightView);
	ApplyShadowShader->SetMatrix("gLightProjectionMatrix", &matLightProjection);

	ApplyShadowShader->SetVector("gWorldLightPosition", &WorldLightPosition);
	ApplyShadowShader->SetTexture("ShadowMap_Tex", ShadowRenderTarget);
	ApplyShadowShader->SetTexture("OriginalMap_Tex", Dungeon0->GetTexture());

	// 쉐이더를 시작한다.
	UINT numPasses = 0;
	ApplyShadowShader->Begin(&numPasses, NULL);
	{
		for (UINT i = 0; i < numPasses; ++i)
		{
			ApplyShadowShader->BeginPass(i);
			{
				ApplyShadowShader->SetMatrix("gWorldMatrix", &Dungeon0->GetWorld());
				ApplyShadowShader->CommitChanges();
				Dungeon0->UpdateAndRender();
			}
			ApplyShadowShader->EndPass();
		}
	}
	ApplyShadowShader->End();
}

void DungeonScene::RenderWater()
{

}

void DungeonScene::OnEnterState()
{
	//DWORD threadID = 0;
	//isReading = false;
	//InitializeCriticalSection(&cs);
	//threadHandle = CreateThread(nullptr, 0, ThreadLodingRend, this, CREATE_SUSPENDED, &threadID);
	//ResumeThread(threadHandle);

	Initialize();
	//DWORD temp = WaitForSingleObject(threadHandle, INFINITE);

	//if (temp == WAIT_FAILED)
	//{
	//	int k = 0;
	//}
	//DeleteCriticalSection(&cs);
	//TerminateThread(threadHandle, 0);

	//DWORD threadID1 = 0, threadID2 = 0;
	//HANDLE threadHandles[2];
	//threadHandles[0] = nullptr;
	//threadHandles[1] = nullptr;
	//InitializeCriticalSection(&cs);
	//threadHandles[0] = CreateThread(
	//	nullptr, 0, ThreadInit, this,
	//	CREATE_SUSPENDED, &threadID1);	//CREATE_SUSPENDED자리가 null이면 바로 시작
	//threadHandles[1] = CreateThread(
	//	nullptr, 0, ThreadLodingRend, this,
	//	CREATE_SUSPENDED, &threadID2);
	//ResumeThread(threadHandles[0]);
	//ResumeThread(threadHandles[1]);
	//WaitForMultipleObjects(2, threadHandles, TRUE, INFINITE);
	//CloseHandle(threadHandles[0]);
	//CloseHandle(threadHandles[1]);
	//DeleteCriticalSection(&cs);

	GameManager::GetCamera()->SetEyeHeight(7.0f);
	GameManager::GetCamera()->SetDistance(20.0f);

}

void DungeonScene::OnLeaveState()
{
	Destroy();
}

LRESULT DungeonScene::InputProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{ 
	LRESULT result;

	result = 0;

	return result;
}

DWORD DungeonScene::ThreadInit(LPVOID lparam)
{
	DungeonScene* test = (DungeonScene*)lparam;
	test->isReading = false;
	test->Initialize();

	//EnterCriticalSection(&test->cs);
	test->isReading = true;
//	LeaveCriticalSection(&test->cs);
	return 0;
}

DWORD DungeonScene::ThreadLodingRend(LPVOID lparam)
{
	DungeonScene* test = (DungeonScene*)lparam;
	int koo = 0;
	while (1)
	{
	//	
		if (!test->isReading)
		{
			EnterCriticalSection(&test->cs);
			GameManager::GetDevice()->Clear(
				0, nullptr,	//화면전체 클리어
				D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
				D3DCOLOR_XRGB(0, 0, 50),
				1.0f,	//z값 0~1. 1이면 가장 멀다
				0);		//스텐실 버퍼용 세팅

						//direct3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);	//와이어 프레임 모드로 그리기

			GameManager::GetDevice()->BeginScene();

			// 그림 그리기 -------------------------------------------------
			//test->succubusTemp->UpdateAndRender();

			RECT fpsRectangle = { RESOLUTION_X / 2, 10, RESOLUTION_X, 11 };
			char fpsText[256];
			sprintf_s(fpsText, "LODING... %d", koo);
			UI_Manager::GetFont()->DrawTextA(NULL, fpsText, strlen(fpsText), &fpsRectangle, DT_TOP | DT_RIGHT | DT_NOCLIP, D3DCOLOR_XRGB(255, 255, 255));
			koo += 1;

			//CharacData::GetLethita()->UpdateAndRender();
			fpsRectangle = { 1200, 600, 1201, 601 };
			char textT[512] = "TIP : 맵에디터와 옵션 같은거는 현재 사용 불가능 합니다~ \n사용하기 싶으시면 돈을 내세요 ^오^ ";
			UI_Manager::GetFont()->DrawTextA(NULL, textT, strlen(textT), &fpsRectangle, DT_TOP | DT_RIGHT | DT_NOCLIP, D3DCOLOR_XRGB(255, 255, 255));


			//--------------------------------------------------------------

			GameManager::GetDevice()->EndScene();
			GameManager::GetDevice()->Present(NULL, NULL, NULL, NULL);
			LeaveCriticalSection(&test->cs);
		}
		else return 0;
	//	

	}
	return 0;
}

void DungeonScene::DieCheck()
{
	for (int i = 0; i < MonstMax; ++i)
	{
		if (hellLady[i])
		{
			if (hellLady[i]->GetCurrentState() == STATE_DELETE)
			{
				SAFE_DELETE(hellLady[i]);
				hellLady[i] = nullptr;
			}
		}
	}

	if (succubus)
	{
		if (succubus->GetCurrentState() == STATE_DELETE)
		{
			SAFE_DELETE(succubus);
			succubus = nullptr;
		}
	}


}
