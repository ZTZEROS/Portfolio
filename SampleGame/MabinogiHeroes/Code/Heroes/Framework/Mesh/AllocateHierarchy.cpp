#include "stdafx.h"
#include "AllocateHierarchy.h"


AllocateHierarchy::AllocateHierarchy()
{
}


AllocateHierarchy::~AllocateHierarchy()
{
}

STDMETHODIMP AllocateHierarchy::CreateFrame(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame)
{
	Bone* bone = new Bone;

	bone->Name = nullptr;
	if (Name)
	{
		int len = strlen(Name) + 1;
		bone->Name = new char[len];
		strcpy_s(bone->Name, len, Name);
	}

	bone->pFrameFirstChild = NULL;
	bone->pFrameSibling = NULL;
	bone->pMeshContainer = NULL;

	D3DXMatrixIdentity(&bone->TransformationMatrix);
	D3DXMatrixIdentity(&bone->CombinedTransformationMatrix);

	*ppNewFrame = bone;

	return S_OK;
}

STDMETHODIMP AllocateHierarchy::CreateMeshContainer(THIS_ LPCSTR Name,
	CONST D3DXMESHDATA *pMeshData,
	CONST D3DXMATERIAL *pMaterials,
	CONST D3DXEFFECTINSTANCE *pEffectInstances,
	DWORD NumMaterials,
	CONST DWORD *pAdjacency,
	LPD3DXSKININFO pSkinInfo,
	LPD3DXMESHCONTAINER *ppNewMeshContainer)
{
	assert(pMeshData && pMeshData->Type == D3DXMESHTYPE_MESH);

	// �޽� �����̳� ����
	BoneMesh* boneMesh = new BoneMesh;

	// �̸� ����
	boneMesh->Name = nullptr;

	// ����, �ؽ��� ���� ����
	for (DWORD i = 0; i < NumMaterials; ++i)
	{
		// �������� ����
		boneMesh->materials.push_back(pMaterials[i].MatD3D);

		// �ؽ��� ���� ����.
		if (pMaterials[i].pTextureFilename)
		{
			std::string fileName(pMaterials[i].pTextureFilename);
			LPDIRECT3DTEXTURE9 texture = TextureManager::GetTexture(directory.c_str(), fileName.c_str());
			boneMesh->textures.push_back(texture);
		}
	}

	// ����Ʈ ����
	boneMesh->pEffects = nullptr;

	// �������� ����
	if (pAdjacency)
	{
		DWORD numFaces = pMeshData->pMesh->GetNumFaces();
		boneMesh->pAdjacency = new DWORD[3 * numFaces];
		memcpy(boneMesh->pAdjacency, pAdjacency, 3 * sizeof(DWORD) * numFaces);
	}

	// pSkinInfo ����
	if (pSkinInfo)
	{
		pSkinInfo->AddRef();
	}
	boneMesh->pSkinInfo = pSkinInfo;

	// pMeshData->pMesh�� ���� �޽��� ����
	if (pMeshData && pMeshData->pMesh)
	{
		D3DXVECTOR3 vMin(0, 0, 0), vMax(0, 0, 0);

		LPVOID pV = nullptr;
		pMeshData->pMesh->LockVertexBuffer(0, &pV);
		D3DXComputeBoundingBox((D3DXVECTOR3*)pV,
			pMeshData->pMesh->GetNumVertices(),
			D3DXGetFVFVertexSize(pMeshData->pMesh->GetFVF()),
			&vMin,
			&vMax);
		D3DXVec3Minimize(&min, &min, &vMin);
		D3DXVec3Maximize(&max, &max, &vMax);
		pMeshData->pMesh->UnlockVertexBuffer();

		pMeshData->pMesh->CloneMeshFVF(
			pMeshData->pMesh->GetOptions(),
			pMeshData->pMesh->GetFVF(),
			GameManager::GetDevice(),
			&boneMesh->originMesh);
	}

	if (pSkinInfo)
	{
		// pSkinInfo->GetNumBones()�� ���� ������� ��ġ�� ��� ���� ���� ��Ʈ���� ���� ����
		// boneMatrixPointers, boneOffsetMatrices�� �����Ҵ�
		DWORD numBones = pSkinInfo->GetNumBones();
		boneMesh->boneMatrixPointers = new D3DXMATRIXA16*[numBones];
		boneMesh->boneOffsetMatrices = new D3DXMATRIX[numBones];

		// ���� �Ҵ�� boneOffsetMatrices ��Ʈ������ �� ����.
		// pSkinInfo->GetBoneOffsetMatrix(i)
		for (DWORD i = 0; i < numBones; ++i)
		{
			boneMesh->boneMatrixPointers[i] = nullptr;
			boneMesh->boneOffsetMatrices[i] = *(pSkinInfo->GetBoneOffsetMatrix(i));
		}

		// ConvertToIndexedBlendedMesh �Լ��� �ȷ�Ʈ ��Ʈ�� ������ �Ѱܾ��ϱ� ������ 
		// ��������� �����ϰ� �ܺο��� �������ֵ��� ����.
		// �޽��� ������ �ִ� �ȷ�Ʈ�� ������ 
		// defaultPaletteSize�� pSkinInfo->GetNumBones()���� Ŭ �� ����.
		// defaultPaletteSize�� ���̴��� ���ǵǾ��ִ�.
		boneMesh->numPaletteEntries = min(defaultPaletteSize, pSkinInfo->GetNumBones());

		// �ִ� �ȷ�Ʈ ������ ������Ʈ
		if (maxPaletteSize < boneMesh->numPaletteEntries)
		{
			maxPaletteSize = boneMesh->numPaletteEntries;
		}

		// blend weights�� �ε����� �̿��� pWorkingMesh ����.
		boneMesh->pSkinInfo->ConvertToIndexedBlendedMesh(
			boneMesh->originMesh,
			D3DXMESH_MANAGED | D3DXMESHOPT_VERTEXCACHE,
			boneMesh->numPaletteEntries,
			boneMesh->pAdjacency,
			nullptr,
			nullptr,
			nullptr,
			&boneMesh->maxNumFaceInfls,
			&boneMesh->numAttrGroups,
			&boneMesh->bufferBoneCombos,
			&boneMesh->workingMesh);

		// ���� �޽��� FVF�� ���ؽ� ���� ������ ���� �� �� ���� ��� FVF�� �����Ѵ�
		DWORD oldFVF = boneMesh->workingMesh->GetFVF();
		DWORD newFVF = (oldFVF & D3DFVF_POSITION_MASK) | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_LASTBETA_UBYTE4;

		if (newFVF != oldFVF)
		{
			// �޽��� �����ϸ鼭 FVF�� �����Ѵ�.
			LPD3DXMESH pMesh = nullptr;

			boneMesh->workingMesh->CloneMeshFVF(
				boneMesh->workingMesh->GetOptions(),
				newFVF,
				GameManager::GetDevice(),
				&pMesh);

			// ���� �޽��� ��ü
			SAFE_RELEASE(boneMesh->workingMesh);
			boneMesh->workingMesh = pMesh;

			// ���� ������ �޽����� ��� ������ �����Ƿ�
			// ���� �޽� FVF�� ��� ������ ������ ���� ����������.
			if (!(oldFVF & D3DFVF_NORMAL))
			{
				D3DXComputeNormals(boneMesh->workingMesh, nullptr);
			}
		}

		// Interpret the UBYTE4 as a D3DCOLOR.
		// The GeForce3 doesn't support the UBYTE4 decl type.  So, we convert any
		// blend indices to a D3DCOLOR semantic, and later in the shader convert
		// it back using the D3DCOLORtoUBYTE4() intrinsic.  Note that we don't
		// convert any data, just the declaration.
		D3DVERTEXELEMENT9 pDecl[MAX_FVF_DECL_SIZE];
		D3DVERTEXELEMENT9* pDeclCur;
		boneMesh->workingMesh->GetDeclaration(pDecl);

		pDeclCur = pDecl;
		while (pDeclCur->Stream != 0xff)
		{
			if ((pDeclCur->Usage == D3DDECLUSAGE_BLENDINDICES) &&
				(pDeclCur->UsageIndex == 0))
				pDeclCur->Type = D3DDECLTYPE_D3DCOLOR;
			pDeclCur++;
		}

		boneMesh->workingMesh->UpdateSemantics(pDecl);
	}

	*ppNewMeshContainer = boneMesh;

	return S_OK;
}

