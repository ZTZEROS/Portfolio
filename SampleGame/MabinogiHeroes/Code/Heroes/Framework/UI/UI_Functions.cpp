#include "stdafx.h"
#include "UI_Functions.h"


UI_Functions::UI_Functions()
{
}


UI_Functions::~UI_Functions()
{
}

void UI_Functions::GameStartButton()
{
	SceneManager::Get().ChangeScene(SCENE_TOWN);
	UI_Mnager::Get().SetUsing(true);
}

void UI_Functions::MapToolButton()
{
	if (!HWnd[WINDOW_MAP_TOOL]) HWnd[WINDOW_MAP_TOOL] = CreateWindowEx(WS_EX_APPWINDOW, MAP_TOOL_WINDOW_CLASS_NAME, MAP_TOOL_WINDOW_CLASS_NAME, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_CAPTION | WS_SYSMENU, 0, 0, RESOLUTION_X, RESOLUTION_Y, NULL, NULL, HInstance, NULL);

	if (!HWnd[WINDOW_MAP_TOOL])
	{
		MessageBox(NULL, L"Call to CreateWindow failed!", MAP_TOOL_WINDOW_CLASS_NAME, MB_OK);
		return;
	}

	ShowWindow(HWnd[WINDOW_MAP_TOOL], SW_SHOWNORMAL);
	SetForegroundWindow(HWnd[WINDOW_MAP_TOOL]);
	SetFocus(HWnd[WINDOW_MAP_TOOL]);
	UpdateWindow(HWnd[WINDOW_MAP_TOOL]);
}

void UI_Functions::OptionButton()
{
	if (!HWnd[WINDOW_OPTION]) HWnd[WINDOW_OPTION] = CreateWindowEx(WS_EX_APPWINDOW, OPTION_WINDOW_CLASS_NAME, OPTION_WINDOW_CLASS_NAME, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_CAPTION | WS_SYSMENU, 0, 0, RESOLUTION_X / 2, RESOLUTION_Y / 2, NULL, NULL, HInstance, NULL);

	if (!HWnd[WINDOW_OPTION])
	{
		MessageBox(NULL, L"Call to CreateWindow failed!", OPTION_WINDOW_CLASS_NAME, MB_OK);
		return;
	}

	ShowWindow(HWnd[WINDOW_OPTION], SW_SHOWNORMAL);
	SetForegroundWindow(HWnd[WINDOW_OPTION]);
	SetFocus(HWnd[WINDOW_OPTION]);
	UpdateWindow(HWnd[WINDOW_OPTION]);
}

void UI_Functions::HelpButton()
{
	//
}

void UI_Functions::ExitButton()
{
	::DestroyWindow(HWnd[WINDOW_MAIN]);
}

void UI_Functions::SetInvenType(Inventory* inven, Inven_Kind out)
{
	inven->SetCurrentInven(out);
//	inven->ItemPositionSetting();
}

//UI_Mnager Button
void UI_Functions::SetUi(int num)
{
	UI_Mnager::Get().SetUiRender(num);
}

void UI_Functions::SetItem(ItemShop * temp)
{
	temp->SetIsShow();
}

void UI_Functions::BuyButton(ItemShop* temp)
{
	for (int i = 0; i < 10; ++i)
	{
		if (temp->GetvItems()[i]->GetIsUp())
		{
			if (CharacData::GetUnEqipInfo(CH_INFOMATION::CH_GOLD) < temp->GetvItems()[i]->GetItem().gold) return; //돈이 더 작다면
			UI_Mnager::Get().GetInventory()->AddItem(temp->GetvItems()[i]->GetItemName());
			CharacData::SetInfomation(CH_INFOMATION::CH_GOLD, -temp->GetvItems()[i]->GetItem().gold);
		}
	}


}

void UI_Functions::ButtonClick()
{
	OutputDebugStringA("Button Click");

}

void UI_Functions::ButtonClick2(UI_Base* ui)
{
	D3DXMATRIXA16 mat = ui->GetWorld();
	ui->SetPosition(mat._41 + 50.0f, mat._42, mat._43);
}

void UI_Functions::ButtonOK(UI_Base* ui)
{
	UI_Text* text = (UI_Text*)ui;
	text->SetText("Accept");
}

void UI_Functions::ButtonCancel(UI_Base* ui)
{
	UI_Text* text = (UI_Text*)ui;
	text->SetText("Cancel");
}