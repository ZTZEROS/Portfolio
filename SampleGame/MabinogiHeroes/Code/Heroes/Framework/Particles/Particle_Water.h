#pragma once
#include "Particle_Base.h"

class Particle_Water :
	public Particle_Base
{
public:
	Particle_Water(D3DXVECTOR3& _origin, int numParticles);
	virtual ~Particle_Water();

	void Update() override;
	//void UpdateAndRender();
	//void RenderShadow();

	void ResetParticle(ParticleAttribute& attribute) override;

protected:
	//std::list<LPD3DXMESH> WaterdropsL;

	D3DXVECTOR3 origin;
	float radius = 10.0f;
	float bombTime = 0.1f;

	void PreRender() override;
	void PostRender() override;
};