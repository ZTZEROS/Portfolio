#pragma once

#include <time.h>
#include "CameraManager.h"
#include "KeyManager.h"

class GameManager
{
private:
	GameManager();
	GameManager(const GameManager& other);
	~GameManager();

public:
	static GameManager& Get()
	{
		static GameManager instance;
		return instance;
	}
	static inline LPDIRECT3DDEVICE9 GetDevice() { return GameManager::Get().GetInstanceDevice(); }
	static inline double GetTick() { return GameManager::Get().GetCurrentTick(); }
	static inline CameraManager* GetCamera() { return GameManager::Get().GetInstanceCamera(); }
	static inline KeyManager* GetKey() { return GameManager::Get().GetInstanceKey(); }

	void Initialize(HWND handle);
	void InitializeManager();
	void Destroy();
	void Loop(double tick);
	LRESULT GameInputProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	inline HWND GetWindowHandle() { return windowHandle; }
	inline POINT getMousePoint() { return mousePoint; }

	//스레드 관련
	static DWORD WINAPI ThreadInit(LPVOID lparam);
	static DWORD WINAPI ThreadLodingRend(LPVOID lparam);
	bool isReading = false;

protected:
	double currentTickTime = 0.0f;

	HWND windowHandle = nullptr;
	LPDIRECT3D9 direct3d = nullptr;
	LPDIRECT3DDEVICE9 direct3dDevice = nullptr;

	CameraManager* camera = nullptr;
	KeyManager* key = nullptr;
	POINT mousePoint;

	D3DLIGHT9 mainLight;

	int fpsCount = 0;
	float fpsTimeCheck = 0.0f;
	char fpsText[32];
	char mouseText[128]; 

	void Update();
	void Render();

	inline LPDIRECT3DDEVICE9 GetInstanceDevice() { return direct3dDevice; }
	inline double GetCurrentTick() { return currentTickTime; }
	inline CameraManager* GetInstanceCamera() { return camera; }
	inline KeyManager* GetInstanceKey() { return key; }
};

