#pragma once
#include "Panel_Texture.h"
class Panel_Shader
	: public Panel_Texture
{
public:
	Panel_Shader();
	~Panel_Shader();

	void Initialize(float xSize, float ySize, char* fileName);
	virtual void Destroy() override;
	virtual void Update() override;
	virtual void Render() override;

protected:
	LPD3DXEFFECT shaderEffect = nullptr;
	LPD3DXEFFECT LoadEffect(const char* fileName);

};

