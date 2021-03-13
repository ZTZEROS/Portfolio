#include "stdafx.h"
#include "Character_MouseMove.h"


Character_MouseMove::Character_MouseMove()
{
}


Character_MouseMove::~Character_MouseMove()
{
}

LRESULT Character_MouseMove::InputProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;
	switch (message)
	{
	case WM_LBUTTONDOWN:
	{
		POINT mousePoint;
		mousePoint.x = GET_X_LPARAM(lParam);
		mousePoint.y = GET_Y_LPARAM(lParam);
		Ray ray = Ray::CalcRayFromSceenPoint(mousePoint);

		if (grid)
		{
			const std::vector<D3DXVECTOR3>& groundVertex = grid->GetGroundVertex();

			for (size_t i = 0; i < groundVertex.size(); i += 3)
			{
				float u, v, distance;
				if (D3DXIntersectTri(&groundVertex[i], &groundVertex[i + 1], &groundVertex[i + 2],
					&ray.origin, &ray.direction, &u, &v, &distance))
				{
					D3DXVECTOR3 targetPosition = ray.origin + ray.direction * distance;
					/*D3DXVECTOR3 targetPosition = groundVertex[i]
						+ (groundVertex[i + 1] - groundVertex[i]) * u
						+ (groundVertex[i + 2] - groundVertex[i]) * v;*/

					D3DXVECTOR3 targetLength = targetPosition - position;
					float length = D3DXVec3Length(&targetLength);

					SAFE_DELETE(action);
					ActionMove* move = new ActionMove;
					move->SetStart(position);
					move->SetGoal(targetPosition);
					move->SetDuration(length * 0.1f);
					move->SetTarget(this);
					move->Start();
					SetAction(move);
				}
			}
		}
	}
		break;
	case WM_RBUTTONDOWN:
	{
		POINT mousePoint;
		mousePoint.x = GET_X_LPARAM(lParam);
		mousePoint.y = GET_Y_LPARAM(lParam);
		Ray ray = Ray::CalcRayFromSceenPoint(mousePoint);

		bool isPick = ray.IsPicked(&currentBoundingSphere);
		aseRootFrame->SetTextureOn(isPick);
	}
		break;
	}

	return result;
}
