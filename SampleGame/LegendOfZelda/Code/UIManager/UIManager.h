#pragma once
#include "../gameNode.h"
#include "Inventories.h"
#include "StatusDisplays.h"
#include "WorldMaps.h"
#include "DungeonMaps.h"
#include "saveLoad.h"
#include "../link/link.h"
#include "../itemNPC/ITEM/itemManager.h"
#include "../MAPS/mapManager.h"

class link;
class itemManager;
class mapManager;

enum MenuEnumeration
{
	isClosed,
	isInventory,
	isWorldMap,
	isDungeonMap,
	isSaveLoad
};

struct tagLinkInfo
{
	image* _image;
	RECT _bodyRc;
	RECT _swordRc;
	float _x;
	float _y;
	float _shadowX;
	float _shadowY;
	float _speed;
	float _bodyRcWidth;
	float _bodyRcHeight;
	int _maxHp;
	int _maxSp;
	int _currentHp;
	int _currentSp;
	int _frameCount;
	LINKSTATE _linkState;
	LINKDIRECTION _linkDir;
	ITEMSTATE _itemState;

	bool _weaponEquip;
	bool _bedJumpStart;
	bool _nonLiftObject;
	bool _liftObject;
	bool _itemOpen;
};

class UIManager : public gameNode
{
private:
	Inventories Inventory;
	StatusDisplays StatusDisplay;
	WorldMaps WorldMap;
	DungeonMaps DungeonMap;
	saveLoad SaveLoad;

	link* Link;
	itemManager* IM;

	items* Item;

	int UK, DK, LK, RK; //UpKey, DownKey, LeftKey, RightKey;

	bool ShowInventory;
	bool ShowStatusDisplay;
	bool ShowWorldMap;
	bool ShowDungeonMap;
	bool ShowCursor;

	bool Paused;
	bool Talking;

	MenuEnumeration CurrentMenu;

	tagLinkInfo CloneLink;

public:
	HRESULT Initialize();
	void Release();
	void Update();
	void Render();

	void ReceiveLinkInfo();

	bool GetPaused() { return Paused; }
	bool GetTalking() { return Talking; }
	MenuEnumeration GetCurrentMenu() { return CurrentMenu; }

	itemManager* GetIM() { return IM; }
	Inventories* GetInventory() { return &Inventory; }

	void SetCloneLink(tagLinkInfo cloneLink) { CloneLink = cloneLink; }
	void SetLinkAddress(link* link) { Link = link; }
	void SetIMAddress(itemManager* im) { IM = im; }
	void SetTalking(bool talking) { Talking = talking; }
	
	UIManager();
	~UIManager();
};