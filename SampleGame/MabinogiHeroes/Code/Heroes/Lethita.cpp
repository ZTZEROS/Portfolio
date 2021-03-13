#include "stdafx.h"
#include "Lethita.h"

Lethita::Lethita()
{
	ZeroMemory(imtemMesh, sizeof(imtemMesh));
}

Lethita::~Lethita()
{
	for (int i = 0; i < Item_Potion; ++i)
	{
		if (imtemMesh[i]) imtemMesh[i]->Destroy();
	}
}

void Lethita::InitXFile()
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


	this->Initialize("./Lethita/", "LethitaAttack4.X");
	this->Initialize("./Lethita/", "LethitaAttack3.X");
	this->Initialize("./Lethita/", "LethitaAttack2.X");
	this->Initialize("./Lethita/", "LethitaAttack1.X");
	this->Initialize("./Lethita/", "LethitaBase.X");
	this->Initialize("./Lethita/", "LethitaRun.X");
	this->Initialize("./Lethita/", "LethitaRunStop.X");
	this->Initialize("./Lethita/", "LethitaItemPick.X");
	this->Initialize("./Lethita/", "LethitaPotionBegin.X");
	this->Initialize("./Lethita/", "LethitaPotionDuring.X");
	this->Initialize("./Lethita/", "LethitaPotionEnd.X");
	this->Initialize("./Lethita/", "LethitaRunStop.X");
	this->Initialize("./Lethita/", "LethitaTownRunBegin.X");
	this->Initialize("./Lethita/", "LethitaTownRunDuring.X");
	this->Initialize("./Lethita/", "LethitaTownRunEnd.X");
	this->Initialize("./Lethita/", "LethitaRest.X");
	this->Initialize("./Lethita/", "LethitaRestBegin.X");
	this->Initialize("./Lethita/", "LethitaDamageFlinch.X");
	this->Initialize("./Lethita/", "LethitaDashS.X");
	this->Initialize("./Lethita/", "LethitaDashE.X");
	this->Initialize("./Lethita/", "LethitaDashW.X");
	this->Initialize("./Lethita/", "LethitaDashN.X");
	this->Initialize("./Lethita/", "LethitaDead.X");
	this->Initialize("./Lethita/", "LethitaTownRest.X");

	ItemMesh* temp = new ItemMesh;
	temp->SetChBone(rootFrame);
	temp->Initialize("./Lethita/Equip/", "IW_DualSword1.X");
	temp->Initialize("./Lethita/Equip/", "IW_DualSword80.X");
	temp->Initialize("./Lethita/Equip/", "IU_NightHawk.X");
	temp->Initialize("./Lethita/Equip/", "IA_NightHawk.X");
	temp->Initialize("./Lethita/Equip/", "IF_NightHawk.X");
	temp->Initialize("./Lethita/Equip/", "IH_NightHawk.X");
	temp->Initialize("./Lethita/Equip/", "IL_NightHawk.X");
	
	temp->Initialize("./Lethita/Equip/", "IU_Armageddon.X");
	temp->Initialize("./Lethita/Equip/", "IA_Armageddon.X");
	temp->Initialize("./Lethita/Equip/", "IF_Armageddon.X");
	temp->Initialize("./Lethita/Equip/", "IH_Armageddon.X");
	temp->Initialize("./Lethita/Equip/", "IL_Armageddon.X");

	isMoveAble[DIRECTION_UP] = true;
	isMoveAble[DIRECTION_DOWN] = true;
	isMoveAble[DIRECTION_RIGHT] = true;
	isMoveAble[DIRECTION_LEFT] = true;
	isMoveAble[DIRECTION_UPRIGHT] = true;
	isMoveAble[DIRECTION_UPLEFT] = true;
	isMoveAble[DIRECTION_DOWNRIGHT] = true;
	isMoveAble[DIRECTION_DOWNLEFT] = true;
	isMoveAble[DIRECTION_DEFAULT] = true;
	isMoveAble[DIRECTION_END] = true;

	atkShow = new WindBlade;
	atkShow2 = new WindBlade;
}

