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
	//ĳ���� ��
	//ĳ���� ���� ���� ���� �����ϴ� �Լ�

	Bone* charaterBone = nullptr;


};

