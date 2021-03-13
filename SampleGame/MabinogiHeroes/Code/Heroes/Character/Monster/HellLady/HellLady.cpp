#include "stdafx.h"
#include "HellLady.h"


HellLady::HellLady()
{
}


HellLady::~HellLady()
{
}

void HellLady::InitXFile()
{
	this->Initialize("./HellLady/", "HellLady_Run.X");
	this->Initialize("./HellLady/", "HellLadyDie.X");
	this->Initialize("./HellLady/", "HellLadyFinch.X");
	this->Initialize("./HellLady/", "HellLadyKick.X");
	this->Initialize("./HellLady/", "HellLadyLaunchBegin.X");
	this->Initialize("./HellLady/", "HellLadyLaunchDuring.X");
	this->Initialize("./HellLady/", "HellLadyLaunchEnd.X");
	this->Initialize("./HellLady/", "HellLadyWalk.X");
	this->Initialize("./HellLady/", "HellLadyRest.X");

	monsterInfo[CH_INFOMATION::CH_ATK] = 10;
	monsterInfo[CH_INFOMATION::CH_DEF] = 10;
	monsterInfo[CH_INFOMATION::CH_HP] = 100;
}

void HellLady::RayMake()
{
	if (currentState != CHARIC_STATE::STATE_ATK) return;
	AttackRay.origin = attackStart;
	AttackRay.direction = -direction;
	AttackRay.AttackDistance = D3DXVec3Length(&(attackStart - attackEnd));
	D3DXVec3Normalize(&AttackRay.direction, &AttackRay.direction);
}

void HellLady::BoxUpdate(Bone* current)
{
	SkinnedMesh::BoxUpdate(current);
	char* temp = (LPSTR)(LPCSTR)current->Name;

	if (temp != NULL &&strcmp(temp, "ValveBiped_Bip01_R_Shoulder01") == 0 && !isFindWidthL)
	{
		if (current->CombinedTransformationMatrix._41 != 0)
		{
			shoulderL = current->CombinedTransformationMatrix._41;
			isFindWidthL = true;
		}
	}
	if (temp != NULL &&strcmp(temp, "ValveBiped_Bip01_L_Shoulder01") == 0 && !isFindWidthR)
	{
		if (current->CombinedTransformationMatrix._41 != 0)
		{
			shoulderR = current->CombinedTransformationMatrix._41;
			isFindWidthR = true;
		}
	}

	if (currentState != CHARIC_STATE::STATE_ATK) return;
	if (temp != NULL &&strcmp(temp, "ValveBiped_Bip01") == 0)
	{
		attackStart = D3DXVECTOR3(current->CombinedTransformationMatrix._41, current->CombinedTransformationMatrix._42, current->CombinedTransformationMatrix._43);
	}

	if (temp != NULL &&strcmp(temp, "ValveBiped_Bip01_R_Foot") == 0)
	{
		attackEnd = D3DXVECTOR3(current->CombinedTransformationMatrix._41, current->CombinedTransformationMatrix._42, current->CombinedTransformationMatrix._43);
	}

}

void HellLady::Control()
{
	
}

void HellLady::StateDmg()
{
	MonsterBase::StateDmg();

	/*if (!CheckAnimation("HellLadyFinch.X"))
	{
		this->Initialize("./HellLady/", "HellLadyFinch.X");
	}
	else*/ 
	if (IsCurrentAniEnd() && CheckAnimation("HellLadyFinch.X"))
	{
		SetCurrentState(STATE_REST);
		CharacData::SetGameSpeed(2.0f);
	}
	Ray ray = CharacData::GetLethita()->GetAttackRay();
	if (!ray.IsPicked(this->GetBoundingBox())) return;
	if (CharacData::GetLethita()->isHit)
	{
		SoundManager::Get().play("lady_hurt_strong_03", 0.8f);
		this->Initialize("./HellLady/", "HellLadyFinch.X");
	}
	CharacData::GetLethita()->isHit = false;
}

void HellLady::StateDie()
{
	if (!CheckAnimation("HellLadyDie.X"))
	{
		SoundManager::Get().play("lady_dying_04", 0.8f);
		SetCurrentState(STATE_REST);
		CharacData::SetGameSpeed(2.0f);
		this->Initialize("./HellLady/", "HellLadyDie.X");
	}
	if (IsCurrentAniEnd())
	{
		CharacData::SetInfomation(CH_EXP, 1);
		isPlay = false;
		currentState = STATE_DELETE;
	}
}

void HellLady::StateAtk()
{
	MonsterBase::StateAtk();
	if (!CheckAnimation("HellLadyKick.X"))
	{
		SoundManager::Get().play("lady_shout_01", 0.8f);
		this->Initialize("./HellLady/", "HellLadyKick.X");
		isAtk = true;
	}
	else if (IsCurrentAniEnd())
	{
		this->Initialize("./HellLady/", "HellLadyRest.X");
	}
	else
	{
		isAtk = false;
	}
}

void HellLady::StateRest()
{
	MonsterBase::StateRest();

	if (isToPlayer)
	{
		if (!CheckAnimation("HellLadyWalk.X"))
		{
			this->Initialize("./HellLady/", "HellLadyWalk.X");
		}
	}
	else
	{
		if (!CheckAnimation("HellLadyRest.X"))
		{
			this->Initialize("./HellLady/", "HellLadyRest.X");
		}
	}
	
}

void HellLady::StateStop()
{
}

void HellLady::StateWalk()
{
	MonsterBase::StateWalk();
	if (!CheckAnimation("HellLadyWalk.X"))
	{
		this->Initialize("./HellLady/", "HellLadyWalk.X");
	}
}