void Lethita::UpdateAndRender()
{
	if (rootFrame)
	{
		ArrowKeyControl();
		if (SceneType::SCENE_DUNGEON == SceneManager::GetCurrentScene())  DungeounMoveControl(); 
		else if (SceneType::SCENE_TOWN == SceneManager::GetCurrentScene()) TownMoveControl();
		else if (SceneType::SCENE_TEST == SceneManager::GetCurrentScene()) DungeounMoveControl();
		
	}
	atkShow->render(world);
	atkShow2->render(world);
	SkinnedMesh::UpdateAndRender();

	if (GameManager::GetKey()->isOnceKeyDown(VK_SPACE))
	{
		isBoxRend = !isBoxRend;
		CharacData::SetIsBoxRend(isBoxRend);
	}
	for (int i = 0; i < Item_Potion; ++i)
	{
		if (imtemMesh[i])
		{
			imtemMesh[i]->SetChBone(rootFrame);
			imtemMesh[i]->SetWorldLightPos(WorldLightPos);
			imtemMesh[i]->UpdateAndRender();
		}
	}
}

void Lethita::RenderShadow()
{
	SkinnedMesh::RenderShadow(rootFrame);

	for (int i = 0; i < Item_Potion; ++i)
	{
		if (imtemMesh[i])
		{
			imtemMesh[i]->SetLightViewProjection(LightViewProjection);
			imtemMesh[i]->RenderShadow(imtemMesh[i]->GetRootFrame());
		}
	}
}

void Lethita::DungeounMoveControl()
{
	if (isPlay)
	{
		if (currentState == STATE_DIE) { StateDie(); return; };
		if (currentState == STATE_DELETE) return;

		AttackControl();
		switch (currentState)
		{
		case STATE_STOP:
			StateStop();
			break;

		case STATE_WALK:
			StateWalk();
			break;

		case STATE_REST:
			StateRest();
			break;

		case STATE_ATK:
			StateAtk();
			break;

		case STATE_DMG:
			StateDmg();
			break;

		case STATE_POTION:
			StatePotion();
			break;

		case STATE_DIE:
			StateDie();
		}
	}
	
}

void Lethita::TownMoveControl()
{
	switch (currentState)
	{
	case STATE_STOP:
		if (!CheckAnimation("LethitaTownRunEnd.X")) this->Initialize("./Lethita/", "LethitaTownRunEnd.X");
		else
		{
			D3DXMATRIXA16 local, localY;
			D3DXMatrixIdentity(&local);
			D3DXMatrixIdentity(&localY);
			D3DXMatrixRotationY(&localY, rotationWatchAngle);
			local *= localY;
			D3DXVec3TransformCoord(&direction, &baseDirection, &local);
			position += (direction * moveSpeed * gameTick*0.03f);
		}
		if (IsCurrentAniEnd())
		{
			this->Initialize("./Lethita/", "LethitaTownRest.X");
			currentState = CHARIC_STATE::STATE_REST;
		}
		isAttackAble = true;
		stackAttack = 0;
		break;

	case STATE_WALK:
		if (!CheckAnimation("LethitaTownRunBegin.X") && !CheckAnimation("LethitaTownRunDuring.X")) this->Initialize("./Lethita/", "LethitaTownRunBegin.X");
		else if(!CheckAnimation("LethitaTownRunDuring.X") )this->Initialize("./Lethita/", "LethitaTownRunDuring.X");
		isAttackAble = true;
		stackAttack = 0;
		break;

	case STATE_REST:
		if (IsCurrentAniEnd())
		{
			if (CheckAnimation("LethitaTownRest.X")) this->Initialize("./Lethita/", "LethitaTownRest.X");
		}
		isAttackAble = true;
		stackAttack = 0;
		break;



	}
}

void Lethita::AttackControl()
{
	if (currentState == STATE_DMG || currentState == STATE_DIE) return;
	if (GameManager::GetKey()->isOnceKeyDown('X'))
	{
		if (isAttackAble) //나중에 죽어 있을때 등등을 추가하자
		{
			isDamg = false;
			currentState = STATE_ATK;
			isNextAttack = true;
		}
	}
}

