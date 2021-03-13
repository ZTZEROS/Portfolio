#pragma once
#include "../../Framework/Mesh/SkinnedMesh.h"
class MonsterBase : public SkinnedMesh
{
public:
	MonsterBase();
	~MonsterBase();

	virtual void UpdateAndRender() override;
	void StateControl(); //���� ������Ʈ�� ���� �ൿ�� �ٸ���

	virtual D3DXVECTOR3& GetPosition() { return position; }
	virtual D3DXVECTOR3* GetPositionAddress() { return &position; }
	virtual void RenderShadow();
	virtual inline void SetOriginPosition(D3DXVECTOR3 v) {  originPosition = v;}
	void Control();

	inline void SetIsLive(bool x) { isLive = x; }

public: //��Ȳ�� ���� �Լ���
	virtual void StateDmg();
	virtual void StateDie();
	virtual void StateAtk();
	virtual void StateRest();
	virtual void StateStop();
	virtual void StateWalk();

	virtual void rageCheck(); //ai���� �Ÿ� üũ
	virtual void CollisionCheck(); //�浹 üũ

protected:
	int monsterInfo[CH_INFOMATION::CH_END];
	float pause = 0; //���� ������ ��Ÿ���� ���Ѱ�.

	float aiRage = 10.0f; //ai���� �Ÿ�
	float aiAttackRage = 5.0f; //ai���� ���� �Ÿ�

	bool isAtk = false;//�����ߴ°�?
	bool isToPlayer = false; //�÷��̾� ���ΰ� �����ΰ�?

	float dieTime = 0;

	D3DXVECTOR3 originPosition; //������ġ

	bool isLive = true;
};

