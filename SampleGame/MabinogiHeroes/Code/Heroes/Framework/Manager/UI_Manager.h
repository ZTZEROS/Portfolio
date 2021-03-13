#pragma once
#include "../UI/UI_Base.h"
#include "../UI/UI_Functions.h"
#include "../UI/UI_Image.h"
#include "../UI/UI_Button.h"
#include "../UI/UI_Text.h"
#include "../UI/UI_Quest.h"

class UI_Manager
{
private:
	UI_Manager();
	UI_Manager(const UI_Manager& other);
	~UI_Manager();

public:
	static UI_Manager& Get()
	{
		static UI_Manager instance;
		return instance;
	}

	virtual void Initialize();
	virtual void Destroy();

	virtual void Update();
	virtual void Render();

	LRESULT InputProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	static inline LPD3DXSPRITE GetSprite() { return UI_Manager::Get().GetInstanceSprite(); }
	//폰트
	static inline LPD3DXFONT GetFont() { return UI_Manager::Get().GetInstanceFont(); }
	static inline LPD3DXFONT GetNPCNameFont() { return UI_Manager::Get().GetInstanceNPCNameFont(); }
	static inline LPD3DXFONT GetNPCDialogueFont() { return UI_Manager::Get().GetInstanceNPCDialogueFont(); }
	static inline LPD3DXFONT GetItemFont() { return UI_Manager::Get().GetInstanceItemFont(); }

	inline UI_Base* GetRoot() { return root; }
	inline void SetRoot(UI_Base* ui) { root = ui; }

	UI_Functions func;

protected:
	void fontInit();

	UI_Base* root = nullptr;

	//폰트
	LPD3DXFONT font = nullptr;
	LPD3DXFONT NPCNameFont = nullptr;
	LPD3DXFONT NPCDialogueFont = nullptr;
	LPD3DXFONT ItemFont = nullptr;
	
	LPD3DXSPRITE sprite = nullptr;

	inline LPD3DXFONT GetInstanceFont() { return font; }
	inline LPD3DXFONT GetInstanceNPCNameFont() { return NPCNameFont; }
	inline LPD3DXFONT GetInstanceNPCDialogueFont() { return NPCDialogueFont; }
	inline LPD3DXFONT GetInstanceItemFont() { return ItemFont; }
	inline LPD3DXSPRITE GetInstanceSprite() { return sprite; }
};

