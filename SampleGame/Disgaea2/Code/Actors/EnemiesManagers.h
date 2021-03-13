#pragma once
#include "../gameNode.h"
#include "IntegratedAIs.h"
#include "Enemies/ArmorKnightFemales.h"
#include "Enemies/HealerMales.h"
#include "HeroesManagers.h"

class IntegratedAIs;
class HeroesManagers;

class EnemiesManagers : public gameNode
{
private:
	//ArmorKnightFemales ArmorKnightFemale;

	vector<Enemies*> EnemiesV;
	vector<Enemies*>::iterator EnemiesVI;

	vector<Enemies**> EnemiesActionEntryV;
	vector<Enemies**>::iterator EnemiesActionEntryVI;
	
	HeroesManagers* HM;
	IntegratedAIs* IAI;

	bool Act;

public:
	virtual HRESULT Initialize();
	virtual void Release();
	virtual void Update();
	virtual void Render();

	void StartEnemiesVAnimation(int index) { EnemiesV[index]->StartAnimation(); }
	void StartEnemiesVAnimation(int index, CurrentActionsEnumeration currentAction) { EnemiesV[index]->StartAnimation(currentAction); }

	void RunEnemiesVStartCurrentAnimation(int index) { EnemiesV[index]->StartCurrentAnimation(); }
	void RunEnemiesVStartAnimation(int index, CurrentActionsEnumeration currentAction) { EnemiesV[index]->StartAnimation(currentAction); }
	void RunEnemiesVStopCurrentAnimation(int index) { EnemiesV[index]->StopCurrentAnimation(); }
	void RunEnemiesVStopAnimation(int index, CurrentActionsEnumeration currentAction) { EnemiesV[index]->StopAnimation(currentAction); }
	
	//void RunHeroesVWalk(int index, vector<POINT>* PathsVAddress) { HeroesV[index]->Walk(PathsVAddress); }
	void RunEnemiesVWalk(int index, vector<POINT> PathsV) { EnemiesV[index]->Walk(PathsV); }
	//void RunHeroesVAttack(int index, POINT* targetTileAddress) { HeroesV[index]->Attack(targetTileAddress); }
	void RunEnemiesVAttack(int index, POINT targetTile) { EnemiesV[index]->Attack(targetTile); }
	void SetEnemiesVTargetTile(int index, POINT targetTile) { EnemiesV[index]->SetTargetTile(targetTile); }
	//void RunHeroesVIsAttacked(int index, POINT* attackerTileAddress) { HeroesV[index]->IsAttacked(attackerTileAddress); }
	void RunEnemiesVIsAttacked(int index, POINT attackerTile) { EnemiesV[index]->IsAttacked(attackerTile); }
	
	//void RunEnemiesVWalkTo(int index, vector<POINT> path) { EnemiesV[index]->WalkTo(path); }
	//void RunEnemiesVAttackOn(int index, POINT targetTile) { EnemiesV[index]->AttackOn(targetTile); }
	//void RunEnemiesVAttackedBy(int index, POINT enemyTile) { EnemiesV[index]->IsAttackedBy(enemyTile); }
	
	vector<Enemies*> GetEnemiesV() { return EnemiesV; }
	vector<Enemies*>::iterator GetEnemiesVI() { return EnemiesVI; }
	vector<Enemies*>* GetEnemiesVAddress() { return &EnemiesV; }
	vector<Enemies*>::iterator* GetEnemiesVIAddress() { return &EnemiesVI; }
	vector<Enemies**> GetEnemiesActionEntryV() { return EnemiesActionEntryV; }
	vector<Enemies**>::iterator GetEnemiesActionEntryVI() { return EnemiesActionEntryVI; }

	//Getter Functions
	DNAsStructure GetEnemiesVDNA(int index) { return EnemiesV[index]->GetDNA(); }

