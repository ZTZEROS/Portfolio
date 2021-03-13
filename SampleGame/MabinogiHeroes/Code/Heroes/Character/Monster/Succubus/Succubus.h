#pragma once
#include "../MonsterBase.h"
#include "../../../Bullet.h"

enum ATTACK_PATTERN
{
	PATTERN_BULLET,
	PATTERN_DASH,
	PATTERN_SPIN,
	PATTERN_END,
};

class Succubus :
	public MonsterBase
{
public:
	Succubus();
	virtual ~Succubus();

	virtual void Destroy() override;

	virtual void InitXFile() override;
	virtual void RayMake() override;
	virtual void BoxUpdate(Bone* current) override;
	virtual void UpdateAndRender() override;

	D3DXVECTOR3& GetPosition() { return position; }
	D3DXVECTOR3* GetPositionAddress() { return &position; }
	
	void Control();


public: //상황에 따른 함수들
	virtual void StateDmg()override;
	virtual void StateDie()override;
	virtual void StateAtk()override;
	virtual void StateRest()override;
	virtual void StateStop()override;
	virtual void StateWalk()override;
	virtual void CollisionCheck() override; //충돌 체크
	virtual void rageCheck() override;

private:
	ATTACK_PATTERN attackPattern;

	float restTime = 3.0f; //공격후 쉬는 시간
	float intanceTime = 0; //공격후 재는 시간

	D3DXVECTOR3 DashStart = D3DXVECTOR3(0, 0, 0);
	D3DXVECTOR3 DashEnd = D3DXVECTOR3(0, 0, 0);

	void BulletPattern();
	bool isReach = false; //광선패턴 위치에 도달 하였는가?

	void DashPattern();
	void SpinPattern();

	void BulletControl();
	void BulletRender();
	void BulletCollisionCheck();

	std::vector<Bullet*> vBullet;

	LPDIRECT3DTEXTURE9 dark;
	float alpha = 0;

};

