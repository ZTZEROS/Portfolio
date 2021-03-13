#pragma once
#include "../gameNode.h"

#define ENEMYSPEED 5       // 몬스터 이동속도
#define TURNMOVE 30        // 몬스터 움직이는 랜덤값 변경에 걸리는 시간
#define RANGE 2			   // 몬스터 충돌 범위
#define MONSWARD 4		   // 몬스터 칼 RECT 곱하는 값
#define MONSWARDRANGE 9	   // 몬스터 칼 RECT 나눠주는 값

class link;

enum MONDIRECTION
{
	MONUP,
	MONDOWN,
	MONLEFT,
	MONRIGHT,
};

class monster : public gameNode
{
protected:
	
	image _image;
	RECT VirtualBody;
	RECT _sword;

	//RECT _gsrc;
	//RECT _btrc;
	int _mondirection;

	link* _link;

	float _gsrcwidth;				//
	float _gsrcheight;				//
	float _centerX;					//
	float _centerY;					//
	int _currentFrameX;				//
	int _currentFrameY;				//
									
	int _count;						//
	int _HitCount;					//
									
	int _currentHP;					//
	int _maxHP;						//
	int _pattern;					//
	int _moveCount;					//
							
	int _distance;					// 철퇴 사이의 거리
	float _x;						// 철퇴의 x 좌표
	float _y;						// 철퇴의 y 좌표
	float _angle;					// 철퇴의 앵글값

	bool _rcview;					// 'T' 눌렀을시에 RECT 표시 on/ off (_rcview : true / flase)

public:
	HRESULT init(void);
	
	HRESULT init(const char* imageName, POINT position, float HP, int pattern);

	void release(void);
	void update(void);
	void render(void);
	
	RECT GetVirtualBody() { return VirtualBody; }

	void direction(void);

	monster();
	~monster();
};
