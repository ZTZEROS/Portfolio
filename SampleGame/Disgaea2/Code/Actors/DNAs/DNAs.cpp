#include "stdafx.h"
#include "DNAs.h"

DNAs::DNAs() {}
DNAs::~DNAs() {}

HRESULT DNAs::Initialize(string characterName, int tileX, int tileY)
{
	ZeroMemory(&DNA, sizeof(DNA)); //memset(&DNA, 0, sizeof(DNA));

	//DNA.Image = (*IMAGEMANAGER->findImage(characterName));
	DNA.Name = characterName;
	
	const char* name = DNA.Name.c_str();
	char imageKey[128];
	char animationKey[128];
	
	sprintf(imageKey, "%sFace", name);
	DNA.Face[0] = IMAGEMANAGER->findImage(imageKey);
	sprintf(imageKey, "%sFace2", name);
	DNA.Face[1] = IMAGEMANAGER->findImage(imageKey);
	
	float fps = 12;
	
	sprintf(imageKey, "%s_is_IDLE", name);
	DNA.Image[is_IDLE] = IMAGEMANAGER->findImage(imageKey);
	
	int IDLE_to_LEFT_UP[6] = { 0, 1, 2, 3, 4, 5 };
	sprintf(animationKey, "%s_is_IDLE_to_LEFT_UP%d%d", name, tileX, tileY);
	KEYANIMANAGER->addArrayFrameAnimation(animationKey, imageKey, IDLE_to_LEFT_UP, 6, fps, 1);
	DNA.Motion[is_LEFT_UP][is_IDLE] = KEYANIMANAGER->findAnimation(animationKey);
	
	int IDLE_to_LEFT_DOWN[6] = { 6, 7, 8, 9, 10, 11 };
	sprintf(animationKey, "%s_is_IDLE_to_LEFT_DOWN%d%d", name, tileX, tileY);
	KEYANIMANAGER->addArrayFrameAnimation(animationKey, imageKey, IDLE_to_LEFT_DOWN, 6, fps, 1);
	DNA.Motion[is_LEFT_DOWN][is_IDLE] = KEYANIMANAGER->findAnimation(animationKey);
	
	int IDLE_to_RIGHT_UP[6] = { 12, 13, 14, 15, 16, 17 };
	sprintf(animationKey, "%s_is_IDLE_to_RIGHT_UP%d%d", name, tileX, tileY);
	KEYANIMANAGER->addArrayFrameAnimation(animationKey, imageKey, IDLE_to_RIGHT_UP, 6, fps, 1);
	DNA.Motion[is_RIGHT_UP][is_IDLE] = KEYANIMANAGER->findAnimation(animationKey);
	
	int IDLE_to_RIGHT_DOWN[6] = { 18, 19, 20, 21, 22, 23 };
	sprintf(animationKey, "%s_is_IDLE_to_RIGHT_DOWN%d%d", name, tileX, tileY);
	KEYANIMANAGER->addArrayFrameAnimation(animationKey, imageKey, IDLE_to_RIGHT_DOWN, 6, fps, 1);
	DNA.Motion[is_RIGHT_DOWN][is_IDLE] = KEYANIMANAGER->findAnimation(animationKey);
	
	
	sprintf(imageKey, "%s_is_WALKING", name);
	DNA.Image[is_WALKING] = IMAGEMANAGER->findImage(imageKey);
	
	int WALKING_to_LEFT_UP[6] = { 0, 1, 2, 3, 4, 5 };
	sprintf(animationKey, "%s_is_WALKING_to_LEFT_UP%d%d", name, tileX, tileY);
	KEYANIMANAGER->addArrayFrameAnimation(animationKey, imageKey, WALKING_to_LEFT_UP, 6, fps, 1);
	DNA.Motion[is_LEFT_UP][is_WALKING] = KEYANIMANAGER->findAnimation(animationKey);
	
	int WALKING_to_LEFT_DOWN[6] = { 6, 7, 8, 9, 10, 11 };
	sprintf(animationKey, "%s_is_WALKING_to_LEFT_DOWN%d%d", name, tileX, tileY);
	KEYANIMANAGER->addArrayFrameAnimation(animationKey, imageKey, WALKING_to_LEFT_DOWN, 6, fps, 1);
	DNA.Motion[is_LEFT_DOWN][is_WALKING] = KEYANIMANAGER->findAnimation(animationKey);
	
	int WALKING_to_RIGHT_UP[6] = { 12, 13, 14, 15, 16, 17 };
	sprintf(animationKey, "%s_is_WALKING_to_RIGHT_UP%d%d", name, tileX, tileY);
	KEYANIMANAGER->addArrayFrameAnimation(animationKey, imageKey, WALKING_to_RIGHT_UP, 6, fps, 1);
	DNA.Motion[is_RIGHT_UP][is_WALKING] = KEYANIMANAGER->findAnimation(animationKey);
	
	int WALKING_to_RIGHT_DOWN[6] = { 18, 19, 20, 21, 22, 23 };
	sprintf(animationKey, "%s_is_WALKING_to_RIGHT_DOWN%d%d", name, tileX, tileY);
	KEYANIMANAGER->addArrayFrameAnimation(animationKey, imageKey, WALKING_to_RIGHT_DOWN, 6, fps, 1);
	DNA.Motion[is_RIGHT_DOWN][is_WALKING] = KEYANIMANAGER->findAnimation(animationKey);
	
	
	sprintf(imageKey, "%s_is_ATTACKING", name);
	DNA.Image[is_ATTACKING] = IMAGEMANAGER->findImage(imageKey);
	
	int ATTACKING_to_LEFT_UP[4] = { 0, 1, 2, 3 };
	sprintf(animationKey, "%s_is_ATTACKING_to_LEFT_UP%d%d", name, tileX, tileY);
	KEYANIMANAGER->addArrayFrameAnimation(animationKey, imageKey, ATTACKING_to_LEFT_UP, 4, fps, 0);
	DNA.Motion[is_LEFT_UP][is_ATTACKING] = KEYANIMANAGER->findAnimation(animationKey);
	
	int ATTACKING_to_LEFT_DOWN[4] = { 4, 5, 6, 7 };
	sprintf(animationKey, "%s_is_ATTACKING_to_LEFT_DOWN%d%d", name, tileX, tileY);
	KEYANIMANAGER->addArrayFrameAnimation(animationKey, imageKey, ATTACKING_to_LEFT_DOWN, 4, fps, 0);
	DNA.Motion[is_LEFT_DOWN][is_ATTACKING] = KEYANIMANAGER->findAnimation(animationKey);
	
	int ATTACKING_to_RIGHT_UP[4] = { 8, 9, 10, 11 };
	sprintf(animationKey, "%s_is_ATTACKING_to_RIGHT_UP%d%d", name, tileX, tileY);
	KEYANIMANAGER->addArrayFrameAnimation(animationKey, imageKey, ATTACKING_to_RIGHT_UP, 4, fps, 0);
	DNA.Motion[is_RIGHT_UP][is_ATTACKING] = KEYANIMANAGER->findAnimation(animationKey);
	
	int ATTACKING_to_RIGHT_DOWN[4] = { 12, 13, 14, 15 };
	sprintf(animationKey, "%s_is_ATTACKING_to_RIGHT_DOWN%d%d", name, tileX, tileY);
	KEYANIMANAGER->addArrayFrameAnimation(animationKey, imageKey, ATTACKING_to_RIGHT_DOWN, 4, fps, 0);
	DNA.Motion[is_RIGHT_DOWN][is_ATTACKING] = KEYANIMANAGER->findAnimation(animationKey);
	
	
	sprintf(imageKey, "%s_is_BEINGATTACKED", name);
	DNA.Image[is_BEINGATTACKED] = IMAGEMANAGER->findImage(imageKey);

	int BEINGATTACKED_to_LEFT_UP[1] = { 0 };
	sprintf(animationKey, "%s_is_BEINGATTACKED_to_LEFT_UP%d%d", name, tileX, tileY);
	KEYANIMANAGER->addArrayFrameAnimation(animationKey, imageKey, BEINGATTACKED_to_LEFT_UP, 1, fps, 0);
	DNA.Motion[is_LEFT_UP][is_BEINGATTACKED] = KEYANIMANAGER->findAnimation(animationKey);
	
	int BEINGATTACKED_to_LEFT_DOWN[1] = { 1 };
	sprintf(animationKey, "%s_is_BEINGATTACKED_to_LEFT_DOWN%d%d", name, tileX, tileY);
	KEYANIMANAGER->addArrayFrameAnimation(animationKey, imageKey, BEINGATTACKED_to_LEFT_DOWN, 1, fps, 0);
	DNA.Motion[is_LEFT_DOWN][is_BEINGATTACKED] = KEYANIMANAGER->findAnimation(animationKey);
	
	int BEINGATTACKED_to_RIGHT_UP[1] = { 2 };
	sprintf(animationKey, "%s_is_BEINGATTACKED_to_RIGHT_UP%d%d", name, tileX, tileY);
	KEYANIMANAGER->addArrayFrameAnimation(animationKey, imageKey, BEINGATTACKED_to_RIGHT_UP, 1, fps, 0);
	DNA.Motion[is_RIGHT_UP][is_BEINGATTACKED] = KEYANIMANAGER->findAnimation(animationKey);
	
	int BEINGATTACKED_to_RIGHT_DOWN[1] = { 3 };
	sprintf(animationKey, "%s_is_BEINGATTACKED_to_RIGHT_DOWN%d%d", name, tileX, tileY);
	KEYANIMANAGER->addArrayFrameAnimation(animationKey, imageKey, BEINGATTACKED_to_RIGHT_DOWN, 1, fps, 0);
	DNA.Motion[is_RIGHT_DOWN][is_BEINGATTACKED] = KEYANIMANAGER->findAnimation(animationKey);
	
	DNA.Appellation = "";
	DNA.TileX = tileX, DNA.TileY = tileY, DNA.TileZ = 0; //DNA.TileZ = tileZ;
	DNA.CenterX = (DNA.TileX - DNA.TileY) * 96 + BACKBUFFERSIZEX * 0.5 + 12, DNA.CenterY = (DNA.TileX + DNA.TileY) * 48 - 20 + (BACKBUFFERSIZEY / 3), DNA.CenterZ = DNA.TileZ;
	DNA.LeftTopX = DNA.CenterX - DNA.Motion[DNA.CurrentDirection][DNA.CurrentAction]->getFrameWidth() / 2, DNA.LeftTopY = DNA.CenterY - DNA.Motion[DNA.CurrentDirection][DNA.CurrentAction]->getFrameHeight() / 2, DNA.LeftTopZ = 0;
	DNA.FakeAltitude = 0;
	DNA.VirtualBody = RectMakeCenter(DNA.CenterX, DNA.CenterY, DNA.Motion[DNA.CurrentDirection][DNA.CurrentAction]->getFrameWidth(), DNA.Motion[DNA.CurrentDirection][DNA.CurrentAction]->getFrameHeight());
	
	DNA.CurrentDirection = is_LEFT_UP;
	DNA.CurrentStance = is_NEUTRAL;
	DNA.CurrentAction = is_IDLE;
	DNA.CurrentState = is_NORMAL;
	
	DNA.Level = 1;
	DNA.MaxExperience = DNA.Level * 1000;
	DNA.MaxHP = 100;
	DNA.MaxSP = 100;
	DNA.CurrentExperience = 0;
	DNA.CurrentHP = DNA.MaxHP;
	DNA.CurrentSP = DNA.MaxSP;
	DNA.Attack = 10;
	DNA.Defence = 10;
	DNA.Hit = 10;
	DNA.Speed = 10;
	DNA.Magic = 10;
	DNA.Resistance = 10;
	DNA.MovableDistance = 4;
	DNA.JumpableAltitude = 1;

	DNA.Moved = 0;
	DNA.Attacked = 0;

	PathsVIndex = 0;
	//PathsVAddress = nullptr;
	//TargetTileAddress = nullptr;
	//AttackerTileAddress = nullptr;
	TimerForIdle = 0;

	DNA.Motion[is_LEFT_UP][DNA.CurrentAction]->start();
	DNA.Motion[is_LEFT_DOWN][DNA.CurrentAction]->start();
	DNA.Motion[is_RIGHT_UP][DNA.CurrentAction]->start();
	DNA.Motion[is_RIGHT_DOWN][DNA.CurrentAction]->start();

	return S_OK;
}

