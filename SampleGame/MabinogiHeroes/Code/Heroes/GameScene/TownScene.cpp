#include "stdafx.h"
#include "TownScene.h"

TownScene::TownScene() { Grid0 = nullptr; TheSun0 = nullptr; TheSky0 = nullptr; Town0 = nullptr; Ti0 = nullptr; Eire0 = nullptr;  }
TownScene::~TownScene() {}

void TownScene::Initialize()
{
	Reset();
	UI_Mnager::SetUsing(true);

	if (!Grid0)
	{
		Grid0 = new Grid;
		Grid0->Initialize();
		Grid0->SetShowGround(1);
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

	if (!Town0)
	{
		Town0 = new Town;
		Town0->Initialize();
	}

	if (!Ti0)
	{
		Ti0 = new Ti;
		Ti0->Initialize();
	}

	if (!Eire0)
	{
		Eire0 = new Eire;
		Eire0->Initialize();
		Eire0->SetTalking(0);
	}

	if (CharacData::GetUnEqipInfo(CH_EXP) > 10)
	{
		D3DXVECTOR3 temp = Town0->GetPortalPosition();
		temp.x += 20.0f;
		CharacData::GetLethita()->SetPosition(temp);
	}
	else
	{
		CharacData::GetLethita()->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 20.0f));
	}
	CharacData::GetLethita()->SetIsPlay(true);
	CharacData::SetInfomation(CH_HP, 120);
	CharacData::GetLethita()->SetAnimationIndex(0);
	CharacData::GetLethita()->Initialize("./Lethita/", "LethitaTownRest.X");
	GameManager::GetCamera()->SetLookTarget(CharacData::GetLethita()->GetPositionAddress());

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

	SoundManager::Get().StopAllSound();
	SoundManager::Get().play("bgm_colhen_01", 0.5f);
	isReading = true;
}

void TownScene::Destroy()
{
	if (threadHandle)
	{
		CloseHandle(threadHandle);
		threadHandle = nullptr;
	}
	SAFE_RELEASE(uvAnimationShader);
	SAFE_RELEASE(ApplyShadowShader);
	SAFE_RELEASE(CreateShadowShader);

	SAFE_RELEASE(ShadowDepthStencil);
	SAFE_RELEASE(ShadowRenderTarget);

	SAFE_DELETE(Eire0);
	SAFE_DELETE(Ti0);
	SAFE_DELETE(Town0);
	SAFE_DELETE(TheSky0);
	SAFE_DELETE(TheSun0);
	SAFE_DELETE(Grid0);
}

void TownScene::Reset()
{
}

