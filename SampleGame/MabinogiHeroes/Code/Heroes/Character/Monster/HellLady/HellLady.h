#pragma once
#include "../MonsterBase.h"

class HellLady : public MonsterBase
{
public:
	HellLady();
	~HellLady();

	virtual void InitXFile() override;
	virtual void RayMake() override;
	virtual void BoxUpdate(Bone* current) override;

	D3DXVECTOR3& GetPosition() { return position; }
	D3DXVECTOR3* GetPositionAddress() { return &position; }

	void Control();


public: //��Ȳ�� ���� �Լ���
	virtual void StateDmg()override;
	virtual void StateDie()override;
	virtual void StateAtk()override;
	virtual void StateRest()override;
	virtual void StateStop()override;
	virtual void StateWalk()override;

private:
	int hellLadyInfo[CH_INFOMATION::CH_END];
	float pause = 0; //���� ������ ��Ÿ���� ���Ѱ�.

};