void DNAs::Release() {}

void DNAs::Update()
{
	//DNA.CenterX = (DNA.TileX - DNA.TileY) * 96 + BACKBUFFERSIZEX * 0.5 + 12, DNA.CenterY = (DNA.TileX + DNA.TileY) * 48 - 20 + (BACKBUFFERSIZEY / 3) + DNA.TileZ * 12, DNA.CenterZ = DNA.TileZ * 12;
	DNA.LeftTopX = DNA.CenterX - DNA.Motion[DNA.CurrentDirection][DNA.CurrentAction]->getFrameWidth() / 2, DNA.LeftTopY = DNA.CenterY - DNA.Motion[DNA.CurrentDirection][DNA.CurrentAction]->getFrameHeight() / 2;
	DNA.VirtualBody = RectMakeCenter(DNA.CenterX, DNA.CenterY, DNA.Motion[DNA.CurrentDirection][DNA.CurrentAction]->getFrameWidth(), DNA.Motion[DNA.CurrentDirection][DNA.CurrentAction]->getFrameHeight());

	switch (DNA.CurrentAction)
	{
	case is_IDLE:
		break;
	case is_WALKING:
		WalkTo(PathsV);
		if (PathsVIndex > PathsV.size() - 1)
		{
			Idle();
			DNA.Moved = 1;
		}
		//PathsVIndex -= 0.1f;
		break;
	//case is_ATTACKING:
	//	//AttackOn(*TargetTileAddress);
	//	AttackOn(TargetTile);
	//	break;
	case is_BEINGATTACKED:
		IsAttackedBy(AttackerTile);
		//IsAttackedBy(*AttackerTileAddress);
		break;
	defalut:
		break;
	}

	if (TimerForIdle > 0) TimerForIdle++;
	if (TimerForIdle > 50)
	{
		TimerForIdle = 0;
		Idle();
	}

}

