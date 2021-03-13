#pragma once
#include "SceneParent.h"
#include "../Lethita.h"
#include "../Character/Monster/HellLady/HellLady.h"
#include "../Character/Monster/Succubus/Succubus.h"
#include "../../Framework/Object/Grid.h"

class TestScene :
	public SceneParent
{
public:
	TestScene();
	virtual ~TestScene();

	virtual void Initialize() override;
	virtual void Destroy() override;
	virtual void Reset() override;
	virtual void Update() override;
	virtual void Render() override;

	virtual void OnEnterState() override;
	virtual void OnLeaveState() override;

	virtual LRESULT InputProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
	void CollisionCheck();
	void DieCheck();
protected:
	Grid* grid = nullptr;
	HellLady* hellLady = nullptr;
	HellLady* hellLady2 = nullptr;
	HellLady* hellLady3 = nullptr;
	Succubus* succubus = nullptr;

	Succubus* succubusTemp = nullptr;


	char* text2 = nullptr;
	char* text1 = nullptr;

	CRITICAL_SECTION cs;
	static DWORD WINAPI ThreadTest(LPVOID lparam);
	static DWORD WINAPI ThreadTest2(LPVOID lparam);
	bool isReading = false;
	int iee = 0; //로딩 퍼센트

};

