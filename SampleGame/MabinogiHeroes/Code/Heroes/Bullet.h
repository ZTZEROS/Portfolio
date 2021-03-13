#pragma once
#include "Framework\Collision\Collider_Sphere.h"

class Bullet
{
public:
	Bullet();
	~Bullet();

	void Initialize(D3DXVECTOR3 _start, D3DXVECTOR3 _direction);
	void Update();
	void Redner();
	void Destroy();
	virtual inline BoundingSphere* GetBoundingSphere() { return &boundingSphere; }

	bool isEnd = false;
	bool isAttack = false;
private:
	D3DXVECTOR3 start; //시작점
	D3DXVECTOR3 direction; //목적지
	D3DXVECTOR3 position; //현재위치
	D3DXVECTOR3 world; //월드

	D3DXVECTOR3 baseDirection;

	float distance = 55.0f;
	float speed = 15.0f;
	float radius = 1.0f;

	Collider_Sphere* colliderSphere = nullptr;
	BoundingSphere boundingSphere = BoundingSphere();

	
};

