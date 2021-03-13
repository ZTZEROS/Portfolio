#include "stdafx.h"
#include "CameraManager.h"


CameraManager::CameraManager()
{
	prevMousePos.x = 0;
	prevMousePos.y = 0;
}

CameraManager::~CameraManager()
{
}

void CameraManager::Initialize(D3DXVECTOR3* target /*= nullptr*/)
{
	lookTarget = target;

	D3DXMATRIXA16 matView;
	D3DXMATRIXA16 matProjection;
	D3DXMatrixLookAtLH(&matView, &eyePosition, &lookAt, &upVector);
	GameManager::GetDevice()->SetTransform(D3DTS_VIEW, &matView);

	D3DXMatrixPerspectiveFovLH(&matProjection, D3DX_PI * 0.25f, (float)RESOLUTION_X / (float)RESOLUTION_Y, 1.0f, 10000.0f);
	GameManager::GetDevice()->SetTransform(D3DTS_PROJECTION, &matProjection);

	frustum.Initialize();

}

void CameraManager::Destroy()
{
}

void CameraManager::Update()
{
	D3DXMATRIXA16 matRotationX, matRotationY, world;
	D3DXMATRIXA16 matRotation;


	D3DXMatrixRotationX(&matRotationX, camRotX);
	D3DXMatrixRotationY(&matRotationY, camRotY);
	matRotation = matRotationX * matRotationY;

	baseDirection = D3DXVECTOR3(0, 0, 1);
	eyePosition = D3DXVECTOR3(0, 0, -camDistance);
	D3DXVec3TransformCoord(&eyePosition, &eyePosition, &matRotation);

	//포지션용
	D3DXMatrixIdentity(&world);
	world *= matRotationY;
	D3DXVec3TransformCoord(&baseDirection, &D3DXVECTOR3(0,0,1), &world);

	if (lookTarget)
	{
		lookAt = (*lookTarget);
		lookAt.y = (*lookTarget).y + EyeHeight;
		eyePosition = (*lookTarget) + eyePosition;
		eyePosition.y = eyePosition.y + EyeHeight;
	}

	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH(&matView, &eyePosition, &lookAt, &upVector);
	GameManager::GetDevice()->SetTransform(D3DTS_VIEW, &matView);

	if (GameManager::GetKey()->isStayKeyDown('E'))
	{
		camRotY += AngleSpeed * (float)GameManager::GetTick();
	}

	if (GameManager::GetKey()->isStayKeyDown('Q'))
	{
		camRotY -= AngleSpeed*(float)GameManager::GetTick();
	}

	if (GameManager::GetKey()->isOnceKeyDown('R'))
	{
		camRotY = 0;
	}
	frustum.Update();
}

void CameraManager::Reset()
{
	eyePosition = D3DXVECTOR3(0, 0, 0);
	lookAt = D3DXVECTOR3(0, 0, 0);
	upVector = D3DXVECTOR3(0, 1, 0);

	camRotX = 0.0f;		//radian
	camRotY = 0.0f;

	camDistance = 10.0f;

	isDrag = 0;

	lookTarget = nullptr;
}

LRESULT CameraManager::CameraInputProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_RBUTTONDOWN:
		isDrag = true;
		prevMousePos.x = GET_X_LPARAM(lParam);
		prevMousePos.y = GET_Y_LPARAM(lParam);
		break;
	case WM_RBUTTONUP:
		isDrag = false;
		break;
	case WM_MOUSEMOVE:
	{
		if (isDrag)
		{
			POINT currentMousePos;
			currentMousePos.x = GET_X_LPARAM(lParam);
			currentMousePos.y = GET_Y_LPARAM(lParam);
			float fDeltaX = (float)(currentMousePos.x - prevMousePos.x);
			float fDeltaY = (float)(currentMousePos.y - prevMousePos.y);

			camRotX += (fDeltaY / 100.f);
			camRotY += (fDeltaX / 100.f);

			if (camRotX < -D3DX_PI / 0.5f + 0.0001f)
				camRotX = -D3DX_PI / 0.5f + 0.0001f;

			if (camRotX > D3DX_PI / 0.5f - 0.0001f)
				camRotX = D3DX_PI / 0.5f - 0.0001f;

			prevMousePos = currentMousePos;
		}
	}
	break;
	case WM_MOUSEWHEEL:
		camDistance += -GET_WHEEL_DELTA_WPARAM(wParam) / 100.f;
		if (camDistance < 1.0f)
			camDistance = 1.0f;
		break;
	}

	return 0;
}
