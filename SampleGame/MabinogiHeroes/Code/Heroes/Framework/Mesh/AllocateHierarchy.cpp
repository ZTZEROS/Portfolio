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

	// 메쉬 컨테이너 생성
	BoneMesh* boneMesh = new BoneMesh;

	// 이름 무시
	boneMesh->Name = nullptr;

	// 재질, 텍스쳐 정보 복사
	for (DWORD i = 0; i < NumMaterials; ++i)
	{
		// 재질정보 저장
		boneMesh->materials.push_back(pMaterials[i].MatD3D);

		// 텍스쳐 정보 저장.
		if (pMaterials[i].pTextureFilename)
		{
			std::string fileName(pMaterials[i].pTextureFilename);
			LPDIRECT3DTEXTURE9 texture = TextureManager::GetTexture(directory.c_str(), fileName.c_str());
			boneMesh->textures.push_back(texture);
		}
	}

	// 이펙트 무시
	boneMesh->pEffects = nullptr;

	// 인접정보 복사
	if (pAdjacency)
	{
		DWORD numFaces = pMeshData->pMesh->GetNumFaces();
		boneMesh->pAdjacency = new DWORD[3 * numFaces];
		memcpy(boneMesh->pAdjacency, pAdjacency, 3 * sizeof(DWORD) * numFaces);
	}

	// pSkinInfo 저장
	if (pSkinInfo)
	{
		pSkinInfo->AddRef();
	}
	boneMesh->pSkinInfo = pSkinInfo;

	// pMeshData->pMesh를 원본 메쉬에 복사
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
		// pSkinInfo->GetNumBones()를 통해 영향력을 미치는 모든 본에 대한 매트릭스 들을 세팅
		// boneMatrixPointers, boneOffsetMatrices를 동적할당
		DWORD numBones = pSkinInfo->GetNumBones();
		boneMesh->boneMatrixPointers = new D3DXMATRIXA16*[numBones];
		boneMesh->boneOffsetMatrices = new D3DXMATRIX[numBones];

		// 동적 할당된 boneOffsetMatrices 매트릭스에 값 저장.
		// pSkinInfo->GetBoneOffsetMatrix(i)
		for (DWORD i = 0; i < numBones; ++i)
		{
			boneMesh->boneMatrixPointers[i] = nullptr;
			boneMesh->boneOffsetMatrices[i] = *(pSkinInfo->GetBoneOffsetMatrix(i));
		}

		// ConvertToIndexedBlendedMesh 함수에 팔렛트 엔트리 개수를 넘겨야하기 때문에 
		// 멤버변수로 세팅하고 외부에서 적용해주도록 설정.
		// 메쉬에 영향을 주는 팔레트의 개수는 
		// defaultPaletteSize와 pSkinInfo->GetNumBones()보다 클 수 없다.
		// defaultPaletteSize는 쉐이더에 정의되어있다.
		boneMesh->numPaletteEntries = min(defaultPaletteSize, pSkinInfo->GetNumBones());

		// 최대 팔레트 사이즈 업데이트
		if (maxPaletteSize < boneMesh->numPaletteEntries)
		{
			maxPaletteSize = boneMesh->numPaletteEntries;
		}

		// blend weights와 인덱스를 이용해 pWorkingMesh 생성.
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

		// 기존 메쉬의 FVF가 버텍스 블랜딩 정보를 전달 할 수 없는 경우 FVF를 변경한다
		DWORD oldFVF = boneMesh->workingMesh->GetFVF();
		DWORD newFVF = (oldFVF & D3DFVF_POSITION_MASK) | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_LASTBETA_UBYTE4;

		if (newFVF != oldFVF)
		{
			// 메쉬를 복사하면서 FVF를 변경한다.
			LPD3DXMESH pMesh = nullptr;

			boneMesh->workingMesh->CloneMeshFVF(
				boneMesh->workingMesh->GetOptions(),
				newFVF,
				GameManager::GetDevice(),
				&pMesh);

			// 기존 메쉬를 대체
			SAFE_RELEASE(boneMesh->workingMesh);
			boneMesh->workingMesh = pMesh;

			// 새로 생성한 메쉬에는 노멀 정보가 있으므로
			// 기존 메쉬 FVF에 노멀 정보가 없으면 새로 계산해줘야함.
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
