#pragma once
#include "gameNode.h"
#include "tileNode.h"
#include "UIstat.h"
#include "Inven&shop/armorStoreView.h"
#include "Inven&shop/weaponStoreView.h"


#define TOWN_TILEWIDTH TILEWIDTH * 0.5
#define TOWN_TILEHEIGHT TILEHEIGHT * 0.5
#define REVX 100
enum NPCS
{
	NPC_WEAPONSHOP,
	NPC_ARMORSHOP,
	NPC_WARP,
	NPC_HOSPITAL,
	NPC_END
};

enum TOWN_MENU
{
	TOWNMENU_ITEM,
	TOWNMENU_EQUIP,
	TOWNMENU_END
};

enum WHAT_SHOWING_NOW
{
	NOW_NONE,
	NOW_WEAPONSHOP,
	NOW_ARMORSHOP,
	NOW_WARPCHAT,
	NOW_HOSPITALCHAT,
	NOW_MENU, 
	NOW_MENU_ITEM,
	NOW_MENU_EQUIP,
	NOW_END
};
struct tagNPC
{
	image* img;
	image* img_uz;
	RECT rc;
	int x, y, z;
	int state;
	int currentX, currentY, timer;
};
class townmap : public gameNode
{
private:
	UIstat* _uistat;
	armorStoreView* _asv;
	weaponStoreView* _wsv;

	tagTile _tiles[TILENUMX][TILENUMY];

	image* _tileGraass;
	image* _tileRock;
	image* _tileStones;
	image* _tileGraassTop;
	image* _tileRockTop;
	image* _tileStonesTop;
	image* _tileWater;
	image* _active;
	image* _tileDeepTile;
	image* _tileLightTile;
	image* _tileGraveTile;
	image* _startTile;
	image* _obj;

	int _cameraX, _cameraY;
	tagNPC npc[NPC_END];
	int revX, revY;

	bool openStore;

	// MENU 변수들 -------------------

	WHAT_SHOWING_NOW whatShowing;
	image* townMenu;
	RECT menuRc[TOWNMENU_END];
	int num;	// Equip에서 현재 선택해있는 캐릭터 번호

	// 대화창 ------------------------
	image* ui_chat;
	image* warp_face;
	image* hospital_face;
	string chat;

	int money;

public:
	townmap();
	~townmap();

	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);

	bool getOpenStore(void) { return openStore; }
	void setOpenStore(bool open) { openStore = open; }

	void load(void);

	
};