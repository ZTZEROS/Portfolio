#include "stdafx.h"
#include "SkinnedMesh.h"
#include "AllocateHierarchy.h"


SkinnedMesh::SkinnedMesh()
{
	D3DXMatrixIdentity(&world);
	D3DXMatrixIdentity(&rotationX);
	D3DXMatrixIdentity(&rotationY);
	D3DXMatrixIdentity(&translation);
}

SkinnedMesh::~SkinnedMesh()
{
}

void SkinnedMesh::InitXFile()
{
}

void SkinnedMesh::Initialize(char* path, char* fileName)
{
	Destroy();
	D3DXMatrixIdentity(&world);
	D3DXMatrixIdentity(&rotationX);
	D3DXMatrixIdentity(&rotationY);
	D3DXMatrixIdentity(&translation);
	colliderBox = new Collider_Box;
	colliderBox->SetWireRender(true);

	strcpy_s(aniName, fileName);

	const char* XFILE_DIRECTORY = "../../ResourceData/Xfile/";
	char fullPath[512];
	
	strcpy_s(fullPath, XFILE_DIRECTORY);
	strcat_s(fullPath, path);
	
	// 원본 정보를 찾아서 나에게 복사
	SkinnedMesh* original = SkinnedMeshManager::Get().GetOriginal(fullPath, fileName);

	rootFrame = original->rootFrame;
	workingPaletteSize = original->workingPaletteSize;
	workingPalette = original->workingPalette;
	effect = original->effect;
	boundingSphere = original->boundingSphere;

	// 에니메이션은 따로 돌아야 하니 클론
	original->animController->CloneAnimationController(
		original->animController->GetMaxNumAnimationOutputs(),
		original->animController->GetMaxNumAnimationSets(),
		original->animController->GetMaxNumTracks(),
		original->animController->GetMaxNumEvents(),
		&animController);

	int x = original->animController->GetMaxNumTracks();
	int y = original->animController->GetMaxNumAnimationSets();

	//현재 애니메이션 정보 읽기
	currentAnim = 0;
	//SAFE_RELEASE(animSet);
	animController->GetAnimationSet(currentAnim, &animSet);
	animPeriod = (float)animSet->GetPeriod() - 0.02f;

	isClone = true;
}

void SkinnedMesh::Destroy()
{
	if (isClone == false)
	{
		// 이것은 공유하는 부분이므로 원본 삭제시만 사용
		AllocateHierarchy ah;
		D3DXFrameDestroy((LPD3DXFRAME)rootFrame, &ah);
		SAFE_DELETE_ARRAY_ONLY(workingPalette);
		SAFE_RELEASE(effect);
	}

	// 이건 클론별로 다 따로 가지므로 일일이 삭제
	SAFE_RELEASE(animController);
	SAFE_DELETE(colliderBox);
}

void SkinnedMesh::UpdateAndRender()
{
	//rootFrame을 공유하기 때문에 본 업데이트가 일어난 후 즉시 그리는 수 밖에 없다
	gameTick = (float)GameManager::GetTick();
	if (isPlay) //->GetTime() < animPeriod) && (gameTick < 1.0f))
	{
		animController->AdvanceTime(gameTick*CharacData::GetGameSpeed()*moveSpeed*0.1, nullptr);
	}

	//바운딩 박스 초기화
	MaxName = nullptr;
	MinName = nullptr;
	heightMax = -10000000.f;
	heightMin = 10000000.f;

	if (rootFrame)
	{
		D3DXMatrixIdentity(&world);
		D3DXMatrixIdentity(&scaleXYZ);
		D3DXMatrixIdentity(&rotationX);
		D3DXMatrixIdentity(&rotationY);
		D3DXMatrixIdentity(&rotationWatch);
		D3DXMatrixIdentity(&translation);

		D3DXMatrixScaling(&scaleXYZ, scale, scale, scale);
		D3DXMatrixRotationY(&rotationY, rotationAngle);
		D3DXMatrixRotationY(&rotationWatch, rotationWatchAngle);

		world *= scaleXYZ;
		world *= rotationY;
		world *= rotationWatch;

		D3DXVec3TransformCoord(&direction, &baseDirection, &world);
		D3DXMatrixTranslation(&translation, position.x, position.y, position.z);

		world *= translation;
		
		Update(rootFrame, &world);
		if(aniName[0] != 'I') BoxMake();
		RayMake();
 		Render(rootFrame);
	}
}

