#pragma once

struct Bone : public D3DXFRAME
{
	D3DXMATRIXA16 CombinedTransformationMatrix;	// 영향 누적용
};

struct BoneMesh : public D3DXMESHCONTAINER
{
	std::vector<LPDIRECT3DTEXTURE9> textures;
	std::vector<D3DMATERIAL9> materials;

	LPD3DXMESH workingMesh;				// 작업메쉬
	LPD3DXMESH originMesh;				// 원본 메쉬 CloneMeshFVF로 복사
	D3DXMATRIXA16** boneMatrixPointers;	// 이 메쉬에 영향을 주는 프레임'들'의 월드매트릭스 포인터 배열
	D3DXMATRIX* boneOffsetMatrices;	// 원본 메쉬를 로컬스페이스로 보내는 매트릭스들.

	DWORD numPaletteEntries;
	DWORD maxNumFaceInfls;
	DWORD numAttrGroups;
	LPD3DXBUFFER bufferBoneCombos;
};

class AllocateHierarchy : public ID3DXAllocateHierarchy
{
public:
	AllocateHierarchy();
	~AllocateHierarchy();


	// ID3DXAllocateHierarchy

	//------------------------------------------------------------------------
	// CreateFrame:
	// ------------
	// Requests allocation of a frame object.
	//
	// Parameters:
	//  Name
	//		Name of the frame to be created
	//	ppNewFrame
	//		Returns the created frame object
	//
	//------------------------------------------------------------------------
	STDMETHOD(CreateFrame)(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame) override;

	//------------------------------------------------------------------------
	// CreateMeshContainer:
	// --------------------
	// Requests allocation of a mesh container object.
	//
	// Parameters:
	//  Name
	//		Name of the mesh
	//	pMesh
	//		Pointer to the mesh object if basic polygon data found
	//	pPMesh
	//		Pointer to the progressive mesh object if progressive mesh data found
	//	pPatchMesh
	//		Pointer to the patch mesh object if patch data found
	//	pMaterials
	//		Array of materials used in the mesh
	//	pEffectInstances
	//		Array of effect instances used in the mesh
	//	NumMaterials
	//		Num elements in the pMaterials array
	//	pAdjacency
	//		Adjacency array for the mesh
	//	pSkinInfo
	//		Pointer to the skininfo object if the mesh is skinned
	//	pBoneNames
	//		Array of names, one for each bone in the skinned mesh. 
	//		The numberof bones can be found from the pSkinMesh object
	//	pBoneOffsetMatrices
	//		Array of matrices, one for each bone in the skinned mesh.
	//
	//------------------------------------------------------------------------
	STDMETHOD(CreateMeshContainer)(THIS_
		LPCSTR Name,
		CONST D3DXMESHDATA *pMeshData,
		CONST D3DXMATERIAL *pMaterials,
		CONST D3DXEFFECTINSTANCE *pEffectInstances,
		DWORD NumMaterials,
		CONST DWORD *pAdjacency,
		LPD3DXSKININFO pSkinInfo,
		LPD3DXMESHCONTAINER *ppNewMeshContainer) override;

	//------------------------------------------------------------------------
	// DestroyFrame:
	// -------------
	// Requests de-allocation of a frame object.
	//
	// Parameters:
	//  pFrameToFree
	//		Pointer to the frame to be de-allocated
	//
	//------------------------------------------------------------------------
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree) override;

	//------------------------------------------------------------------------
	// DestroyMeshContainer:
	// ---------------------
	// Requests de-allocation of a mesh container object.
	//
	// Parameters:
	//  pMeshContainerToFree
	//		Pointer to the mesh container object to be de-allocated
	//
	//------------------------------------------------------------------------
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree) override;

	inline std::string& GetDirectory() { return directory; }
	inline D3DXVECTOR3& GetMin() { return min; }
	inline D3DXVECTOR3& GetMax() { return max; }
	inline DWORD GetMaxPaletteSize() { return maxPaletteSize; }

	inline void SetDirectory(std::string str) { directory = str; }
	inline void SetDefaultPaletteSize(DWORD size) { defaultPaletteSize = size; }

private:
	std::string directory = "";
	D3DXVECTOR3 min = D3DXVECTOR3(0, 0, 0);
	D3DXVECTOR3 max = D3DXVECTOR3(0, 0, 0);
	DWORD defaultPaletteSize = 0;
	DWORD maxPaletteSize = 0;
};