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
	//friend class SkinnedMeshManager;	// �̷��� �ϸ� SkinnedMeshManager�� SkinnedMesh�� private�� ������� ��� �� �� �ְ� �ȴ�.

public:
	SkinnedMesh();
	~SkinnedMesh();

	virtual void InitXFile(); //���� X���� �̴�.
	virtual void Initialize(char* path, char* fileName);	// ���ϴ� ������ Ŭ�δ� �ϴ� �Լ�
	virtual void Destroy();
	virtual void UpdateAndRender();
	virtual void RenderShadow(Bone* current);

	void SetWorldLightPos(D3DXVECTOR4 worldLightPos) { WorldLightPos = worldLightPos; }
	void SetLightViewProjection(D3DXMATRIXA16& vp) { LightViewProjection = vp; }
	virtual void SetAnimationIndex(int index);	// ��� �� �ִϸ��̼� ����
	virtual void SetRandomTrackPosition();		// �׽�Ʈ��(�ִϸ��̼� ��� ��ġ�� �����ϰ� ����)
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
	 virtual inline bool IsCurrentAniEnd() { return animController->GetTime() > animSet->GetPeriod() - 0.07f; } //���� �ִϸ��̼� ����ð��� �� ���̸� �Ѿ��°�?

	virtual void Update(Bone* current, D3DXMATRIXA16* parentMatrix);
	virtual void Load(char* path, char* fileName);	//���� X���� �θ��� �Լ�. SkinnedMeshManager�ȿ����� ��� �� ��

protected:
	// �ϳ��� ����(������ ����)
	SkinnedMesh* movement = nullptr;
	Bone* rootFrame = nullptr;
	DWORD workingPaletteSize = 0;
	D3DXMATRIX* workingPalette = nullptr;
	LPD3DXEFFECT effect = nullptr;
	LPD3DXEFFECT effect2 = nullptr;


	BoundingSphere boundingSphere = BoundingSphere();

	// ��ü���� ������ �־�� �� ��
	LPD3DXANIMATIONCONTROLLER animController = nullptr;

	D3DXMATRIXA16 world;
	D3DXMATRIXA16 scaleXYZ;
	D3DXMATRIXA16 rotationX;
	D3DXMATRIXA16 rotationY;
	D3DXMATRIXA16 translation;

	D3DXMATRIXA16 rotationWatch;

	D3DXVECTOR3 position = D3DXVECTOR3(0, 0, 0);
	D3DXVECTOR3 direction = D3DXVECTOR3(0, 0, 1);//�����̴� ����
	D3DXVECTOR3 baseDirection = D3DXVECTOR3(0, 0, 1);
	float scale = 1.0f;
	float rotationAngle = 0.0f;
	float rotationWatchAngle = 0.0f;
	float rotationSpeed = 1.0f;
	float moveSpeed = 10.0f;
	float animationSpeed = 2.0f;

	// Ŭ�� �Ȱ��� �������� ǥ�ÿ�
	bool isClone = false;

	// ��Ÿ ���������� �ʿ��� �Լ���
	virtual void Render(Bone* bone);
	virtual void SetupBoneMatrixPointers(Bone* bone);	//�� �ʱ�ȭ

	virtual LPD3DXEFFECT LoadEffect(char* fileName);	//����Ʈ �θ��� �Լ�. �ڼ��Ѱ� ���̴���

	//���� �ִϸ��̼� ������
	LPD3DXANIMATIONSET animSet = nullptr; //���� �ִϸ��̼� ����
	int currentAnim = 0; // ���� �ִϸ��̼� �ѹ�
	float animPeriod = 0;//�ִϸ��̼� �� �Ⱓ

	float gameTick = 0;//���� ƽ

	//���İ�
	float alpha = 1.0f;
	bool isPlay = true;

	//
	CHARIC_STATE currentState = STATE_REST;

	//�ٿ�� �ڽ��� ����� ���� ����
	BoundingBox boundingBox;
	Collider_Box* colliderBox = nullptr;
	LPCSTR MaxName = nullptr;
	LPCSTR MinName = nullptr;
	float heightMax = -10000000.f;
	float heightMin = 10000000.f;
	float shoulderR = 0;
	float shoulderL =0; //�¿� �����
	bool isFindWidthR = false;
	bool isFindWidthL = false;
	bool isRender = true;//�ٿ�� �ڽ��� ���� �� ���̳�>
	virtual void BoxMake();
	virtual void BoxUpdate(Bone* current);

	//���ݿ�
	D3DXVECTOR3 attackEnd = D3DXVECTOR3(0,0,0); //������ 
	D3DXVECTOR3 attackStart = D3DXVECTOR3(0, 0, 0); //������
	Ray AttackRay;
	bool isDistanceCheck = false;
	virtual void RayMake();

	//�ִϸ��̼� üũ��
	char aniName[256];

	//

	D3DXMATRIXA16 LightViewProjection;
	D3DXVECTOR4 WorldLightPos = D3DXVECTOR4(500.0f, 500.0f, 500.0f, 1.0f);
};

