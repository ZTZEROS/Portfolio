#pragma once

enum BoundingVolumeType
{
	SPHERE = 0,
	AABB,
};

struct BoundingSphere
{
	BoundingSphere(){};
	BoundingSphere(D3DXVECTOR3& _center, float _radius)
	{
		radius = _radius;
		center = _center;
	}

	float radius = 0.0f;
	D3DXVECTOR3 center = D3DXVECTOR3(0, 0, 0);
};

struct BoundingBox
{
	BoundingBox() {};
	BoundingBox(D3DXVECTOR3 _min, D3DXVECTOR3 _max)
	{
		min = _min;
		max = _max;
		bounds[0] = _min;
		bounds[1] = _max;
	}

	bool IsPointIn(D3DXVECTOR3& point)
	{
		if (point.x >= min.x && point.y >= min.y && point.z >= min.z &&
			point.x <= max.x && point.y <= max.y && point.z <= max.z)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	D3DXVECTOR3 min = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	D3DXVECTOR3 max = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
	D3DXVECTOR3 bounds[2];
};