	//image GetEnemiesVImage(int index) { return EnemiesV[index]->GetImage(); }
	image* GetEnemiesVImage(int EnemiesVIndex, int imageIndex) { return EnemiesV[EnemiesVIndex]->GetImage(imageIndex); }
	//const char* GetEnemiesVName(int index) { return EnemiesV[index]->GetName(); }
	string GetEnemiesVName(int index) { return EnemiesV[index]->GetName(); }
	image* GetEnemiesVFace(int EnemiesVIndex, int faceIndex) { return EnemiesV[EnemiesVIndex]->GetFace(faceIndex); }
	animation* GetEnemiesVMotion(int index, int direction, int action) { return EnemiesV[index]->GetMotion(direction, action); }
	int GetEnemiesVTileX(int index) { return EnemiesV[index]->GetTileX(); }
	int GetEnemiesVTileY(int index) { return EnemiesV[index]->GetTileY(); }
	int GetEnemiesVCenterX(int index) { return EnemiesV[index]->GetCenterX(); }
	int GetEnemiesVCenterY(int index) { return EnemiesV[index]->GetCenterY(); }
	int GetEnemiesVLeftTopX(int index) { return EnemiesV[index]->GetLeftTopX(); }
	int GetEnemiesVLeftTopY(int index) { return EnemiesV[index]->GetLeftTopY(); }
	int GetEnemiesVFakeAltitude(int index) { return EnemiesV[index]->GetFakeAltitude(); }
	RECT GetEnemiesVVirtualBody(int index) { return EnemiesV[index]->GetVirtualBody(); }

	CurrentDirectionsEnumeration GetEnemiesVCurrentDirection(int index) { return EnemiesV[index]->GetCurrentDirection(); }
	CurrentStancesEnumeration GetEnemiesVCurrentStance(int index) { return EnemiesV[index]->GetCurrentStance(); }
	CurrentActionsEnumeration GetEnemiesVCurrentAction(int index) { return EnemiesV[index]->GetCurrentAction(); }
	CurrentStatesEnumeration GetEnemiesVCurrentState(int index) { return EnemiesV[index]->GetCurrentState(); }

	int GetEnemiesVLevel(int index) { return EnemiesV[index]->GetLevel(); }
	int GetEnemiesVMaxHP(int index) { return EnemiesV[index]->GetMaxHP(); }
	int GetEnemiesVMaxSP(int index) { return EnemiesV[index]->GetMaxSP(); }
	int GetEnemiesVCurrentHP(int index) { return EnemiesV[index]->GetCurrentHP(); }
	int GetEnemiesVCurrentSP(int index) { return EnemiesV[index]->GetCurrentSP(); }
	int GetEnemiesVAttack(int index) { return EnemiesV[index]->GetAttack(); }
	int GetEnemiesVDefence(int index) { return EnemiesV[index]->GetDefence(); }
	int GetEnemiesVHit(int index) { return EnemiesV[index]->GetHit(); }
	int GetEnemiesVSpeed(int index) { return EnemiesV[index]->GetSpeed(); }
	int GetEnemiesVMagic(int index) { return EnemiesV[index]->GetMagic(); }
	int GetEnemiesVResistance(int index) { return EnemiesV[index]->GetResistance(); }
	int GetEnemiesVMovableDistance(int index) { return EnemiesV[index]->GetMovableDistance(); }
	int GetEnemiesVJumpableAltitude(int index) { return EnemiesV[index]->GetJumpableAltitude(); }

	bool GetEnemiesVMoved(int index) { return EnemiesV[index]->GetMoved(); }
	bool GetEnemiesVAttacked(int index) { return EnemiesV[index]->GetAttacked(); }

	HeroesManagers* GetHMAddress() { return HM; }
	IntegratedAIs* GetIAIAddress() { return IAI; }
	//Getter Functions


	//Setter Functions
	void SetEnemiesVDNA(int index, DNAsStructure dna) { EnemiesV[index]->SetDNA(dna); }

