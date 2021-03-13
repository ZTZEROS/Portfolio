#include "stdafx.h"
#include "TestScene.h"

TestScene::TestScene()
{
}


TestScene::~TestScene()
{
}

void TestScene::Initialize()
{
	if (grid == nullptr)
	{
		grid = new Grid;
		grid->Initialize();
		grid->SetShowArrow(true);
		grid->SetShowGround(true);
	}

	CharacData::GetLethita()->SetPosition(D3DXVECTOR3(0, 0, 0));
	CharacData::GetLethita()->SetAnimationIndex(0);

	GameManager::GetCamera()->SetLookTarget(CharacData::GetLethita()->GetPositionAddress());


	if (succubus == nullptr)
	{
		succubus = new Succubus();
		succubus->InitXFile();
		succubus->SetPosition(D3DXVECTOR3(-10, 0, 0));
		succubus->SetAnimationIndex(0);
	}


}

void TestScene::Destroy()
{
	SAFE_DELETE(hellLady3);
	SAFE_DELETE(hellLady2);
	SAFE_DELETE(succubus);
	SAFE_DELETE(hellLady);
	SAFE_DELETE(grid);
}

void TestScene::Reset()
{
}

void TestScene::Update()
{
	if (GameManager::GetKey()->isOnceKeyDown('M'))
	{
		SoundManager::Get().stop("아이디-01-Sign (Feat. Loopy)");
	}

	if (GameManager::GetKey()->isOnceKeyDown('N'))
	{
		SoundManager::Get().play("아이디-01-Sign (Feat. Loopy)", 0.4f);
	}
	if (GameManager::GetKey()->isOnceKeyDown('B'))
	{
		SoundManager::Get().SetSoundTime("아이디-01-Sign (Feat. Loopy)", 30);

	}

	if (GameManager::GetKey()->isOnceKeyDown('Z'))
	{
		SAFE_DELETE(hellLady);
		hellLady = nullptr;
	}



	DieCheck();
	CollisionCheck();
}

void TestScene::Render()
{
	if (grid)
	{
		grid->Render();
	}


	CharacData::GetLethita()->UpdateAndRender();

	if (succubus)
	{
		succubus->UpdateAndRender();
	}

	if (hellLady)
	{
		hellLady->UpdateAndRender();
		hellLady2->UpdateAndRender();
		hellLady3->UpdateAndRender();
	}

	RECT rc = { 10, RESOLUTION_Y - 55, 11, 11 };
//	UI_Manager::GetFont()->DrawTextA(NULL, "GameState_SkinnedMesh", strlen("GameState_SkinnedMesh"), &rc, DT_TOP | DT_LEFT | DT_NOCLIP, D3DCOLOR_XRGB(255, 255, 255));
	if (text1 != NULL)
	{
		UI_Manager::GetItemFont()->DrawTextA(NULL, text1, strlen(text1), &rc, DT_TOP | DT_LEFT | DT_NOCLIP, D3DCOLOR_XRGB(255, 255, 255));
		rc = { 60, RESOLUTION_Y - 55, 11, 11 };
		UI_Manager::GetItemFont()->DrawTextA(NULL, text2, strlen(text2), &rc, DT_TOP | DT_LEFT | DT_NOCLIP, D3DCOLOR_XRGB(255, 255, 255));
	}

}

void TestScene::OnEnterState()
{
	GameManager::GetCamera()->SetDistance(20.0f);
	Initialize();
	//succubusTemp = new Succubus;
	//succubusTemp->Initialize("./Lethita/", "LethitaAttack4.X");
	//
	//DWORD threadID1 = 0, threadID2 = 0;
	//HANDLE threadHandles[2];

	//threadHandles[0] = nullptr;
	//threadHandles[1] = nullptr;

	//threadHandles[0] = CreateThread(
	//	nullptr, 0, ThreadTest, this,
	//	CREATE_SUSPENDED, &threadID1);	//CREATE_SUSPENDED자리가 null이면 바로 시작
	//threadHandles[1] = CreateThread(
	//	nullptr, 0, ThreadTest2, this,
	//	CREATE_SUSPENDED, &threadID2);

	//ResumeThread(threadHandles[0]);
	//ResumeThread(threadHandles[1]);

	//WaitForMultipleObjects(2, threadHandles, TRUE, INFINITE);

	//CloseHandle(threadHandles[0]);
	//CloseHandle(threadHandles[1]);

	//succubusTemp->Destroy();

}

void TestScene::OnLeaveState()
{
	SkinnedMeshManager::Get().Destroy();
	Destroy();
}

LRESULT TestScene::InputProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;
	return result;
}

void TestScene::CollisionCheck()
{
	
}

void TestScene::DieCheck()
{
	
	
}

DWORD TestScene::ThreadTest(LPVOID lparam)
{
	TestScene* test = (TestScene*)lparam;
	
	test->isReading = false;
	test->Initialize();
//	EnterCriticalSection(&test->cs);
	test->isReading = true;
	//LeaveCriticalSection(&test->cs);

	return 0;
}

DWORD TestScene::ThreadTest2(LPVOID lparam)
{
	
		TestScene* test = (TestScene*)lparam;
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
		char fpsText[256] ;
		sprintf_s(fpsText, "%d", test->iee);
		UI_Manager::GetFont()->DrawTextA(NULL, fpsText, strlen(fpsText), &fpsRectangle, DT_TOP | DT_RIGHT | DT_NOCLIP, D3DCOLOR_XRGB(255, 255, 255));
		test->iee += 1;


		//--------------------------------------------------------------

		GameManager::GetDevice()->EndScene();

		GameManager::GetDevice()->Present(NULL, NULL, NULL, NULL);

	}
		return 0;

}
