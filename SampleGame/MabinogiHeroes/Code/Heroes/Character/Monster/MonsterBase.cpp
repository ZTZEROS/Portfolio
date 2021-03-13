#include "stdafx.h"
#include "MonsterBase.h"


MonsterBase::MonsterBase()
{
}


MonsterBase::~MonsterBase()
{
}

void MonsterBase::UpdateAndRender()
{
	if (isLive)
	{
		rageCheck();
		CollisionCheck();
		StateControl();
		Control();
		SkinnedMesh::UpdateAndRender();

		char text2[256];
		RECT rc = { 100, RESOLUTION_Y - 55, 11, 11 };
		sprintf_s(text2, "%d", monsterInfo[CH_INFOMATION::CH_HP]);
		UI_Manager::GetItemFont()->DrawTextA(NULL, text2, strlen(text2), &rc, DT_TOP | DT_LEFT | DT_NOCLIP, D3DCOLOR_XRGB(255, 255, 255));
	}
}

void MonsterBase::StateControl()
{
	if (currentState == STATE_DIE)
	{
		StateDie();
		return;
	}
	if (currentState != STATE_DMG)
	{
		if (Collision::IsBoxToBox(*CharacData::GetLethita()->GetBoundingBox(), boundingBox))
		{
			CharacData::GetLethita()->SetIsMoveAble((int)CharacData::GetLethita()->GettempKey());
		}
	}

	else
	{
		if (Collision::IsBoxToBox(*CharacData::GetLethita()->GetBoundingBox(), boundingBox))
		{
			if (CharacData::GetLethita()->GetCurrentState() == CHARIC_STATE::STATE_ATK)
				position += (CharacData::GetLethita()->GetDirection() * moveSpeed * gameTick);
			else CharacData::GetLethita()->SetIsMoveAble((int)CharacData::GetLethita()->GettempKey());
		}
	}

	switch (currentState)
	{
	case CHARIC_STATE::STATE_ATK:
		StateAtk();
		break;

	case CHARIC_STATE::STATE_DIE:
		StateDie();
		break;

	case CHARIC_STATE::STATE_DMG:
		StateDmg();
		break;

	case CHARIC_STATE::STATE_REST:
		StateRest();
		break;

	case CHARIC_STATE::STATE_STOP:
		StateStop();
		break;

	case CHARIC_STATE::STATE_WALK:
		StateWalk();
		break;
	}
}

void MonsterBase::Control()
{
}

void MonsterBase::StateDmg()
{
	pause += gameTick;
	if (pause > gameTick * 5)
	{
		CharacData::SetGameSpeed(2.0f);
	}
	Ray ray = CharacData::GetLethita()->GetAttackRay();
	if (!ray.IsPicked(this->GetBoundingBox())) return;
	if (CharacData::GetLethita()->isHit == false)
	{
		return;
	}
	pause = 0;
	monsterInfo[CH_INFOMATION::CH_HP] -= CharacData::GetEqipInfo(CH_ATK);
	CharacData::SetGameSpeed(0);
	SoundManager::Get().play("Hit_bolt_01", 0.7f);
}

void MonsterBase::StateDie()
{
}

void MonsterBase::StateAtk()
{
	if (CharacData::GetLethita()->isDamg == true) return;
	if (AttackRay.IsPicked(CharacData::GetLethita()->GetBoundingBox()))
	{
		if (isAtk == true)
		{
			CharacData::SetInfomation(CH_INFOMATION::CH_HP, -monsterInfo[CH_INFOMATION::CH_ATK]);
			SoundManager::Get().play("Hit_bolt_01", 0.7f);
			CharacData::GetLethita()->SetCurrentState(CHARIC_STATE::STATE_DMG);
		}
	}
}

void MonsterBase::StateRest()
{
	D3DXVECTOR3 userP;
	D3DXVECTOR3 goal;
	float dot;
	float radian;
	userP = originPosition;
	userP.y = position.y;
	goal = userP - position;

	if (abs(goal.x) < 0.1 && abs(goal.z) < 0.1) { isToPlayer = false; return; }

	D3DXVec3Normalize(&goal, &goal);
	position += (goal * (moveSpeed - 5.0f) * gameTick);

	D3DXVECTOR3 temp;
	D3DXVec3Normalize(&temp, &baseDirection);

	dot = D3DXVec3Dot(&D3DXVECTOR3(0, 0, -1), &goal);
	radian = (float)acos(dot);

	////캐릭터가 우측인지 좌측인지
	D3DXVECTOR3 right;
	D3DXVec3Cross(&right, &baseDirection, &D3DXVECTOR3(0, 1, 0));
	if (D3DXVec3Dot(&right, &goal) > 0) radian;
	else radian = -radian;

	rotationWatchAngle = radian;
}

void MonsterBase::StateStop()
{
}

void MonsterBase::StateWalk()
{
	D3DXVECTOR3 userP;
	D3DXVECTOR3 goal;
	float dot;
	float radian;
	if (isToPlayer) userP = CharacData::GetLethita()->GetPosition();
	else
	{
		userP = originPosition;
		userP.y = position.y;
		if (position == originPosition ) currentState == STATE_REST; return;
	}
	goal = userP - position;
	D3DXVec3Normalize(&goal, &goal);
	position += (goal * (moveSpeed - 5.0f) * gameTick);

	D3DXVECTOR3 temp;
	D3DXVec3Normalize(&temp, &baseDirection);

	dot = D3DXVec3Dot(&D3DXVECTOR3(0, 0, -1), &goal);
	radian = (float)acos(dot);

	////캐릭터가 우측인지 좌측인지
	D3DXVECTOR3 right;
	D3DXVec3Cross(&right, &baseDirection, &D3DXVECTOR3(0, 1, 0));
	if (D3DXVec3Dot(&right, &goal) > 0) radian;
	else radian = -radian;

	rotationWatchAngle = radian;
}

void MonsterBase::rageCheck()
{
	if (monsterInfo[CH_INFOMATION::CH_HP] < 1)
	{
		currentState = CHARIC_STATE::STATE_DIE;
		return;
	}
	if (currentState == CHARIC_STATE::STATE_DMG || currentState == CHARIC_STATE::STATE_DIE) return;
	D3DXVECTOR3 userP = CharacData::GetLethita()->GetPosition();
	D3DXVECTOR3 goal = userP - position;
	float rage = D3DXVec3Length(&goal);
	if (rage < aiRage && rage > aiAttackRage)
	{
		currentState = CHARIC_STATE::STATE_WALK;
		isToPlayer = true;
	}
	else if (rage < aiAttackRage && (currentState == STATE_REST || currentState == STATE_WALK))
	{
		currentState = CHARIC_STATE::STATE_ATK;
	}
	else if(rage > aiRage)
	{
		currentState = CHARIC_STATE::STATE_REST;
	}
}

void MonsterBase::CollisionCheck()
{
	if (CharacData::GetLethita()->GetCurrentState() != CHARIC_STATE::STATE_ATK || currentState == STATE_DIE) return;
	Ray ray = CharacData::GetLethita()->GetAttackRay();
	if (ray.IsPicked(this->GetBoundingBox()))
	{
		this->SetCurrentState(CHARIC_STATE::STATE_DMG);
	}
}

void MonsterBase::RenderShadow()
{
	SkinnedMesh::RenderShadow(rootFrame);
}