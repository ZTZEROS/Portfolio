#include "stdafx.h"
#include "Succubus.h"


Succubus::Succubus()
{
}


Succubus::~Succubus()
{
	for (auto iter = vBullet.begin(); iter != vBullet.end(); )
	{
		(*iter)->Destroy();
		iter = vBullet.erase(iter);
	}
}

void Succubus::Destroy()
{
	MonsterBase::Destroy();
	
}

void Succubus::InitXFile()
{
	this->Initialize("./Succubus/", "SuccubusAttackBalletTurn.X");
	this->Initialize("./Succubus/", "SuccubusDamage.X");
	this->Initialize("./Succubus/", "SuccubusIdle.X");
	this->Initialize("./Succubus/", "SuccubusDie.X");
	this->Initialize("./Succubus/", "SuccubusSpawn.X");
	this->Initialize("./Succubus/", "SuccubusLowHealth.X");
	this->Initialize("./Succubus/", "SuccubusWalk.X");
	this->Initialize("./Succubus/", "SuccubusRun.X");
	this->Initialize("./Succubus/", "SuccubuDashAttack.X");
	this->Initialize("./Succubus/", "SuccubusThreat.X");

	monsterInfo[CH_INFOMATION::CH_ATK] = 10;
	monsterInfo[CH_INFOMATION::CH_DEF] = 10;
	monsterInfo[CH_INFOMATION::CH_HP] = 20;

	char path[254] = "./UI/";
	strcat_s(path, "dark.bmp");
	dark = TextureManager::GetTexture(path);
}


void Succubus::Control()
{
}

void Succubus::RayMake()
{
}

