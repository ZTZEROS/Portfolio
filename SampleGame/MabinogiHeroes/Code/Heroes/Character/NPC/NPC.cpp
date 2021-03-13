#include "stdafx.h"
#include "NPC.h"

NPC::NPC() { PortraitsV.clear(); DialoguesV.clear(); DialogueTypesV.clear(); DialogueBox = nullptr; DialogueBoxBackground = nullptr; NPCName = nullptr; Dialogue = nullptr; Talking = 0; }
NPC::~NPC() {}

void NPC::Initialize()
{
	Reset();

	if (!NPCName)
	{
		NPCName = new UI_Text;
		NPCName->SetFont(UI_Manager::GetNPCNameFont());
		NPCName->SetPosition(10.0f, -5.0f, 0.0f);
		NPCName->SetSize(312, 70);
		NPCName->SetTextColor(D3DCOLOR_XRGB(255, 255, 0));
		NPCName->SetTextFormat(DT_LEFT | DT_VCENTER | DT_WORDBREAK);
		NPCName->SetRect();
		NPCName->SetID(EVENT_DIALOGUE);
	}

	if (!Dialogue)
	{
		Dialogue = new UI_Text;
		Dialogue->SetFont(UI_Manager::GetNPCDialogueFont());
		Dialogue->SetPosition(10.0f, 50.0f, 0.0f);
		Dialogue->SetSize(512, 96);
		Dialogue->SetTextColor(D3DCOLOR_XRGB(255, 255, 255));
		Dialogue->SetTextFormat(DT_LEFT | DT_TOP | DT_WORDBREAK);
		Dialogue->SetRect();
		Dialogue->SetID(EVENT_DIALOGUE);
	}

	if (!DialogueBox)
	{
		DialogueBox = new UI_Image;

		char fullPath[256];
		strcpy_s(fullPath, NPC_DIRECTORY);
		strcat_s(fullPath, "DialogueBox.dds");

		DialogueBox->SetTexture(fullPath);
		DialogueBox->SetPosition(DialogueBox->GetSize().x * 0.0075f, DialogueBox->GetSize().y * 0.04f, 0.0f);
		DialogueBox->SetRect();
		DialogueBox->AddChild(NPCName);
		DialogueBox->AddChild(Dialogue);
		//NPCName->SetSize(DialogueBox->GetSize().x, DialogueBox->GetSize().y);
		//Dialogue->SetSize(DialogueBox->GetSize().x, DialogueBox->GetSize().y);
	}

	if (!DialogueBoxBackground)
	{
		DialogueBoxBackground = new UI_Image;

		char fullPath[256];
		strcpy_s(fullPath, NPC_DIRECTORY);
		strcat_s(fullPath, "DialogueBoxBackground.dds");

		DialogueBoxBackground->SetTexture(fullPath);
		DialogueBoxBackground->SetPosition(0.0f, RESOLUTION_Y - DialogueBoxBackground->GetSize().y, 0.0f);
		DialogueBoxBackground->SetRect();
		DialogueBoxBackground->AddChild(DialogueBox);
	}



	CurrentFacialExpression0 = 0;
	FacialExpressionCount = 0;
	
	CurrentDialogue0 = 0;
	
	DialogueProcedure = 0;
	DialogueRange = 0;
}

void NPC::Destroy()
{
	SAFE_DELETE(DialogueBoxBackground);
	//SAFE_DELETE(DialogueBox);
	//SAFE_DELETE(Dialogue);
	//SAFE_DELETE(NPCName);

	//for (DialoguesVI = DialoguesV.rbegin(); DialoguesVI != DialoguesV.rend(); ++DialoguesVI)
	for (DialogueTypesVI = DialogueTypesV.begin(); DialogueTypesVI != DialogueTypesV.end(); ++DialogueTypesVI)
	{
		delete (*DialogueTypesVI);
	}

	for (DialoguesVI = DialoguesV.begin(); DialoguesVI != DialoguesV.end(); ++DialoguesVI)
	{
		delete (*DialoguesVI);
	}
	
	DialoguesV.clear();

	for (PortraitsVI = PortraitsV.begin(); PortraitsVI != PortraitsV.end(); ++PortraitsVI)
	{
		SAFE_DELETE(*PortraitsVI);
	}

	PortraitsV.clear();
	SAFE_DELETE(itemShop);
}

