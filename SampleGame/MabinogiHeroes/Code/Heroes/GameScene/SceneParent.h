#pragma once
#include <Windows.h>
#include <d3dx9.h>

class SceneParent
{
public:
	virtual void Initialize() = 0;		//필요 데이터 생성
	virtual void Destroy() = 0;			//생성한 데이터 삭제

	virtual void Update() = 0;			//데이터 갱신
	virtual void Render() = 0;			//데이터를 기반으로 그리기

	virtual void OnEnterState() = 0;	//이 state가 시작될때 실행될 함수
	virtual void OnLeaveState() = 0;	//이 state에서 나갈때 실행될 함수

	virtual LRESULT InputProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) = 0;	//이 state용 입력 메세지 처리용 프로시저

protected:
	virtual void Reset() = 0;			//초기 상태로 데이터 되돌리기
};