void SkinnedMesh::Update(Bone* current, D3DXMATRIXA16* parentMatrix)
{
	//재귀적으로 모든 프레임(본)에 대해서 실행.
	current->CombinedTransformationMatrix = current->TransformationMatrix;
	if (parentMatrix)
	{
		current->CombinedTransformationMatrix = current->CombinedTransformationMatrix * (*parentMatrix);
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

void SkinnedMesh::Render(Bone* current)
{
	assert(current);

	// 각 프레임의 메시 컨테이너에 있는 pSkinInfo를 이용하여 영향받는 모든 
	// 프레임의 매트릭스를 boneMatrixPointers에 연결한다.
	if (current->pMeshContainer)
	{
		BoneMesh* boneMesh = (BoneMesh*)current->pMeshContainer;

		// get bone combinations
		LPD3DXBONECOMBINATION pBoneCombos = (LPD3DXBONECOMBINATION)(boneMesh->bufferBoneCombos->GetBufferPointer());

		D3DXMATRIXA16 matViewProj, matView, matProj;
		GameManager::GetDevice()->GetTransform(D3DTS_VIEW, &matView);
		GameManager::GetDevice()->GetTransform(D3DTS_PROJECTION, &matProj);
		matViewProj = matView * matProj;

		D3DXMATRIXA16 mView, mInvView;
		GameManager::GetDevice()->GetTransform(D3DTS_VIEW, &mView);
		D3DXMatrixInverse(&mInvView, 0, &mView);
		D3DXVECTOR3 vEye = D3DXVECTOR3(0, 0, 0);
		D3DXVec3TransformCoord(&vEye, &vEye, &mInvView);

		effect->SetFloat("alpha", alpha);
		// for each palette
		for (DWORD dwAttrib = 0; dwAttrib < boneMesh->numAttrGroups; ++dwAttrib)
		{
			// set each transform into the palette
			for (DWORD dwPalEntry = 0; dwPalEntry < boneMesh->numPaletteEntries; ++dwPalEntry)
			{
				DWORD dwMatrixIndex = pBoneCombos[dwAttrib].BoneId[dwPalEntry];
				if (dwMatrixIndex != UINT_MAX)
				{
					workingPalette[dwPalEntry] =
						boneMesh->boneOffsetMatrices[dwMatrixIndex] *
						(*boneMesh->boneMatrixPointers[dwMatrixIndex]);
				}
			}

			// set the matrix palette into the effect
			effect->SetMatrixArray("amPalette", workingPalette,	boneMesh->numPaletteEntries);

			//effect->SetMatrix("g_mWorld", &world);
			effect->SetMatrix("g_mViewProj", &matViewProj);
			//effect->SetMatrix("g_mViewProj", &LightViewProjection);

			effect->SetVector("vLightDiffuse", &D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
			effect->SetVector("vWorldLightPos", &WorldLightPos);
			effect->SetVector("vWorldCameraPos", &D3DXVECTOR4(vEye, 1.0f));
			effect->SetVector("vMaterialAmbient", &D3DXVECTOR4(0.53f, 0.53f, 0.53f, 0.53f));
			effect->SetVector("vMaterialDiffuse", &D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));

			// we're pretty much ignoring the materials we got from the x-file; just set
			// the texture here
			effect->SetTexture("g_txScene", boneMesh->textures[pBoneCombos[dwAttrib].AttribId]);

			// set the current number of bones; this tells the effect which shader to use
			effect->SetInt("CurNumBones", boneMesh->maxNumFaceInfls - 1);

			// set the technique we use to draw
			effect->SetTechnique("Skinning20");

			UINT uiPasses, uiPass;

			// run through each pass and draw
			effect->Begin(&uiPasses, 0);
			for (uiPass = 0; uiPass < uiPasses; ++uiPass)
			{
				effect->BeginPass(uiPass);
				boneMesh->workingMesh->DrawSubset(dwAttrib);
				effect->EndPass();
			}
			effect->End();
		}
	}

	//재귀적으로 모든 프레임(본)에 대해서 실행.
	if (current->pFrameSibling)
	{
		Render((Bone*)current->pFrameSibling);
	}

	if (current->pFrameFirstChild)
	{
		Render((Bone*)current->pFrameFirstChild);
	}
}

void SkinnedMesh::RenderShadow(Bone* current)
{
	assert(current);

	// 각 프레임의 메시 컨테이너에 있는 pSkinInfo를 이용하여 영향받는 모든 
	// 프레임의 매트릭스를 boneMatrixPointers에 연결한다.
	if (current->pMeshContainer)
	{
		BoneMesh* boneMesh = (BoneMesh*)current->pMeshContainer;

		// get bone combinations
		LPD3DXBONECOMBINATION pBoneCombos = (LPD3DXBONECOMBINATION)(boneMesh->bufferBoneCombos->GetBufferPointer());

		D3DXMATRIXA16 matViewProj, matView, matProj;
		GameManager::GetDevice()->GetTransform(D3DTS_VIEW, &matView);
		GameManager::GetDevice()->GetTransform(D3DTS_PROJECTION, &matProj);
		matViewProj = matView * matProj;

		D3DXMATRIXA16 mView, mInvView;
		GameManager::GetDevice()->GetTransform(D3DTS_VIEW, &mView);
		D3DXMatrixInverse(&mInvView, 0, &mView);
		D3DXVECTOR3 vEye = D3DXVECTOR3(0, 0, 0);
		D3DXVec3TransformCoord(&vEye, &vEye, &mInvView);


		effect->SetFloat("alpha", alpha);
		// for each palette
		for (DWORD dwAttrib = 0; dwAttrib < boneMesh->numAttrGroups; ++dwAttrib)
		{
			// set each transform into the palette
			for (DWORD dwPalEntry = 0; dwPalEntry < boneMesh->numPaletteEntries; ++dwPalEntry)
			{
				DWORD dwMatrixIndex = pBoneCombos[dwAttrib].BoneId[dwPalEntry];
				if (dwMatrixIndex != UINT_MAX)
				{
					workingPalette[dwPalEntry] =
						boneMesh->boneOffsetMatrices[dwMatrixIndex] *
						(*boneMesh->boneMatrixPointers[dwMatrixIndex]);
				}
			}

			// set the matrix palette into the effect
			effect->SetMatrixArray("amPalette", workingPalette, boneMesh->numPaletteEntries);

			//effect->SetMatrix("g_mWorld", &world);
			
			//effect->SetMatrix("g_mViewProj", &matViewProj);
			effect->SetMatrix("g_mViewProj", &LightViewProjection);

			effect->SetVector("vLightDiffuse", &D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
			effect->SetVector("vWorldLightPos", &D3DXVECTOR4(500.0f, 500.0f, 500.0f, 1.0f));
			effect->SetVector("vWorldCameraPos", &D3DXVECTOR4(vEye, 1.0f));
			effect->SetVector("vMaterialAmbient", &D3DXVECTOR4(0.53f, 0.53f, 0.53f, 0.53f));
			effect->SetVector("vMaterialDiffuse", &D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));

			// we're pretty much ignoring the materials we got from the x-file; just set
			// the texture here
			effect->SetTexture("g_txScene", boneMesh->textures[pBoneCombos[dwAttrib].AttribId]);

			// set the current number of bones; this tells the effect which shader to use
			effect->SetInt("CurNumBones", boneMesh->maxNumFaceInfls - 1);

			// set the technique we use to draw
			effect->SetTechnique("Skinning20");

			UINT uiPasses, uiPass;

			// run through each pass and draw
			effect->Begin(&uiPasses, 0);
			for (uiPass = 0; uiPass < uiPasses; ++uiPass)
			{
				effect->BeginPass(uiPass);
				boneMesh->workingMesh->DrawSubset(dwAttrib);
				effect->EndPass();
			}
			effect->End();
		}
	}

	if (current->pFrameSibling) RenderShadow((Bone*)current->pFrameSibling);
	if (current->pFrameFirstChild) RenderShadow((Bone*)current->pFrameFirstChild);
}

void SkinnedMesh::SetAnimationIndex(int index)
{
	//애니메이션 변경
	if (!animController)
		return;

	SAFE_RELEASE(animSet);
	animController->ResetTime();
	currentAnim = index;
	animController->GetAnimationSet(index, &animSet);
	animController->SetTrackAnimationSet(0, animSet);
	animController->SetTrackPosition(0, 0);
}

void SkinnedMesh::SetRandomTrackPosition()
{
	//랜덤으로  애니메이션 시작점 조절
	animController->SetTrackPosition(0, (rand() % 100) / 10.0f);
}

void SkinnedMesh::Load(char* path, char* fileName)
{
	effect = LoadEffect("../ResourceData/Shader/MultiAnimation.hpp");

	int paletteSize = 0;
	effect->GetInt("MATRIX_PALETTE_SIZE", &paletteSize);

	AllocateHierarchy ah;
	ah.SetDirectory(path);
	ah.SetDefaultPaletteSize(paletteSize);

	boundingSphere.center = (ah.GetMin() + ah.GetMax()) / 2.0f;
	boundingSphere.radius = D3DXVec3Length(&(ah.GetMin() - ah.GetMax()));

	char fullPath[512];
	strcpy_s(fullPath, path);
	strcat_s(fullPath, fileName);
	HRESULT result = D3DXLoadMeshHierarchyFromXA(
		fullPath,
		D3DXMESH_MANAGED,
		GameManager::GetDevice(),
		&ah,
		NULL,
		(LPD3DXFRAME*)&rootFrame,
		&animController);

	if (workingPalette)
		delete[] workingPalette;

	workingPaletteSize = ah.GetMaxPaletteSize();
	workingPalette = new D3DXMATRIX[workingPaletteSize];
	if (workingPalette == NULL)
	{
		workingPaletteSize = 0;
	}

	if (rootFrame)
		SetupBoneMatrixPointers(rootFrame);
}

LPD3DXEFFECT SkinnedMesh::LoadEffect(char* fileName)
{
	LPD3DXEFFECT pEffect = NULL;

	D3DXMACRO mac[2] =
	{
		{ "MATRIX_PALETTE_SIZE_DEFAULT", "35" },
		{ NULL, NULL }
	};

	D3DCAPS9 caps;
	D3DXMACRO *pmac = NULL;
	GameManager::GetDevice()->GetDeviceCaps(&caps);
	if (caps.VertexShaderVersion > D3DVS_VERSION(1, 1))
		pmac = mac;

	DWORD dwShaderFlags = 0;

#if defined( DEBUG ) || defined( _DEBUG )
	// Set the D3DXSHADER_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DXSHADER_DEBUG;
#endif

#ifdef DEBUG_VS
	dwShaderFlags |= D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT;
#endif
#ifdef DEBUG_PS
	dwShaderFlags |= D3DXSHADER_FORCE_PS_SOFTWARE_NOOPT;
#endif

	ID3DXBuffer* pBuffer = NULL;
	if (FAILED(D3DXCreateEffectFromFileA(GameManager::GetDevice(),
		fileName,
		pmac,
		NULL,
		dwShaderFlags,
		NULL,
		&pEffect,
		&pBuffer)))
	{
		// if creation fails, and debug information has been returned, output debug info
		if (pBuffer)
		{
			OutputDebugStringA((char*)pBuffer->GetBufferPointer());
			SAFE_RELEASE(pBuffer);
		}

		return NULL;
	}

	return pEffect;
}

void SkinnedMesh::BoxMake()
{
	D3DXVECTOR3 tempMax(shoulderR+ 0.5f, heightMax, shoulderR + 0.5f);
	D3DXVECTOR3 tempMin(shoulderL - 0.5f, heightMin, shoulderL - 0.5f);
	colliderBox->Initialize(tempMin, tempMax, &world);
	float width =( shoulderR+0.5f) - (shoulderL- 0.5f);
	boundingBox.min = D3DXVECTOR3(position.x - width *0.5f, heightMin, position.z - width *0.5f);
	boundingBox.max = D3DXVECTOR3(position.x + width *0.5f, heightMax, position.z + width *0.5f);
	boundingBox.bounds[1] = D3DXVECTOR3(position.x + width *0.5f, heightMax, position.z + width *0.5f);
	boundingBox.bounds[0] = D3DXVECTOR3(position.x - width *0.5f, heightMin, position.z - width *0.5f);

	D3DXMATRIXA16 translation;
	D3DXMatrixTranslation(&translation, position.x, position.y, position.z);
	colliderBox->SetParent(&translation);
	colliderBox->Update();

	colliderBox->SetMaterialColor(D3DXCOLOR(0.f, 0.f, 0.8f, 1.0f));
	if (CharacData::GetIsBoxRend())
	{
		colliderBox->Render();
	}
}

void SkinnedMesh::BoxUpdate(Bone * current)
{
	if (current->CombinedTransformationMatrix._42 > heightMax)
	{
		heightMax = current->CombinedTransformationMatrix._42;
		MaxName = current->Name;
	}
	if (current->CombinedTransformationMatrix._42 < heightMin)
	{
		MinName = current->Name;
		heightMin = current->CombinedTransformationMatrix._42;
	}
}

void SkinnedMesh::RayMake()
{
//	AttackRay.origin = bipSpot;
//	AttackRay.direction = footSpot;
//	AttackRay.AttackDistance = D3DXVec3Length(&(bipSpot - footSpot));
//	D3DXVec3Normalize(&AttackRay.direction, &AttackRay.direction);
}

bool SkinnedMesh::CheckAnimation(char * name)
{
	if (strcmp(name, aniName) == 0) return true;
	else return false;
}

void SkinnedMesh::SetupBoneMatrixPointers(Bone* bone)
{
	assert(bone);
	// 각 프레임의 메시 컨테이너에 있는 pSkinInfo를 이용하여 영향받는 모든 
	// 프레임의 매트릭스를 boneMatrixPointers에 연결한다.
	if (bone->pMeshContainer)
	{
		BoneMesh* boneMesh = (BoneMesh*)bone->pMeshContainer;
		if (boneMesh->pSkinInfo)
		{
			LPD3DXSKININFO pSkinInfo = boneMesh->pSkinInfo;
			// pSkinInfo->GetNumBones() 으로 영향받는 본의 개수를 찾음.
			// pSkinInfo->GetBoneName(i) 로 i번 프레임의 이름을 찾음
			// D3DXFrameFind(루트 프레임, 프레임 이름) 로 프레임을 찾음.
			// 찾아서 월드매트릭스를 걸어줘라.
			DWORD dwNumBones = pSkinInfo->GetNumBones();
			for (DWORD i = 0; i < dwNumBones; ++i)
			{
				LPCSTR boneName = pSkinInfo->GetBoneName(i);
				if (boneName == nullptr || strlen(boneName) == 0)
					continue;
				Bone* influence = (Bone*)D3DXFrameFind(rootFrame, boneName);
				boneMesh->boneMatrixPointers[i] = &(influence->CombinedTransformationMatrix);
			}
		}
	}

	//재귀적으로 모든 프레임에 대해서 실행.
	if (bone->pFrameSibling)
	{
		
		SetupBoneMatrixPointers((Bone*)bone->pFrameSibling);
	}

	if (bone->pFrameFirstChild)
	{
		SetupBoneMatrixPointers((Bone*)bone->pFrameFirstChild);
	}
}