	//void SetImage(int index, image img) { EnemiesV[index]->SetImage(img); }
	void SetImage(int HeroesVIndex, int imageIndex, image* img) { EnemiesV[HeroesVIndex]->SetImage(imageIndex, img); }
	void SetEnemiesVName(int index, string name) { EnemiesV[index]->SetName(name.c_str()); }
	void SetEnemiesVFace(int EnemiesVIndex, int faceIndex, image* face) { EnemiesV[EnemiesVIndex]->SetFace(faceIndex, face); }
	void SetEnemiesVMotion(int index, int direction, int action, animation* motion) { EnemiesV[index]->SetMotion(direction, action, motion); }
	void SetEnemiesVTileX(int index, int tileX) { EnemiesV[index]->SetTileX(tileX); }
	void SetEnemiesVTileY(int index, int tileY) { EnemiesV[index]->SetTileY(tileY); }
	void SetEnemiesVCenterX(int index, int centerX) { EnemiesV[index]->SetCenterX(centerX); }
	void SetEnemiesVCenterY(int index, int centerY) { EnemiesV[index]->SetCenterY(centerY); }
	void SetEnemiesVLeftTopX(int index, int leftTopX) { EnemiesV[index]->SetLeftTopX(leftTopX); }
	void SetEnemiesVLeftTopY(int index, int leftTopY) { EnemiesV[index]->SetLeftTopY(leftTopY); }
	void SetEnemiesVFakeAltitude(int index, int fakeAltitude) { EnemiesV[index]->SetFakeAltitude(fakeAltitude); }
	void SetEnemiesVVirtualBody(int index, RECT virtualBody) { EnemiesV[index]->SetVirtualBody(virtualBody); }

	void SetEnemiesVCurrentDirection(int index, CurrentDirectionsEnumeration currentDirection) { EnemiesV[index]->SetCurrentDirection(currentDirection); }
	void SetEnemiesVCurrentStance(int index, CurrentStancesEnumeration currentStance) { EnemiesV[index]->SetCurrentStance(currentStance); }
	void SetEnemiesVCurrentAction(int index, CurrentActionsEnumeration currentAction) { EnemiesV[index]->SetCurrentAction(currentAction); }
	void SetEnemiesVCurrentState(int index, CurrentStatesEnumeration currentState) { EnemiesV[index]->SetCurrentState(currentState); }

	void SetEnemiesVLevel(int index, int level) { EnemiesV[index]->SetLevel(level); }
	void SetEnemiesVMaxHP(int index, int maxHP) { EnemiesV[index]->SetMaxHP(maxHP); }
	void SetEnemiesVMaxSP(int index, int maxSP) { EnemiesV[index]->SetMaxSP(maxSP); }
	void SetEnemiesVCurrentHP(int index, int currentHP) { EnemiesV[index]->SetCurrentHP(currentHP); }
	void SetEnemiesVCurrentSP(int index, int currentSP) { EnemiesV[index]->SetCurrentSP(currentSP); }
	void SetEnemiesVAttack(int index, int attack) { EnemiesV[index]->SetAttack(attack); }
	void SetEnemiesVDefence(int index, int defence) { EnemiesV[index]->SetDefence(defence); }
	void SetEnemiesVHit(int index, int hit) { EnemiesV[index]->SetHit(hit); }
	void SetEnemiesVSpeed(int index, int speed) { EnemiesV[index]->SetSpeed(speed); }
	void SetEnemiesVMagic(int index, int magic) { EnemiesV[index]->SetMagic(magic); }
	void SetEnemiesVResistance(int index, int resistance) { EnemiesV[index]->SetResistance(resistance); }
	void SetEnemiesVMovableDistance(int index, int movableDistance) { EnemiesV[index]->SetMovableDistance(movableDistance); }
	void SetEnemiesVJumpableAltitude(int index, int jumpableAltitude) { EnemiesV[index]->SetJumpableAltitude(jumpableAltitude); }

	void SetEnemiesVMoved(int index, bool moved) { return EnemiesV[index]->SetMoved(moved); }
	void SetEnemiesVAttacked(int index, bool attacked) { return EnemiesV[index]->SetAttacked(attacked); }
	//Setter Functions

	void SetEnemiesV(vector<Enemies*> enemiesV) { EnemiesV = enemiesV; }
	void SetEnemiesVI(vector<Enemies*>::iterator enemiesVI) { EnemiesVI = enemiesVI; }
	void SetEnemiesActionEntryV(vector<Enemies**> enemiesActionEntryV) { EnemiesActionEntryV = enemiesActionEntryV; }
	void SetEnemiesActionEntryV(vector<Enemies**>::iterator enemiesActionEntryVI) { EnemiesActionEntryVI = enemiesActionEntryVI; }
	void AddEnemiesActionEntryV(int index) { EnemiesActionEntryV.push_back(&EnemiesV[index]); }

	void SetAct(bool act) { Act = act; }
	
	void SetHMAddress(HeroesManagers* hm) { HM = hm; }
	void SetIAIAddress(IntegratedAIs* iai) { IAI = iai; }

	EnemiesManagers();
	~EnemiesManagers();
};