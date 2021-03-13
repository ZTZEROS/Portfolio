#include "stdafx.h"
#include "Ti.h"

Ti::Ti() {}
Ti::~Ti() {}

void Ti::Initialize()
{
	NPC::Initialize();

	Reset();

	CurrentFacialExpression0 = TI_NORMAL;
	FacialExpressionCount = TI_FACIAL_EXPRESSION_COUNT;
	
	CurrentDialogue0 = DIALOGUE_0;

	DialogueProcedure = DIALOGUE_0;
	DialogueRange = DIALOGUE_COUNT;
	
	char fullPath[256];

	sprintf_s(fullPath, "%s%s", TI_DIRECTORY, "ti.dds");
	AddPortrait(fullPath);

	sprintf_s(fullPath, "%s%s", TI_DIRECTORY, "ti_smile_normal.dds");
	AddPortrait(fullPath);

	sprintf_s(fullPath, "%s%s", TI_DIRECTORY, "ti_smile_teeth.dds");
	AddPortrait(fullPath);

	sprintf_s(fullPath, "%s%s", TI_DIRECTORY, "ti_morrighan.dds");
	AddPortrait(fullPath);

	sprintf_s(fullPath, "%s%s", TI_DIRECTORY, "ti_morrighan_sad.dds");
	AddPortrait(fullPath);

	sprintf_s(fullPath, "%s%s", TI_DIRECTORY, "ti_morrighan_smile.dds");
	AddPortrait(fullPath);

	sprintf_s(fullPath, "%s%s", TI_DIRECTORY, "ti_xmas.dds");
	AddPortrait(fullPath);

	sprintf_s(fullPath, "%s%s", TI_DIRECTORY, "ti_smile_normal_xmas.dds");
	AddPortrait(fullPath);

	sprintf_s(fullPath, "%s%s", TI_DIRECTORY, "ti_smile_teeth_xmas.dds");
	AddPortrait(fullPath);

	NPCName->SetText("Ƽ��");

	//AddDialogue("����?");
	//AddDialogue("���̽�Ű!");
	//AddDialogue("");
	
	AddDialogue("������� �����ڴ�.");
	AddDialogue("�θ��ٰ� �� ȣ������ ���ּż� ���� ������.");
	AddDialogue("����...");
	AddDialogue("���� ��ġ�� ��Ź�帮���� �ϴµ�, �������Ǳ��?");
	AddDialogue("�װ�... ���� �� �ֺ��� ��ť������ ����ó�� ���� ���Ⱦ��.");
	AddDialogue("�ٺ����� �� �����ٰ� �ű� ���� ������ �����Ʋ�������� ������ ������ ���Ѹ����� ���� ������ ���ڻ������� �ѵ��� �ƴϿ���.");
	AddDialogue("���� ������...�ƴϾƴϾƴ� ������! ����!");
	//AddDialogue("(...�����ڴ� ������ �����ߴ�.)");
	AddDialogue("������! ������! ũ������! ����!");
	AddDialogue("............");
	AddDialogue("���޾� �����ױ��� �̻��� �ҹ��� ������...");
	AddDialogue("����....");
	AddDialogue("��Ź����� �����ڴ�.");
	AddDialogue("��ť������ ��ġ�� �ּ���. ������ �����µ�... �̴�θ� �� �̹����� ������ �ſ�.");

	Dialogue->SetText(*(DialoguesV[CurrentDialogue0]));
	Dialogue->SetID(*(DialogueTypesV[CurrentDialogue0]));

	ShowPortrait = 1;

	ItemSetting();
}

void Ti::Reset()
{
}

void Ti::Update()
{
	NPC::Update();


	switch (CurrentDialogue0)
	{
	case DIALOGUE_0:
		ShowPortrait = 1;
		DialogueRange = DIALOGUE_9;
		break;
	case DIALOGUE_1:
		CurrentFacialExpression0 = TI_SMILE_TEETH;
		break;
	case DIALOGUE_2:
		CurrentFacialExpression0 = TI_SMILE_NORMAL;
		break;
	case DIALOGUE_4:
		CurrentFacialExpression0 = TI_NORMAL;
		break;
	case DIALOGUE_7:
		ShowPortrait = 0;
		break;
	case DIALOGUE_8:
		ShowPortrait = 1;
		break;
	case DIALOGUE_9:	
		DialogueProcedure = DIALOGUE_10;
		DialogueRange = DIALOGUE_12;
		break;
	}

}

void Ti::Render()
{
	if(ShowPortrait) PortraitsV[CurrentFacialExpression0]->Render();
	
	NPC::Render();
}

void Ti::ItemSetting()
{
	itemShop = new ItemShop;
	TradeButton = new UI_Button;
	TradeButton->SetTexture("./UI/ButtonUp.dds", "./UI/ButtonMiddle.dds", "./UI/ButtonDown.dds");
	TradeButton->SetPosition(475, 123, 0);
	TradeButton->SetScale(1.0);
	TradeButton->SetText("����ǰ");
	TradeButton->OnClick = std::bind(&UI_Functions::SetItem, std::ref(UI_Manager::Get().func), itemShop);
	TradeButton->SetFont(UI_Manager::GetItemFont());
	TradeButton->SetTextPosition(10, 7, 0);
	DialogueBoxBackground->AddChild(TradeButton);

	itemShop->ItemInit();
	itemShop->AddItem("IA_Armageddon");
	itemShop->AddItem("IF_Armageddon");
	itemShop->AddItem("IH_Armageddon");
	itemShop->AddItem("IU_Armageddon");
	itemShop->AddItem("IL_Armageddon");
	itemShop->AddItem("IW_DualSword80");
	itemShop->AddItem("testPotion");
	itemShop->SetInven(UI_Mnager::Get().GetInventory());

	UI_Mnager::Get().GetInventory()->SettingItemShop(itemShop);

	itemShop->Initilize();
}
