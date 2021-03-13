#pragma once

class TheSun
{
private:
	LPD3DXMESH SunSphere; //Orb;

	D3DXVECTOR4 Position;

	D3DXMATRIXA16 Scale;
	D3DXMATRIXA16 Rotation;
	D3DXMATRIXA16 Translation;
	D3DXMATRIXA16 World;

	D3DMATERIAL9 Material;
	D3DLIGHT9 Light;
	LPDIRECT3DTEXTURE9 SunTexture;

public:
	TheSun();
	~TheSun();

	void Initialize(float radius = 19.5, unsigned int slices = 100, unsigned int stacks = 100);
	void Destroy();
	void Reset();
	void Update();
	void Render();
	void Shader();

	D3DXVECTOR4 GetPosition() { return Position; }
	D3DXMATRIXA16 GetWorld() { return World; }

	void SetPosition(D3DXVECTOR4 position) { Position = position; }
};

