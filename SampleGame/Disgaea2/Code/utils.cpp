#include "stdafx.h"
#include "utils.h"

namespace DISGAEA_UTIL
{
	float getDistance(float startX, float startY, float endX, float endY)
	{
		float x = endX - startX;
		float y = endY - startY;

		return sqrtf(x * x + y * y);
	}

	//앵글값을 얻어오는 함수 3D에서 벡터개념과 비슷
	float getAngle(float x1, float y1, float x2, float y2)
	{
		float x = x2 - x1;
		float y = y2 - y1;

		float distance = sqrtf(x * x + y * y);
		float angle = acosf(x / distance);

		if (y2 > y1)
		{
			angle = PI2 - angle;

			if (angle >= PI2) angle -= PI2;
		}

		return angle;
	}
	bool ptInIsoTile(const RECT* rc, POINT pt)
	{
		float slope = 0.5f;
		POINT point1 = PointMake(rc->left + (rc->right - rc->left) / 2, rc->top);
		POINT point2 = PointMake(rc->right, rc->top + (rc->bottom - rc->top) / 2);
		POINT point3 = PointMake(rc->left + (rc->right - rc->left) / 2, rc->bottom);
		POINT point4 = PointMake(rc->left, rc->top + (rc->bottom - rc->top) / 2);

		float distance1 = point1.y - (-slope*point1.x);
		float distance2 = point2.y - (slope*point2.x);
		float distance3 = point3.y - (-slope*point3.x);
		float distance4 = point4.y - (slope*point4.x);

		float result1 = -slope *pt.x - pt.y + distance1;
		float result2 = slope * pt.x - pt.y + distance2;
		float result3 = -slope *pt.x - pt.y + distance3;
		float result4 = slope * pt.x - pt.y + distance4;

		if (result1 <= 0.0f && result2 <= 0.0f && result3 >= 0.0f && result4 >= 0.0f)
		{
			return true;
		}
		else return false;
	}

	void drawSelectedIsoTile(HDC hdc, const RECT* rc)
	{
		HPEN oldPen, myPen;
		myPen = CreatePen(PS_DASHDOT, 3, RGB(120, 20, 120));
		oldPen = (HPEN)SelectObject(hdc, myPen);
		LineMake(hdc, rc->left + (rc->right - rc->left) / 2, rc->top, rc->right, rc->top + (rc->bottom - rc->top) / 2);
		LineMake(hdc, rc->right, rc->top + (rc->bottom - rc->top) / 2, rc->left + (rc->right - rc->left) / 2, rc->bottom);
		LineMake(hdc, rc->left + (rc->right - rc->left) / 2, rc->bottom, rc->left, rc->top + (rc->bottom - rc->top) / 2);
		LineMake(hdc, rc->left, rc->top + (rc->bottom - rc->top) / 2, rc->left + (rc->right - rc->left) / 2, rc->top);
		SelectObject(hdc, oldPen);
		DeleteObject(myPen);
	}
	void drawIsoMetricMap(HDC hdc, const RECT* rc, POINT pt, int tileNumX, int tileNumY)
	{
		char str[13];
		sprintf(str, "(%d,%d)", tileNumX, tileNumY);
		TextOut(hdc, pt.x, pt.y, str, strlen(str));
		HPEN oldPen, myPen;
		myPen = CreatePen(PS_SOLID, 1, RGB(10, 22, 130));
		oldPen = (HPEN)SelectObject(hdc, myPen);
		MoveToEx(hdc, rc->left, pt.y, NULL);
		LineTo(hdc, pt.x, rc->top);
		LineTo(hdc, rc->right, pt.y);
		LineTo(hdc, pt.x, rc->bottom);
		LineTo(hdc, rc->left, pt.y);
		SelectObject(hdc, oldPen);
		DeleteObject(myPen);

	}
}