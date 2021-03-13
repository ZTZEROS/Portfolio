#pragma once
#include "../gameNode.h"
#include "Heroes/Flonnes.h"
#include "Heroes/Etnas.h"
#include "Heroes/Priers.h"
#include "Heroes/Adells.h"
#include "Heroes/Axels.h"
#include "Heroes/Fenrichs.h"
#include "EnemiesManagers.h"
//#include "../inventory.h"

class EnemiesManagers;
class inventory;

class HeroesManagers : public gameNode
{
private:
	//Flonnes Flonne; //B77 W53 H78
	//Etnas Etna; //B71 W50 H75
	//Priers Prier;

	//vector<Heroes> HeroesV;
	//vector<Heroes>::iterator HeroesVI;

	vector<Heroes*> HeroesV;
	vector<Heroes*>::iterator HeroesVI;

	vector<Heroes**> HeroesActionEntryV;
	vector<Heroes**>::iterator HeroesActionEntryVI;

	EnemiesManagers* EM;
	inventory* Inventory;

	bool Act;

public:
	virtual HRESULT Initialize();
	virtual void Release();
	virtual void Update();
	//virtual void HeroAction();
	virtual void Render();
	
	//Runner Functions
	void StartHeroesVAnimation(int index) { HeroesV[index]->StartAnimation(); }
	void StartHeroesVAnimation(int index, CurrentActionsEnumeration currentAction) { HeroesV[index]->StartAnimation(currentAction); }

	void RunHeroesVStartCurrentAnimation(int index) { HeroesV[index]->StartCurrentAnimation(); }
	void RunHeroesVStartAnimation(int index, CurrentActionsEnumeration currentAction) { HeroesV[index]->StartAnimation(currentAction); }
	void RunHeroesVStopCurrentAnimation(int index) { HeroesV[index]->StopCurrentAnimation(); }
	void RunHeroesVStopAnimation(int index, CurrentActionsEnumeration currentAction) { HeroesV[index]->StopAnimation(currentAction); }

	//void RunHeroesVWalk(int index, vector<POINT>* PathsVAddress) { HeroesV[index]->Walk(PathsVAddress); }
	void RunHeroesVWalk(int index, vector<POINT> PathsV) { HeroesV[index]->Walk(PathsV); }
	//void RunHeroesVAttack(int index, POINT* targetTileAddress) { HeroesV[index]->Attack(targetTileAddress); }
	void RunHeroesVAttack(int index, POINT targetTile) { HeroesV[index]->Attack(targetTile); }
	void SetHeroesVTargetTile(int index, POINT targetTile) { HeroesV[index]->SetTargetTile(targetTile); }
	//void RunHeroesVIsAttacked(int index, POINT* attackerTileAddress) { HeroesV[index]->IsAttacked(attackerTileAddress); }
	void RunHeroesVIsAttacked(int index, POINT attackerTile) { HeroesV[index]->IsAttacked(attackerTile); }
	
	//void RunHeroesVWalkTo(int index, vector<POINT> path) { HeroesV[index]->WalkTo(path); }
	//void RunHeroesVAttackOn(int index, POINT targetTile) { HeroesV[index]->AttackOn(targetTile); }
	//void RunHeroesVAttackedBy(int index, POINT enemyTile) { HeroesV[index]->IsAttackedBy(enemyTile); }
	//Runner Functions

	vector<Heroes*> GetHeroesV() { return HeroesV; }
	vector<Heroes*>::iterator GetHeroesVI() { return HeroesVI; }
	vector<Heroes*>* GetHeroesVAddress() { return &HeroesV; }
	vector<Heroes*>* GetHeroesVAddress(int index) { return &HeroesV; }
	vector<Heroes*>::iterator* GetHeroesVIAddress() { return &HeroesVI; }
	vector<Heroes**> GetHeroesActionEntryV() { return HeroesActionEntryV; }
	vector<Heroes**>::iterator GetHeroesActionEntryVI() { return HeroesActionEntryVI; }
	
	//Getter Functions
	DNAsStructure GetHeroesVDNA(int index) { return HeroesV[index]->GetDNA(); }

