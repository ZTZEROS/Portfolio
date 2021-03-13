#pragma once
#include "SceneParent.h"
class LoadingScene : public SceneParent
{
public:
	LoadingScene();
	~LoadingScene();


	virtual void Initialize() override;
	virtual void Destroy() override;
	virtual void Reset() override;
	virtual void Update() override;
	virtual void Render() override;

	virtual void OnEnterState() override;
	virtual void OnLeaveState() override;

	virtual LRESULT InputProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
};

