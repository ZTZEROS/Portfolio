#pragma once
#include <vector>

inline POINT pointMake(int x, int y)
{
	POINT pt = { x, y };
	return pt;
}

inline void LineMake(HDC hdc, int x1, int y1, int x2, int y2)
{
	MoveToEx(hdc, x1, y1, NULL);
	LineTo(hdc, x2, y2);
}

inline void LineMake(HDC hdc, std::vector<POINT> vPt)
{
	std::vector<POINT>::iterator viPoint;

	int i = 0;
	for (viPoint = vPt.begin(); viPoint != vPt.end(); ++viPoint, i++)
	{
		if (i == 0) MoveToEx(hdc, viPoint->x, viPoint->y, NULL);
		else LineTo(hdc, viPoint->x, viPoint->y);
	}

	vPt.clear();
}

inline RECT RectMake(int x, int y, int width, int height)
{
	RECT rc = { x, y, x + width, y + height };
	return rc;
}

inline RECT RectMakeCenter(int x, int y, int width, int height)
{
	RECT rc = { x - width / 2, y - height / 2, x + width / 2, y + height / 2 };
	return rc;
}

inline void EllipseMake(HDC hdc, int x, int y, int width, int height)
{
	Ellipse(hdc, x, y, x + width, y + height);
}

inline void EllipseMakeCenter(HDC hdc, int x, int y, int width, int height)
{
	Ellipse(hdc, x - width / 2, y - height / 2, x + width / 2, y + height / 2);
}