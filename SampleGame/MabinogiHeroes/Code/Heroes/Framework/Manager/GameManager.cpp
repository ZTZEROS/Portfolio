#include "stdafx.h"
#include "GameManager.h"
#include "../../Heroes/HeroesSound.h"

GameManager::GameManager()
{
}


GameManager::~GameManager()
{
}

void GameManager::Initialize(HWND handle)
{
	windowHandle = handle;

	direct3d = Direct3DCreate9(D3D_SDK_VERSION);

	D3DCAPS9 caps;	//�ϵ������ �ɷ�ġ
	int	vertexProcessing;
	direct3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		vertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	else
	{
		vertexProcessing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	D3DPRESENT_PARAMETERS d3dPP;		//ȭ�� ǥ�� �Ķ����
	ZeroMemory(&d3dPP, sizeof(D3DPRESENT_PARAMETERS));
	d3dPP.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dPP.Windowed = true;
	d3dPP.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dPP.EnableAutoDepthStencil = true;
	d3dPP.AutoDepthStencilFormat = D3DFMT_D16;

	HRESULT result = direct3d->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		windowHandle,
		vertexProcessing,
		&d3dPP,
		&direct3dDevice);

	/*
	�������� ����,D3D_OK �� �����ش�.
	�������� ����, ������ ��� ���� �����ش�.

	D3DERR_INVALIDCALL �޼����� ȣ���� ��ȿ�̴�. ���� ���, �޼����� �Ķ���Ϳ� ��ȿ�� ���� �����Ǿ� �ִ� ��� ���̴�.
	D3DERR_NOTAVAILABLE �� ��ġ��, ������ ��ũ���� ���� �ϰ� ���� �ʴ�.
	D3DERR_OUTOFVIDEOMEMORY Direct3D �� ó���� �ǽ��ϴµ� ����� ���÷��� �޸𸮰� ����.

	*/
	if (result != D3D_OK)
	{
		MessageBoxA(windowHandle, "CreateDevice Fail!!", "D3D Critical Error", MB_OK);
		return;
	}

	InitializeManager();

	////������ ����
	//DWORD threadID1 = 0, threadID2 = 0;
	//HANDLE threadHandles[2];

	//threadHandles[0] = nullptr;
	//threadHandles[1] = nullptr;

	//threadHandles[0] = CreateThread(
	//	nullptr, 0, ThreadInit, this,
	//	CREATE_SUSPENDED, &threadID1);	//CREATE_SUSPENDED�ڸ��� null�̸� �ٷ� ����
	//threadHandles[1] = CreateThread(
	//	nullptr, 0, ThreadLodingRend, this,
	//	CREATE_SUSPENDED, &threadID2);

	//ResumeThread(threadHandles[0]);
	//ResumeThread(threadHandles[1]);

	//WaitForMultipleObjects(2, threadHandles, TRUE, INFINITE);

	//CloseHandle(threadHandles[0]);
	//CloseHandle(threadHandles[1]);
	
}

void GameManager::InitializeManager()
{

	//FPS �ʱ�ȭ
	fpsCount = 0;
	fpsTimeCheck = 0.0f;
	strcpy_s(fpsText, "FPS : 0");

	//���� �ʱ�ȭ �� ����
	ZeroMemory(&mainLight, sizeof(D3DLIGHT9));
	mainLight.Type = D3DLIGHTTYPE::D3DLIGHT_DIRECTIONAL;
	mainLight.Ambient = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
	mainLight.Diffuse = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
	mainLight.Specular = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
	D3DXVECTOR3 direction(1.0f, -2.0f, 1.0f);
	D3DXVec3Normalize(&direction, &direction);
	mainLight.Direction = direction;

	direct3dDevice->SetLight(0, &mainLight);
	direct3dDevice->LightEnable(0, true);

	key = new KeyManager();
	key->Initialize();

	camera = new CameraManager();
	camera->Initialize();

	//���� �ʱ�ȭ
	RandomUtil::Initialize();
	SoundManager::Get().Initilize();

	//DataBase �ʱ�ȭ
	CharacData::Get().Initialize();
	ItemData::Get().Initialize();

	//UI�ʱ�ȭ
	UI_Manager::Get().Initialize();
	UI_Mnager::Get().Initilize();


	HeroesSound* soundPack = new HeroesSound;
	soundPack->Initilize();

	//GameState�� �ʱ�ȭ
	SceneManager::Get().Initialize();
}

