#pragma once
#include "maps.h"

class outsideHouse : public maps
{
private:
	image* _exteriorGate;
	RECT _rcGate;
	int _exteriorGateX, _exteriorGateY;
public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);
public:
	RECT getExteriorGate(void) { return _rcGate; }
	int getExteriorGateX(void) { return _exteriorGateX; }
	int getExteriorGateY(void) { return _exteriorGateY; }
	void setExteriorGateX(int exteriorGateX) { _exteriorGateX = exteriorGateX; }
	void setExteriorGateY(int exteriorGateY) { _exteriorGateY = exteriorGateY; }
	outsideHouse();
	~outsideHouse();
};