#pragma once
#include <Windows.h>
#include <string>
#include "UI_Base.h"

enum QUEST_STEP
{
	QUEST_NOT_ACCEPT = 0,
	QUEST_ACCEPT,
	QUEST_DONE
};

class UI_Quest :
	public UI_Base
{
public:
	UI_Quest();
	virtual ~UI_Quest();

	virtual void Render() override;

	virtual LRESULT InputProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;

	void Initialize();

	QUEST_STEP GetQuestStep() { return questStep; }
	bool GetOpenByStartNode() { return openByStartNode; }
	void SetTitle(char* title);
	void SetDescription(char* desc);
	void SetQuestStep(QUEST_STEP step);
	void SetOpenByStartNode(bool startNode) { openByStartNode = startNode; }

protected:

	enum CHILD_TYPE
	{
		IMAGE_BACKGROUND = 0,
		TEXT_TITLE,
		TEXT_DESC,
		BUTTON_OK,
		BUTTON_CANCEL
	};

	bool isDrag = false;
	POINT prevMouse;
	QUEST_STEP questStep = QUEST_NOT_ACCEPT;
	bool openByStartNode = true;
};

