#include "stdafx.h"
#include "Eire.h"
#include "../../../Shop/Shop.h"

Eire::Eire() { CurrentFacialExpression0 = EIRE_NORMAL; /*CurrentDialogue0 = ;*/ }
Eire::~Eire() {}

void Eire::Initialize()
{
	NPC::Initialize();

	Reset();

	CurrentFacialExpression0 = EIRE_NORMAL;
	FacialExpressionCount = EIRE_FACIAL_EXPRESSION_COUNT;
	
	CurrentDialogue0 = DIALOGUE_0;

	DialogueProcedure = DIALOGUE_0;
	DialogueRange = DIALOGUE_COUNT;



	char fullPath[256];

	sprintf_s(fullPath, "%s%s", EIRE_DIRECTORY, "eire.dds");
	AddPortrait(fullPath);
	
	NPCName->SetText("에이레");

	AddDialogue("네~ 나가요~");
	AddDialogue("어머! 어서오세요! 손님.");
	AddDialogue("아, 티이를 찾아오셨다고요?");
	AddDialogue("어쩌죠? 티이는 오늘 예약이 다 차서...");
	AddDialogue("다른 아이는 어떠세요? 로리부터 거유까지 여러 취향으로...");
	AddDialogue("네? 예약이라니 무슨 말씀이냐고요?");
	AddDialogue("어머 손님~ 다 아시면서.");
	AddDialogue("네? 그런 얘기가 아니라 서큐버스를 쓰러뜨리셨다고요?");
	AddDialogue("예? 그런 데인 줄 모르셨다고요?");
	AddDialogue("에, 그러니까, '그그그렇고그런거'엔 전혀 관심이 없으시다고요?");
	AddDialogue("아... 미성년자시라고요...");
	AddDialogue(".......");
	AddDialogue("(정적이 흘렀다.)");
	AddDialogue(".......");
	AddDialogue("아하하, 어 어쨌든 서큐버스를 퇴치해 주셔서 고마워요.");
	AddDialogue("그럼 다음에도 또 부탁드릴게요.");
	AddDialogue("(문이 닫혔다.)");
	AddDialogue("......");
	AddDialogue("(리시타는 한동안 그 자리에 멍하니 있었다.)");
	AddDialogue("(칼에 묻은 서큐버스의 피가 뜨거운 햇살 아래 말라가는 가운데)");
	AddDialogue("(그렇게, 리시타의 순정은 나이들어 간다...)");

	//AddDialogue("섹스...?");
	//AddDialogue("......변태새끼...");

	ShowPortrait = 1;

	Dialogue->SetText(*(DialoguesV[CurrentDialogue0]));
	Dialogue->SetID(*(DialogueTypesV[CurrentDialogue0]));
}

void Eire::Reset()
{
}

void Eire::Update()
{
	NPC::Update();

	switch (CurrentDialogue0)
	{
	case DIALOGUE_0:
		ShowPortrait = 1;
		DialogueRange = DIALOGUE_16;
		break;
	case DIALOGUE_12:
		ShowPortrait = 0;
		break;
	case DIALOGUE_13:
		ShowPortrait = 1;
		break;
	case DIALOGUE_16:
		ShowPortrait = 0;
		NPCName->SetText("");
		DialogueProcedure = DIALOGUE_17;
		DialogueRange = DIALOGUE_20;
		break;
	}
}

void Eire::Render()
{
	if (ShowPortrait) PortraitsV[CurrentFacialExpression0]->Render();
	
	NPC::Render();
}
