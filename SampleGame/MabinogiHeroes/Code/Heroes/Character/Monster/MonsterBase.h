#pragma once
#include "../../Framework/Mesh/SkinnedMesh.h"
class MonsterBase : public SkinnedMesh
{
public:
	MonsterBase();
	~MonsterBase();

	virtual void UpdateAndRender() override;
	void StateControl(); //현재 스테이트에 따라 행동을 다르게

	virtual D3DXVECTOR3& GetPosition() { return position; }
	virtual D3DXVECTOR3* GetPositionAddress() { return &position; }
	virtual void RenderShadow();
	virtual inline void SetOriginPosition(D3DXVECTOR3 v) {  originPosition = v;}
	void Control();

	inline void SetIsLive(bool x) { isLive = x; }

public: //상황에 따른 함수들
	virtual void StateDmg();
	virtual void StateDie();
	virtual void StateAtk();
	virtual void StateRest();
	virtual void StateStop();
	virtual void StateWalk();

	virtual void rageCheck(); //ai반응 거리 체크
	virtual void CollisionCheck(); //충돌 체크

protected:
	int monsterInfo[CH_INFOMATION::CH_END];
	float pause = 0; //순간 경직을 나타내기 위한것.

	float aiRage = 10.0f; //ai반응 거리
	float aiAttackRage = 5.0f; //ai공격 반응 거리

	bool isAtk = false;//공격했는가?
	bool isToPlayer = false; //플레이어 쪽인가 집쪽인가?

	float dieTime = 0;

	D3DXVECTOR3 originPosition; //원래위치

	bool isLive = true;
};