void Lethita::ArrowKeyControl()
{
	isDamg = false;
	if (currentState == STATE_ATK || currentState == STATE_DMG) return;
	 tempKey = 0;
	if (GameManager::GetKey()->isStayKeyDown(VK_UP) || GameManager::GetKey()->isStayKeyDown(VK_DOWN)
		|| GameManager::GetKey()->isStayKeyDown(VK_RIGHT) || GameManager::GetKey()->isStayKeyDown(VK_LEFT))
	{
		float x = 0;
		x = (float)animSet->GetPeriodicPosition(animController->GetTime());
		if (abs(x - 0.592) < gameTick*0.65)
		{
			SoundManager::Get().play("walk_03", 0.7f);
		}
		else if (abs(x - 1.24977777778) < gameTick*0.65)
		{
			SoundManager::Get().play("walk_03", 0.7f);
		}
	}

	//테스트용 키
	if (GameManager::GetKey()->isOnceKeyDown('C'))
	{	
		double x = 0;
		x = animController->GetTime();
		x = animSet->GetPeriodicPosition(x);
		if (x == 0.592)
		{
			SoundManager::Get().play("walk_03", 0.7f);
		}
		else if (x == 1.2497)
		{
			SoundManager::Get().play("walk_03", 0.7f);
		}
	}

	//위쪽키
	if (GameManager::GetKey()->isStayKeyDown(VK_UP)) tempKey += KEYBOARD_DIRECTION::DIRECTION_UP;
	//아래키
	if (GameManager::GetKey()->isStayKeyDown(VK_DOWN)) tempKey += KEYBOARD_DIRECTION::DIRECTION_DOWN;
	//오른키
	if (GameManager::GetKey()->isStayKeyDown(VK_RIGHT)) tempKey += KEYBOARD_DIRECTION::DIRECTION_RIGHT;
	//왼키
	if (GameManager::GetKey()->isStayKeyDown(VK_LEFT)) tempKey += KEYBOARD_DIRECTION::DIRECTION_LEFT;
	//올릴때
	if (GameManager::GetKey()->isOnceKeyUp(VK_LEFT) || GameManager::GetKey()->isOnceKeyUp(VK_RIGHT)
		|| GameManager::GetKey()->isOnceKeyUp(VK_UP) || GameManager::GetKey()->isOnceKeyUp(VK_DOWN))
	{
		tempKey = KEYBOARD_DIRECTION::DIRECTION_DEFAULT;
	}
	rotationAngle = GameManager::GetCamera()->GetCamRotY();
	D3DXMATRIXA16 local, localY;
	D3DXMatrixIdentity(&local);
	D3DXMatrixRotationY(&localY, rotationAngle);
	local *= localY;
	D3DXVec3TransformCoord(&direction, &baseDirection, &local);
	switch (tempKey)
	{
	case DIRECTION_UP:
		if (!isMoveAble[tempKey]) break;
		D3DXMatrixRotationY(&localY, D3DXToRadian(0));
		local *= localY;
		D3DXVec3TransformCoord(&direction, &baseDirection, &local);
		rotationWatchAngle = D3DXToRadian(0);
		currentState = CHARIC_STATE::STATE_WALK;
		position += (direction * moveSpeed * gameTick);
		break;

	case DIRECTION_DOWN:
		if (!isMoveAble[tempKey]) break;
		D3DXMatrixRotationY(&localY, D3DXToRadian(180));
		local *= localY;
		D3DXVec3TransformCoord(&direction, &baseDirection, &local);
		rotationWatchAngle = D3DXToRadian(180);
		currentState = CHARIC_STATE::STATE_WALK;
		position += (direction * moveSpeed * gameTick);
		break;

	case DIRECTION_RIGHT:
		if (!isMoveAble[tempKey]) break;
		D3DXMatrixRotationY(&localY, D3DXToRadian(90));
		local *= localY;
		D3DXVec3TransformCoord(&direction, &baseDirection, &local);
		rotationWatchAngle = D3DXToRadian(90);
		currentState = CHARIC_STATE::STATE_WALK;
		position += (direction * moveSpeed * gameTick);
		break;

	case DIRECTION_LEFT:
		if (!isMoveAble[tempKey]) break;
		D3DXMatrixRotationY(&localY, D3DXToRadian(270));
		local *= localY;
		D3DXVec3TransformCoord(&direction, &baseDirection, &local);
		rotationWatchAngle = D3DXToRadian(270);
		currentState = CHARIC_STATE::STATE_WALK;
		position += (direction * moveSpeed * gameTick);
		break;

	case DIRECTION_UPRIGHT:
		if (!isMoveAble[tempKey]) break;
		D3DXMatrixRotationY(&localY, D3DXToRadian(45));
		local *= localY;
		D3DXVec3TransformCoord(&direction, &baseDirection, &local);
		rotationWatchAngle = D3DXToRadian(45);
		currentState = CHARIC_STATE::STATE_WALK;
		position += (direction * moveSpeed * gameTick);
		break;

	case DIRECTION_UPLEFT:
		if (!isMoveAble[tempKey]) break;
		D3DXMatrixRotationY(&localY, D3DXToRadian(315));
		local *= localY;
		D3DXVec3TransformCoord(&direction, &baseDirection, &local);
		rotationWatchAngle = D3DXToRadian(315);
		currentState = CHARIC_STATE::STATE_WALK;
		position += (direction * moveSpeed * gameTick);
		break;

	case DIRECTION_DOWNRIGHT :
		if (!isMoveAble[tempKey]) break;
		D3DXMatrixRotationY(&localY, D3DXToRadian(135));
		local *= localY;
		D3DXVec3TransformCoord(&direction, &baseDirection, &local);
		rotationWatchAngle = D3DXToRadian(135);
		currentState = CHARIC_STATE::STATE_WALK;
		position += (direction * moveSpeed * gameTick);
		break;

	case DIRECTION_DOWNLEFT:
		if (!isMoveAble[tempKey]) break;
		D3DXMatrixRotationY(&localY, D3DXToRadian(225));
		local *= localY;
		D3DXVec3TransformCoord(&direction, &baseDirection, &local);
		rotationWatchAngle = D3DXToRadian(225);
		currentState = CHARIC_STATE::STATE_WALK;
		position += (direction * moveSpeed * gameTick);
		break;

	case DIRECTION_DEFAULT:
		if(currentState != CHARIC_STATE::STATE_REST) currentState = STATE_STOP;
		isMoveAble[DIRECTION_UP] = true;
		isMoveAble[DIRECTION_DOWN] = true;
		isMoveAble[DIRECTION_RIGHT] = true;
		isMoveAble[DIRECTION_LEFT] = true;
		isMoveAble[DIRECTION_UPRIGHT] = true;
		isMoveAble[DIRECTION_UPLEFT] = true;
		isMoveAble[DIRECTION_DOWNRIGHT] = true;
		isMoveAble[DIRECTION_DOWNLEFT] = true;
		isMoveAble[DIRECTION_DEFAULT] = true;
		isMoveAble[DIRECTION_END] = true;
		break;
	}
}

