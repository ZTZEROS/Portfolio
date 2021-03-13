#pragma once
#include "Panel_Texture.h"
class Panel_AlphaTexture :
	public Panel_Texture
{
public:
	Panel_AlphaTexture();
	~Panel_AlphaTexture();

	virtual void Render() override;
};

