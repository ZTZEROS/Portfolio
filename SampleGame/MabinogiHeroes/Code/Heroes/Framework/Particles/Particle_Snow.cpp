#include "stdafx.h"
#include "Particle_Snow.h"


Particle_Snow::Particle_Snow(BoundingBox& _box, int numParticles)
{
	box = _box;
	size = 0.25f;
	for (int i = 0; i < numParticles; ++i)
	{
		AddParticle();
	}
}


Particle_Snow::~Particle_Snow()
{
}

void Particle_Snow::Update()
{
	for (auto iter = particles.begin(); iter != particles.end(); ++iter)
	{
		iter->position += (iter->velocity * (float)GameManager::GetTick());

		if (box.IsPointIn(iter->position) == false)
		{
			ResetParticle(*iter);
		}
	}
}

void Particle_Snow::ResetParticle(ParticleAttribute& attribute)
{
	attribute.isAlive = true;

	RandomUtil::GetVector(attribute.position, box.max, box.min);
	attribute.position.y = box.max.y;

	attribute.velocity.x = RandomUtil::GetFloat(1.0f) * -3.0f;
	attribute.velocity.y = RandomUtil::GetFloat(1.0f) * -10.0f;
	attribute.velocity.z = 0.0f;

	attribute.color = D3DCOLOR_XRGB(255, 255, 255);
}