void DNAs::Render()
{
	//Rectangle(getMemDC(), DNA.VirtualBody.left, DNA.VirtualBody.top, DNA.VirtualBody.right, DNA.VirtualBody.bottom);
	IMAGEMANAGER->alphaRender("shadow", getMemDC(), DNA.CenterX - IMAGEMANAGER->findImage("shadow")->getWidth() / 2, DNA.CenterY - DNA.FakeAltitude + 50, 150);
	DNA.Image[DNA.CurrentAction]->aniRender(getMemDC(), DNA.LeftTopX, DNA.LeftTopY - DNA.FakeAltitude, DNA.Motion[DNA.CurrentDirection][DNA.CurrentAction]);
	
	//Ellipse(getMemDC(), DNA.CenterX - 10, DNA.CenterY - 10, DNA.CenterX + 10, DNA.CenterY + 10);
}

void DNAs::Action()
{
	AttackOn(TargetTile);
	DNA.Attacked = 1;
}

void DNAs::Idle()
{
	DNA.Motion[is_LEFT_UP][DNA.CurrentAction]->stop();
	DNA.Motion[is_LEFT_DOWN][DNA.CurrentAction]->stop();
	DNA.Motion[is_RIGHT_UP][DNA.CurrentAction]->stop();
	DNA.Motion[is_RIGHT_DOWN][DNA.CurrentAction]->stop();

	DNA.CenterX = (DNA.TileX - DNA.TileY) * 96 + BACKBUFFERSIZEX * 0.5 + 12, DNA.CenterY = (DNA.TileX + DNA.TileY) * 48 - 20 + (BACKBUFFERSIZEY / 3) + DNA.TileZ * 12, DNA.CenterZ = DNA.TileZ * 12;
	DNA.CurrentAction = is_IDLE;
	
	DNA.Motion[is_LEFT_UP][DNA.CurrentAction]->start();
	DNA.Motion[is_LEFT_DOWN][DNA.CurrentAction]->start();
	DNA.Motion[is_RIGHT_UP][DNA.CurrentAction]->start();
	DNA.Motion[is_RIGHT_DOWN][DNA.CurrentAction]->start();
}

