#include "stdafx.h"
#include "Windows.h"

int APIENTRY WinMain(_In_ HINSTANCE instanceHandle, _In_opt_ HINSTANCE previousInstanceHandle, _In_ LPSTR lpCommandLine, _In_ int nCommandShow)
{
	//ChatClient(instanceHandle, previousInstanceHandle, lpCommandLine, nCommandShow);
	//ExecuteODBC_Client(instanceHandle, previousInstanceHandle, lpCommandLine, nCommandShow);

	return 0;
}

/*
HINSTANCE MAIN_INSTANCE_HANDLE;
HWND MAIN_WINDOW_HANDLE;
HDC MAIN_WINDOW_DC_HANDLE;

WCHAR MAIN_WINDOW_TITLE[MAX_LOADSTRING];
WCHAR MAIN_WINDOW_CLASS_NAME[MAX_LOADSTRING];

int APIENTRY WinMain(_In_ HINSTANCE instanceHandle, _In_opt_ HINSTANCE previousInstanceHandle, _In_ LPSTR lpCommandLine, _In_ int nCommandShow)
{
	UNREFERENCED_PARAMETER(previousInstanceHandle);
	UNREFERENCED_PARAMETER(lpCommandLine);

	LoadString(instanceHandle, IDS_APP_TITLE, MAIN_WINDOW_TITLE, MAX_LOADSTRING);
	LoadString(instanceHandle, IDC_INTEGRATEDAPPLICATIONPROJECT, MAIN_WINDOW_CLASS_NAME, MAX_LOADSTRING);
	
	WNDCLASSEXW windowClassEx;

	windowClassEx.cbSize = sizeof(WNDCLASSEX);

	windowClassEx.style = CS_HREDRAW | CS_VREDRAW;
	windowClassEx.lpfnWndProc = MainWindowProcedure;
	windowClassEx.cbClsExtra = 0;
	windowClassEx.cbWndExtra = 0;
	windowClassEx.hInstance = instanceHandle;
	windowClassEx.hIcon = LoadIcon(instanceHandle, MAKEINTRESOURCE(IDI_INTEGRATEDAPPLICATIONPROJECT));
	windowClassEx.hCursor = LoadCursor(nullptr, IDC_ARROW);
	windowClassEx.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	windowClassEx.lpszMenuName = MAKEINTRESOURCEW(IDC_INTEGRATEDAPPLICATIONPROJECT);
	windowClassEx.lpszClassName = MAIN_WINDOW_CLASS_NAME;
	windowClassEx.hIconSm = LoadIcon(windowClassEx.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	RegisterClassEx(&windowClassEx);



	MAIN_INSTANCE_HANDLE = instanceHandle;

	MAIN_WINDOW_HANDLE = CreateWindow(MAIN_WINDOW_CLASS_NAME, MAIN_WINDOW_TITLE, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, instanceHandle, nullptr);

	if (!MAIN_WINDOW_HANDLE) return 0;

	MAIN_WINDOW_DC_HANDLE = GetDC(MAIN_WINDOW_HANDLE);

	ShowWindow(MAIN_WINDOW_HANDLE, nCommandShow);
	UpdateWindow(MAIN_WINDOW_HANDLE);



	HACCEL hAccelTable = LoadAccelerators(instanceHandle, MAKEINTRESOURCE(IDC_INTEGRATEDAPPLICATIONPROJECT));

	MSG windowMessage;

	//while (GetMessage(&windowMessage, nullptr, 0, 0))
	//{
	//	if (!TranslateAccelerator(windowMessage.hwnd, hAccelTable, &windowMessage))
	//	{
	//		TranslateMessage(&windowMessage);
	//		DispatchMessage(&windowMessage);
	//	}
	//}

	InitializeMain();

	while (1)
	{
		if (PeekMessage(&windowMessage, nullptr, 0, 0, PM_REMOVE)) //����ÿ� �޼��� ó���� �켱���� ���ڴ� �ǹ�
		{
			if (windowMessage.message == WM_QUIT) break;

			TranslateMessage(&windowMessage);
			DispatchMessage(&windowMessage); //���ο��� ������ ���ν��� ȣ��
		}

		if (1)
		{
			UpdateMain();

			//if (TIMER.CheckElapsedTime()) RenderMain();
			RenderMain();
		}
	}

	TerminalizeMain();

	return (int)windowMessage.wParam;
}



//
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ����:  �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND  - ���� ���α׷� �޴��� ó���մϴ�.
//  WM_PAINT    - �� â�� �׸��ϴ�.
//  WM_DESTROY  - ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
//
//
LRESULT CALLBACK MainWindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		{
			//SetWindowPos(hWnd, nullptr, 100, 100, 640, 520, SWP_NOZORDER | SWP_NOREPOSITION);
			//GetClientRect(MAIN_WINDOW_HANDLE, &CLIENT_RECTANGLE);
			//SetBkMode(MAIN_WINDOW_DC_HANDLE, TRANSPARENT);
			//SetTimer(hWnd, 0, 100, nullptr);
		}
		break;
	case WM_COMMAND:
		{
			int wmId = LOWORD(wParam);
			// �޴� ������ ���� �м��մϴ�.
			switch (wmId)
			{
			case IDM_ABOUT:
				DialogBox(MAIN_INSTANCE_HANDLE, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
				break;
			case IDM_EXIT:
				DestroyWindow(hWnd);
				break;
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
		}
		break;
	case WM_NETWORK:
		{
			NetworkProcedure(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			// TODO: ���⿡ hdc�� ����ϴ� �׸��� �ڵ带 �߰��մϴ�.
			EndPaint(hWnd, &ps);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// ���� ��ȭ ������ �޽��� ó�����Դϴ�.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

LRESULT CALLBACK MainNetworkProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int returnValue;

	if (WSAGETSELECTERROR(lParam))
	{
		closesocket(wParam);

		return 0;
	}

	switch (WSAGETSELECTEVENT(lParam))
	{
	case FD_CONNECT:
		{
			if (wParam == INVALID_SOCKET) return 0;
			else
			{
				//returnValue = WSAAsyncSelect(wParam, hWnd, WM_NETWORK, FD_READ | FD_WRITE | FD_CLOSE);
				if (returnValue == SOCKET_ERROR)
				{
					//CLIENT.Connected = 0;
					//closesocket(CLIENT.Socket);
				}
				//else CLIENT.Connected = 1;
			}
		}
		break;
	case FD_READ:
		{
			ReceiveMainPacket();
		}
		break;
	case FD_WRITE:
		{
			//CLIENT.Sendable = 1;

			SendMainPacket();
		}
		break;
	case FD_CLOSE:
		closesocket(wParam);
		break;
	}

	return 0;
}
*/