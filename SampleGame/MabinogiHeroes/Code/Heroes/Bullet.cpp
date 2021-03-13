#include "stdafx.h"
#include "Bullet.h"


Bullet::Bullet()
{
}


Bullet::~Bullet()
{
}

void Bullet::Initialize(D3DXVECTOR3 _start, D3DXVECTOR3 _direction)
{
	colliderSphere = new Collider_Sphere;
	start = _start;
	position = _start;
	direction = _direction;

	position.y += 3.5f;
	//direction.y += 3.5f;

	colliderSphere->Initialize(position, radius);
	boundingSphere.center = position;
	boundingSphere.radius = radius;
	colliderSphere->Update();
}

void Bullet::Update()
{
	D3DXVECTOR3 goal;
	goal = direction - start;
	D3DXVec3Normalize(&goal, &goal);

	float moveRage = D3DXVec3LengthSq(&(position - start)); //이동거리
	if (moveRage < distance * distance )
	{
		position += goal * speed * GameManager::GetTick();
		colliderSphere->Initialize(position, radius);
		boundingSphere.center = position;
		boundingSphere.radius = radius;
		colliderSphere->Update();
	}
	else isEnd = true;

}

void Bullet::Redner()
{
	colliderSphere->Render();
}

void Bullet::Destroy()
{
	SAFE_DELETE(colliderSphere);
}