void Lethita::BoxUpdate(Bone * current)
{
	SkinnedMesh::BoxUpdate(current);
	char* temp = (LPSTR)(LPCSTR)current->Name;
	if (temp != NULL &&strcmp(temp, "ValveBiped_Bip01_L_Shoulder01") == 0 && !isFindWidthL)
	{
		if (current->CombinedTransformationMatrix._41 != 0)
		{
			shoulderL = current->CombinedTransformationMatrix._41;
			isFindWidthL = true;
		}
	}
	if (temp != NULL &&strcmp(temp, "ValveBiped_Bip01_R_Shoulder01") == 0 && !isFindWidthR)
	{
		if (current->CombinedTransformationMatrix._41 != 0)
		{
			shoulderR = current->CombinedTransformationMatrix._41;
			isFindWidthR = true;
		}
	}

	if (currentState != CHARIC_STATE::STATE_ATK)
	{
		attackEnd = D3DXVECTOR3(0, 0, 0);
		attackStart = D3DXVECTOR3(0, 0, 0);
		return;
	}
	
	if (temp != NULL &&strcmp(temp, attackName) == 0)
	{
		attackStart = D3DXVECTOR3(current->CombinedTransformationMatrix._41, current->CombinedTransformationMatrix._42, current->CombinedTransformationMatrix._43);
	}

	if (temp != NULL && attackName2!=nullptr &&strcmp(temp, attackName2) == 0)
	{
		attackStart2 = D3DXVECTOR3(current->CombinedTransformationMatrix._41, current->CombinedTransformationMatrix._42, current->CombinedTransformationMatrix._43);
	}
}

