#pragma once
#include "ObjFileGroup.h"


class Obj
{
public:
	Obj();
	~Obj();

	void Initialize(char* mapFileName, char* groundFileName = nullptr);
	void Destroy();
	void Update();
	void Render();


	bool GroundCheck(IN OUT D3DXVECTOR3& groundPos) const;

	inline D3DXMATRIX GetWorld() { return world; }
	inline D3DXVECTOR3 GetPosition() { return position; }

	inline void SetScale(float scale) { D3DXMatrixScaling(&Scale, scale, scale, scale); }

	inline void SetRotationX(float angle) { D3DXMatrixRotationX(&RotationX, angle); }
	inline void SetRotationY(float angle) { D3DXMatrixRotationY(&RotationY, angle); }
	inline void SetRotationZ(float angle) { D3DXMatrixRotationZ(&RotationZ, angle); }
	
	inline void SetPosition(D3DXVECTOR3& pos){ position = pos; }


private:
		
	D3DXMATRIX world;
	D3DXMATRIX Scale, RotationX, RotationY, RotationZ, Translation;

	D3DXVECTOR3 position = D3DXVECTOR3(0, 0, 0);

	std::vector<ObjFileGroup*> objGroups;
	std::vector<D3DXVECTOR3> objGround;
	
};

