#pragma once
#include "Inventory.h"
#include "Skill.h"
#include "StatInfomation.h"
#include "ProgressBar.h"

enum UI_KIND
{
	UI_INVENTORY,
	UI_SKILL,
	UI_STAT,
	UI_END
};
class UI_Mnager
{
private:
	UI_Mnager();
	UI_Mnager(const UI_Manager& other);
	~UI_Mnager();

public:
	static UI_Mnager& Get()
	{
		static UI_Mnager instance;
		return instance;
	}
	void Initilize();
	void Destroy();
	void Render();
	void Update();
	LRESULT InputProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	static inline void SetUsing(bool use) { UI_Mnager::Get().InstanceSetUsing(use); }
	static inline Inventory* GetInventory() { return UI_Mnager::Get().GetInstanceInventory(); }

	void SetUiRender(int kind) { IsUiUsing[kind] = !IsUiUsing[kind]; };
	void CheckKey();

private:
	Inventory* inven;
	Skill* skill;
	StatInfomation* Stat;
	ProgressBar* UserHpBar;
	UI_Button* UiButton[UI_END];

	bool IsUiUsing[UI_END];
	bool isUsing = true;

	void UiButtonInit();
	 inline Inventory* GetInstanceInventory() { return inven; }
	inline void InstanceSetUsing(bool use) { isUsing = use; }
};

