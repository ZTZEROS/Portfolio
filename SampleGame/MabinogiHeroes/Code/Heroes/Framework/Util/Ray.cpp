#include "stdafx.h"
#include "Ray.h"


Ray::Ray()
{
}


Ray::~Ray()
{
}


Ray Ray::CalcRayFromSceenPoint(POINT mousePoint)
{
	D3DVIEWPORT9 viewPort;
	GameManager::GetDevice()->GetViewport(&viewPort);
	D3DXMATRIXA16 projection;
	GameManager::GetDevice()->GetTransform(D3DTS_PROJECTION, &projection);

	Ray resultRay;

	//뷰포트와 프로젝션의 영향 제거
	resultRay.direction.x = ((2.0f * mousePoint.x) / viewPort.Width - 1.0f) / projection(0, 0);	//projection._11
	resultRay.direction.y = ((-2.0f * mousePoint.y) / viewPort.Height + 1.0f) / projection(1, 1);
	resultRay.direction.z = 1.0f;

	D3DXMATRIXA16 view, inverseView;
	GameManager::GetDevice()->GetTransform(D3DTS_VIEW, &view);
	D3DXMatrixInverse(&inverseView, nullptr, &view);

	//뷰 매트릭스의 영향 제거(원점은 위치고 방향은 벡터임)
	D3DXVec3TransformCoord(&resultRay.origin, &resultRay.origin, &inverseView);			//위치
	D3DXVec3TransformNormal(&resultRay.direction, &resultRay.direction, &inverseView);	//방향
	D3DXVec3Normalize(&resultRay.direction, &resultRay.direction);

	return resultRay;
}

bool Ray::IsPicked(const BoundingSphere* sphere)
{
	if (sphere)
	{
		D3DXVECTOR3 lengthVector = sphere->center - origin;
		float lengthSquare = D3DXVec3LengthSq(&lengthVector);
		float radiusSquare = sphere->radius * sphere->radius;
		if (lengthSquare < radiusSquare)
		{
			return true;	//광선이 구 안에서 시작된다.
		}

		float s = D3DXVec3Dot(&lengthVector, &direction);
		if (s < 0)
		{
			//광선의 시작점은 구의 밖에 있다.
			//두 백터의 내각이 90도가 넘어섰다.
			// == 둘이 만날일은 존재하지 않는다.
			return false;
		}

		float mSquare = lengthSquare - s * s;	//피타고라스의 정리를 이용해 m을 구한다.
		if (mSquare > radiusSquare)
		{
			//m이 구의 반지름보다 크면 이것도 만날일이 존재하지 않는다.
			return false;
		}
		else
		{
			//m이 구의 반지름보다 작다면 무조건 만난다.
			return true;
		}
	}

	return false;
}

bool Ray::IsPicked(const BoundingBox * box)
{
	//인터넷 방법
	/*D3DXVECTOR3 invdir;
	invdir.x = 1 / direction.x;
	invdir.y = 1 / direction.y;
	invdir.z = 1 / direction.z;

	int sign[3];
	sign[0] = (invdir.x < 0);
	sign[1] = (invdir.y < 0);
	sign[2] = (invdir.z < 0);

	float tmin, tmax, tymin, tymax, tzmin, tzmax;
	tmin = (box->bounds[sign[0]].x - origin.x)*invdir.x;
	tmax = (box->bounds[1 - sign[0]].x - origin.x)*invdir.x;
	tymin = (box->bounds[sign[1]].y - origin.y)*invdir.y;
	tymax = (box->bounds[1 - sign[1]].y - origin.y)*invdir.y;

	if ((tmin > tymax) || (tymin > tmax)) return false;
	if (tymin > tmin) tmin = tymin;
	if (tymax < tmax) tmax = tymax;

	tzmin = (box->bounds[sign[2]].z - origin.z)*invdir.z;
	tzmax = (box->bounds[1 - sign[2]].z - origin.z)*invdir.z;

	if ((tmin > tzmax) || (tzmin > tmax)) return false;
	if (tzmin > tmin) tmin = tzmin;
	if (tzmax < tmax) tmax = tzmax;

	return true;*/

	//내방법
	float distance = 0;
	float diff = 0;
	if (box)
	{
		
		if (origin.x < box->min.x)
		{
			diff = box->min.x - origin.x;
			distance += (diff * diff);
		}
		else if (origin.x > box->max.x)
		{
			diff = origin.x - box->max.x;
			distance += (diff * diff);
		}
		if (origin.y < box->min.y)
		{
			diff = box->min.y - origin.y;
			distance += (diff * diff);
		}
		else if (origin.y > box->max.y)
		{
			diff = origin.y - box->max.y;
			distance += (diff * diff);
		}
		if (origin.z < box->min.z)
		{
			diff = box->min.z - origin.z;
			distance += (diff * diff);
		}
		else if (origin.z > box->max.z)
		{
			diff = origin.z - box->max.z;
			distance += (diff * diff);
		}
		if (distance > (AttackDistance*AttackDistance))
		{
			return false;
		}
		D3DXVECTOR3 boxCenter = (box->max - box->min) * 0.5 + box->min;
		D3DXVECTOR3 lengthVector = boxCenter - origin;
		float lengthSquare = D3DXVec3LengthSq(&lengthVector);
		float s = D3DXVec3Dot(&lengthVector, &direction);
		if (s < 0)
		{
			//광선의 시작점은 구의 밖에 있다.
			//두 백터의 내각이 90도가 넘어섰다.
			// == 둘이 만날일은 존재하지 않는다.
			return false;
		}
		float mSquare = lengthSquare - s * s;	//피타고라스의 정리를 이용해 m을 구한다.
		if (mSquare > distance)
		{
			//m이 구의 반지름보다 크면 이것도 만날일이 존재하지 않는다.
			return false;
		}
	}
	return true;
}