void GameManager::Destroy()
{
	
	//DataBse ����
	ItemData::Get().Destroy();
	CharacData::Get().Destroy();

	UI_Mnager::Get().Destroy();

	//GameState�� ����
	SceneManager::Get().Destroy();

	UI_Manager::Get().Destroy();

	SoundManager::Get().Destroy();

	SAFE_DELETE(camera);
	SAFE_DELETE(key);

	TextureManager::Get().Destroy();
	SkinnedMeshManager::Get().Destroy();

	//����̽� ������
	ULONG result = 0;
	result = SAFE_RELEASE(direct3dDevice);
#ifdef _DEBUG
	assert(result == 0 && "����̽��� �̿��� ������ ��ü �� �������� ���� ��ü�� �ֽ��ϴ�.");
#endif

	SAFE_RELEASE(direct3d);
}

void GameManager::Loop(double tick)
{
	currentTickTime = tick;

	Update();
	Render();
}

void GameManager::Update()
{
	//FPS ������Ʈ
	fpsCount++;
	fpsTimeCheck += (float)currentTickTime;
	if (fpsTimeCheck > 1.0f)
	{
		sprintf_s(fpsText, "FPS : %d", fpsCount);
		fpsCount = 0;
		fpsTimeCheck = 0.0f;
	}


	SoundManager::Get().update();
	//GameState ������Ʈ
	SceneManager::Get().Update();

	if (camera)
	{
		camera->Update();
	}

	UI_Mnager::Get().Update();

	UI_Manager::Get().Update();

}

void GameManager::Render()
{
	direct3dDevice->Clear(
		0, nullptr,	//ȭ����ü Ŭ����
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(0, 0, 50),
		1.0f,	//z�� 0~1. 1�̸� ���� �ִ�
		0);		//���ٽ� ���ۿ� ����

				//direct3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);	//���̾� ������ ���� �׸���

	direct3dDevice->BeginScene();

	// �׸� �׸��� -------------------------------------------------

	//GameState �׸���
	SceneManager::Get().Render();

	UI_Manager::Get().Render();
	UI_Mnager::Get().Render();

	//FPS �׸���
	//RECT fpsRectangle = { RESOLUTION_X / 2, 10, RESOLUTION_X, 11 };
	//RECT mousePositionRectangle = { RESOLUTION_X / 2, RESOLUTION_Y / 2, RESOLUTION_X, RESOLUTION_Y };
	//UI_Manager::GetFont()->DrawTextA(NULL, fpsText, strlen(fpsText), &fpsRectangle, DT_TOP | DT_RIGHT | DT_NOCLIP, D3DCOLOR_XRGB(255, 255, 255));
	//UI_Manager::GetFont()->DrawTextA(NULL, mouseText, strlen(mouseText), &mousePositionRectangle, DT_BOTTOM | DT_RIGHT | DT_NOCLIP, D3DCOLOR_XRGB(255, 255, 255));


	//--------------------------------------------------------------

	direct3dDevice->EndScene();

	direct3dDevice->Present(NULL, NULL, NULL, NULL);
}

LRESULT GameManager::GameInputProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	mousePoint.x = GET_X_LPARAM(lParam);
	mousePoint.y = GET_Y_LPARAM(lParam);
	sprintf_s(mouseText, "%d, %d", mousePoint.x, mousePoint.y);
	//�����̵� �� �޼����� ���� ó���� �־����� �� ������ ����
	LRESULT result = 0;
	if (result == 0 && camera)
	{
		camera->CameraInputProc(hWnd, message, wParam, lParam);
	}
	if (result == 0)
	{
		result = UI_Manager::Get().InputProc(hWnd, message, wParam, lParam);	//UI�� �ֿ켱 ó��
	}
	if (result == 0)
	{
		result = SceneManager::Get().InputProc(hWnd, message, wParam, lParam);
	}
	if (result == 0)
	{
		result = UI_Mnager::Get().InputProc(hWnd, message, wParam, lParam);
	}

	return result;
}

DWORD GameManager::ThreadInit(LPVOID lparam)
{
	GameManager* test = (GameManager*)lparam;
	test->isReading = false;
	test->InitializeManager();

	test->isReading = true;
	return 0;
}

DWORD GameManager::ThreadLodingRend(LPVOID lparam)
{
	GameManager* test = (GameManager*)lparam;
	int koo = 0;
	while (!test->isReading)
	{
		char temp[254];
		printf("%d �ε���...", koo);
		sprintf_s(temp, "%d �ε���...", koo);
		++koo;
		TextOutA(GetDC(test->windowHandle), 0, 0, temp, strlen(temp));
	}
	return 0;
}