void DNAs::Walk(vector<POINT> pathsV)
{
	DNA.Motion[is_LEFT_UP][DNA.CurrentAction]->stop();
	DNA.Motion[is_LEFT_DOWN][DNA.CurrentAction]->stop();
	DNA.Motion[is_RIGHT_UP][DNA.CurrentAction]->stop();
	DNA.Motion[is_RIGHT_DOWN][DNA.CurrentAction]->stop();
	
	DNA.CurrentAction = is_WALKING;
	PathsV = pathsV;
	PathsVIndex = 0;
	
	DNA.Motion[is_LEFT_UP][DNA.CurrentAction]->start();
	DNA.Motion[is_LEFT_DOWN][DNA.CurrentAction]->start();
	DNA.Motion[is_RIGHT_UP][DNA.CurrentAction]->start();
	DNA.Motion[is_RIGHT_DOWN][DNA.CurrentAction]->start();
}

//void DNAs::Walk(vector<POINT>* PathsVAddress)
//{
//	DNA.Motion[DNA.CurrentDirection][DNA.CurrentAction]->stop();
//	DNA.CurrentAction = is_WALKING;
//	PathsVAddress = PathsVAddress;
//	PathsVIndex = (*PathsVAddress).size() - 1;
//	DNA.Motion[is_LEFT_UP][DNA.CurrentAction]->start();
//	DNA.Motion[is_LEFT_DOWN][DNA.CurrentAction]->start();
//	DNA.Motion[is_RIGHT_UP][DNA.CurrentAction]->start();
//	DNA.Motion[is_RIGHT_DOWN][DNA.CurrentAction]->start();
//}

