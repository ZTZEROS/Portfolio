#pragma once
#include "Object/HeightMap.h"
#include "Particles/Particle_Water.h"

class Town : public HeightMap
{
private:
	enum CurrentBarrel { BARREL_COUNT = 3 };
	enum CurrentTree { TREE_COUNT = 10 };

private:
	SkinnedMesh* Inn0;
	SkinnedMesh* Barrels[BARREL_COUNT];
	SkinnedMesh* Trees[TREE_COUNT];
	SkinnedMesh* Fountain;
	
	Particle_Water* FountainWater;
	
	LPD3DXMESH Portal;
	D3DMATERIAL9 PortalMaterial;
	LPDIRECT3DTEXTURE9 PortalTexture;
	float PortalAlpha;
	bool PortalAlphaOn;
	D3DXVECTOR3 PortalPosition;
	D3DXMATRIXA16 PortalScale;
	D3DXMATRIXA16 PortalRotation;
	D3DXMATRIXA16 PortalTranslation;
	D3DXMATRIXA16 PortalWorld;

	D3DXMATRIXA16 LightViewProjection;
	D3DXVECTOR4 WorldLightPosition;

private:
	LPD3DXEFFECT DefaultShader;

public:
	Town();
	~Town();

	virtual void Initialize();
	virtual void Destroy() override;
	virtual void Update();
	virtual void Render() override;
	void UpdateAndRender(); //to Render SkinnedMeshes
	void RenderShadow(); //to Render Shadows of Objects
	void RenderWater();


	D3DXVECTOR3 GetPortalPosition() { return PortalPosition; }
	void SetWorldLightPos(D3DXVECTOR4 worldLightPosition) { WorldLightPosition = worldLightPosition; }
	void SetLightViewProjection(D3DXMATRIXA16& lightViewProjection) { LightViewProjection = lightViewProjection; }
};