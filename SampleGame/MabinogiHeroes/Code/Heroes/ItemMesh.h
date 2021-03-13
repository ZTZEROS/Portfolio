#pragma once
#include "Framework\Mesh\SkinnedMesh.h"

class ItemMesh : public SkinnedMesh
{

public:
	ItemMesh();
	~ItemMesh();
	virtual void Update(Bone* current, D3DXMATRIXA16* parentMatrix) override;


	inline void SetChBone(Bone* cha) { charaterBone = cha; }

protected:
	//캐릭터 본
	//캐릭터 본과 현재 본을 연결하는 함수

	Bone* charaterBone = nullptr;


};