	//image GetHeroesVImage(int index) { return HeroesV[index]->GetImage(); }
	image* GetHeroesVImage(int HeroesVIndex, int imageIndex) { return HeroesV[HeroesVIndex]->GetImage(imageIndex); }
	//const char* GetHeroesVName(int index) { return HeroesV[index]->GetName(); }
	string GetHeroesVName(int index) { return HeroesV[index]->GetName(); }
	image* GetHeroesVFace(int HeroesVIndex, int faceIndex) { return HeroesV[HeroesVIndex]->GetFace(faceIndex); }
	animation* GetHeroesVMotion(int index, int direction, int action) { return HeroesV[index]->GetMotion(direction, action); }
	int GetHeroesVTileX(int index) { return HeroesV[index]->GetTileX(); }
	int GetHeroesVTileY(int index) { return HeroesV[index]->GetTileY(); }
	int GetHeroesVCenterX(int index) { return HeroesV[index]->GetCenterX(); }
	int GetHeroesVCenterY(int index) { return HeroesV[index]->GetCenterY(); }
	int GetHeroesVLeftTopX(int index) { return HeroesV[index]->GetLeftTopX(); }
	int GetHeroesVLeftTopY(int index) { return HeroesV[index]->GetLeftTopY(); }
	int GetHeroesVFakeAltitude(int index) { return HeroesV[index]->GetFakeAltitude(); }
	RECT GetHeroesVVirtualBody(int index) { return HeroesV[index]->GetVirtualBody(); }

	CurrentDirectionsEnumeration GetHeroesVCurrentDirection(int index) { return HeroesV[index]->GetCurrentDirection(); }
	CurrentStancesEnumeration GetHeroesVCurrentStance(int index) { return HeroesV[index]->GetCurrentStance(); }
	CurrentActionsEnumeration GetHeroesVCurrentAction(int index) { return HeroesV[index]->GetCurrentAction(); }
	CurrentStatesEnumeration GetHeroesVCurrentState(int index) { return HeroesV[index]->GetCurrentState(); }

	int GetHeroesVLevel(int index) { return HeroesV[index]->GetLevel(); }
	int GetHeroesVMaxHP(int index) { return HeroesV[index]->GetMaxHP(); }
	int GetHeroesVMaxSP(int index) { return HeroesV[index]->GetMaxSP(); }
	int GetHeroesVCurrentHP(int index) { return HeroesV[index]->GetCurrentHP(); }
	int GetHeroesVCurrentSP(int index) { return HeroesV[index]->GetCurrentSP(); }
	int GetHeroesVAttack(int index) { return HeroesV[index]->GetAttack(); }
	int GetHeroesVDefence(int index) { return HeroesV[index]->GetDefence(); }
	int GetHeroesVHit(int index) { return HeroesV[index]->GetHit(); }
	int GetHeroesVSpeed(int index) { return HeroesV[index]->GetSpeed(); }
	int GetHeroesVMagic(int index) { return HeroesV[index]->GetMagic(); }
	int GetHeroesVResistance(int index) { return HeroesV[index]->GetResistance(); }
	int GetHeroesVMovableDistance(int index) { return HeroesV[index]->GetMovableDistance(); }
	int GetHeroesVJumpableAltitude(int index) { return HeroesV[index]->GetJumpableAltitude(); }

	bool GetHeroesVMoved(int index) { return HeroesV[index]->GetMoved(); }
	bool GetHeroesVAttacked(int index) { return HeroesV[index]->GetAttacked(); }
	//Getter Functions


	//Setter Functions
	void SetHeroesVDNA(int index, DNAsStructure dna) { HeroesV[index]->SetDNA(dna); }

