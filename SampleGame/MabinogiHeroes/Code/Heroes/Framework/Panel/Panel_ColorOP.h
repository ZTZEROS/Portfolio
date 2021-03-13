#pragma once
#include "Panel_Alpha.h"
class Panel_ColorOP :
	public Panel_Alpha
{
public:
	Panel_ColorOP();
	~Panel_ColorOP();

	virtual void Initialize(float xSize, float ySize, D3DCOLOR color) override;
	virtual void Render();
};

