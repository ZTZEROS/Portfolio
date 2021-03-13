#pragma once
#include "./UI/UI_Image.h"
#include "./UI/UI_Button.h"
#include "Item_Button.h"

class ItemShop;

enum Inven_Kind
{
	Inven_Equip,
	Inven_Potion,
	Inven_Source,
	Inven_End
};
class Inventory
{
public:
	Inventory();
	~Inventory();

	void Initilize();
	void Destroy();
	void Render();
	void Update();

	void AddItem(char* name);
	void DropItem();
	void SellItem();
	void CheckKey();
	void CheckMouse();
	void QickSlotCheckMouse();

	LRESULT InputProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	inline UI_Image* GetQuickSlot() { return QuickSlot; }
	inline Inven_Kind GetCurrentInven() { return currentInven; }

	void EquipItem(Item_Button* item);//아이템 장착하기
	void SetCurrentInven(Inven_Kind x) ;
	void ItemPositionSetting();
	inline void SettingItemShop(ItemShop* x) { itemShop = x; }

protected:
	static const int InvenCount = 48; //보유 가능한 아이템 갯수
	int ItemCount[Inven_End];//가지고 있는 아이템 개수
	
	Inven_Kind currentInven = Inven_Equip; //현재 띄우고 있는 인벤토리창
	UI_Image* InvenWindow; //인벤창
	std::vector<Item_Button*> InvenItem[Inven_End]; //가지고 있는 아이템
	std::vector<Item_Button*> equipItem; //장착하고 있는 아이템
	std::vector<Item_Button*> QuickItem; //퀵슬롯에 있는 아이템
	POINT prevMouse;
	
	UI_Button* Button[Inven_Kind::Inven_End];
	UI_Image* goldImage;

	bool test = false;

	UI_Image* QuickSlot; //퀵슬롯 UI
	UI_Image* backGround1;
	Item_Button* clickButton; //클릭되어 있는 애

	bool isDrag;
	void WindowInit();
	void InvenInit();

	HDC _hdc;//렉트 확인용
	
	ItemShop* itemShop = nullptr;

	
};

