#pragma once
#include "BoundingVolume.h"

class Collision
{
public:
	Collision();
	~Collision();

	static bool IsSphereToSphere(BoundingSphere& s1, BoundingSphere& s2);
	static bool IsBoxToBox(BoundingBox& b1, BoundingBox& b2);
	static bool IsBoxToSphere(BoundingBox& b, BoundingSphere& s);
};