void DNAs::Attack(POINT targetTile)
{
	DNA.Motion[is_LEFT_UP][DNA.CurrentAction]->stop();
	DNA.Motion[is_LEFT_DOWN][DNA.CurrentAction]->stop();
	DNA.Motion[is_RIGHT_UP][DNA.CurrentAction]->stop();
	DNA.Motion[is_RIGHT_DOWN][DNA.CurrentAction]->stop();
	
	DNA.CurrentAction = is_ATTACKING;
	TargetTile = targetTile;
	TimerForIdle = 1;
}

//void DNAs::Attack(POINT* targetTileAddress)
//{
//	DNA.Motion[DNA.CurrentDirection][DNA.CurrentAction]->stop();
//	DNA.CurrentAction = is_ATTACKING;
//	TargetTileAddress = targetTileAddress;
//}

void DNAs::IsAttacked(POINT attackerTile)
{
	DNA.Motion[is_LEFT_UP][DNA.CurrentAction]->stop();
	DNA.Motion[is_LEFT_DOWN][DNA.CurrentAction]->stop();
	DNA.Motion[is_RIGHT_UP][DNA.CurrentAction]->stop();
	DNA.Motion[is_RIGHT_DOWN][DNA.CurrentAction]->stop();
	DNA.CurrentAction = is_BEINGATTACKED;
	AttackerTile = attackerTile;
	TimerForIdle = 1;
}

//void DNAs::IsAttacked(POINT* attackerTileAddress)
//{
//	DNA.Motion[DNA.CurrentDirection][DNA.CurrentAction]->stop();
//	DNA.CurrentAction = is_BEINGATTACKED;
//	AttackerTileAddress = attackerTileAddress;
//	DNA.Motion[DNA.CurrentDirection][DNA.CurrentAction]->start();
//}

//void DNAs::MoveTo(int destinationTileX, int destinationTileY) {}
void DNAs::WalkTo(vector<POINT> pathV)
{
	POINT destinationTile;
	
	destinationTile = pathV[PathsVIndex];

	if (DNA.TileX > destinationTile.x && DNA.TileY == destinationTile.y) DNA.CurrentDirection = is_LEFT_UP;
	if (DNA.TileX < destinationTile.x && DNA.TileY == destinationTile.y) DNA.CurrentDirection = is_RIGHT_DOWN;
	if (DNA.TileY > destinationTile.y && DNA.TileX == destinationTile.x) DNA.CurrentDirection = is_RIGHT_UP;
	if (DNA.TileY < destinationTile.y && DNA.TileX == destinationTile.x) DNA.CurrentDirection = is_LEFT_DOWN;

	switch (DNA.CurrentDirection)
	{
	case is_LEFT_UP:
		WalkingToLeftUp();
		break;
	case is_LEFT_DOWN:
		WalkingToLeftDown();
		break;
	case is_RIGHT_UP:
		WalkingToRightUp();
		break;
	case is_RIGHT_DOWN:
		WalkingToRightDown();
		break;
	default:
		break;
	}
}

