#pragma once
#include "maps.h"
#include "../itemNPC/NPC/uncle.h"

class insideHouse : public maps
{
private:
	//������ aka 163��~
	BYTE _alphaNight;
	int _alphaCount; //���ĺ��� �����ִ� ī��Ʈ!


public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);
	//���ȿ����� �̺�Ʈ�� ���� �Լ���
	void alphaNight();
public:
	insideHouse();
	~insideHouse();
};