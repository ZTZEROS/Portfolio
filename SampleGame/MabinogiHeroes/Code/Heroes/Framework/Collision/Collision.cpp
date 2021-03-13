#include "stdafx.h"
#include "Collision.h"


Collision::Collision()
{
}


Collision::~Collision()
{
}

bool Collision::IsSphereToSphere(BoundingSphere& s1, BoundingSphere& s2)
{
	bool result = false;
	D3DXVECTOR3 dv = s1.center - s2.center;
	float d = D3DXVec3LengthSq(&dv);
	float rSum = (s1.radius + s2.radius) * (s1.radius + s2.radius);

	if (d <= rSum)
	{
		result = true;
	}

	return result;
}

bool Collision::IsBoxToBox(BoundingBox& b1, BoundingBox& b2)
{
	if (b1.max.x < b2.min.x || b1.min.x > b2.max.x)
	{
		return false;
	}
	if (b1.max.y < b2.min.y || b1.min.y > b2.max.y)
	{
		return false;
	}
	if (b1.max.z < b2.min.z || b1.min.z > b2.max.z)
	{
		return false;
	}

	return true;
}

bool Collision::IsBoxToSphere(BoundingBox& b, BoundingSphere& s)
{
	float distance = 0.0f;
	float diff = 0.0f;

	if (s.center.x < b.min.x)
	{
		diff = b.min.x - s.center.x;
		distance += (diff * diff);
	}
	else if (s.center.x > b.max.x)
	{
		diff = s.center.x - b.max.x;
		distance += (diff * diff);
	}

	if (s.center.y < b.min.y)
	{
		diff = b.min.y - s.center.y;
		distance += (diff * diff);
	}
	else if (s.center.y > b.max.y)
	{
		diff = s.center.y - b.max.y;
		distance += (diff * diff);
	}

	if (s.center.z < b.min.z)
	{
		diff = b.min.z - s.center.z;
		distance += (diff * diff);
	}
	else if (s.center.z > b.max.z)
	{
		diff = s.center.z - b.max.z;
		distance += (diff * diff);
	}

	if (distance > (s.radius * s.radius))
	{
		return false;
	}

	return true;
}
