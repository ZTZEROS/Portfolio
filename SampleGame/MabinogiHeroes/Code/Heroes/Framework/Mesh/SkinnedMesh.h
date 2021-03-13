#pragma once
#include "AllocateHierarchy.h"
#include "../Collision/Collider_Box.h"
#include "../Util/Ray.h"

enum CHARIC_STATE
{
	STATE_STOP,
	STATE_WALK,
	STATE_RUN,
	STATE_ATK,
	STATE_DMG,
	STATE_DIE,
	STATE_REST,
	STATE_DELETE,
	STATE_POTION,
};

class SkinnedMesh 
{
	//friend class SkinnedMeshManager;	// 이렇게 하면 SkinnedMeshManager는 SkinnedMesh의 private도 마음대로 사용 할 수 있게 된다.

public:
	SkinnedMesh();
	~SkinnedMesh();

	virtual void InitXFile(); //각자 X파일 이닛.
	virtual void Initialize(char* path, char* fileName);	// 원하는 파일을 클로닝 하는 함수
	virtual void Destroy();
	virtual void UpdateAndRender();
	virtual void RenderShadow(Bone* current);

	void SetWorldLightPos(D3DXVECTOR4 worldLightPos) { WorldLightPos = worldLightPos; }
	void SetLightViewProjection(D3DXMATRIXA16& vp) { LightViewProjection = vp; }
	virtual void SetAnimationIndex(int index);	// 재생 될 애니메이션 세팅
	virtual void SetRandomTrackPosition();		// 테스트용(애니메이션 재생 위치를 랜덤하게 세팅)
	virtual inline void SetPosition(D3DXVECTOR3 v) { position = v; boundingSphere.center = v; }
	virtual inline void SetScale(float scl) { scale = scl; }
	inline void SetCurrentState(CHARIC_STATE x) { currentState = x; }
	inline void SetMoveSpeed(float x) { moveSpeed = x; }
	inline void SetIsPlay(bool x) { isPlay = x; }

	virtual inline Bone* GetRootFrame() { return rootFrame; }
	inline D3DXVECTOR3& GetPosition() { return position; }
	inline D3DXVECTOR3* GetPositionAddress() { return &position; }
	virtual inline D3DXMATRIXA16 GetWorld() { return world; }
	virtual inline BoundingSphere* GetBoundingSphere() { return &boundingSphere; }
	virtual inline BoundingBox* GetBoundingBox() { return &boundingBox; }
	virtual inline Ray GetAttackRay() { return AttackRay; }
	inline CHARIC_STATE GetCurrentState() { return currentState; }

	 bool CheckAnimation(char* name);
	 virtual inline bool IsCurrentAniEnd() { return animController->GetTime() > animSet->GetPeriod() - 0.07f; } //현재 애니메이션 경과시간이 총 길이를 넘었는가?

	virtual void Update(Bone* current, D3DXMATRIXA16* parentMatrix);
	virtual void Load(char* path, char* fileName);	//실제 X파일 부르는 함수. SkinnedMeshManager안에서만 사용 할 것

protected:
	// 하나만 생성(원본과 공유)
	SkinnedMesh* movement = nullptr;
	Bone* rootFrame = nullptr;
	DWORD workingPaletteSize = 0;
	D3DXMATRIX* workingPalette = nullptr;
	LPD3DXEFFECT effect = nullptr;
	LPD3DXEFFECT effect2 = nullptr;


	BoundingSphere boundingSphere = BoundingSphere();

	// 객체마다 가지고 있어야 할 것
	LPD3DXANIMATIONCONTROLLER animController = nullptr;

	D3DXMATRIXA16 world;
	D3DXMATRIXA16 scaleXYZ;
	D3DXMATRIXA16 rotationX;
	D3DXMATRIXA16 rotationY;
	D3DXMATRIXA16 translation;

	D3DXMATRIXA16 rotationWatch;

	D3DXVECTOR3 position = D3DXVECTOR3(0, 0, 0);
	D3DXVECTOR3 direction = D3DXVECTOR3(0, 0, 1);//움직이는 방향
	D3DXVECTOR3 baseDirection = D3DXVECTOR3(0, 0, 1);
	float scale = 1.0f;
	float rotationAngle = 0.0f;
	float rotationWatchAngle = 0.0f;
	float rotationSpeed = 1.0f;
	float moveSpeed = 10.0f;
	float animationSpeed = 2.0f;

	// 클론 된건지 원본인지 표시용
	bool isClone = false;

	// 기타 내부적으로 필요한 함수들
	virtual void Render(Bone* bone);
	virtual void SetupBoneMatrixPointers(Bone* bone);	//본 초기화

	virtual LPD3DXEFFECT LoadEffect(char* fileName);	//이펙트 부르는 함수. 자세한건 쉐이더때

	//현재 애니메이션 정보들
	LPD3DXANIMATIONSET animSet = nullptr; //현재 애니메이션 정보
	int currentAnim = 0; // 지금 애니메이션 넘버
	float animPeriod = 0;//애니메이션 총 기간

	float gameTick = 0;//게임 틱

	//알파값
	float alpha = 1.0f;
	bool isPlay = true;

	//
	CHARIC_STATE currentState = STATE_REST;

	//바운딩 박스를 만들기 위한 영역
	BoundingBox boundingBox;
	Collider_Box* colliderBox = nullptr;
	LPCSTR MaxName = nullptr;
	LPCSTR MinName = nullptr;
	float heightMax = -10000000.f;
	float heightMin = 10000000.f;
	float shoulderR = 0;
	float shoulderL =0; //좌우 어깨값
	bool isFindWidthR = false;
	bool isFindWidthL = false;
	bool isRender = true;//바운딩 박스를 렌더 할 것이냐>
	virtual void BoxMake();
	virtual void BoxUpdate(Bone* current);

	//공격용
	D3DXVECTOR3 attackEnd = D3DXVECTOR3(0,0,0); //끝날점 
	D3DXVECTOR3 attackStart = D3DXVECTOR3(0, 0, 0); //시작점
	Ray AttackRay;
	bool isDistanceCheck = false;
	virtual void RayMake();

	//애니메이션 체크용
	char aniName[256];

	//

	D3DXMATRIXA16 LightViewProjection;
	D3DXVECTOR4 WorldLightPos = D3DXVECTOR4(500.0f, 500.0f, 500.0f, 1.0f);
};

