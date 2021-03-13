#pragma once
#include "SceneParent.h"
#include "UI/UI_Button.h"

class OpeningScene : public SceneParent
{
private:
	enum CurrentOpeningMenu { OPENING_MENU_GAME_START, OPENING_MENU_MAP_TOOL, OPENING_MENU_OPTION, OPENING_MENU_HELP, OPENING_MENU_EXIT, OPENING_MENU_COUNT };

private:
	LPDIRECT3DTEXTURE9 Background;
	TextureTriangle BackgroundTriangles[2];
	D3DXMATRIXA16 BackgroundWorld;

	UI_Button* OpeningMenus[OPENING_MENU_COUNT];
	
private:
	LPD3DXEFFECT Effect = nullptr;
	LPDIRECT3DVERTEXDECLARATION9 VertexDeclaration = nullptr;
	LPDIRECT3DVERTEXSHADER9 VertexShader = nullptr;

	LPDIRECT3DBASETEXTURE9 BaseTexture = nullptr;
	LPD3DXBUFFER Error = nullptr;
	D3DXMATRIX World;
	D3DXMATRIX View;
	D3DXMATRIX Projection;

public:
	OpeningScene();
	~OpeningScene();

	virtual void Initialize() override;
	virtual void Destroy() override;
	virtual void Reset() override;
	virtual void Update() override;
	virtual void Render() override;

	virtual void OnEnterState() override;
	virtual void OnLeaveState() override;

	virtual LRESULT InputProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;

public:
	HRESULT Initializefx();
	void Setupfx();
	void Renderfx();


	//스레드 관련
	static DWORD WINAPI ThreadInit(LPVOID lparam);
	static DWORD WINAPI ThreadLodingRend(LPVOID lparam);
	bool isReading = false;
};

