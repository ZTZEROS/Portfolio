#pragma once
class Ray
{
public:
	Ray();
	~Ray();

	D3DXVECTOR3 origin = D3DXVECTOR3(0, 0, 0);
	D3DXVECTOR3 direction = D3DXVECTOR3(0, 0, 0);
	float AttackDistance = 0;

	static Ray CalcRayFromSceenPoint(POINT mousePoint);

	bool IsPicked(const BoundingSphere* sphere);
	bool IsPicked(const BoundingBox* box);
};