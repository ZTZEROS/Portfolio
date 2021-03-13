#include "stdafx.h"
#include "Particle_Water.h"

Particle_Water::Particle_Water(D3DXVECTOR3& _origin, int numParticles)
{
	origin = _origin;
	size = 0.5f;

	for (int i = 0; i < numParticles; ++i)
	{
		AddParticle();

		//LPD3DXMESH waterDrops;
		//
		//D3DXCreateSphere(GameManager::GetDevice(), 1, 5, 5, &waterDrops, nullptr);
		//
		//WaterdropsL.push_back(waterDrops);
	}
}


Particle_Water::~Particle_Water()
{
}

void Particle_Water::Update()
{
	for (auto iter = particles.begin(); iter != particles.end(); ++iter)
	{
		if (iter->isAlive)
		{
			iter->position += (iter->velocity * (float)GameManager::GetTick());
			iter->age += (float)GameManager::GetTick();

			if (iter->age > iter->lifeTime)
			{
				iter->isAlive = false;
			}
		}
	}

	if (IsDead())
	{
		Reset();
	}
}

void Particle_Water::ResetParticle(ParticleAttribute& attribute)
{
	attribute.isAlive = true;
	attribute.position = origin;

	D3DXVECTOR3 min = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
	D3DXVECTOR3 max = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	RandomUtil::GetVector(attribute.velocity, max, min);

	D3DXVec3Normalize(&attribute.velocity, &attribute.velocity);
	attribute.velocity *= radius;

	attribute.color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);//RandomUtil::GetFloat(1.0f), RandomUtil::GetFloat(1.0f), RandomUtil::GetFloat(1.0f), 1.0f);

	attribute.age = 0.0f;
	attribute.lifeTime = bombTime;
}

void Particle_Water::PreRender()
{
	Particle_Base::PreRender();

	GameManager::GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	GameManager::GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	GameManager::GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, false);
}

void Particle_Water::PostRender()
{
	Particle_Base::PostRender();
	GameManager::GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, true);
}