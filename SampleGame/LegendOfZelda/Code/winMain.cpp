
#include "stdafx.h"
#include "playGround.h"

//=========================================================
// - 2015. 10. 16 - ## 전역 변수 ##
//=========================================================
HINSTANCE _hInstance;
HWND _hWnd;
POINT _ptMouse;

LPTSTR _lpszClass = TEXT("Lima API");

playGround _pg;

//=========================================================
// - 2015. 10. 16 - ## 함수의 프로토 타입 선언 ##
//=========================================================
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//윈도우 클라이언트 영역 재조정을 위한 함수
void setWindowsSize(int x, int y, int width, int height);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int cmdShow)
{
	MSG message;		//운영체제에서 발생하는 메시지 정보를 저장하기 위한 구조체
	WNDCLASS wndClass;	//윈도우의 정보를 저장하기 위한 구조체 선언

	_hInstance = hInstance;

	wndClass.cbClsExtra = 0;										//클래스 여분 메모리
	wndClass.cbWndExtra = 0;										//윈도우 여분 메모리
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	//윈도우 백그라운드 색상
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);					//윈도우 커서
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);				//윈도우 아이콘
	wndClass.hInstance = hInstance;									//윈도우 인스턴스
	wndClass.lpfnWndProc = (WNDPROC)WndProc;						//프로시져
	wndClass.lpszClassName = WINNAME;								//클래스 이름
	wndClass.lpszMenuName = NULL;									//메뉴 이름
	wndClass.style = CS_HREDRAW | CS_VREDRAW;						//윈도우 스따일~

	//등록
	RegisterClass(&wndClass);

	//윈도우 생성
	_hWnd = CreateWindow(
		WINNAME,				//윈도우 클래스 이름
		WINNAME,				//윈도우 타이틀바에 나타낼 문자열
		WS_OVERLAPPEDWINDOW,	//윈도우 스타일
		WINSTARTX,				//윈도우 창 시작 X좌표
		WINSTARTY,				//윈도우 창 시작 Y좌표
		WINSIZEX,				//윈도우 창 가로크기
		WINSIZEY,				//윈도우 창 세로크기
		NULL,					//부모 윈도우 유무
		(HMENU)NULL,			//메뉴 핸들
		hInstance,				//인스턴스 윈도우 지정
		NULL);

	setWindowsSize(WINSTARTX, WINSTARTY, WINSIZEX, WINSIZEY);

	//윈도우 창을 눈으로 목격하게끔 띄워줌
	ShowWindow(_hWnd, cmdShow);

	
	if (FAILED(_pg.init()))
	{
		//메인게임 클라스가 초기화 실패하면 걍 꺼

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
	//메시지 루프
	while (GetMessage(&message, 0, 0, 0))
	{
		TranslateMessage(&message);		//키보드 입력 메시지 처리를 담당한다
		DispatchMessage(&message);		//실제로 윈도우로 메시지를 전달해 준다
	}
	*/


	//메시지 다 돌았으면
	_pg.release();

	//등록해준 클라스도 해제

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

	//비작업 영역을 제외한 작업 영역안의 크기만 맞춰줌
	AdjustWindowRect(&winRect, WINSTYLE, false);

	SetWindowPos(_hWnd, NULL, x, y, (winRect.right - winRect.left),
		(winRect.bottom - winRect.top), SWP_NOZORDER | SWP_NOMOVE);
}