#pragma once

struct Particle
{
	D3DXVECTOR3 position = D3DXVECTOR3(0, 0, 0);
	D3DCOLOR color = D3DCOLOR_XRGB(0, 0, 0);

	Particle()
	{
	}
	Particle(D3DXVECTOR3 _position, D3DCOLOR _color)
		:position(_position), color(_color)
	{
	}

	enum{ FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };
};

struct ParticleAttribute
{
	D3DXVECTOR3 position = D3DXVECTOR3(0, 0, 0);
	D3DXVECTOR3 velocity = D3DXVECTOR3(0, 0, 0);

	float lifeTime = 0.0f;
	float age = 0.0f;
	D3DXCOLOR color = D3DCOLOR_XRGB(255, 255, 255);
	D3DXCOLOR colorFade = D3DCOLOR_XRGB(0, 0, 0);

	bool isAlive = false;
};