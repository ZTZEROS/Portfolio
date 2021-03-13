#include "stdafx.h"
#include "Particle_Firework.h"


Particle_Firework::Particle_Firework(D3DXVECTOR3& _origin, int numParticles)
{
	origin = _origin;
	size = 0.9f;

	for (int i = 0; i < numParticles; ++i)
	{
		AddParticle();
	}
}


Particle_Firework::~Particle_Firework()
{
}

void Particle_Firework::Update()
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

void Particle_Firework::ResetParticle(ParticleAttribute& attribute)
{
	attribute.isAlive = true;
	attribute.position = origin;

	D3DXVECTOR3 min = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
	D3DXVECTOR3 max = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	RandomUtil::GetVector(attribute.velocity,max, min );

	D3DXVec3Normalize(&attribute.velocity, &attribute.velocity);
	attribute.velocity *= radius;

	attribute.color = D3DXCOLOR(RandomUtil::GetFloat(1.0f), RandomUtil::GetFloat(1.0f), RandomUtil::GetFloat(1.0f), 1.0f);

	attribute.age = 0.0f;
	attribute.lifeTime = bombTime;
}

void Particle_Firework::PreRender()
{
	Particle_Base::PreRender();

	GameManager::GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	GameManager::GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	GameManager::GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, false);
}

void Particle_Firework::PostRender()
{
	Particle_Base::PostRender();
	GameManager::GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, true);
}