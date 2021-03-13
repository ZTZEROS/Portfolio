#pragma once
#include "../../gameNode.h"

enum CurrentDirectionsEnumeration { is_LEFT_UP, is_LEFT_DOWN, is_RIGHT_UP = 3, is_RIGHT_DOWN = 2, }; //CurrentDirectionsEnumeraton
enum CurrentStancesEnumeration { is_NEUTRAL, is_STAND, is_CROUCH }; //CurrentStancesEnumeration
enum CurrentActionsEnumeration { is_IDLE, is_WALKING, is_ATTACKING, is_BEINGATTACKED }; //CurrentActionsEnumeration
enum CurrentStatesEnumeration {	is_NORMAL, is_FAINTED, is_POISONED }; //StatesEnumeration

struct DNAsStructure
{
	//image Image;
	image* Image[4];
	string Name;
	//const char* Name;
	image* Face[2]; // Face[0] for status pannel, Face[1] for battle effect
	animation* Motion[4][4];
	int TileX, TileY, TileZ; //TileZ = altitude
	int CenterX, CenterY, CenterZ;
	int LeftTopX, LeftTopY, LeftTopZ;
	int FakeAltitude; //Just for Rendering
	RECT VirtualBody;

	const char* Appellation; //Title, Another name

	CurrentDirectionsEnumeration CurrentDirection;
	CurrentStancesEnumeration CurrentStance;
	CurrentActionsEnumeration CurrentAction;
	CurrentStatesEnumeration CurrentState;
	
	int Level;
	int MaxExperience, MaxHP, MaxSP;
	int CurrentExperience, CurrentHP, CurrentSP;
	int Attack, Defence; // 공격력, 방어력
	int Hit, Speed; // 명중률, 회피율
	int Magic, Resistance; //마법공격력, 마법방어력 
	int MovableDistance, JumpableAltitude; //이동력, 점프력

	bool Moved;
	bool Attacked;

	//unsigned short int Level;
	//unsigned short int MaxExperience;
	//unsigned short int MaxHP;
	//unsigned short int MaxSP;
	//unsigned short int CurrentExperience;
	//unsigned short int CurrentHP;
	//unsigned short int CurrentSP;
	//unsigned short int Attack; //공격력
	//unsigned short int Defence; //방어력
	//unsigned short int Hit; //명중률
	//unsigned short int Speed; //회피율
	//unsigned short int Magic; //마법공격력
	//unsigned short int Resistance; //마법방어력
	//unsigned short int MovableDistance; //이동력
};

class DNAs : public gameNode
{
protected:
	DNAsStructure DNA;

	vector<POINT> PathsV;
	vector<POINT>::iterator PathsVI;
	//vector<POINT>* PathsVAddress;
	//vector<POINT>::iterator* PathsVIAddress;
	int PathsVIndex;
	//POINT* TargetTileAddress;
	POINT TargetTile;
	//POINT* AttackerTileAddress;
	POINT AttackerTile;

	int TimerForIdle;

public:
	virtual HRESULT Initialize(string characterName, int tileX, int tileY);
	virtual void Release();
	virtual void Update();
	virtual void Render();

	virtual void Action();

	void Idle();
	void Walk(vector<POINT> pathsV);
	void SetPathsV(vector<POINT> pathsV) { PathsV = pathsV; }
	//void Walk(vector<POINT>* PathsVAddress);
	
	void Attack(POINT targetTile);
	POINT GetTargetTile() { return TargetTile; }
	void SetTargetTile(POINT targetTile) { TargetTile = targetTile; }
	//void Attack(POINT* targetTileAddress);
	
	void IsAttacked(POINT attackerTile);
	void SetAttackerTile(POINT attackerTile) { AttackerTile = attackerTile; }
	//void IsAttacked(POINT* attackerTileAddress);
	
	//void MoveTo(int destinationTileX, int destinationTileY);
	void WalkTo(vector<POINT> path);
	void AttackOn(POINT targetTile);
	void IsAttackedBy(POINT attackerTile);

	void WalkingToLeftUp();
	void WalkingToLeftDown();
	void WalkingToRightUp();
	void WalkingToRightDown();

	static void AttackingOnLeftUp();
	static void AttackingOnLeftDown();
	static void AttackingOnRightUp();
	static void AttackingOnRightDown();

	void IsBeingAttackedByLeftUp();
	void IsBeingAttackedByLeftDown();
	void IsBeingAttackedByRightUp();
	void IsBeingAttackedByRightDown();

	void StartAnimation();
	void StartCurrentAnimation();
	void StartAnimation(CurrentActionsEnumeration currentAction);
	void StopAnimation();
	void StopCurrentAnimation();
	void StopAnimation(CurrentActionsEnumeration currentAction);

	//Getter Functions
	DNAsStructure GetDNA() { return DNA; }

