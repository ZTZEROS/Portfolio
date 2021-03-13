#pragma once
#include "../FrustumCulling.h"

class CameraManager
{
public:
	CameraManager();
	virtual ~CameraManager();

	void Initialize(D3DXVECTOR3* target = nullptr);
	void Destroy();
	void Update();
	
	void Reset();

	LRESULT CameraInputProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	inline void SetLookTarget(D3DXVECTOR3* target) { lookTarget = target; }
	inline void SetEyeHeight(float height) { EyeHeight = height; }
	inline void SetDistance(float distance) { camDistance = distance; }
	inline void SetRotateX(float rotation) { camRotX = rotation; }
	inline void SetRotateY(float rotation) { camRotY = rotation; }
	inline bool IsInCameraFrustum(BoundingSphere* target) { return frustum.IsInFrustum(target); }

	inline D3DXVECTOR3 GetDirect() { return baseDirection; }
	inline D3DXVECTOR3 GetEyePosition() { return eyePosition; }
	inline float GetCamRotY() { return camRotY; }

private:
	D3DXVECTOR3 eyePosition = D3DXVECTOR3(0, 0, 0);
	D3DXVECTOR3 lookAt = D3DXVECTOR3(0, 0, 0);
	D3DXVECTOR3 upVector = D3DXVECTOR3(0, 1, 0);

	//화면 이동용
	D3DXVECTOR3 baseDirection = D3DXVECTOR3(0, 0, 1);
	
	float camRotX = 0.0f;		//radian
	float camRotY = 0.0f;
	float camDistance = 10.0f;

	float EyeHeight = 0.0f;

	float AngleSpeed = 3.0f;

	bool isDrag = false;
	POINT prevMousePos;

	D3DXVECTOR3* lookTarget = nullptr;

	FrustumCulling frustum;

};

