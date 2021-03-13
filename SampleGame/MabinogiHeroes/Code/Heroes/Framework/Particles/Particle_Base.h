#pragma once
#include "../Particle.h"

class Particle_Base
{
public:
	Particle_Base();
	virtual ~Particle_Base();

	virtual bool Initialize(char* textureFileName);
	virtual void Destroy();
	virtual void Update() = 0;
	virtual void Render();

	virtual void AddParticle();

	virtual void Reset();
	virtual void ResetParticle(ParticleAttribute& attribute) = 0;

	bool IsEmpty();
	bool IsDead();

protected:
	D3DXMATRIXA16 world;
	DWORD FloatToDWORD(float f){ return *((DWORD*)(&f)); }

	float size = 0.1f;
	LPDIRECT3DTEXTURE9 texture = nullptr;
	LPDIRECT3DVERTEXBUFFER9 vertexBuffer = nullptr;

	std::list<ParticleAttribute> particles;


	DWORD vbSize = 2048;
	DWORD vbOffset = 0;
	DWORD vbBatchSize = 512;

	virtual void PreRender();
	virtual void PostRender();
};