	//void SetImage(int index, image img) { HeroesV[index]->SetImage(img); }
	void SetImage(int HeroesVIndex, int imageIndex, image* img) { HeroesV[HeroesVIndex]->SetImage(imageIndex, img); }
	void SetHeroesVName(int index, string name) { HeroesV[index]->SetName(name.c_str()); }
	void SetHeroesVFace(int HeroesVIndex, int faceIndex, image* face) { HeroesV[HeroesVIndex]->SetFace(faceIndex, face); }
	void SetHeroesVMotion(int index, int direction, int action, animation* motion) { HeroesV[index]->SetMotion(direction, action, motion); }
	void SetHeroesVTileX(int index, int tileX) { HeroesV[index]->SetTileX(tileX); }
	void SetHeroesVTileY(int index, int tileY) { HeroesV[index]->SetTileY(tileY); }
	void SetHeroesVCenterX(int index, int centerX) { HeroesV[index]->SetCenterX(centerX); }
	void SetHeroesVCenterY(int index, int centerY) { HeroesV[index]->SetCenterY(centerY); }
	void SetHeroesVLeftTopX(int index, int leftTopX) { HeroesV[index]->SetLeftTopX(leftTopX); }
	void SetHeroesVLeftTopY(int index, int leftTopY) { HeroesV[index]->SetLeftTopY(leftTopY); }
	void SetHeroesVFakeAltitude(int index, int fakeAltitude) { HeroesV[index]->SetFakeAltitude(fakeAltitude); }
	void SetHeroesVVirtualBody(int index, RECT virtualBody) { HeroesV[index]->SetVirtualBody(virtualBody); }

	void SetHeroesVCurrentDirection(int index, CurrentDirectionsEnumeration currentDirection) { HeroesV[index]->SetCurrentDirection(currentDirection); }
	void SetHeroesVCurrentStance(int index, CurrentStancesEnumeration currentStance) { HeroesV[index]->SetCurrentStance(currentStance); }
	void SetHeroesVCurrentAction(int index, CurrentActionsEnumeration currentAction) { HeroesV[index]->SetCurrentAction(currentAction); }
	void SetHeroesVCurrentState(int index, CurrentStatesEnumeration currentState) { HeroesV[index]->SetCurrentState(currentState); }

	void SetHeroesVLevel(int index, int level) { HeroesV[index]->SetLevel(level); }
	void SetHeroesVMaxHP(int index, int maxHP) { HeroesV[index]->SetMaxHP(maxHP); }
	void SetHeroesVMaxSP(int index, int maxSP) { HeroesV[index]->SetMaxSP(maxSP); }
	void SetHeroesVCurrentHP(int index, int currentHP) { HeroesV[index]->SetCurrentHP(currentHP); }
	void SetHeroesVCurrentSP(int index, int currentSP) { HeroesV[index]->SetCurrentSP(currentSP); }
	void SetHeroesVAttack(int index, int attack) { HeroesV[index]->SetAttack(attack); }
	void SetHeroesVDefence(int index, int defence) { HeroesV[index]->SetDefence(defence); }
	void SetHeroesVHit(int index, int hit) { HeroesV[index]->SetHit(hit); }
	void SetHeroesVSpeed(int index, int speed) { HeroesV[index]->SetSpeed(speed); }
	void SetHeroesVMagic(int index, int magic) { HeroesV[index]->SetMagic(magic); }
	void SetHeroesVResistance(int index, int resistance) { HeroesV[index]->SetResistance(resistance); }
	void SetHeroesVMovableDistance(int index, int movableDistance) { HeroesV[index]->SetMovableDistance(movableDistance); }
	void SetHeroesVJumpableAltitude(int index, int jumpableAltitude) { HeroesV[index]->SetJumpableAltitude(jumpableAltitude); }

	void SetHeroesVMoved(int index, bool moved) { return HeroesV[index]->SetMoved(moved); }
	void SetHeroesVAttacked(int index, bool attacked) { return HeroesV[index]->SetAttacked(attacked); }
	//Setter Functions

	void SetHeroesV(vector<Heroes*> heroesV) { HeroesV = heroesV; }
	void SetHeroesVI(vector<Heroes*>::iterator heroesVI) { HeroesVI = heroesVI; }
	void SetHeroesActionEntryV(vector<Heroes**> heroesActionEntryV) { HeroesActionEntryV = heroesActionEntryV; }
	void SetHeroesActionEntryVI(vector<Heroes**>::iterator heroesActionEntryVI) { HeroesActionEntryVI = heroesActionEntryVI; }
	void AddHeroesActionEntryV(int index) { HeroesActionEntryV.push_back(&HeroesV[index]); }
	//void SetHeroesVHeroActionVI(Heroes hero)

	void SetAct(bool act) { Act = act; }

	void SetEMAddress(EnemiesManagers* em) { EM = em; }
	void SetInventoryAddress(inventory* inven) { Inventory = inven; }

	HeroesManagers();
	~HeroesManagers();
};