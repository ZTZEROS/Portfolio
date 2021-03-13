
#include "stdafx.h"
#include "playGround.h"

//=========================================================
// - 2015. 10. 16 - ## ���� ���� ##
//=========================================================
HINSTANCE _hInstance;
HWND _hWnd;
POINT _ptMouse;

LPTSTR _lpszClass = TEXT("Lima API");

playGround _pg;

//=========================================================
// - 2015. 10. 16 - ## �Լ��� ������ Ÿ�� ���� ##
//=========================================================
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//������ Ŭ���̾�Ʈ ���� �������� ���� �Լ�
void setWindowsSize(int x, int y, int width, int height);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int cmdShow)
{
	MSG message;		//�ü������ �߻��ϴ� �޽��� ������ �����ϱ� ���� ����ü
	WNDCLASS wndClass;	//�������� ������ �����ϱ� ���� ����ü ����

	_hInstance = hInstance;

	wndClass.cbClsExtra = 0;										//Ŭ���� ���� �޸�
	wndClass.cbWndExtra = 0;										//������ ���� �޸�
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	//������ ��׶��� ����
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);					//������ Ŀ��
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);				//������ ������
	wndClass.hInstance = hInstance;									//������ �ν��Ͻ�
	wndClass.lpfnWndProc = (WNDPROC)WndProc;						//���ν���
	wndClass.lpszClassName = WINNAME;								//Ŭ���� �̸�
	wndClass.lpszMenuName = NULL;									//�޴� �̸�
	wndClass.style = CS_HREDRAW | CS_VREDRAW;						//������ ������~

	//���
	RegisterClass(&wndClass);

	//������ ����
	_hWnd = CreateWindow(
		WINNAME,				//������ Ŭ���� �̸�
		WINNAME,				//������ Ÿ��Ʋ�ٿ� ��Ÿ�� ���ڿ�
		WS_OVERLAPPEDWINDOW,	//������ ��Ÿ��
		WINSTARTX,				//������ â ���� X��ǥ
		WINSTARTY,				//������ â ���� Y��ǥ
		WINSIZEX,				//������ â ����ũ��
		WINSIZEY,				//������ â ����ũ��
		NULL,					//�θ� ������ ����
		(HMENU)NULL,			//�޴� �ڵ�
		hInstance,				//�ν��Ͻ� ������ ����
		NULL);

	setWindowsSize(WINSTARTX, WINSTARTY, WINSIZEX, WINSIZEY);

	//������ â�� ������ ����ϰԲ� �����
	ShowWindow(_hWnd, cmdShow);

	
	if (FAILED(_pg.init()))
	{
		//���ΰ��� Ŭ�󽺰� �ʱ�ȭ �����ϸ� �� ��

		return 0;
	}


	
	while (true)
	{
		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			if (message.message == WM_QUIT) break;
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		else
		{
			TIMEMANAGER->update(60.0f);
			_pg.update();
			_pg.render();
	
		}
	}
	

	/*
	//�޽��� ����
	while (GetMessage(&message, 0, 0, 0))
	{
		TranslateMessage(&message);		//Ű���� �Է� �޽��� ó���� ����Ѵ�
		DispatchMessage(&message);		//������ ������� �޽����� ������ �ش�
	}
	*/


	//�޽��� �� ��������
	_pg.release();

	//������� Ŭ�󽺵� ����

	UnregisterClass(WINNAME, hInstance);

	return message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	return _pg.MainProc(hWnd, iMessage, wParam, lParam);
}

void setWindowsSize(int x, int y, int width, int height)
{
	RECT winRect;

	winRect.left = 0;
	winRect.top = 0;
	winRect.right = width;
	winRect.bottom = height;

	//���۾� ������ ������ �۾� �������� ũ�⸸ ������
	AdjustWindowRect(&winRect, WINSTYLE, false);

	SetWindowPos(_hWnd, NULL, x, y, (winRect.right - winRect.left),
		(winRect.bottom - winRect.top), SWP_NOZORDER | SWP_NOMOVE);
}