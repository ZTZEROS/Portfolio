#pragma once
//혹시 2010 쓰는 사람이 있을까봐 걸어둔다
#include <cmath>

#define DEG_TO_RAD 0.017453f	//1도의 대한 라디안 값
#define PI		3.141592654f
#define PI2		PI * 2
#define PI8		float(PI / 8.0f)
#define PI16	float(PI / 16.0f)
#define PI32	float(PI / 32.0f)
#define PI64	float(PI / 64.0f)
#define PI128	float(PI / 128.0f)


#define FLOAT_EPSILON 0.001f	//실수의 가장 작은 단위를 지칭하는 그리스어
#define FLOAT_TO_INT(f1) static_cast<int>(f1 + FLOAT_EPSILON) //실수형을 정수형으로~
#define FLOAT_EQUAL(f1, f2) (fabs(f1, f2) <= FLOAT_EPSILON) //두 실수가 같은지 비교~


namespace DISGAEA_UTIL
{
	//거리가져오는 함수
	float getDistance(float startX, float startY, float endX, float endY);

	float getAngle(float x1, float y1, float x2, float y2);

	bool ptInIsoTile(const RECT* rc, POINT pt);

	void drawSelectedIsoTile(HDC hdc, const RECT* rc);

	void drawIsoMetricMap(HDC hdc, const RECT* rc, POINT pt, int tileNumX, int tileNumY);
}