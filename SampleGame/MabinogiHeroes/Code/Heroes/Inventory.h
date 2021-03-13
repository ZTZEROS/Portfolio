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

	void EquipItem(Item_Button* item);//������ �����ϱ�
	void SetCurrentInven(Inven_Kind x) ;
	void ItemPositionSetting();
	inline void SettingItemShop(ItemShop* x) { itemShop = x; }

protected:
	static const int InvenCount = 48; //���� ������ ������ ����
	int ItemCount[Inven_End];//������ �ִ� ������ ����
	
	Inven_Kind currentInven = Inven_Equip; //���� ���� �ִ� �κ��丮â
	UI_Image* InvenWindow; //�κ�â
	std::vector<Item_Button*> InvenItem[Inven_End]; //������ �ִ� ������
	std::vector<Item_Button*> equipItem; //�����ϰ� �ִ� ������
	std::vector<Item_Button*> QuickItem; //�����Կ� �ִ� ������
	POINT prevMouse;
	
	UI_Button* Button[Inven_Kind::Inven_End];
	UI_Image* goldImage;

	bool test = false;

	UI_Image* QuickSlot; //������ UI
	UI_Image* backGround1;
	Item_Button* clickButton; //Ŭ���Ǿ� �ִ� ��

	bool isDrag;
	void WindowInit();
	void InvenInit();

	HDC _hdc;//��Ʈ Ȯ�ο�
	
	ItemShop* itemShop = nullptr;

	
};

