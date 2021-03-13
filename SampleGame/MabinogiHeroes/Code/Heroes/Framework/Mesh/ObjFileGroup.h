#pragma once

#include "../FVF.h"
#include "MaterialTexture.h"


class ObjFileGroup
{
public:
	ObjFileGroup(MaterialTexture* _materialTexture, std::vector<FVF_PositionNormalTexture>& _vertex);
	~ObjFileGroup();

	void Render();
	void RenderGround();

	void SetPrimitiveType(_D3DPRIMITIVETYPE primitiveType) { PrimitiveType = primitiveType; (PrimitiveType == D3DPT_TRIANGLEFAN) ? PrimitiveCount = vertex.size() - 2 : PrimitiveCount = vertex.size() / 3; }

private:
	MaterialTexture* materialTexture = nullptr;
	std::vector<FVF_PositionNormalTexture> vertex;
	std::vector<FVF_PositionNormalTexture> groundVertex;

	_D3DPRIMITIVETYPE PrimitiveType = D3DPT_TRIANGLEFAN;
	unsigned int PrimitiveCount;
};