void TownScene::Update()
{
	if (TheSky0) TheSky0->Update();

	if (TheSun0)
	{
		TheSun0->Update();

		D3DXVECTOR4 positionOfTheSun0 = TheSun0->GetPosition();

		if (GameManager::GetKey()->isStayKeyDown(VK_NUMPAD7)) positionOfTheSun0.x += 10.0f;
		if (GameManager::GetKey()->isStayKeyDown(VK_NUMPAD8)) positionOfTheSun0.y += 10.0f;
		if (GameManager::GetKey()->isStayKeyDown(VK_NUMPAD9)) positionOfTheSun0.z += 10.0f;
		if (GameManager::GetKey()->isStayKeyDown(VK_NUMPAD4)) positionOfTheSun0.x -= 10.0f;
		if (GameManager::GetKey()->isStayKeyDown(VK_NUMPAD5)) positionOfTheSun0.y -= 10.0f;
		if (GameManager::GetKey()->isStayKeyDown(VK_NUMPAD6)) positionOfTheSun0.z -= 10.0f;

		TheSun0->SetPosition(positionOfTheSun0);
		Town0->SetWorldLightPos(-TheSun0->GetPosition());
		CharacData::GetLethita()->SetWorldLightPos(-TheSun0->GetPosition());
	}

	if (Town0) Town0->Update();

	if (Ti0 && Ti0->GetTalking()) Ti0->Update();

	if (Eire0 && Eire0->GetTalking()) Eire0->Update();
		
	D3DXVECTOR3 distance = CharacData::GetLethita()->GetPosition() - D3DXVECTOR3(-5.0f, 0.0f, 0.0f);
	
	if ((distance.x * distance.x + distance.y * distance.y + distance.z * distance.z) < 150)
	{
		if (CharacData::GetUnEqipInfo(CH_EXP) < 10)
		{
			if (!Ti0->GetTalking()) Ti0->SetTalking(1);

			if (!SoundManager::Get().isPlaySound("bgm_colhen_inn"))
			{
				SoundManager::Get().StopAllSound();
				SoundManager::Get().play("bgm_colhen_inn", 0.5f);
			}
		}
		else
		{
			if (!Eire0->GetTalking()) Eire0->SetTalking(1);

			if (!SoundManager::Get().isPlaySound("bgm_colhen_inn_sereuha"))
			{
				SoundManager::Get().StopAllSound();
				SoundManager::Get().play("bgm_colhen_inn_sereuha", 0.5f);
			}
		}
		
	}
	else
	{
		if (!SoundManager::Get().isPlaySound("bgm_colhen_01"))
		{
			SoundManager::Get().StopAllSound();
			SoundManager::Get().play("bgm_colhen_01", 0.5f);
		}

		if (Ti0->GetTalking())
		{
			Ti0->SetTalking(0);
			Ti0->ResetDialogue();
		}

		if (Eire0->GetTalking())
		{
			Eire0->SetTalking(0);
			Eire0->ResetDialogue();
		}
	}
	
	distance = CharacData::GetLethita()->GetPosition() - Town0->GetPortalPosition();

	if ((distance.x * distance.x + distance.y * distance.y + distance.z * distance.z) < 100)
	{
		SoundManager::Get().StopAllSound();
		SceneManager::Get().ChangeScene(SCENE_DUNGEON);
	}
}

void TownScene::Render()
{
	RenderShadow();
	//RenderWater();
	
	if (Grid0) Grid0->Render();
	if (TheSky0) TheSky0->Render();
	if (TheSun0) TheSun0->Render(); //TheSun0->Shader();
	
	if (Town0) Town0->Render();
	
	CharacData::GetLethita()->UpdateAndRender();

	if (Ti0 && Ti0->GetTalking()) Ti0->Render();
	if (Eire0 && Eire0->GetTalking()) Eire0->Render();
}

void TownScene::RenderShadow()
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
	
	Town0->SetLightViewProjection(matLightViewProjection);
	CharacData::GetLethita()->SetLightViewProjection(matLightViewProjection);

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
					CreateShadowShader->SetMatrix("gWorldMatrix", &Town0->GetWorld());
					CreateShadowShader->CommitChanges();
					Town0->RenderShadow();

					CreateShadowShader->SetMatrix("gWorldMatrix", &(CharacData::GetLethita()->GetWorld()));
					CreateShadowShader->CommitChanges();
					CharacData::GetLethita()->RenderShadow();
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
	ApplyShadowShader->SetTexture("OriginalMap_Tex", Town0->GetTexture());

	// 쉐이더를 시작한다.
	UINT numPasses = 0;
	ApplyShadowShader->Begin(&numPasses, NULL);
	{
		for (UINT i = 0; i < numPasses; ++i)
		{
			ApplyShadowShader->BeginPass(i);
			{
				ApplyShadowShader->SetMatrix("gWorldMatrix", &Town0->GetWorld());
				ApplyShadowShader->CommitChanges();
				Town0->UpdateAndRender();
			}
			ApplyShadowShader->EndPass();
		}
	}
	ApplyShadowShader->End();
}

