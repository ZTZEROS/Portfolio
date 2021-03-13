#include "stdafx.h"
#include "OpeningScene.h"

OpeningScene::OpeningScene() { Background = nullptr; ZeroMemory(&OpeningMenus, sizeof(UI_Button*) * OPENING_MENU_COUNT); }
OpeningScene::~OpeningScene() {}

void OpeningScene::Initialize()
{
	Reset();
	UI_Mnager::SetUsing(0);

	if (!Background) Background = TextureManager::GetTexture("OpeningBackground.jpg");

	D3DXVECTOR3 backgroundVertexes[4];

	float width, height;

	width = 1920.f * 0.0075f;
	height = 1200.f * 0.0075f;

	backgroundVertexes[0] = D3DXVECTOR3(-width * 0.5f, height * 0.5f, 0.0f);
	backgroundVertexes[1] = D3DXVECTOR3(width * 0.5f, height * 0.5f, 0.0f);
	backgroundVertexes[2] = D3DXVECTOR3(-width * 0.5f, -height * 0.5f, 0.0f);
	backgroundVertexes[3] = D3DXVECTOR3(width * 0.5f, -height * 0.5f, 0.0f);

	D3DXVECTOR2 uvs[4];

	uvs[0] = D3DXVECTOR2(0, 0);
	uvs[1] = D3DXVECTOR2(1, 0);
	uvs[2] = D3DXVECTOR2(0, 1);
	uvs[3] = D3DXVECTOR2(1, 1);

	BackgroundTriangles[0] = TextureTriangle(backgroundVertexes[0], backgroundVertexes[1], backgroundVertexes[2], uvs[0], uvs[1], uvs[2]);
	BackgroundTriangles[1] = TextureTriangle(backgroundVertexes[1], backgroundVertexes[2], backgroundVertexes[3], uvs[1], uvs[2], uvs[3]);
	
	D3DXMatrixIdentity(&BackgroundWorld);

	for (unsigned int i = 0; i < OPENING_MENU_COUNT; ++i)
	{
		if (!OpeningMenus[i]) //if (OpeningMenus[i]) continue;
		{
			OpeningMenus[i] = new UI_Button;
			OpeningMenus[i]->SetTexture("./UI/OpeningMenuButtonNormal.dds", "./UI/OpeningMenuButtonMouseOver.dds", "./UI/OpeningMenuButtonPressed.dds");
			OpeningMenus[i]->SetSize(256.0f, 32.0f);
			OpeningMenus[i]->SetScale(1.25f);
			OpeningMenus[i]->SetPosition(RESOLUTION_X * 0.5f - OpeningMenus[i]->GetSize().x * 0.5f, RESOLUTION_Y * 0.625f + i * OpeningMenus[i]->GetSize().y * 1.25f, 0.0f);
		}
	}


	OpeningMenus[OPENING_MENU_GAME_START]->OnClick = std::bind(&UI_Functions::GameStartButton, std::ref(UI_Manager::Get().func));
	OpeningMenus[OPENING_MENU_GAME_START]->SetText("GAME START");
	OpeningMenus[OPENING_MENU_GAME_START]->SetFont(UI_Manager::GetItemFont());
	OpeningMenus[OPENING_MENU_GAME_START]->SetTextPosition(120, 14, 0);
	OpeningMenus[OPENING_MENU_MAP_TOOL]->OnClick = std::bind(&UI_Functions::MapToolButton, std::ref(UI_Manager::Get().func));
	OpeningMenus[OPENING_MENU_MAP_TOOL]->SetText("MAP TOOL");
	OpeningMenus[OPENING_MENU_MAP_TOOL]->SetFont(UI_Manager::GetItemFont());
	OpeningMenus[OPENING_MENU_MAP_TOOL]->SetTextPosition(125, 14, 0);
	OpeningMenus[OPENING_MENU_OPTION]->OnClick = std::bind(&UI_Functions::OptionButton, std::ref(UI_Manager::Get().func));
	OpeningMenus[OPENING_MENU_OPTION]->SetText("GAME OPTION");
	OpeningMenus[OPENING_MENU_OPTION]->SetFont(UI_Manager::GetItemFont());
	OpeningMenus[OPENING_MENU_OPTION]->SetTextPosition(115, 14, 0);
	OpeningMenus[OPENING_MENU_HELP]->OnClick = std::bind(&UI_Functions::HelpButton, std::ref(UI_Manager::Get().func));
	OpeningMenus[OPENING_MENU_HELP]->SetText("GAME HELP");
	OpeningMenus[OPENING_MENU_HELP]->SetFont(UI_Manager::GetItemFont());
	OpeningMenus[OPENING_MENU_HELP]->SetTextPosition(120, 14, 0);
	OpeningMenus[OPENING_MENU_EXIT]->OnClick = std::bind(&UI_Functions::ExitButton, std::ref(UI_Manager::Get().func));
	OpeningMenus[OPENING_MENU_EXIT]->SetText("GAME EXIT");
	OpeningMenus[OPENING_MENU_EXIT]->SetFont(UI_Manager::GetItemFont());
	OpeningMenus[OPENING_MENU_EXIT]->SetTextPosition(120, 14, 0);

	//Initializefx();
	//Setupfx();
}

