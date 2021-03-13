#pragma once
#include "../../Inventory.h"
#include "../../ItemShop.h"
//#include "./UI_Mnager.h"

class UI_Functions
{
public:
	UI_Functions();
	~UI_Functions();

	//OpenScene Button
	void GameStartButton();
	void MapToolButton();
	void OptionButton();
	void HelpButton();
	void ExitButton();

	//InvenTory Button
	void SetInvenType(Inventory* inven, Inven_Kind out);

	//UI_Mnager Button
	void SetUi(int num);
	void SetItem(ItemShop* temp);

	//ItemShop
	void BuyButton(ItemShop* temp);

	void ButtonClick();
	void ButtonClick2(UI_Base* ui);

	void ButtonOK(UI_Base* ui);
	void ButtonCancel(UI_Base* ui);

};