void Lethita::RayMake()
{
	if (currentState != CHARIC_STATE::STATE_ATK) return;

	AttackRay.origin = attackStart;
	AttackRay.direction = direction;
	AttackRay.AttackDistance = 3;
	D3DXVec3Normalize(&AttackRay.direction, &AttackRay.direction);

	D3DXVECTOR3 temp2;
	position;
	D3DXVec3Normalize(&temp2, &(attackStart - position));
	atkShow->addVPosition(attackStart + temp2 * 2, attackStart + temp2 * 5);

	if (attackName2 != nullptr)
	{
		position;
		D3DXVec3Normalize(&temp2, &(attackStart2 - position));
		atkShow2->addVPosition(attackStart2 + temp2 * 2, attackStart2 + temp2 * 5);
	}

}

void Lethita::StateDmg()
{
	if (CharacData::GetBattleInfo(CH_INFOMATION::CH_HP) < 1)
	{
		currentState = STATE_DIE;
		return;
	}

	if (IsCurrentAniEnd() && CheckAnimation("LethitaDamageFlinch.X"))
	{
		SetCurrentState(STATE_REST);
		CharacData::SetGameSpeed(2.0f);
		isDamg = false;
	}
	
	if (isDamg == true)
	{
		return;
	}
	
	if (!CheckAnimation("LethitaDamageFlinch.X"))
	{
		atkShow->release();
		atkShow2->release();
		this->Initialize("./Lethita/", "LethitaDamageFlinch.X");
		isDamg = true;
	}
}

void Lethita::StateDie()
{
	RECT fpsRectangle = { 1200, 600, 1201, 601 };
	char textT[512] = "으앙 플레이어 주금 ㅠㅠ ";
	UI_Manager::GetFont()->DrawTextA(NULL, textT, strlen(textT), &fpsRectangle, DT_TOP | DT_RIGHT | DT_NOCLIP, D3DCOLOR_XRGB(255, 255, 255));
	if (!CheckAnimation("LethitaDead.X"))
	{
		this->Initialize("./Lethita/", "LethitaDead.X");
		isDamg = true;
	}

	else if (IsCurrentAniEnd())
	{
		currentState = CHARIC_STATE::STATE_DELETE;
		isPlay = false;
		SceneManager::Get().ChangeScene(SCENE_TOWN);

	}
	
}

void Lethita::StateAtk()
{
	if (!isNextAttack && IsCurrentAniEnd())
	{
		atkShow->release();
		atkShow2->release();
		this->SetAnimationIndex(1);
		stackAttack = 0;
		isAttackAble = false;
		isHit = false;
		currentState = CHARIC_STATE::STATE_REST;
		return;
	}
	if (isNextAttack)
	{
		switch (stackAttack)
		{
		case 0:
			if (!CheckAnimation("LethitaAttack1.X"))
			{
				atkShow->release();
				this->Initialize("./Lethita/", "LethitaAttack1.X");
				SoundManager::Get().play("lethita_attack1", 0.2f);
				attackName = "ValveBiped_Anim_Attachment_RH";
				attackName2 = nullptr;
				isNextAttack = false;
				isHit = true;
			}
			if (IsCurrentAniEnd())
			{
				stackAttack = 1;
			}
			else position += (direction * moveSpeed * gameTick*0.1f);
			break;

		case 1:
			if (!CheckAnimation("LethitaAttack2.X"))
			{
				atkShow->release();
				this->Initialize("./Lethita/", "LethitaAttack2.X");
				SoundManager::Get().play("lethita_attack2", 0.5f);
				attackName = "ValveBiped_Anim_Attachment_LH";
				attackName2 = nullptr;
				isNextAttack = false;
				isHit = true;
			}
			if (IsCurrentAniEnd())
			{
				stackAttack = 2;
			}

			else position += (direction * moveSpeed * gameTick*0.1f);
			break;

		case 2:
			if (!CheckAnimation("LethitaAttack3.X"))
			{
				atkShow->release();
				this->Initialize("./Lethita/", "LethitaAttack3.X");
				SoundManager::Get().play("lethita_attack3", 0.5f);
				attackName = "ValveBiped_Anim_Attachment_RH";
				attackName2 = "ValveBiped_Anim_Attachment_LH";
				isNextAttack = false;
				isHit = true;
			}
			if (IsCurrentAniEnd())
			{
				stackAttack = 3;
			}
			else position += (direction * moveSpeed * gameTick*0.1f);
			break;

		case 3:
			if (!CheckAnimation("LethitaAttack4.X"))
			{
				atkShow->release();
				atkShow2->release();
				this->Initialize("./Lethita/", "LethitaAttack4.X");
				SoundManager::Get().play("lethita_attack4", 0.5f);
				attackName = "ValveBiped_Anim_Attachment_LH";
				attackName2 = nullptr;
				isAttackAble = false;
				isHit = true;
			}
			if (IsCurrentAniEnd())
			{
				stackAttack = 3;
				isNextAttack = false;
				isHit = false;
			}
			else position += (direction * moveSpeed * gameTick*0.1f);
			break;
		}
	}


}

