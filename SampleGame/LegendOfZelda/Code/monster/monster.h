#pragma once
#include "../gameNode.h"

#define ENEMYSPEED 5       // ���� �̵��ӵ�
#define TURNMOVE 30        // ���� �����̴� ������ ���濡 �ɸ��� �ð�
#define RANGE 2			   // ���� �浹 ����
#define MONSWARD 4		   // ���� Į RECT ���ϴ� ��
#define MONSWARDRANGE 9	   // ���� Į RECT �����ִ� ��

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
							
	int _distance;					// ö�� ������ �Ÿ�
	float _x;						// ö���� x ��ǥ
	float _y;						// ö���� y ��ǥ
	float _angle;					// ö���� �ޱ۰�

	bool _rcview;					// 'T' �������ÿ� RECT ǥ�� on/ off (_rcview : true / flase)

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