void DNAs::AttackOn(POINT targetTile)
{
	if (DNA.TileX > targetTile.x && DNA.TileY == targetTile.y) DNA.CurrentDirection = is_LEFT_UP;
	if (DNA.TileX < targetTile.x && DNA.TileY == targetTile.y) DNA.CurrentDirection = is_RIGHT_DOWN;
	if (DNA.TileY > targetTile.y && DNA.TileX == targetTile.x) DNA.CurrentDirection = is_RIGHT_UP;
	if (DNA.TileY < targetTile.y && DNA.TileX == targetTile.x) DNA.CurrentDirection = is_LEFT_DOWN;

	DNA.Motion[DNA.CurrentDirection][DNA.CurrentAction]->start();
	

	switch (DNA.CurrentDirection)
	{
		case is_LEFT_UP:
			AttackingOnLeftUp();
			break;
		case is_LEFT_DOWN:
			AttackingOnLeftDown();
			break;
		case is_RIGHT_UP:
			AttackingOnRightUp();
			break;
		case is_RIGHT_DOWN:
			AttackingOnRightDown();
			break;
	}
}

void DNAs::IsAttackedBy(POINT attackerTile)
{
	if (DNA.TileX > attackerTile.x && DNA.TileY == attackerTile.y) DNA.CurrentDirection = is_LEFT_UP;
	if (DNA.TileX < attackerTile.x && DNA.TileY == attackerTile.y) DNA.CurrentDirection = is_RIGHT_DOWN;
	if (DNA.TileY > attackerTile.y && DNA.TileX == attackerTile.x) DNA.CurrentDirection = is_RIGHT_UP;
	if (DNA.TileY < attackerTile.y && DNA.TileX == attackerTile.x) DNA.CurrentDirection = is_LEFT_DOWN;

	DNA.Motion[DNA.CurrentDirection][DNA.CurrentAction]->start();
	

	switch (DNA.CurrentDirection)
	{
	case is_LEFT_UP:
		IsBeingAttackedByLeftUp();
		break;
	case is_LEFT_DOWN:
		IsBeingAttackedByLeftDown();
		break;
	case is_RIGHT_UP:
		IsBeingAttackedByRightUp();
		break;
	case is_RIGHT_DOWN:
		IsBeingAttackedByRightDown();
		break;
	}
}

void DNAs::WalkingToLeftUp()
{
	DNA.CenterX -= 4, DNA.CenterY -= 2; //TileWidth = 192, TileHeight = 96 from tileNode.h
	//DNA.TileX -= 1;
	
	DNA.TileX = (DNA.CenterY + 80 - (BACKBUFFERSIZEY / 3) + (DNA.CenterX - BACKBUFFERSIZEX * 0.5) * 0.5) / 96;
	DNA.TileY = (DNA.CenterY + 80 - (BACKBUFFERSIZEY / 3) - (DNA.CenterX - BACKBUFFERSIZEX * 0.5) * 0.5) / 96;

	if ((DNA.CenterX == ((DNA.TileX - DNA.TileY) * 96 + BACKBUFFERSIZEX * 0.5 + 12)) && (DNA.CenterY == (DNA.TileX + DNA.TileY) * 48 - 20 + (BACKBUFFERSIZEY / 3) + DNA.TileZ * 12)) PathsVIndex += 1;
}