void Succubus::BoxUpdate(Bone * current)
{
	MonsterBase::BoxUpdate(current);
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

void Succubus::UpdateAndRender()
{
	MonsterBase::UpdateAndRender();
	
	D3DXMATRIXA16 tempWorld;
	D3DXMatrixIdentity(&tempWorld);

}

void Succubus::StateDmg()
{
	MonsterBase::StateDmg();

//		this->Initialize("./Succubus/", "SuccubusDamage.X");
	 if (IsCurrentAniEnd()&& CheckAnimation("SuccubusDamage.X"))
	{
		SetCurrentState(STATE_REST);	
		CharacData::SetGameSpeed(1.3f);
	}
	 Ray ray = CharacData::GetLethita()->GetAttackRay();
	 if (!ray.IsPicked(this->GetBoundingBox())) return;
	// if (CharacData::GetLethita()->isHit) this->Initialize("./Succubus/", "SuccubusDamage.X");
	 CharacData::GetLethita()->isHit = false;
	
}

void Succubus::StateDie()
{
	if(!CheckAnimation("SuccubusDie.X"))
	{
		SoundManager::Get().play("succubus_dying_02", 0.8f);
		this->Initialize("./Succubus/", "SuccubusDie.X");
		isPlay = false;
		CharacData::GetLethita()->SetIsPlay(false);
		dieTime = 0;
	}
	dieTime += gameTick;
	char text2[256];
	RECT rc = { 300, RESOLUTION_Y - 55, 301, 11 };
	sprintf_s(text2, "%f", dieTime);
	UI_Manager::GetItemFont()->DrawTextA(NULL, text2, strlen(text2), &rc, DT_TOP | DT_LEFT | DT_NOCLIP, D3DCOLOR_XRGB(255, 255, 255));
	
	RECT fpsRectangle = { 1200, 600, 1201, 601 };
	char textT[512] = "으앙 서큐버스 주금 ㅠㅠ ";
	UI_Manager::GetFont()->DrawTextA(NULL, textT, strlen(textT), &fpsRectangle, DT_TOP | DT_RIGHT | DT_NOCLIP, D3DCOLOR_XRGB(255, 255, 255));

	
	if ((int)dieTime > 6)
	{
		isPlay = true;
		CharacData::GetLethita()->SetIsPlay(true);
		GameManager::GetCamera()->SetRotateX(0);
		GameManager::GetCamera()->SetRotateX(0);
	}

	else if ((int)dieTime < (int)2)
	{
		GameManager::GetCamera()->SetRotateX(D3DX_PI / 4);
	}

	else if ((int)dieTime > (int)2 && (int)dieTime <4)
	{
		GameManager::GetCamera()->SetRotateY(D3DX_PI / 2);
	}
	else if ((int)dieTime > (int)4 && (int)dieTime < 6)
	{
		GameManager::GetCamera()->SetRotateY(D3DX_PI);
	}

	if (IsCurrentAniEnd())
	{
		CharacData::SetInfomation(CH_EXP, 10);
		isPlay = false;
		currentState = STATE_DELETE;
	}
}

void Succubus::StateAtk()
{
	MonsterBase::StateAtk();
	switch (attackPattern)
	{
	case ATTACK_PATTERN::PATTERN_BULLET:
		BulletPattern();
		break;

	case ATTACK_PATTERN::PATTERN_DASH:
		DashPattern();
		break;

	case ATTACK_PATTERN::PATTERN_SPIN:
		SpinPattern();
		break;
	}

}

void Succubus::StateRest()
{
	if (!CheckAnimation("SuccubusThreat.X"))
	{
		SoundManager::Get().play("succubus_laugh_long_01", 0.8f);
		CharacData::SetGameSpeed(2.0f);
		CharacData::GetLethita()->SetMoveSpeed(10.0f);
		this->Initialize("./Succubus/", "SuccubusThreat.X");
		D3DXVECTOR3 userP;
		D3DXVECTOR3 goal;
		float dot;
		float radian;
		userP = CharacData::GetLethita()->GetPosition();
		goal = userP - position;

		D3DXVec3Normalize(&goal, &goal);

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
	else
	{
		intanceTime += 0.01f;
	}

	if (intanceTime > restTime)
	{
		attackPattern = (ATTACK_PATTERN)RandomUtil::GetInteger(1, 0);
		currentState = STATE_ATK;
		intanceTime = 0;
	}

	BulletControl();

}

void Succubus::StateStop()
{
}


void Succubus::StateWalk()
{
	//MonsterBase::StateWalk();
	//if (!CheckAnimation("SuccubusWalk.X"))
	//{
	//	this->Initialize("./Succubus/", "SuccubusWalk.X");
	//}
}

void Succubus::CollisionCheck()
{
	if (CharacData::GetLethita()->GetCurrentState() != CHARIC_STATE::STATE_ATK) return;
	Ray ray = CharacData::GetLethita()->GetAttackRay();
	if (ray.IsPicked(this->GetBoundingBox()))
	{
		StateDmg();
	}
}

void Succubus::rageCheck()
{
	if (monsterInfo[CH_INFOMATION::CH_HP] < 0)
	{
		currentState = CHARIC_STATE::STATE_DIE;
	}
}

void Succubus::BulletPattern()
{
	if (!isReach)
	{
		if (!CheckAnimation("SuccubusRun.X"))
		{
			SoundManager::Get().play("succubus_casting_fire_02", 0.8f);
			this->Initialize("./Succubus/", "SuccubusRun.X");
		}
		//몬스터를 일정 위치로 이동하게 만들자
		D3DXVECTOR3 userP;
		D3DXVECTOR3 goal;
		float dot;
		float radian;
		userP = CharacData::GetLethita()->GetPosition();
		userP.x += 20.0f;
		goal = userP - position;

		if (abs(goal.x) < 1.0f && abs(goal.z) < 1.0f) { isReach = true; rotationWatchAngle = 0; return; }

		D3DXVec3Normalize(&goal, &goal);
		position += (goal * (moveSpeed*4 ) * gameTick);

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
	
	else
	{
		CharacData::GetLethita()->SetMoveSpeed(1.0f);
		//이동한 다음 캐릭터가 한바퀴 회전한다.
		if (abs(rotationAngle - 2*D3DX_PI) > 0.1f)
		{
			rotationAngle += 0.03f; //앵글이 PI가 될떄까지 증가한다.
			position += (-direction * (moveSpeed*4) * gameTick);

			int head = (int)(rotationAngle / 0.55);
			float tail = (rotationAngle / 0.55) - head;

			if (tail  < 0.05455 )//일정 수치마다
			{
				Bullet* temp = new Bullet;
				temp->Initialize(position, CharacData::GetLethita()->GetPosition());
				vBullet.push_back(temp);
			}
		}
		else
		{
			currentState = STATE_REST;
			isReach = false;
			rotationAngle = 0;
		}
	}
	BulletRender();
}

void Succubus::DashPattern()
{
	if (!CheckAnimation("SuccubuDashAttack.X"))
	{
		SoundManager::Get().play("succubus_laugh_spin_04", 0.8f);
		this->Initialize("./Succubus/", "SuccubuDashAttack.X");
		DashStart = position;
		DashEnd = CharacData::GetLethita()->GetPosition();
		SetAnimationIndex(2);
	}
	
	//몬스터를 일정 위치로 이동하게 만들자
	if (currentAnim == 2)
	{
		if (IsCurrentAniEnd())
		{
			SoundManager::Get().play("succubus_queen_camera3_stereo", 0.8f);
			SetAnimationIndex(1);
		}
	}
	else if (currentAnim == 1)
	{
		D3DXVECTOR3 goal;
		float dot;
		float radian;
		goal = DashEnd - DashStart;

		D3DXVec3Normalize(&goal, &goal);
		position += (goal * (moveSpeed * 5.0f) * gameTick);

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
		if (IsCurrentAniEnd()) SetAnimationIndex(0);

		BoundingBox tempbox;
		tempbox = *CharacData::GetLethita()->GetBoundingBox();
		if (Collision::IsBoxToBox(tempbox, boundingBox))
		{
			CharacData::SetInfomation(CH_INFOMATION::CH_HP, -monsterInfo[CH_INFOMATION::CH_ATK]);
			SoundManager::Get().play("Hit_bolt_01", 0.7f);
			CharacData::GetLethita()->SetCurrentState(CHARIC_STATE::STATE_DMG);
			SoundManager::Get().play("succubus_laugh_01", 0.8f);
		}
	}
	else
	{
		if (IsCurrentAniEnd())
		{
			currentState = STATE_REST;
		}
	}
}

void Succubus::SpinPattern()
{

}

void Succubus::BulletControl()
{
	for (auto iter = vBullet.begin(); iter != vBullet.end(); )
	{
		BoundingSphere temp;
		BoundingBox tempbox;
		temp = *((*iter)->GetBoundingSphere());
		tempbox = *CharacData::GetLethita()->GetBoundingBox();

		if ((*iter)->isEnd)
		{
			(*iter)->Destroy();
			iter = vBullet.erase(iter);
		}
		
		else if (Collision::IsBoxToSphere(tempbox, temp))
		{
			CharacData::SetInfomation(CH_INFOMATION::CH_HP, -monsterInfo[CH_INFOMATION::CH_ATK]);
			SoundManager::Get().play("Hit_bolt_01", 0.7f);
			CharacData::GetLethita()->SetCurrentState(CHARIC_STATE::STATE_DMG);
			(*iter)->Destroy();
			iter = vBullet.erase(iter);
		}
		else
		{
			(*iter)->Update();
			++iter;
		}
	}
	BulletRender();
}

void Succubus::BulletRender()
{
	for (auto iter = vBullet.begin(); iter != vBullet.end();  ++iter)
	{
		(*iter)->Redner();;
	}
}

void Succubus::BulletCollisionCheck()
{
}
