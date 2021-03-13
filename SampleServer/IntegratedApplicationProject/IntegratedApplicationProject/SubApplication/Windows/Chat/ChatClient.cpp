#include "stdafx.h"
#include "Protocol.h"
#include "ChatClient.h"

HINSTANCE CHAT_CLIENT_INSTANCE_HANDLE;
HWND CHAT_CLIENT_WINDOW_HANDLE;
HDC CHAT_CLIENT_WINDOW_DC_HANDLE;

WCHAR CHAT_CLIENT_WINDOW_TITLE[MAX_LOADSTRING];
WCHAR CHAT_CLIENT_WINDOW_CLASS_NAME[MAX_LOADSTRING];

int APIENTRY ChatClient(_In_ HINSTANCE instanceHandle, _In_opt_ HINSTANCE previousInstanceHandle, _In_ LPSTR lpCommandLine, _In_ int nCommandShow)
{
	UNREFERENCED_PARAMETER(previousInstanceHandle);
	UNREFERENCED_PARAMETER(lpCommandLine);

	LoadString(instanceHandle, IDS_APP_TITLE, CHAT_CLIENT_WINDOW_TITLE, MAX_LOADSTRING);
	LoadString(instanceHandle, IDC_INTEGRATEDAPPLICATIONPROJECT, CHAT_CLIENT_WINDOW_CLASS_NAME, MAX_LOADSTRING);

	WNDCLASSEXW windowClassEx;

	windowClassEx.cbSize = sizeof(WNDCLASSEX);

	windowClassEx.style = CS_HREDRAW | CS_VREDRAW;
	windowClassEx.lpfnWndProc = ChatClientWindowProcedure;
	windowClassEx.cbClsExtra = 0;
	windowClassEx.cbWndExtra = 0;
	windowClassEx.hInstance = instanceHandle;
	windowClassEx.hIcon = LoadIcon(instanceHandle, MAKEINTRESOURCE(IDI_INTEGRATEDAPPLICATIONPROJECT));
	windowClassEx.hCursor = LoadCursor(nullptr, IDC_ARROW);
	windowClassEx.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	windowClassEx.lpszMenuName = MAKEINTRESOURCEW(IDC_INTEGRATEDAPPLICATIONPROJECT);
	windowClassEx.lpszClassName = CHAT_CLIENT_WINDOW_CLASS_NAME;
	windowClassEx.hIconSm = LoadIcon(windowClassEx.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	RegisterClassEx(&windowClassEx);



	CHAT_CLIENT_INSTANCE_HANDLE = instanceHandle;

	CHAT_CLIENT_WINDOW_HANDLE = CreateWindow(CHAT_CLIENT_WINDOW_CLASS_NAME, CHAT_CLIENT_WINDOW_TITLE, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, instanceHandle, nullptr);

	if (!CHAT_CLIENT_WINDOW_HANDLE) return 0;

	CHAT_CLIENT_WINDOW_DC_HANDLE = GetDC(CHAT_CLIENT_WINDOW_HANDLE);

	ShowWindow(CHAT_CLIENT_WINDOW_HANDLE, nCommandShow);
	UpdateWindow(CHAT_CLIENT_WINDOW_HANDLE);



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

	InitializeChatClient();

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
			UpdateChatClient();

			//if (TIMER.CheckElapsedTime()) RenderChatClient();
			RenderChatClient();
		}
	}

	TerminalizeChatClient();

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
LRESULT CALLBACK ChatClientWindowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
				DialogBox(CHAT_CLIENT_INSTANCE_HANDLE, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
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
			ChatClientNetworkProcedure(hWnd, message, wParam, lParam);
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

LRESULT CALLBACK ChatClientNetworkProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
			ReceiveChatServerPacket();
		}
		break;
	case FD_WRITE:
		{
			//CLIENT.Sendable = 1;

			SendChatClientPacket();
		}
		break;
	case FD_CLOSE:
		closesocket(wParam);
		break;
	}

	return 0;
}