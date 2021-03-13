#pragma once
#include <Windows.h>
#include <d3dx9.h>

class SceneParent
{
public:
	virtual void Initialize() = 0;		//�ʿ� ������ ����
	virtual void Destroy() = 0;			//������ ������ ����

	virtual void Update() = 0;			//������ ����
	virtual void Render() = 0;			//�����͸� ������� �׸���

	virtual void OnEnterState() = 0;	//�� state�� ���۵ɶ� ����� �Լ�
	virtual void OnLeaveState() = 0;	//�� state���� ������ ����� �Լ�

	virtual LRESULT InputProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) = 0;	//�� state�� �Է� �޼��� ó���� ���ν���

protected:
	virtual void Reset() = 0;			//�ʱ� ���·� ������ �ǵ�����
};

