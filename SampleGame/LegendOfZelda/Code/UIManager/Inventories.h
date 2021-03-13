#pragma once
#include "../gameNode.h"
#include "../itemNPC/ITEM/items.h"
#include "Cursors.h"

class items;

class Inventories : public gameNode
{
private:
	vector<items*> ItemV;
	vector<items*>::iterator ItemVI;

	bool Show;
	
	float LeftTopX, LeftTopY;
	int SelectedSubWeapon;

	Cursors Cursor;

public:
	HRESULT Initialize();
	void Release();
	void Update();
	void Render();

	bool GetShow() { return Show; }
	float GetLeftTopY() { return LeftTopY; }
	vector<items*> GetItemV() { return ItemV; }
	vector<items*>::iterator GetItemVI() { return ItemVI; }
	Cursors GetCursor() { return Cursor; }
	float GetCursorCenterX() { return Cursor.GetCenterX(); }
	float GetCursorCenterY() { return Cursor.GetCenterY(); }
	int GetSelectedSubWeapon() { return SelectedSubWeapon; }
	
	void SetShow(bool show) { Show = show; }
	void SetLeftTopY(float leftTopY) { LeftTopY = leftTopY; }
	void SetItemV(items* item) { ItemV.push_back(item); }
	void SetEquip(int arrayNumber, bool equip=1) { ItemV[arrayNumber]->setEquip(equip); }
	void SetCursor(Cursors cursor) { Cursor = cursor; }
	void SetCursorCenterX(float centerX) { Cursor.SetCenterX(centerX); }
	void SetCursorCenterY(float centerX) { Cursor.SetCenterY(centerX); }

	Inventories();
	~Inventories();
};