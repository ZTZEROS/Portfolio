#include "stdafx.h"
#include "StatInfomation.h"


StatInfomation::StatInfomation()
{
}


StatInfomation::~StatInfomation()
{
}

void StatInfomation::Initilize()
{
	StatWidnow = new UI_Image;
	StatWidnow->SetTexture("./UI/StatWindow1.dds");
	StatWidnow->SetPosition(200, 100, 0);
	StatWidnow->SetIsMove(true);

	UI_Image* BackWindow = new UI_Image;
	BackWindow->SetTexture("./UI/StatWindow2.dds");
	BackWindow->SetPosition(31, 50, 0);
	BackWindow->SetIsMove(false);
	StatWidnow->AddChild(BackWindow);

	for (int i = 0; i < CH_END; ++i)
	{
		if (i == CH_ATK || i == CH_LEVEL || i == CH_HP || i == CH_MP || i == CH_DEF || i == CH_EXP || i == CH_SP)
		{
			ChInfomation[i] = new UI_Text;
			ChInfomation[i]->SetFont(UI_Manager::GetItemFont());
			ChInfomation[i]->SetPosition((float)40, (float)60 + 20*i, (float)0);
			ChInfomation[i]->SetSize(312, 300);
			ChInfomation[i]->SetTextColor(D3DCOLOR_XRGB(255, 255, 255));
			ChInfomation[i]->SetTextFormat(DT_VCENTER | DT_WORDBREAK);
			StatWidnow->AddChild(ChInfomation[i]);
		}
	}
	/*char temp[246];
	ChInfomation[CH_LEVEL] = new UI_Text;
	ChInfomation[CH_LEVEL]->SetFont(UI_Manager::GetItemFont());
	ChInfomation[CH_LEVEL]->SetPosition(40, 60, 0);
	ChInfomation[CH_LEVEL]->SetSize(312, 300);
	ChInfomation[CH_LEVEL]->SetTextColor(D3DCOLOR_XRGB(255, 255, 255));
	ChInfomation[CH_LEVEL]->SetTextFormat(DT_VCENTER | DT_WORDBREAK);
	sprintf_s(temp, "레벨 : %d", CharacData::GetEqipInfo(CH_LEVEL));
	ChInfomation[CH_LEVEL]->SetText(temp);
	StatWidnow->AddChild(ChInfomation[CH_LEVEL]);

	ChInfomation[CH_HP] = new UI_Text;
	ChInfomation[CH_HP]->SetFont(UI_Manager::GetItemFont());
	ChInfomation[CH_HP]->SetPosition(40, 80, 0);
	ChInfomation[CH_HP]->SetSize(312, 300);
	ChInfomation[CH_HP]->SetTextColor(D3DCOLOR_XRGB(255, 255, 255));
	ChInfomation[CH_HP]->SetTextFormat(DT_VCENTER | DT_WORDBREAK);
	sprintf_s(temp, "체력 : %d / %d", CharacData::GetEqipInfo(CH_HP), CharacData::GetBattleInfo(CH_HP));
	ChInfomation[CH_HP]->SetText(temp);
	StatWidnow->AddChild(ChInfomation[CH_HP]);

	ChInfomation[CH_MP] = new UI_Text;
	ChInfomation[CH_MP]->SetFont(UI_Manager::GetItemFont());
	ChInfomation[CH_MP]->SetPosition(40, 100, 0);
	ChInfomation[CH_MP]->SetSize(312, 300);
	ChInfomation[CH_MP]->SetTextColor(D3DCOLOR_XRGB(255, 255, 255));
	ChInfomation[CH_MP]->SetTextFormat(DT_VCENTER | DT_WORDBREAK);
	sprintf_s(temp, "마나 : %d / %d", CharacData::GetEqipInfo(CH_MP), CharacData::GetBattleInfo(CH_MP));
	ChInfomation[CH_MP]->SetText(temp);
	StatWidnow->AddChild(ChInfomation[CH_MP]);

	ChInfomation[CH_ATK] = new UI_Text;
	ChInfomation[CH_ATK]->SetFont(UI_Manager::GetItemFont());
	ChInfomation[CH_ATK]->SetPosition(40, 120, 0);
	ChInfomation[CH_ATK]->SetSize(312, 300);
	ChInfomation[CH_ATK]->SetTextColor(D3DCOLOR_XRGB(255, 255, 255));
	ChInfomation[CH_ATK]->SetTextFormat(DT_VCENTER | DT_WORDBREAK);
	sprintf_s(temp, "공격력 : %d", CharacData::GetEqipInfo(CH_ATK));
	ChInfomation[CH_ATK]->SetText(temp);
	StatWidnow->AddChild(ChInfomation[CH_ATK]);

	ChInfomation[CH_DEF] = new UI_Text;
	ChInfomation[CH_DEF]->SetFont(UI_Manager::GetItemFont());
	ChInfomation[CH_DEF]->SetPosition(40, 140, 0);
	ChInfomation[CH_DEF]->SetSize(312, 300);
	ChInfomation[CH_DEF]->SetTextColor(D3DCOLOR_XRGB(255, 255, 255));
	ChInfomation[CH_DEF]->SetTextFormat(DT_VCENTER | DT_WORDBREAK);
	sprintf_s(temp, "방어력 : %d", CharacData::GetEqipInfo(CH_DEF));
	ChInfomation[CH_DEF]->SetText(temp);
	StatWidnow->AddChild(ChInfomation[CH_DEF]);*/
	StatWidnow->Update();
}

void StatInfomation::Destroy()
{
	StatWidnow->Destroy();
}

void StatInfomation::Render()
{
	StatWidnow->Render();
	char temp[254];

	sprintf_s(temp, "레벨 : %d", CharacData::GetEqipInfo(CH_LEVEL));
	ChInfomation[CH_LEVEL]->SetText(temp);

	sprintf_s(temp, "체력 : %d / %d", CharacData::GetBattleInfo(CH_HP), CharacData::GetEqipInfo(CH_HP));
	ChInfomation[CH_HP]->SetText(temp);

	sprintf_s(temp, "마나 : %d / %d", CharacData::GetBattleInfo(CH_MP), CharacData::GetEqipInfo(CH_MP));
	ChInfomation[CH_MP]->SetText(temp);

	sprintf_s(temp, "공격력 : %d", CharacData::GetEqipInfo(CH_ATK));
	ChInfomation[CH_ATK]->SetText(temp);

	sprintf_s(temp, "방어력 : %d", CharacData::GetEqipInfo(CH_DEF));
	ChInfomation[CH_DEF]->SetText(temp);

	sprintf_s(temp, "스테미나 : %d / %d", CharacData::GetBattleInfo(CH_SP), CharacData::GetEqipInfo(CH_SP));
	ChInfomation[CH_SP]->SetText(temp);

	sprintf_s(temp, "경험치 : %d / %d", CharacData::GetUnEqipInfo(CH_EXP), CharacData::GetEqipInfo(CH_LEVEL)*20 + 100);
	ChInfomation[CH_EXP]->SetText(temp);

}

void StatInfomation::Update()
{
	CheckKey();
	StatWidnow->Update();
}

void StatInfomation::CheckKey()
{
	
}

void StatInfomation::CheckMouse()
{
}

LRESULT StatInfomation::InputProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	StatWidnow->InputProc(hWnd, message, wParam, lParam);
	return 0;
}
