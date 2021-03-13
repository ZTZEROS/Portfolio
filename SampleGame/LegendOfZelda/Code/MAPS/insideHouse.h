#pragma once
#include "maps.h"
#include "../itemNPC/NPC/uncle.h"

class insideHouse : public maps
{
private:
	//조현재 aka 163꺼~
	BYTE _alphaNight;
	int _alphaCount; //알파블랜드 감아주는 카운트!


public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);
	//집안에서의 이벤트시 쓰는 함수들
	void alphaNight();
public:
	insideHouse();
	~insideHouse();
};