void TownScene::RenderWater()
{
	D3DXVECTOR4 WorldLightPosition = TheSun0->GetPosition();
	
	D3DXMATRIXA16 matWorld;
	D3DXMATRIXA16 matView, matInverseView, matProjection, matViewProjection, matWorldViewProjection;
	D3DXVECTOR3 vEye = D3DXVECTOR3(0, 0, 0);
	
	GameManager::GetDevice()->GetTransform(D3DTS_VIEW, &matView);
	GameManager::GetDevice()->GetTransform(D3DTS_PROJECTION, &matProjection);

	D3DXMatrixMultiply(&matViewProjection, &matView, &matProjection);
	D3DXMatrixMultiply(&matWorldViewProjection, &matWorld, &matViewProjection);
	D3DXMatrixInverse(&matInverseView, 0, &matView);
	
	D3DXVec3TransformCoord(&vEye, &vEye, &matInverseView);

	D3DXVECTOR4 WorldCameraPosition = D3DXVECTOR4(vEye.x, vEye.y, vEye.z, 1.0f);
	D3DXVECTOR4 LightColor = D3DXVECTOR4(0.7f, 0.7f, 1.0f, 1.0f);

	// 쉐이더 전역변수들을 설정
	uvAnimationShader->SetMatrix("gWorldMatrix", &matWorld);
	uvAnimationShader->SetMatrix("gViewMatrix", &matView);
	uvAnimationShader->SetMatrix("gProjectionMatrix", &matProjection);
	uvAnimationShader->SetMatrix("gWorldViewProjectionMatrix", &matWorldViewProjection);

	uvAnimationShader->SetVector("gWorldLightPosition", &WorldLightPosition);
	uvAnimationShader->SetVector("gWorldCameraPosition", &WorldCameraPosition);
	uvAnimationShader->SetVector("gLightColor", &LightColor);

	//uvAnimationShader->SetTexture("DiffuseMap_Tex", gpStoneDM);
	//uvAnimationShader->SetTexture("SpecularMap_Tex", gpStoneSM);
	//uvAnimationShader->SetTexture("NormalMap_Tex", gpStoneNM);
	//uvAnimationShader->SetTexture("EnvironmentMap_Tex", gpSnowENV);

	ULONGLONG tick = GetTickCount64();
	uvAnimationShader->SetFloat("gTime", tick / 1000.0f);
	uvAnimationShader->SetFloat("gWaveHeight", 3);
	uvAnimationShader->SetFloat("gSpeed", 2);
	uvAnimationShader->SetFloat("gWaveFrequency", 10);
	uvAnimationShader->SetFloat("gUVSpeed", 0.25f);

	UINT numPasses = 0;
	uvAnimationShader->Begin(&numPasses, NULL);
	{
		for (UINT i = 0; i < numPasses; ++i)
		{
			uvAnimationShader->BeginPass(i);
			{
				//gpTeapot->DrawSubset(0);
			}
			uvAnimationShader->EndPass();
		}
	}
	uvAnimationShader->End();
}

void TownScene::OnEnterState()
{
	//DWORD threadID = 0;
	//isReading = false;
	//InitializeCriticalSection(&cs);
	//threadHandle = CreateThread(nullptr, 0, ThreadLodingRend, this, CREATE_SUSPENDED, &threadID);
	//ResumeThread(threadHandle);

	Initialize();
	//DWORD temp = WaitForSingleObject(threadHandle, INFINITE);
	/*
	if (temp == WAIT_FAILED)
	{
		int k = 0;
	}*/
	
//	TerminateThread(threadHandle, 0);
	//DeleteCriticalSection(&cs);

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

void TownScene::OnLeaveState()
{
	Destroy();
}

LRESULT TownScene::InputProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result;

	result = 0;

	if (!result) result = Ti0->InputProc(hWnd, message, wParam, lParam);
	if (!result) result = Eire0->InputProc(hWnd, message, wParam, lParam);

	return result;
}

DWORD TownScene::ThreadInit(LPVOID lparam)
{
	TownScene* test = (TownScene*)lparam;
	test->isReading = false;
	test->Initialize();

	EnterCriticalSection(&test->cs);
	test->isReading = true;
	LeaveCriticalSection(&test->cs);

	return 0;
}

DWORD TownScene::ThreadLodingRend(LPVOID lparam)
{
	TownScene* test = (TownScene*)lparam;
	int koo = 0;
	while (1)
	{
//		EnterCriticalSection(&test->cs);
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
	//	LeaveCriticalSection(&test->cs);

	}
	return 0;
}