void NPC::Reset()
{
}

void NPC::Update()
{
	for (PortraitsVI = PortraitsV.begin(); PortraitsVI != PortraitsV.end(); ++PortraitsVI)
	{
		(*PortraitsVI)->Update();
	}

	DialogueBoxBackground->Update();

	if (itemShop != nullptr)
	{
		itemShop->Update();
	}
}

void NPC::Render()
{
	DialogueBoxBackground->Render();
	if (itemShop != nullptr)
	{
		itemShop->Render();
	}
}

LRESULT NPC::InputProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result;

	result = 0;
	if (itemShop != nullptr)
	{
		itemShop->InputProc(hWnd, message, wParam, lParam);
		TradeButton->InputProc(hWnd, message, wParam, lParam);
	}
	if (result == 0 && Talking)
	{
		D3DXMATRIX world = DialogueBox->GetWorld();
		D3DXVECTOR2 size = DialogueBox->GetSize();
		RECT dialogueBoxRect = { (int)world._41, (int)world._42, (int)(world._41 + size.x), (int)(world._42 + size.y) };

		switch (message)
		{
		case WM_LBUTTONDOWN:
		{
			POINT mousePoint;
			mousePoint.x = GET_X_LPARAM(lParam);
			mousePoint.y = GET_Y_LPARAM(lParam);

			if (mousePoint.x > dialogueBoxRect.left && mousePoint.x < dialogueBoxRect.right && mousePoint.y > dialogueBoxRect.top && mousePoint.y < dialogueBoxRect.bottom)
			{
				Talk();
				result = WM_LBUTTONDOWN;
			}
		}
		break;
		}
	}

	return result;
}

void NPC::AddPortrait(char* fileName)
{
	UI_Image* portrait;
	portrait = new UI_Image;

	char fullPath[256];
	strcpy_s(fullPath, NPC_DIRECTORY);
	strcat_s(fullPath, fileName);

	portrait->SetTexture(fullPath);
	portrait->SetPosition(0.0f, RESOLUTION_Y - portrait->GetSize().y * 0.985f - DialogueBoxBackground->GetSize().y, 0.0f);
	PortraitsV.push_back(portrait);
}

void NPC::AddDialogue(std::string text, CurrentDialogueType currentDialogueType)
{
	string* dialogue;
	dialogue = new string;
	*dialogue = text;

	CurrentDialogueType* dialogueType;
	dialogueType = new CurrentDialogueType;
	*dialogueType = currentDialogueType;

	DialoguesV.push_back(dialogue);
	DialogueTypesV.push_back(dialogueType);
}

void NPC::Talk()
{
	//if (CurrentDialogue0 == DialogueRange) Talking = 0;

	if (*(DialogueTypesV[DialogueProcedure]) == EVENT_DIALOGUE) CurrentDialogue0 = DialogueProcedure + (CurrentDialogue0 - DialogueProcedure + 1) % (DialogueRange - DialogueProcedure + 1);
	else CurrentDialogue0 = RandomUtil::GetInteger(DialogueRange, DialogueProcedure);
	
	Dialogue->SetText(*(DialoguesV[CurrentDialogue0]));
	Dialogue->SetID(*(DialogueTypesV[CurrentDialogue0]));
}

void NPC::ResetDialogue()
{
	CurrentDialogue0 = DialogueProcedure;

	Dialogue->SetText(*(DialoguesV[CurrentDialogue0]));
	Dialogue->SetID(*(DialogueTypesV[CurrentDialogue0]));
}

void NPC::ItemSetting()
{

}
