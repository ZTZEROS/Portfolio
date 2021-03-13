#pragma once
#include "Object/HeightMap.h"
#include "../../Character/Monster/MonsterBase.h"

class Dungeon : public HeightMap
{
private:
	SkinnedMesh* Castle0;

	LPD3DXMESH Portal;
	D3DXVECTOR3 PortalPosition;
	D3DXMATRIXA16 PortalScale;
	D3DXMATRIXA16 PortalRotation;
	D3DXMATRIXA16 PortalTranslation;
	D3DXMATRIXA16 PortalWorld;
	D3DMATERIAL9 PortalMaterial;
	LPDIRECT3DTEXTURE9 PortalTexture;
	float PortalAlpha;
	bool PortalAlphaOn;

	LPD3DXMESH SuccubusRoom;
	TextureNormalTriangle SuccubusRoomTriangles[2];
	D3DXVECTOR3 SuccubusRoomPosition;
	D3DXMATRIXA16 SuccubusRoomScale;
	D3DXMATRIXA16 SuccubusRoomRotation;
	D3DXMATRIXA16 SuccubusRoomTranslation;
	D3DXMATRIXA16 SuccubusRoomWorld;
	D3DMATERIAL9 SuccubusRoomMaterial;
	LPDIRECT3DTEXTURE9 SuccubusRoomTexture;

	D3DXMATRIXA16 LightViewProjection;
	D3DXVECTOR4 WorldLightPosition;

private:
	LPD3DXEFFECT DefaultShader;
	LPD3DXEFFECT MirrorShader;

public:
	Dungeon();
	~Dungeon();

	void Initialize();
	void Destroy();
	void Update();
	void Render();
	void UpdateAndRender(); //to Render SkinnedMeshes
	void RenderShadow(); //to Render Shadows of Objects

	D3DXVECTOR3 GetPortalPosition() { return PortalPosition; }
	void SetWorldLightPos(D3DXVECTOR4 worldLightPosition) { WorldLightPosition = worldLightPosition; }
	void SetLightViewProjection(D3DXMATRIXA16& lightViewProjection) { LightViewProjection = lightViewProjection; }

	void MonsterUpdate(MonsterBase* monster);
};