void OpeningScene::Destroy()
{
	for (unsigned int i = 0; i < OPENING_MENU_COUNT; ++i)
	{
		SAFE_DELETE(OpeningMenus[i]);
		OpeningMenus[i] = nullptr;
	}

	SAFE_RELEASE(Effect);
	SAFE_RELEASE(VertexDeclaration);
	SAFE_RELEASE(VertexShader);
	SAFE_RELEASE(BaseTexture);
}

void OpeningScene::Reset()
{
}

void OpeningScene::Update()
{
	for (unsigned int i = 0; i < OPENING_MENU_COUNT; ++i)
	{
		if (OpeningMenus[i]) OpeningMenus[i]->Update();
	}
}

void OpeningScene::Render()
{
	GameManager::GetDevice()->Clear(0, nullptr,	D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,	D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	
	GameManager::GetDevice()->SetRenderState(D3DRS_LIGHTING, 0);
	GameManager::GetDevice()->SetFVF(FVF_PositionTexture::FVF);
	GameManager::GetDevice()->SetTransform(D3DTS_WORLD, &BackgroundWorld);
	GameManager::GetDevice()->SetTexture(0, Background);
	GameManager::GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 4, &BackgroundTriangles[0], sizeof(FVF_PositionTexture));
	
	for (unsigned int i = 0; i < OPENING_MENU_COUNT; ++i)
	{
		if (OpeningMenus[i]) OpeningMenus[i]->Render();
	}
	
	//Renderfx();
}

void OpeningScene::OnEnterState()
{
	//DWORD threadID1 = 0, threadID2 = 0;
	//HANDLE threadHandles[2];
	//
	//threadHandles[0] = nullptr;
	//threadHandles[1] = nullptr;
	//
	//threadHandles[0] = CreateThread(
	//	nullptr, 0, ThreadInit, this,
	//	CREATE_SUSPENDED, &threadID1);	//CREATE_SUSPENDED자리가 null이면 바로 시작
	//threadHandles[1] = CreateThread(
	//	nullptr, 0, ThreadLodingRend, this,
	//	CREATE_SUSPENDED, &threadID2);
	//
	//ResumeThread(threadHandles[0]);
	//ResumeThread(threadHandles[1]);
	//
	//WaitForMultipleObjects(2, threadHandles, TRUE, INFINITE);
	//
	//CloseHandle(threadHandles[0]);
	//CloseHandle(threadHandles[1]);

	Initialize();
}

void OpeningScene::OnLeaveState()
{
	Destroy();
}

LRESULT OpeningScene::InputProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;

	for (unsigned int i = 0; i < OPENING_MENU_COUNT; ++i)
	{
		if (OpeningMenus[i]) result = OpeningMenus[i]->InputProc(hWnd, message, wParam, lParam);
	}

	return result;
}


