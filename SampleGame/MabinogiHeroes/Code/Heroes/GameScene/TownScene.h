#pragma once
#include "SceneParent.h"
#include "Object/Grid.h"
#include "../Place/Town/Town.h"
#include "../Character/NPC/Ti/Ti.h"
#include "../Character/NPC/Eire/Eire.h"
#include "../Character/TheSun.h"
#include "../Place/TheSky.h"

class TownScene : public SceneParent
{
private:
	Grid* Grid0;

	TheSky* TheSky0;
	TheSun* TheSun0;
	Town* Town0;

	Ti* Ti0;
	Eire* Eire0;

private:
	LPD3DXEFFECT ApplyShadowShader = nullptr;
	LPD3DXEFFECT CreateShadowShader = nullptr;
	LPD3DXEFFECT uvAnimationShader = nullptr;

	LPDIRECT3DTEXTURE9 ShadowRenderTarget = nullptr;
	LPDIRECT3DSURFACE9 ShadowDepthStencil = nullptr;

public:
	TownScene();
	~TownScene();

	virtual void Initialize() override;
	virtual void Destroy() override;
	virtual void Reset() override;
	virtual void Update() override;
	virtual void Render() override;
	void RenderShadow();
	void RenderWater();

	virtual void OnEnterState() override;
	virtual void OnLeaveState() override;

	virtual LRESULT InputProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;

	//스레드 관련
	static DWORD WINAPI ThreadInit(LPVOID lparam);
	static DWORD WINAPI ThreadLodingRend(LPVOID lparam);
	bool isReading = false;
	HANDLE threadHandle = nullptr;
	CRITICAL_SECTION cs;
};

