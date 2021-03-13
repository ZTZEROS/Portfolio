#pragma once
#include "GameObject.h"
class Barricade :
	public GameObject
{
public:
	Barricade();
	virtual ~Barricade();
	
	void Initialize(D3DXVECTOR3& pos, float radius = 1.0f);
	virtual void Destroy() override;
	virtual void Render() override;

	inline BoundingSphere* GetBoundingSphere(){ return &bs; }
	inline float GetRadius(){ return bs.radius; }

protected:
	BoundingSphere bs = BoundingSphere(D3DXVECTOR3(0, 0, 0), 1.0f);
	
	LPD3DXMESH barricadeMesh = nullptr;
	LPD3DXMESH bsMesh = nullptr;

	D3DXMATRIXA16 boundingWorld;
	D3DXVECTOR3 localPivot = D3DXVECTOR3(0, 0, 0);
	D3DMATERIAL9 baseMaterial;

};

