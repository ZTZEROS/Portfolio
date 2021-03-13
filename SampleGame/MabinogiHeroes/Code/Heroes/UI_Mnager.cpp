#include "stdafx.h"
#include "UI_Mnager.h"


UI_Mnager::UI_Mnager()
{
}


UI_Mnager::~UI_Mnager()
{
}

void UI_Mnager::Initilize()
{
	for (int i = 0; i < UI_KIND::UI_END; ++i)
	{
		IsUiUsing[i] = false;
	}

	UserHpBar = new ProgressBar;
	UserHpBar->Initilize("HPFrontBar.dds", "HPBehindBar.dds");
	UserHpBar->gaugeWidth((float)CharacData::GetBattleInfo(CH_HP), (float)CharacData::GetEqipInfo(CH_HP));
	UserHpBar->SetPostion(600, 2, 0);
	UserHpBar->Update();
	inven = new Inventory;
	inven->Initilize();

	Stat = new StatInfomation;
	Stat->Initilize();

	UiButtonInit();
}

void UI_Mnager::Destroy()
{
	UserHpBar->Destory();
	SAFE_DELETE(Stat);
	SAFE_DELETE(inven);
}

void UI_Mnager::Render()
{
	if (!isUsing) return;
	UserHpBar->Render();
	inven->GetQuickSlot()->Render();
	UiButton[UI_KIND::UI_INVENTORY]->Render();

	for (int i = 0; i < UI_KIND::UI_END; ++i)
	{
		if (IsUiUsing[i])
		{
			switch (i)
			{
			case UI_KIND::UI_INVENTORY:
				inven->Render();
				break;

			case UI_KIND::UI_SKILL:
				//skill->Render();
				break;

			case UI_KIND::UI_STAT:
				Stat->Render();
				break;
			}
		}
	}
}

void UI_Mnager::Update()
{
	if(!isUsing) return;
	UserHpBar->gaugeWidth((float)CharacData::GetBattleInfo(CH_HP), (float)CharacData::GetEqipInfo(CH_HP)); //나중에 이걸 히트할때만 하게 한다.
//	UserHpBar->Update();
	inven->QickSlotCheckMouse();
	inven->CheckKey();

	CheckKey();
	for (int i = 0; i < UI_KIND::UI_END; ++i)
	{
		if (IsUiUsing[i])
		{
			switch (i)
			{
			case UI_KIND::UI_INVENTORY:
				inven->Update();
				break;

			case UI_KIND::UI_SKILL:
				//skill->Update();
				break;

			case UI_KIND::UI_STAT:
				Stat->Update();
				break;
			}
		}
	}
}

void UI_Mnager::CheckKey()
{
	if (GameManager::GetKey()->isOnceKeyDown('I'))
	{
		if (SceneManager::GetCurrentScene() == SCENE_DUNGEON) return;
		int i = 0;
		IsUiUsing[UI_KIND::UI_INVENTORY] = !IsUiUsing[UI_KIND::UI_INVENTORY];
	}

	if (GameManager::GetKey()->isOnceKeyDown('K'))
	{
		IsUiUsing[UI_KIND::UI_SKILL] = !IsUiUsing[UI_KIND::UI_SKILL];
	}

	if (GameManager::GetKey()->isOnceKeyDown('U'))
	{
		IsUiUsing[UI_KIND::UI_STAT] = !IsUiUsing[UI_KIND::UI_STAT];
	}

}

LRESULT UI_Mnager::InputProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (!isUsing) return 0;
	UiButton[UI_KIND::UI_INVENTORY]->InputProc(hWnd, message, wParam, lParam);
	inven->GetQuickSlot()->InputProc(hWnd, message, wParam, lParam);
	for (int i = 0; i < UI_KIND::UI_END; ++i)
	{
		if (IsUiUsing[i])
		{
			switch (i)
			{
			case UI_KIND::UI_INVENTORY:
				inven->InputProc(hWnd, message, wParam, lParam);
				break;

			case UI_KIND::UI_SKILL:
				//skill->Update();
				break;

			case UI_KIND::UI_STAT:
				Stat->InputProc(hWnd, message, wParam, lParam);
				break;
			}
		}
	}
	return 0;
}

void  UI_Mnager::UiButtonInit()
{
	UiButton[UI_KIND::UI_INVENTORY] = new UI_Button;
	UiButton[UI_KIND::UI_INVENTORY]->SetTexture("./UI/ButtonUp.dds", "./UI/ButtonMiddle.dds", "./UI/ButtonDown.dds");
	UiButton[UI_KIND::UI_INVENTORY]->SetPosition(0, 685, 0);
	UiButton[UI_KIND::UI_INVENTORY]->SetScale(1.5);
	UiButton[UI_KIND::UI_INVENTORY]->SetText("소지품");
	UiButton[UI_KIND::UI_INVENTORY]->SetFont(UI_Manager::GetItemFont());
	UiButton[UI_KIND::UI_INVENTORY]->SetTextPosition(20, 12, 0);

	UiButton[UI_KIND::UI_SKILL] = new UI_Button;
	UiButton[UI_KIND::UI_SKILL]->SetTexture("./UI/ButtonUp.dds", "./UI/ButtonMiddle.dds", "./UI/ButtonDown.dds");
	UiButton[UI_KIND::UI_SKILL]->SetPosition(60, 0, 0);
	UiButton[UI_KIND::UI_SKILL]->SetText("스킬");
	UiButton[UI_KIND::UI_SKILL]->SetTextPosition(20, 12, 0);
	UiButton[UI_KIND::UI_SKILL]->SetFont(UI_Manager::GetItemFont());
	UiButton[UI_KIND::UI_INVENTORY]->AddChild(UiButton[UI_KIND::UI_SKILL]);

	UiButton[UI_KIND::UI_STAT] = new UI_Button;
	UiButton[UI_KIND::UI_STAT]->SetTexture("./UI/ButtonUp.dds", "./UI/ButtonMiddle.dds", "./UI/ButtonDown.dds");
	UiButton[UI_KIND::UI_STAT]->SetPosition(120, 0, 0);
	UiButton[UI_KIND::UI_STAT]->SetText("캐릭터");
	UiButton[UI_KIND::UI_STAT]->SetTextPosition(20, 12, 0);
	UiButton[UI_KIND::UI_STAT]->SetFont(UI_Manager::GetItemFont());
	UiButton[UI_KIND::UI_INVENTORY]->AddChild(UiButton[UI_KIND::UI_STAT]);

	for (int i = 0; i < UI_END; ++i)
	{
		UiButton[i]->OnClick = std::bind(&UI_Functions::SetUi, std::ref(UI_Manager::Get().func), i);
	}


	UiButton[UI_KIND::UI_INVENTORY]->Update();
}