HRESULT OpeningScene::Initializefx()
{
	D3DVERTEXELEMENT9 declarations[MAX_FVF_DECL_SIZE];

	D3DXDeclaratorFromFVF(FVF_PositionTexture::FVF, declarations);

	GameManager::GetDevice()->CreateVertexDeclaration(declarations, &VertexDeclaration);
	
	D3DXMACRO mac[2] =
	{
		{ "MATRIX_PALETTE_SIZE_DEFAULT", "35" },
		{ NULL, NULL }
	};
	D3DCAPS9 caps;
	D3DXMACRO *pmac = NULL;
	GameManager::GetDevice()->GetDeviceCaps(&caps);
	if (caps.VertexShaderVersion > D3DVS_VERSION(1, 1))
		pmac = mac;
	
	if ((FAILED(D3DXCreateEffectFromFile(GameManager::GetDevice(), L"../ResourceData/Shader/shader.fx", pmac, nullptr, 0, nullptr, &Effect, &Error))))
		return E_FAIL;

	return S_OK;
}

void OpeningScene::Setupfx()
{
	Effect->SetTexture("tex0", BaseTexture);
	Effect->SetMatrix("matW", &World);
	Effect->SetMatrix("matV", &View);
	Effect->SetMatrix("matP", &Projection);
}

void OpeningScene::Renderfx()
{
	UINT nPass;

	GameManager::GetDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(200, 200, 200), 1.0f, 0);
	GameManager::GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	//Animate();

	LPD3DXMESH sphere;
	D3DXCreateSphere(GameManager::GetDevice(), 5, 5, 5, &sphere, nullptr);
	//sphere->DrawSubset(0);

	{
		//GameManager::GetDevice()->SetVertexDeclaration(VertexDeclaration);
	
		Effect->SetTechnique("Default_DirectX_Effect");
		Effect->Begin(&nPass, NULL);
	
		for (UINT i = 0; i < nPass; ++i)
		{
			Effect->BeginPass(i);
			sphere->DrawSubset(0);
			//GameManager::GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 4, &BackgroundTriangles[0], sizeof(FVF_PositionTexture));
			Effect->EndPass();
		}
		
		Effect->End();
	}
	//sphere->DrawSubset(0);

	SAFE_RELEASE(sphere);
}

DWORD OpeningScene::ThreadInit(LPVOID lparam)
{
	OpeningScene* test = (OpeningScene*)lparam;
	test->isReading = false;
	test->Initialize();

	test->isReading = true;
	return 0;
}

DWORD OpeningScene::ThreadLodingRend(LPVOID lparam)
{
	OpeningScene* test = (OpeningScene*)lparam;
	int koo = 0;
	while (!test->isReading)
	{

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

		//FPS 그리기
		RECT fpsRectangle = { RESOLUTION_X / 2, 10, RESOLUTION_X, 11 };
		char fpsText[256];
		sprintf_s(fpsText, "LODING... %d", koo);
		UI_Manager::GetFont()->DrawTextA(NULL, fpsText, strlen(fpsText), &fpsRectangle, DT_TOP | DT_RIGHT | DT_NOCLIP, D3DCOLOR_XRGB(255, 255, 255));
		koo += 1;

		fpsRectangle = { 1200, 600, 1201, 601 };
		sprintf_s(fpsText, "TIP : 맵에디터와 옵션 같은거는 현재 사용 불가능 합니다~ \n사용하기 싶으시면 돈을 내세요 ^오^ ", koo);
		UI_Manager::GetFont()->DrawTextA(NULL, fpsText, strlen(fpsText), &fpsRectangle, DT_TOP | DT_RIGHT | DT_NOCLIP, D3DCOLOR_XRGB(255, 255, 255));

		//--------------------------------------------------------------

		GameManager::GetDevice()->EndScene();

		GameManager::GetDevice()->Present(NULL, NULL, NULL, NULL);

	}
	return 0;
}
