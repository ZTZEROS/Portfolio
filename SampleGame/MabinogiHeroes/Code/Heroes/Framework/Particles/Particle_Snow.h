#pragma once
#include "Particle_Base.h"
#include "../Collision//BoundingVolume.h"

class Particle_Snow :
	public Particle_Base
{
public:
	Particle_Snow(BoundingBox& _box, int numParticles);
	virtual ~Particle_Snow();

	void Update() override;
	void ResetParticle(ParticleAttribute& attribute) override;

protected:
	BoundingBox box;
};