void Lethita::StateRest()
{
	if (IsCurrentAniEnd())
	{
		if (currentAnim == 1)
		{
			this->Initialize("./Lethita/", "LethitaRestBegin.X");
			currentState = CHARIC_STATE::STATE_REST;
			isAttackAble = true;
		}
		else if (CheckAnimation("LethitaDamageFlinch.X")) this->Initialize("./Lethita/", "LethitaRest.X");
		else if (CheckAnimation("LethitaPotionEnd.X")) this->Initialize("./Lethita/", "LethitaRestBegin.X");
		else if (CheckAnimation("LethitaRestBegin.X")) this->Initialize("./Lethita/", "LethitaRest.X");
	}
	isAttackAble = true;
	stackAttack = 0;
}

void Lethita::StateStop()
{
	if (!CheckAnimation("LethitaRunStop.X"))
	{
		SoundManager::Get().play("walk_03", 0.7f);
		this->Initialize("./Lethita/", "LethitaRunStop.X");
	}
	else
	{
		D3DXMATRIXA16 local, localY;
		D3DXMatrixIdentity(&local);
		D3DXMatrixIdentity(&localY);
		D3DXMatrixRotationY(&localY, rotationWatchAngle);
		local *= localY;
		position += (direction * moveSpeed * gameTick*0.03f);
	}
	if (IsCurrentAniEnd())
	{
		this->Initialize("./Lethita/", "LethitaRestBegin.X");
		currentState = CHARIC_STATE::STATE_REST;
	}
	isAttackAble = true;
	stackAttack = 0;
}

void Lethita::StateWalk()
{
	if (!CheckAnimation("LethitaRun.X")) this->Initialize("./Lethita/", "LethitaRun.X");
	isAttackAble = true;
	stackAttack = 0;
}

void Lethita::StatePotion()
{
	//if (CheckAnimation("LethitaRest.X"))
	//{
	//	//this->Initialize("./Lethita/", "LethitaPotionBegin.X");
	//}
	if (/*IsCurrentAniEnd() && */CheckAnimation("LethitaRest.X"))
	{
		this->Initialize("./Lethita/", "LethitaPotionDuring.X");
	}
	else if (IsCurrentAniEnd() && CheckAnimation("LethitaPotionDuring.X"))
	{
		this->Initialize("./Lethita/", "LethitaPotionEnd.X");
	}
	else if (IsCurrentAniEnd() && CheckAnimation("LethitaPotionEnd.X"))
	{
		currentState = CHARIC_STATE::STATE_REST;
	}
}

void Lethita::ItemEquip(Item_Kind kind, char * name)
{
	if (strcmp(name, "Null") == 0)
	{
		SAFE_DELETE(imtemMesh[kind]);
	}
	else
	{
		SAFE_DELETE(imtemMesh[kind]);
		imtemMesh[kind] = new ItemMesh;
		imtemMesh[kind]->SetChBone(rootFrame);
		char temp2[256] = "";
		strcat_s(temp2, name);
		char temp[256] = ".X";
		strcat_s(temp2, temp);
		imtemMesh[kind]->Initialize("./Lethita/Equip/", temp2);
	}
}
