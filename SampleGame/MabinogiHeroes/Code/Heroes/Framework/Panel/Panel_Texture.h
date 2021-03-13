#pragma once
#include "../FVF.h"

class Panel_Texture
{
public:
	Panel_Texture();
	~Panel_Texture();

	void Initialize(float xSize, float ySize, char* fileName);
	virtual void Destroy();
	virtual void Update();
	virtual void Render();

	inline const D3DXVECTOR3& GetPosition(){ return position; }
	inline void SetPosition(const D3DXVECTOR3& pos){ position = pos; }
	inline void SetRotationY(float angle){ rotationY = angle; }

protected:
	D3DXVECTOR3 position = D3DXVECTOR3(0, 0, 0);
	std::vector<FVF_PositionTexture> vertex;

	D3DXMATRIXA16 world;
	float rotationY = 0.0f;
	D3DMATERIAL9 material;
	LPDIRECT3DTEXTURE9 texture = nullptr;
};

