#pragma once
#include "./UI/UI_Image.h"
#include "./UI/UI_Text.h"
#include "../../ItemShop.h"

class NPC
{
protected:
	enum CurrentDialogueType { EVENT_DIALOGUE, DAILY_DIALOGUE, DIALOGUE_TYPE_COUNT };

protected:
	vector<UI_Image*> PortraitsV;
	vector<UI_Image*>::iterator PortraitsVI;

	//vector<UI_Text*> DialoguesV;
	//vector<UI_Text*>::iterator DialoguesVI;

	vector<string*> DialoguesV;
	vector<string*>::iterator DialoguesVI;

	vector<CurrentDialogueType*> DialogueTypesV;
	vector<CurrentDialogueType*>::iterator DialogueTypesVI;

protected:
	const char* NPC_DIRECTORY = "./NPC/";

	UI_Text* NPCName;
	UI_Text* Dialogue;

	UI_Image* DialogueBox;
	UI_Image* DialogueBoxBackground;

	unsigned int CurrentFacialExpression0, FacialExpressionCount;
	unsigned int CurrentDialogue0;
	unsigned int DialogueProcedure, DialogueRange;
	bool ShowPortrait;
	bool Talking;

protected: //아이템 관련
	ItemShop* itemShop = nullptr;
	UI_Button* TradeButton = nullptr;
	bool IsShopShow = false;

public:
	NPC();
	virtual ~NPC();

	virtual void Initialize();
	virtual void Destroy();
	virtual void Reset();
	virtual void Update();
	virtual void Render();

	virtual LRESULT InputProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void AddPortrait(char* fileName);
	void AddDialogue(std::string text, CurrentDialogueType currentDialogueType = EVENT_DIALOGUE);

	void ResetDialogue();
	void Talk();

	unsigned int GetCurrentDialogue0() { return CurrentDialogue0; }
	unsigned int GetDialogueProcedure() { return DialogueProcedure; }
	unsigned int GetDialogueRange() { return DialogueRange; }
	unsigned int GetTalking() { return Talking; }

	void SetCurrentDialogue0(unsigned int currentDialogue0) { CurrentDialogue0 = currentDialogue0; }
	void SetTalking(bool talking) { Talking = talking; }

public: //아이템 관련
	virtual	void ItemSetting();
};

