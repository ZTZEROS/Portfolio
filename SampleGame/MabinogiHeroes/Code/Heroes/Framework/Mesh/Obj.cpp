#include "stdafx.h"
#include "Obj.h"
#include "ObjLoader.h"

Obj::Obj() { D3DXMatrixIdentity(&world); D3DXMatrixIdentity(&Scale); D3DXMatrixIdentity(&RotationX); D3DXMatrixIdentity(&RotationY); D3DXMatrixIdentity(&RotationZ); D3DXMatrixIdentity(&Translation); }
Obj::~Obj() {}

void Obj::Initialize(char* mapFileName, char* groundFileName /*= nullptr*/)
{
	Destroy();
	ObjLoader::LoadObjFileData(objGroups, mapFileName);

	if (groundFileName)
	{
		ObjLoader::LoadObj_OnlyVertex(objGround, groundFileName);
	}

	D3DXMatrixIdentity(&world);
	D3DXMatrixIdentity(&Scale);
	D3DXMatrixIdentity(&RotationX);
	D3DXMatrixIdentity(&RotationY);
	D3DXMatrixIdentity(&RotationZ);
	D3DXMatrixIdentity(&Translation);
}

void Obj::Destroy()
{
	for ( auto iter = objGroups.begin(); iter != objGroups.end(); ++iter )
	{
		delete *iter;
		*iter = nullptr;
	}
	objGroups.clear();
	objGround.clear();
}

void Obj::Update()
{
	D3DXMATRIX rotation;

	rotation = RotationX * RotationY * RotationZ;

	D3DXMatrixTranslation(&Translation, position.x, position.y, position.z);

	world = Scale * rotation * Translation;
}

void Obj::Render()
{
	D3DMATERIAL9 temp;
	temp.Ambient = D3DXCOLOR(0.8f, 0.0f, 0.0f, 1.0f);
	temp.Diffuse = D3DXCOLOR(0.8f, 0.0f, 0.0f, 1.0f);
	temp.Specular = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);

	GameManager::GetDevice()->SetRenderState(D3DRS_LIGHTING, true);
	GameManager::GetDevice()->SetTransform(D3DTS_WORLD, &world);
	GameManager::GetDevice()->SetMaterial(&temp);
	for ( auto iter = objGroups.begin(); iter != objGroups.end(); ++iter )
	{
		( *iter )->Render();
	}
}

bool Obj::GroundCheck(IN OUT D3DXVECTOR3& groundPos) const
{
	bool find = false;
	D3DXVECTOR3 rayStart(groundPos.x, 1000.0f, groundPos.z);
	D3DXVECTOR3 rayDirection(0, -1, 0);

	for (size_t i = 0; i < objGround.size(); i += 3)
	{
		float u, v, distance;
		find = D3DXIntersectTri(
			&objGround[i], &objGround[i + 1], &objGround[i + 2],
			&rayStart, &rayDirection,
			&u, &v, &distance) != 0;

		if (find == true)
		{
			//groundPos = rayStart - rayDirection * distance;
			groundPos.y = 1000.0f - distance;
			//groundPos = objGround[i]
			//			 + ( ( objGround[i + 1] - objGround[i] ) * u ) 
			//			 + ( ( objGround[i + 2] - objGround[i] ) * v );
			break;
		}

	}

	return find;
}