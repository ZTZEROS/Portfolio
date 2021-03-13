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
	
	NPCName->SetText("���̷�");

	AddDialogue("��~ ������~");
	AddDialogue("���! �������! �մ�.");
	AddDialogue("��, Ƽ�̸� ã�ƿ��̴ٰ��?");
	AddDialogue("��¼��? Ƽ�̴� ���� ������ �� ����...");
	AddDialogue("�ٸ� ���̴� �����? �θ����� �������� ���� ��������...");
	AddDialogue("��? �����̶�� ���� �����̳İ��?");
	AddDialogue("��� �մ�~ �� �ƽø鼭.");
	AddDialogue("��? �׷� ��Ⱑ �ƴ϶� ��ť������ �����߸��̴ٰ��?");
	AddDialogue("��? �׷� ���� �� �𸣼̴ٰ��?");
	AddDialogue("��, �׷��ϱ�, '�ױױ׷���׷���'�� ���� ������ �����ôٰ��?");
	AddDialogue("��... �̼����ڽö���...");
	AddDialogue(".......");
	AddDialogue("(������ �귶��.)");
	AddDialogue(".......");
	AddDialogue("������, �� ��·�� ��ť������ ��ġ�� �ּż� ������.");
	AddDialogue("�׷� �������� �� ��Ź�帱�Կ�.");
	AddDialogue("(���� ������.)");
	AddDialogue("......");
	AddDialogue("(����Ÿ�� �ѵ��� �� �ڸ��� ���ϴ� �־���.)");
	AddDialogue("(Į�� ���� ��ť������ �ǰ� �߰ſ� �޻� �Ʒ� ���󰡴� ���)");
	AddDialogue("(�׷���, ����Ÿ�� ������ ���̵�� ����...)");

	//AddDialogue("����...?");
	//AddDialogue("......���»���...");

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
