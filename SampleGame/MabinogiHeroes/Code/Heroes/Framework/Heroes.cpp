#include "stdafx.h"
#include "Heroes.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//윈도우 창 모양 결정
	WNDCLASSEX wcex[WINDOW_COUNT];

	ZeroMemory(&wcex, sizeof(WNDCLASSEX) * WINDOW_COUNT);

	wcex[WINDOW_MAIN].cbSize = sizeof(WNDCLASSEX);
	wcex[WINDOW_MAIN].style = CS_HREDRAW | CS_VREDRAW;	//Window Class Styles
	wcex[WINDOW_MAIN].lpfnWndProc = WndProc;
	wcex[WINDOW_MAIN].cbClsExtra = 0;
	wcex[WINDOW_MAIN].cbWndExtra = 0;
	wcex[WINDOW_MAIN].hInstance = hInstance;
	wcex[WINDOW_MAIN].hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex[WINDOW_MAIN].hCursor = LoadCursor(NULL, IDC_ARROW);
	//wcex.hbrBackground = (HBRUSH)COLOR_WINDOWFRAME;
	wcex[WINDOW_MAIN].hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
	wcex[WINDOW_MAIN].lpszMenuName = NULL;
	wcex[WINDOW_MAIN].lpszClassName = WINDOW_CLASS_NAME;
	wcex[WINDOW_MAIN].hIconSm = LoadIcon(wcex[WINDOW_MAIN].hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	//결정된 창모양으로 등록
	if (!RegisterClassEx(&wcex[WINDOW_MAIN]))
	{
		MessageBox(NULL, L"Call to RegisterClassEx failed!", WINDOW_TITLE_CAPTION, MB_OK);

		return 1;
	}

	//실제로 윈도우 생성
	HWnd[WINDOW_MAIN] = CreateWindowEx(WS_EX_APPWINDOW, WINDOW_CLASS_NAME, WINDOW_TITLE_CAPTION,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_CAPTION | WS_SYSMENU, //WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		0, 0, RESOLUTION_X, RESOLUTION_Y, NULL, NULL, hInstance, NULL);

	if (!HWnd[WINDOW_MAIN])
	{
		MessageBox(NULL, L"Call to CreateWindow failed!", WINDOW_TITLE_CAPTION, MB_OK);
		return 1;
	}

	//UnregisterClass(WINDOW_CLASS_NAME, wcex[WINDOW_MAIN].hInstance);



	//WINDOW_MAP_TOOL
	wcex[WINDOW_MAP_TOOL].cbSize = sizeof(WNDCLASSEX);
	wcex[WINDOW_MAP_TOOL].style = CS_HREDRAW | CS_VREDRAW;
	wcex[WINDOW_MAP_TOOL].lpfnWndProc = MapToolProcedure;
	wcex[WINDOW_MAP_TOOL].cbClsExtra = 0;
	wcex[WINDOW_MAP_TOOL].cbWndExtra = 0;
	wcex[WINDOW_MAP_TOOL].hInstance = hInstance;
	wcex[WINDOW_MAP_TOOL].hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex[WINDOW_MAP_TOOL].hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex[WINDOW_MAP_TOOL].hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
	wcex[WINDOW_MAP_TOOL].lpszMenuName = NULL;
	wcex[WINDOW_MAP_TOOL].lpszClassName = MAP_TOOL_WINDOW_CLASS_NAME;
	wcex[WINDOW_MAP_TOOL].hIconSm = LoadIcon(wcex[WINDOW_MAP_TOOL].hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	if (!RegisterClassEx(&wcex[WINDOW_MAP_TOOL]))
	{
		MessageBox(NULL, L"Call to RegisterClassEx failed!", MAP_TOOL_WINDOW_CLASS_NAME, MB_OK);

		return 1;
	}

	HWnd[WINDOW_MAP_TOOL] = nullptr;

	//UnregisterClass(MAP_TOOL_WINDOW_CLASS_NAME, wcex[WINDOW_MAP_TOOL].hInstance);
	
	//WINDOW_OPTION
	wcex[WINDOW_OPTION].cbSize = sizeof(WNDCLASSEX);
	wcex[WINDOW_OPTION].style = CS_HREDRAW | CS_VREDRAW;
	wcex[WINDOW_OPTION].lpfnWndProc = OptionProcedure;
	wcex[WINDOW_OPTION].cbClsExtra = 0;
	wcex[WINDOW_OPTION].cbWndExtra = 0;
	wcex[WINDOW_OPTION].hInstance = hInstance;
	wcex[WINDOW_OPTION].hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex[WINDOW_OPTION].hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex[WINDOW_OPTION].hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
	wcex[WINDOW_OPTION].lpszMenuName = NULL;
	wcex[WINDOW_OPTION].lpszClassName = OPTION_WINDOW_CLASS_NAME;
	wcex[WINDOW_OPTION].hIconSm = LoadIcon(wcex[WINDOW_OPTION].hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	if (!RegisterClassEx(&wcex[WINDOW_OPTION]))
	{
		MessageBox(NULL, L"Call to RegisterClassEx failed!", OPTION_WINDOW_CLASS_NAME, MB_OK);

		return 1;
	}

	HWnd[WINDOW_OPTION] = nullptr;

	//UnregisterClass(OPTION_WINDOW_CLASS_NAME, wcex[WINDOW_OPTION].hInstance);




	HInstance = hInstance;

	ShowWindow(HWnd[WINDOW_MAIN], nCmdShow);
	SetForegroundWindow(HWnd[WINDOW_MAIN]);
	SetFocus(HWnd[WINDOW_MAIN]);
	UpdateWindow(HWnd[WINDOW_MAIN]);


	//g_Gamanager.Initialize(hWnd);
	GameManager::Get().Initialize(HWnd[WINDOW_MAIN]);

	MSG message = MSG();
	while (message.message != WM_QUIT)
	{
		if (PeekMessageW(&message, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		else
		{
			//타임겟타임 사용
			static DWORD prevTime = timeGetTime();
			DWORD nowTime = timeGetTime();

			double tickSecond = (nowTime - prevTime) * 0.001f;

			GameManager::Get().Loop(tickSecond);

			prevTime = nowTime;
		}
	}

	GameManager::Get().Destroy();

	return (int)message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		RECT wantRect = { 0, 0, RESOLUTION_X, RESOLUTION_Y };
		AdjustWindowRect(&wantRect, WS_CAPTION | WS_SYSMENU, false); //윈도우 크기를 계산( 원하는 클라이언트 영역, 윈도우 스타일, 메뉴 포함되는지)
		MoveWindow(hWnd, 0, 0, wantRect.right - wantRect.left, wantRect.bottom - wantRect.top, true);	//윈도우 크기 변경(마지막은 다시 그릴껀지)
	}
	break;
	case WM_DESTROY:
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MOUSEMOVE:
	case WM_MOUSEWHEEL:
		GameManager::Get().GameInputProc(hWnd, message, wParam, lParam);
		break;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			//::DestroyWindow(hWnd);
		}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

LRESULT CALLBACK MapToolProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		RECT wantRect = { 0, 0, RESOLUTION_X, RESOLUTION_Y };
		AdjustWindowRect(&wantRect, WS_CAPTION | WS_SYSMENU, false);
		MoveWindow(hWnd, 0, 0, wantRect.right - wantRect.left, wantRect.bottom - wantRect.top, true);
	}
		break;
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MOUSEMOVE:
	case WM_MOUSEWHEEL:
		GameManager::Get().GameInputProc(hWnd, message, wParam, lParam);
		break;
	case WM_KEYDOWN:
		if (wParam != VK_ESCAPE) break;
	case WM_CLOSE:
		//::CloseWindow(hWnd);
	case WM_DESTROY:
		::DestroyWindow(hWnd);
		HWnd[WINDOW_MAP_TOOL] = nullptr;
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

LRESULT CALLBACK OptionProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		RECT wantRect = { 0, 0, RESOLUTION_X / 2, RESOLUTION_Y / 2 };
		AdjustWindowRect(&wantRect, WS_CAPTION | WS_SYSMENU, false);
		MoveWindow(hWnd, RESOLUTION_X / 4, RESOLUTION_Y / 4, wantRect.right - wantRect.left, wantRect.bottom - wantRect.top, true);
	}
	break;

	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MOUSEMOVE:
	case WM_MOUSEWHEEL:
		GameManager::Get().GameInputProc(hWnd, message, wParam, lParam);
		break;
	case WM_KEYDOWN:
		if (wParam != VK_ESCAPE) break;
	case WM_CLOSE:
		//::CloseWindow(hWnd);
	case WM_DESTROY:
		::DestroyWindow(hWnd);
		HWnd[WINDOW_OPTION] = nullptr;
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}