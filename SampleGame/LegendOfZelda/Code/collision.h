#pragma once

namespace ZELING_UTIL
{
	//포인트 구조체
	typedef struct tagPoint
	{
		float x;
		float y;

		tagPoint() : x(0.0f), y(0.0f) {}

		void move(float dx, float dy)
		{
			x += dx;
			y += dy;
		}
	}MYPOINT, *LPMYPOINT;

	//원 구조체
	typedef struct tagCircle
	{
		float x;	//원의 중심x좌표
		float y;	//원의 중심y좌표
		float r;	//원의 반지름

		tagCircle() : x(0.0f), y(0.0f), r(0.0f) {}
		tagCircle(float _x, float _y, float _r) : x(_x), y(_y), r(_r) {}

		void move(float dx, float dy)
		{
			x += dx;
			y += dy;
		}

		void set(float centerX, float centerY, float radius)
		{
			x = centerX;
			y = centerY;
			r = radius;
		}

		void setCenterPos(float posX, float posY)
		{
			x = posX;
			y = posY;
		}

		void render(HDC hdc)
		{
			int x1 = FLOAT_TO_INT(x);
			int y1 = FLOAT_TO_INT(y);
			int r1 = FLOAT_TO_INT(r);

			Ellipse(hdc, x1 - r1, y1 - r1, x1 + r1, y1 + r1);
		}
	}MYCIRCLE, *LPMYCIRCLE;

	//사각형 구조체
	typedef struct tagMYRECT
	{
		float left;
		float top;
		float right;
		float bottom;

		tagMYRECT() : left(0.0f), top(0.0f), right(0.0f), bottom(0.0f) {}
		tagMYRECT(float _left, float _top, float _right, float _bottom) :
			left(_left), top(_top), right(_right), bottom(_bottom) {}

		void move(float dx, float dy)
		{
			left += dx;
			right += dx;
			top += dy;
			bottom += dy;
		}

		void set(float _left, float _top, float _right, float _bottom)
		{
			left = _left;
			top = _top;
			right = _right;
			bottom = _bottom;
		}

		//레프트 탑으로 셋팅
		void setLeftTopPos(float _left, float _top)
		{
			float width = getWidth();
			float height = getHeight();

			left = _left;
			top = _top;
			right = _left + width;
			bottom = _top + height;
		}

		void setCenterPos(float centerX, float centerY)
		{
			float halfW = getWidth() * 0.5f;
			float halfH = getHeight() * 0.5f;

			left = centerX - halfW;
			top = centerY - halfH;
			right = centerX + halfW;
			bottom = centerY + halfH;
		}

		void render(HDC hdc)
		{
			Rectangle(hdc,
				FLOAT_TO_INT(left),
				FLOAT_TO_INT(top),
				FLOAT_TO_INT(right),
				FLOAT_TO_INT(bottom));
		}
		
		float getWidth(void) { return right - left; }
		float getHeight(void) { return bottom - top; }
	}MYRECT, *LPMYRECT;

	//POINT가 사각형 안에 있니?
	bool checkPointInRect(const RECT& rc, const POINT& pt);
	bool checkPointInRect(const RECT& rc, int x, int y);
	bool checkPointInRect(const MYRECT& rc, float x, float y);
	bool checkPointInRect(const MYRECT& rc, const MYPOINT& pt);
	
	//POINT가 원 안에 있니?
	bool checkPointInCircle(float cX, float cY, float cR, const MYPOINT& pt);
	bool checkPointInCircle(float cX, float cY, float cR, float x, float y);
	bool checkPointInCircle(const MYCIRCLE& rc, float x, float y);
	bool checkPointInCircle(const MYCIRCLE& rc, const MYPOINT& pt);

	//사각형이 사각형과 충돌했니?
	bool isCollision(const MYRECT& rc1, const MYRECT& rc2);
	bool isCollision(const RECT& rc1, const RECT& rc2);

	//원과 원이 충돌했니?
	bool isCollision(const MYCIRCLE& cir1, const MYCIRCLE& cir2);
	
	//원과 사각형이 충돌했니?
	bool isCollision(const MYCIRCLE& cir, const RECT& rc);
	bool isCollision(const MYCIRCLE& cir, const MYRECT& rc);


	//충돌 했다면 리액션은 어떻게 취하니?
	bool isCollisionReaction(const RECT& rcHold, RECT& rcMove);
	bool isCollisionReaction(const MYRECT& mrcHold, MYRECT& mrcMove);
	bool isCollisionReaction(const MYCIRCLE& cirHold, MYCIRCLE& cirMove);
	
}