void DNAs::WalkingToLeftDown()
{
	DNA.CenterX -= 4, DNA.CenterY += 2;
	//DNA.TileY += 1;

	DNA.TileX = (DNA.CenterY + 80 - (BACKBUFFERSIZEY / 3) + (DNA.CenterX - BACKBUFFERSIZEX * 0.5) * 0.5) / 96;
	DNA.TileY = (DNA.CenterY + 80 - (BACKBUFFERSIZEY / 3) - (DNA.CenterX - BACKBUFFERSIZEX * 0.5) * 0.5) / 96;
	
	if ((DNA.CenterX == ((DNA.TileX - DNA.TileY) * 96 + BACKBUFFERSIZEX * 0.5 + 12)) && (DNA.CenterY == (DNA.TileX + DNA.TileY) * 48 - 20 + (BACKBUFFERSIZEY / 3) + DNA.TileZ * 12)) PathsVIndex += 1;
}

void DNAs::WalkingToRightUp()
{
	DNA.CenterX += 4, DNA.CenterY -= 2;
	//DNA.TileY -= 1;

	DNA.TileX = (DNA.CenterY + 80 - (BACKBUFFERSIZEY / 3) + (DNA.CenterX - BACKBUFFERSIZEX * 0.5) * 0.5) / 96;
	DNA.TileY = (DNA.CenterY + 80 - (BACKBUFFERSIZEY / 3) - (DNA.CenterX - BACKBUFFERSIZEX * 0.5) * 0.5) / 96;
	
	if ((DNA.CenterX == ((DNA.TileX - DNA.TileY) * 96 + BACKBUFFERSIZEX * 0.5 + 12)) && (DNA.CenterY == (DNA.TileX + DNA.TileY) * 48 - 20 + (BACKBUFFERSIZEY / 3) + DNA.TileZ * 12)) PathsVIndex += 1;
}

void DNAs::WalkingToRightDown()
{
	DNA.CenterX += 4, DNA.CenterY += 2;
	//DNA.TileX += 1;

	DNA.TileX = (DNA.CenterY + 80 - (BACKBUFFERSIZEY / 3) + (DNA.CenterX - BACKBUFFERSIZEX * 0.5) * 0.5) / 96;
	DNA.TileY = (DNA.CenterY + 80 - (BACKBUFFERSIZEY / 3) - (DNA.CenterX - BACKBUFFERSIZEX * 0.5) * 0.5) / 96;
	
	if ((DNA.CenterX == ((DNA.TileX - DNA.TileY) * 96 + BACKBUFFERSIZEX * 0.5 + 12)) && (DNA.CenterY == (DNA.TileX + DNA.TileY) * 48 - 20 + (BACKBUFFERSIZEY / 3) + DNA.TileZ * 12)) PathsVIndex += 1;
}

void DNAs::AttackingOnLeftUp()
{

}

void DNAs::AttackingOnLeftDown()
{

}

void DNAs::AttackingOnRightUp()
{

}

void DNAs::AttackingOnRightDown()
{

}

void DNAs::IsBeingAttackedByLeftUp()
{
	//DNA.CurrentHP -= ;
}

void DNAs::IsBeingAttackedByLeftDown()
{
	//DNA.CurrentHP -= ;
}

void DNAs::IsBeingAttackedByRightUp()
{
	//DNA.CurrentHP -= ;
}

void DNAs::IsBeingAttackedByRightDown()
{
	//DNA.CurrentHP -= ;
}

void DNAs::StartAnimation()
{
	DNA.Motion[DNA.CurrentDirection][DNA.CurrentAction]->start();
}

void DNAs::StartCurrentAnimation()
{
	DNA.Motion[DNA.CurrentDirection][DNA.CurrentAction]->start();
}

void DNAs::StartAnimation(CurrentActionsEnumeration currentAction)
{
	DNA.CurrentAction = currentAction;
	DNA.Motion[DNA.CurrentDirection][DNA.CurrentAction]->start();
}

void DNAs::StopAnimation()
{
	DNA.Motion[DNA.CurrentDirection][DNA.CurrentAction]->stop();
}

void DNAs::StopCurrentAnimation()
{
	DNA.Motion[DNA.CurrentDirection][DNA.CurrentAction]->stop();
}

void DNAs::StopAnimation(CurrentActionsEnumeration currentAction)
{
	DNA.CurrentAction = currentAction;
	DNA.Motion[DNA.CurrentDirection][DNA.CurrentAction]->stop();
}