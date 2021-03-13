#pragma once
#include "Panel_Texture.h"

class Panel_AlphaMaterial
	:public Panel_Texture
{
public:
	Panel_AlphaMaterial();
	~Panel_AlphaMaterial();

	void Initialize(float xSize, float ySize, char* fileName);
	virtual void Update() override;
	virtual void Render() override;
};

