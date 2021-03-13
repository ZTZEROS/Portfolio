#pragma once
#include "BoundingVolume.h"

class Collider_Box
{
public:
	Collider_Box();
	~Collider_Box();

	void Initialize(D3DXVECTOR3& min, D3DXVECTOR3& max, D3DXMATRIXA16* _parent = nullptr);
	void Destroy();
	void Update();
	void Render();

	inline void SetParent(D3DXMATRIXA16* _parent){ parent = _parent; }
	inline void SetWireRender(bool wire){ wireMode = wire; }
	inline void SetPosition(D3DXVECTOR3& pos){ position = pos; }
	inline BoundingBox& GetBoundingBox(){ return currentBoundingBox; }

	inline void SetMaterialColor(D3DXCOLOR& color){ material.Ambient = color; material.Diffuse = color; }

protected:
	BoundingBox boundingBox = BoundingBox(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(1, 1, 1));
	BoundingBox currentBoundingBox = boundingBox;
	LPD3DXMESH aabbMesh = nullptr;

	float baseHeight = 0.0f;

	D3DXMATRIXA16 world;
	D3DXMATRIXA16* parent = nullptr;	//rotation Á¦¿Ü

	bool wireMode = false;

	D3DXVECTOR3 position = D3DXVECTOR3(0, 0, 0);

	D3DMATERIAL9 material;
};

