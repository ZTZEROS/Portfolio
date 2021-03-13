#pragma once
#include "Item_Button.h"
#include "./UI/UI_Image.h"
#include "./UI/UI_Button.h"

class Inventoy;
class ItemShop
{
public:
	ItemShop();
	~ItemShop();

	void Initilize();
	void AddItem(char* name);
	void ItemInit();
	void Render();
	void Update();
	void Destroy();
	void CheckMouse();

	HRESULT InputProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	inline bool GetIsShow() { return isShow; }
	inline void SetIsShow() {  isShow = !isShow; }
	inline void SetInven(Inventory* x) { inven = x;}
	inline vector<Item_Button*> GetvItems() { return vItems; }

private:
	UI_Image* ShopWindow;
	UI_Button* BuyButton;
	UI_Image* clickItem;

	std::vector<Item_Button*> vItems;
	int k = 0;
	bool isShow = false;
	Inventory* inven = nullptr;
};