	//image GetImage() { return DNA.Image; }
	image* GetImage(int index) { return DNA.Image[index]; }
	//const char* GetName() { return DNA.Name; }
	string GetName() { return DNA.Name; }
	image* GetFace(int index) { return DNA.Face[index]; }
	animation* GetMotion(int direction, int action) { return DNA.Motion[direction][action]; }
	int GetTileX() { return DNA.TileX; }
	int GetTileY() { return DNA.TileY; }
	int GetCenterX() { return DNA.CenterX; }
	int GetCenterY() { return DNA.CenterY; }
	int GetLeftTopX() { return DNA.LeftTopX; }
	int GetLeftTopY() { return DNA.LeftTopY; }
	int GetFakeAltitude() { return DNA.FakeAltitude; }
	RECT GetVirtualBody() { return DNA.VirtualBody; }

	CurrentDirectionsEnumeration GetCurrentDirection() { return DNA.CurrentDirection; }
	CurrentStancesEnumeration GetCurrentStance() { return DNA.CurrentStance; }
	CurrentActionsEnumeration GetCurrentAction() { return DNA.CurrentAction; }
	CurrentStatesEnumeration GetCurrentState() { return DNA.CurrentState; }

	int GetLevel() { return DNA.Level; }
	int GetMaxHP() { return DNA.MaxHP; }
	int GetMaxSP() { return DNA.MaxSP; }
	int GetCurrentHP() { return DNA.CurrentHP; }
	int GetCurrentSP() { return DNA.CurrentSP; }
	int GetAttack() { return DNA.Attack; }
	int GetDefence() { return DNA.Defence; }
	int GetHit() { return DNA.Hit; }
	int GetSpeed() { return DNA.Speed; }
	int GetMagic() { return DNA.Magic; }
	int GetResistance() { return DNA.Resistance; }
	int GetMovableDistance() { return DNA.MovableDistance; }
	int GetJumpableAltitude() { return DNA.JumpableAltitude; }

	bool GetMoved() { return DNA.Moved; }
	bool GetAttacked() { return DNA.Attacked; }
	//Getter Functions


	//Setter Functions
	void SetDNA(DNAsStructure dna) { DNA = dna; }

	//void SetImage(image img) { DNA.Image = img; }
	void SetImage(int index, image* img) { DNA.Image[index] = img; }
	void SetName(string name) { DNA.Name = name.c_str(); }
	void SetFace(int index, image* face) { DNA.Face[index] = face; }
	void SetMotion(int direction, int action, animation* motion) { DNA.Motion[direction][action] = motion; }
	void SetTileX(int tileX) { DNA.TileX = tileX; }
	void SetTileY(int tileY) { DNA.TileY = tileY; }
	void SetCenterX(int centerX) { DNA.CenterX = centerX; }
	void SetCenterY(int centerY) { DNA.CenterY = centerY; }
	void SetLeftTopX(int leftTopX) { DNA.LeftTopX = leftTopX; }
	void SetLeftTopY(int leftTopY) { DNA.LeftTopY = leftTopY; }
	void SetFakeAltitude(int fakeAltitude) { DNA.FakeAltitude = fakeAltitude; }
	void SetVirtualBody(RECT virtualBody) { DNA.VirtualBody = virtualBody; }

	void SetCurrentDirection(CurrentDirectionsEnumeration currentDirection) { DNA.CurrentDirection = currentDirection; }
	void SetCurrentStance(CurrentStancesEnumeration currentStance) { DNA.CurrentStance = currentStance; }
	void SetCurrentAction(CurrentActionsEnumeration currentAction) { DNA.CurrentAction = currentAction; }
	void SetCurrentState(CurrentStatesEnumeration currentState) { DNA.CurrentState = currentState; }

	void SetLevel(int level) { DNA.Level = level; }
	void SetMaxHP(int maxHP) { DNA.MaxHP = maxHP; }
	void SetMaxSP(int maxSP) { DNA.MaxSP = maxSP; }
	void SetCurrentHP(int currentHP) { DNA.CurrentHP = currentHP; }
	void SetCurrentSP(int currentSP) { DNA.CurrentSP = currentSP; }
	void SetAttack(int attack) { DNA.Attack = attack; }
	void SetDefence(int defence) { DNA.Defence = defence; }
	void SetHit(int hit) { DNA.Hit = hit; }
	void SetSpeed(int speed) { DNA.Speed = speed; }
	void SetMagic(int magic) { DNA.Magic = magic; }
	void SetResistance(int resistance) { DNA.Resistance = resistance; }
	void SetMovableDistance(int movableDistance) { DNA.MovableDistance = movableDistance; }
	void SetJumpableAltitude(int jumpableAltitude) { DNA.JumpableAltitude = jumpableAltitude; }

	void SetMoved(bool moved) { DNA.Moved = moved; }
	void SetAttacked(bool attacked) { DNA.Attacked = attacked; }
	//Setter Functions

	DNAs();
	~DNAs();
};