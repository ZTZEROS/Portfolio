#include "stdafx.h"
#include "ItemMesh.h"


ItemMesh::ItemMesh()
{
}


ItemMesh::~ItemMesh()
{
}

void ItemMesh::Update(Bone* current, D3DXMATRIXA16* parentMatrix)
{
//	
	current->CombinedTransformationMatrix = current->TransformationMatrix;

	if (parentMatrix)
	{
		char* currentName = (LPSTR)(LPCSTR)current->Name;
		Bone* influence = (Bone*)D3DXFrameFind(charaterBone, currentName);
		if (influence != NULL)	current->CombinedTransformationMatrix = (influence->CombinedTransformationMatrix);
		else current->CombinedTransformationMatrix = current->CombinedTransformationMatrix * (*parentMatrix);
	}

	if (current->pFrameSibling)	//형제
	{
		Update((Bone*)current->pFrameSibling, parentMatrix);
	}

	if (current->pFrameFirstChild)	//첫번째 자식
	{
		Update((Bone*)current->pFrameFirstChild, &(current->CombinedTransformationMatrix));
	}
	//박스 찾기
	BoxUpdate(current);
}