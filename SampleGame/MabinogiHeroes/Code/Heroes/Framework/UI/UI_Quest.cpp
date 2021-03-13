#include "stdafx.h"
#include "UI_Quest.h"


UI_Quest::UI_Quest()
{
}


UI_Quest::~UI_Quest()
{
}

void UI_Quest::Initialize()
{
	UI_Image* backGround = new UI_Image;
	backGround->SetTexture("./UI/panel-info.png");
	backGround->SetPosition(0, 0, 0);
	this->size = backGround->GetSize();
	this->AddChild(backGround);

	UI_Text* questTitleText = new UI_Text;
	questTitleText->SetFont(UI_Manager::GetFont());
	questTitleText->SetPosition(100, 60, 0);
	questTitleText->SetSize(312, 70);
	questTitleText->SetTextColor(D3DCOLOR_XRGB(255, 255, 0));
	questTitleText->SetTextFormat(DT_CENTER | DT_VCENTER | DT_WORDBREAK);
	questTitleText->SetID(TEXT_TITLE);
	this->AddChild(questTitleText);

	UI_Text* questDescText = new UI_Text;
	questDescText->SetFont(UI_Manager::GetFont());
	questDescText->SetPosition(100, 90, 0);
	questDescText->SetSize(312, 300);
	questDescText->SetTextColor(D3DCOLOR_XRGB(255, 255, 255));
	questDescText->SetTextFormat(DT_CENTER | DT_VCENTER | DT_WORDBREAK);
	questDescText->SetID(TEXT_DESC);
	this->AddChild(questDescText);

	UI_Button* buttonOK = new UI_Button;
	buttonOK->SetTexture("UI/btn-med-up.png", "UI/btn-med-over.png", "UI/btn-med-down.png");
	buttonOK->SetPosition(135, 330, 0);
	buttonOK->SetID(BUTTON_OK);
	buttonOK->OnClick = std::bind(&UI_Functions::ButtonOK,
		std::ref(UI_Manager::Get().func), questDescText);
	this->AddChild(buttonOK);

	UI_Button* buttonCancel = new UI_Button;
	buttonCancel->SetTexture("UI/btn-med-up.png", "UI/btn-med-over.png", "UI/btn-med-down.png");
	buttonCancel->SetPosition(135, 400, 0);
	buttonCancel->SetID(BUTTON_CANCEL);
	buttonCancel->OnClick = std::bind(&UI_Functions::ButtonCancel,
		std::ref(UI_Manager::Get().func), questDescText);
	this->AddChild(buttonCancel);

	isShow = true;
}

void UI_Quest::Render()
{
	if (isShow != true)
	{
		return;
	}
	UI_Base::Render();
}

void UI_Quest::SetTitle(char* title)
{
	UI_Text* titleUI = (UI_Text*)GetChildByID(TEXT_TITLE);
	titleUI->SetText(title);
}

void UI_Quest::SetDescription(char* desc)
{
	UI_Text* descUI = (UI_Text*)GetChildByID(TEXT_DESC);
	descUI->SetText(desc);
}

void UI_Quest::SetQuestStep(QUEST_STEP step)
{
	switch (step)
	{
	case QUEST_NOT_ACCEPT:
		SetDescription("왼쪽 블럭을 클릭해보자.");
		break;
	case QUEST_ACCEPT:
		SetDescription("왼쪽 블럭을 클릭해보자.\n(진행중)");
		break;
	case QUEST_DONE:
		SetDescription("이 퀘스트는 완료되었음.");
		break;
	default:
		break;
	}
	questStep = step;
}

LRESULT UI_Quest::InputProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (isShow != true)
	{
		return 0;
	}

	LRESULT result = 0;
	result = UI_Base::InputProc(hWnd, message, wParam, lParam);
	if (result == 0)
	{
		RECT rect = { (int)world._41, (int)world._42, (int)(world._41 + size.x), (int)(world._42 + size.y) };
		switch (message)
		{
		case WM_LBUTTONDOWN:
		{
			POINT mousePoint;
			mousePoint.x = GET_X_LPARAM(lParam);
			mousePoint.y = GET_Y_LPARAM(lParam);
			if (mousePoint.x > rect.left && mousePoint.x < rect.right
				&& mousePoint.y > rect.top && mousePoint.y < rect.bottom)
			{
				//눌러졌다. 드래그 시작
				prevMouse.x = mousePoint.x;
				prevMouse.y = mousePoint.y;
				isDrag = true;

				result = WM_LBUTTONDOWN;
			}
		}
		break;
		case WM_LBUTTONUP:
		{
			isDrag = false;
		}
		break;
		case WM_RBUTTONDOWN:
		{
			POINT mousePoint;
			mousePoint.x = GET_X_LPARAM(lParam);
			mousePoint.y = GET_Y_LPARAM(lParam);
			if (mousePoint.x > rect.left && mousePoint.x < rect.right
				&& mousePoint.y > rect.top && mousePoint.y < rect.bottom)
			{
				result = WM_RBUTTONDOWN;
			}
		}
		break;
		case WM_MOUSEMOVE:
		{
			POINT mousePoint;
			mousePoint.x = GET_X_LPARAM(lParam);
			mousePoint.y = GET_Y_LPARAM(lParam);

			if (isDrag == true)
			{
				position.x += (float)(mousePoint.x - prevMouse.x);
				position.y += (float)(mousePoint.y - prevMouse.y);

				prevMouse.x = mousePoint.x;
				prevMouse.y = mousePoint.y;
			}
		}
		break;
		}

	}
	return result;
}
