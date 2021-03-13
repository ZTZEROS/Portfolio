#pragma once
class TheSky
{
private:
	LPD3DXMESH SkySphere; //Orb;
	LPD3DXMESH CloudSphere;

	D3DXVECTOR4 Position;

	D3DXMATRIXA16 Scale;
	D3DXMATRIXA16 Rotation;
	D3DXMATRIXA16 Translation;
	D3DXMATRIXA16 World;

	D3DMATERIAL9 Material;
	LPDIRECT3DTEXTURE9 SkyTexture;
	LPDIRECT3DTEXTURE9 CloudTexture;

public:
	TheSky();
	~TheSky();

	void Initialize(unsigned int radius = 122.5, unsigned int slices = 100, unsigned int stacks = 100);
	void Destroy();
	void Reset();
	void Update();
	void Render();
	void RenderShadow();
	void Shader();

	D3DXVECTOR4 GetPosition() { return Position; }
	D3DXMATRIXA16 GetWorld() { return World; }

	void SetPosition(D3DXVECTOR4 position) { Position = position; }
};

