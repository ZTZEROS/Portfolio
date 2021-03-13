#pragma once
#include "Particle_Base.h"
class Particle_Firework :
	public Particle_Base
{
public:
	Particle_Firework(D3DXVECTOR3& _origin, int numParticles);
	virtual ~Particle_Firework();

	void Update() override;
	void ResetParticle(ParticleAttribute& attribute) override;

protected:
	D3DXVECTOR3 origin;
	float radius = 100.0f;
	float bombTime = 2.0f;

	void PreRender() override;
	void PostRender() override;
};

