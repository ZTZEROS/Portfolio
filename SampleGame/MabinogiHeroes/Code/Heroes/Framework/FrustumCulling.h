#pragma once
#include "./Collision/BoundingVolume.h"

class FrustumCulling
{
public:
	FrustumCulling();
	~FrustumCulling();

	void Initialize();
	void Update();
	bool IsInFrustum(BoundingSphere* target);

private:
	std::vector<D3DXVECTOR3> projectionSpacePointArray;
	std::vector<D3DXPLANE> frustumPlaneArray;
};

