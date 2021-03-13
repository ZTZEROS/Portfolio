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

	NPCName->SetText("티이");

	//AddDialogue("섹스?");
	//AddDialogue("다이스키!");
	//AddDialogue("");
	
	AddDialogue("어서오세요 모험자님.");
	AddDialogue("부른다고 또 호구같이 와주셔서 정말 고마워요.");
	AddDialogue("후후...");
	AddDialogue("마족 퇴치를 부탁드리려고 하는데, 괜찮으실까요?");
	AddDialogue("그게... 저희 집 주변에 서큐버스가 은신처를 차려 버렸어요.");
	AddDialogue("바보같이 또 오란다고 거길 가서 자지가 말라비틀어지도록 정액을 빨리고 좆뿌리까지 뽑힌 머저리 고자새끼들이 한둘이 아니예요.");
	AddDialogue("차라리 저한테...아니아니아니 에엣취! 엣취!");
	//AddDialogue("(...모험자는 만감이 교차했다.)");
	AddDialogue("에엣취! 엣취히! 크하흐윽! 엣취!");
	AddDialogue("............");
	AddDialogue("덩달아 저한테까지 이상한 소문이 퍼지고...");
	AddDialogue("흑흑....");
	AddDialogue("부탁드려요 모험자님.");
	AddDialogue("서큐버스를 퇴치해 주세요. 시집도 못갔는데... 이대로면 제 이미지가 엉망이 돼요.");

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
	TradeButton->SetText("소지품");
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
