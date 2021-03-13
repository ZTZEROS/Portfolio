#pragma once
#include "../Framework/Mesh/SkinnedMesh.h"
#include "ItemMesh.h"
#include "../WindBlade.h"

class Lethita : public SkinnedMesh
{
public:
	Lethita();
	~Lethita();
	
	void InitXFile();//�ִϸ��̼� ���� �ֱ�
	virtual void UpdateAndRender() override;
	void RenderShadow();
	void DungeounMoveControl();
	void TownMoveControl();
	void AttackControl();
	void ArrowKeyControl();
	void ItemEquip(Item_Kind kind, char* name); //������ �����ϱ�
	void BoxUpdate(Bone* current) override;
	void RayMake() override;

	inline ItemMesh** GetEquiItem() { return &imtemMesh[0]; }
	inline D3DXVECTOR3 GetDirection() { return direction; }
	inline int GettempKey() { return tempKey; }

	inline void SetIsMoveAble(int x) { isMoveAble[x] = false; }

public://��Ȳ�� ���� �Լ���
	void StateDmg();
	void StateDie();
	void StateAtk();
	void StateRest();
	void StateStop();
	void StateWalk();
	void StatePotion();

	bool isHit = false; //���ݿ� ���� �Ͽ��°�?
	bool isDamg = false; //������ �޾Ҵ°�?
	bool isDash = false;//���� ���ΰ�?

private:
	static const int MAXATTACK = 5;
	int stackAttack = 0;
	bool isNextAttack = false;
	bool isAttackAble = true; //���� ������ �����ΰ�?
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
	bool isMoveAble[DIRECTION_END]; //�������� �����Ѱ�?
	int tempKey = 0; //������ ���Ⱚ
	char* attackName = nullptr; //���� ���� �̸�
	char* attackName2 = nullptr; //���� ���� �̸�

	D3DXVECTOR3 attackStart2;

	WindBlade* atkShow = nullptr;
	WindBlade* atkShow2 = nullptr;
	float atkShowTime = 0;
	
	bool isBoxRend = false;
	bool isLive = true;
};

