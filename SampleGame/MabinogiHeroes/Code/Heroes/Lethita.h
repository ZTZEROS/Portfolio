#pragma once
#include "../Framework/Mesh/SkinnedMesh.h"
#include "ItemMesh.h"
#include "../WindBlade.h"

class Lethita : public SkinnedMesh
{
public:
	Lethita();
	~Lethita();
	
	void InitXFile();//애니메이션 정보 넣기
	virtual void UpdateAndRender() override;
	void RenderShadow();
	void DungeounMoveControl();
	void TownMoveControl();
	void AttackControl();
	void ArrowKeyControl();
	void ItemEquip(Item_Kind kind, char* name); //아이템 장착하기
	void BoxUpdate(Bone* current) override;
	void RayMake() override;

	inline ItemMesh** GetEquiItem() { return &imtemMesh[0]; }
	inline D3DXVECTOR3 GetDirection() { return direction; }
	inline int GettempKey() { return tempKey; }

	inline void SetIsMoveAble(int x) { isMoveAble[x] = false; }

public://상황에 따른 함수들
	void StateDmg();
	void StateDie();
	void StateAtk();
	void StateRest();
	void StateStop();
	void StateWalk();
	void StatePotion();

	bool isHit = false; //공격에 성공 하였는가?
	bool isDamg = false; //데미지 받았는가?
	bool isDash = false;//데쉬 중인가?

private:
	static const int MAXATTACK = 5;
	int stackAttack = 0;
	bool isNextAttack = false;
	bool isAttackAble = true; //공격 가능한 상태인가?
	enum KEYBOARD_DIRECTION
	{
		DIRECTION_UP = 10,
		DIRECTION_DOWN = 20,
		DIRECTION_RIGHT = 100,
		DIRECTION_LEFT = 200,
		DIRECTION_UPRIGHT = 110,
		DIRECTION_UPLEFT = 210,
		DIRECTION_DOWNRIGHT = 120,
		DIRECTION_DOWNLEFT = 220,
		DIRECTION_DEFAULT= 300,
		DIRECTION_END,
	};

	ItemMesh* imtemMesh[Item_End];
	bool isMoveAble[DIRECTION_END]; //움직임이 가능한가?
	int tempKey = 0; //움직일 방향값
	char* attackName = nullptr; //공격 부위 이름
	char* attackName2 = nullptr; //공격 부위 이름

	D3DXVECTOR3 attackStart2;

	WindBlade* atkShow = nullptr;
	WindBlade* atkShow2 = nullptr;
	float atkShowTime = 0;
	
	bool isBoxRend = false;
	bool isLive = true;
};