STDMETHODIMP AllocateHierarchy::DestroyFrame(THIS_ LPD3DXFRAME pFrameToFree)
{
	Bone* bone = (Bone*)pFrameToFree;

	SAFE_DELETE_ARRAY_ONLY(bone->Name);

	delete pFrameToFree;

	return S_OK;
}

STDMETHODIMP AllocateHierarchy::DestroyMeshContainer(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree)
{
	BoneMesh* boneMesh = (BoneMesh*)pMeshContainerToFree;

	SAFE_RELEASE(boneMesh->pSkinInfo);
	SAFE_RELEASE(boneMesh->workingMesh);
	SAFE_RELEASE(boneMesh->originMesh);
	SAFE_RELEASE(boneMesh->bufferBoneCombos);

	SAFE_DELETE_ARRAY_ONLY(boneMesh->Name);
	SAFE_DELETE_ARRAY_ONLY(boneMesh->pAdjacency);
	SAFE_DELETE_ARRAY_ONLY(boneMesh->boneMatrixPointers);
	SAFE_DELETE_ARRAY_ONLY(boneMesh->boneOffsetMatrices);

	if (pMeshContainerToFree)
	{
		delete pMeshContainerToFree;
		pMeshContainerToFree = nullptr;
	}

	return S_OK;
}
