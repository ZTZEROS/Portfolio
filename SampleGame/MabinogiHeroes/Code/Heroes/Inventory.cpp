#include "stdafx.h"
#include "Inventory.h"
#include "ItemShop.h"

Inventory::Inventory()
{
}


Inventory::~Inventory()
{
}

void Inventory::Initilize()
{
	_hdc = GetDC(HWnd[0]);
	clickButton = new Item_Button;
	clickButton->SetShow(false);
	for (int i = 0; i < Inven_End; ++i)
	{
		ItemCount[i] = 0;
	}

	InvenInit();
	WindowInit();
	SetCurrentInven(Inven_Equip);
	AddItem("IW_DualSword1");
	AddItem("IW_DualSword1");
	AddItem("IA_NightHawk");
	AddItem("IF_NightHawk");
	AddItem("IU_NightHawk");
	AddItem("IL_NightHawk");
	AddItem("IH_NightHawk");
	AddItem("testPotion");
	AddItem("testPotion");
	AddItem("testPotion");
	AddItem("testPotion");

	QuickSlot->Update();
	InvenWindow->Update();
	if (clickButton->GetIsShow())
	{
		clickButton->SetPosition(GameManager::Get().getMousePoint().x - clickButton->GetSize().x / 2, GameManager::Get().getMousePoint().y - clickButton->GetSize().y / 2, 0);
		clickButton->Update();

	}
}

void Inventory::Destroy()
{
	InvenWindow->Destroy();
	clickButton->Destroy();
	QuickSlot->Destroy();

	InvenItem[0].clear();
	InvenItem[1].clear();
	InvenItem[2].clear();
}

void Inventory::Render()
{
	InvenWindow->Render();
	clickButton->Render();
	
	char text[256];
	sprintf_s(text, "%d", CharacData::GetUnEqipInfo(CH_INFOMATION::CH_GOLD));
	
	RECT tempRc = { (int)(goldImage->GetWorld()._41+20) ,(int)(goldImage->GetWorld()._42+3), 
		(int)(goldImage->GetWorld()._41 + 101), (int)(goldImage->GetWorld()._41 + 2) };
	UI_Manager::GetItemFont()->DrawTextA(nullptr, text, strlen(text), &tempRc, DT_TOP | DT_LEFT | DT_NOCLIP, D3DCOLOR_XRGB(255, 255, 255));
	
	//�κ� ����
	for (int i = 0; i < 48; ++i)
	{
			if (strcmp(InvenItem[currentInven][i]->GetItemName(), "Null") != 0 && PtInRect(&InvenItem[currentInven][i]->getRect(), GameManager::Get().getMousePoint())) //���콺�� ��Ʈ �ȿ� ������
			{
				UI_Image* ti = new UI_Image;
				ti->SetScale(2);
				ti->SetTexture("./UI/ItemToolTip.dds");
				RECT temp = InvenItem[currentInven][i]->getRect();
				temp.left += (int)InvenItem[currentInven][i]->GetSize().x;
				temp.right += (int)InvenItem[currentInven][i]->GetSize().x;
				temp.top += (int)InvenItem[currentInven][i]->GetSize().y;
				temp.bottom += (int)InvenItem[currentInven][i]->GetSize().y;
				char text[1024];
				ti->SetPosition((float)temp.left-20, (float)temp.top-20, 0);
				ti->Update();
				ti->Render();
				sprintf_s(text, "\n\n�̸� : %s\n\n���·� : %d\n\n���� : %d\n\n�������� : %d\n\n���� : %d", InvenItem[currentInven][i]->GetItemName(), 
					InvenItem[currentInven][i]->GetItem().atk, InvenItem[currentInven][i]->GetItem().def, InvenItem[currentInven][i]->GetItem().level, InvenItem[currentInven][i]->GetItem().gold);
				UI_Manager::GetItemFont()->DrawTextA(NULL, text, strlen(text), &temp, DT_TOP | DT_LEFT | DT_NOCLIP, D3DCOLOR_XRGB(255, 255, 255));

			}
	}
}

void Inventory::Update()
{
	//if (!isShow) return;
	clickButton->SetPosition(GameManager::Get().getMousePoint().x - clickButton->GetSize().x / 2, GameManager::Get().getMousePoint().y - clickButton->GetSize().y / 2, 0);
	clickButton->Update();
	//CheckKey();
	//QuickSlot->Update();
	InvenWindow->Update();
	
}

LRESULT Inventory::InputProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	InvenWindow->InputProc(hWnd, message, wParam, lParam);
	QuickSlot->InputProc(hWnd, message, wParam, lParam);
	CheckMouse();

	switch (message)
	{
	case WM_LBUTTONDOWN:
	{
		CheckMouse();
		QuickSlot->Update();
		InvenWindow->Update();
		if (clickButton->GetIsShow())
		{
			clickButton->SetPosition(GameManager::Get().getMousePoint().x - clickButton->GetSize().x / 2, GameManager::Get().getMousePoint().y - clickButton->GetSize().y / 2, 0);
		}

	}
	break;
	case WM_LBUTTONUP:
	{
		CheckMouse();
		clickButton->SetShow(false);
		//QuickSlot->Update();
		//InvenWindow->Update();
		if (clickButton->GetIsShow())
		{
			clickButton->SetPosition(GameManager::Get().getMousePoint().x - clickButton->GetSize().x / 2, GameManager::Get().getMousePoint().y - clickButton->GetSize().y / 2, 0);
		//	clickButton->Update();
		}
	}
	break;

	case WM_RBUTTONDOWN:
	{
		CheckMouse();
	//	QuickSlot->Update();
	//	InvenWindow->Update();
		if (clickButton->GetIsShow())
		{
			clickButton->SetPosition(GameManager::Get().getMousePoint().x - clickButton->GetSize().x / 2, GameManager::Get().getMousePoint().y - clickButton->GetSize().y / 2, 0);
		//	clickButton->Update();
		}

	}
	break;
	case WM_RBUTTONUP:
	{
		CheckMouse();
	//	QuickSlot->Update();
	//	InvenWindow->Update();
		if (clickButton->GetIsShow())
		{
			clickButton->SetPosition(GameManager::Get().getMousePoint().x - clickButton->GetSize().x / 2, GameManager::Get().getMousePoint().y - clickButton->GetSize().y / 2, 0);
		//	clickButton->Update();
		}
	}
	break;
	case WM_MOUSEMOVE:
	{
	}
	break;
	}
return 0;
}

void Inventory::AddItem(char * name)
{
	stItem temp;
	temp = ItemData::Get().findItem(name);
	if (temp.name)
	{
		switch (ItemData::Get().findItem(name).kind)
		{
		case Item_Potion:
			for (int i = 0; i < InvenCount; ++i)
			{
				if (strcmp(InvenItem[Inven_Potion][i]->GetItemName(), "Null") == 0) //���� ��ĭ�̶��
				{
					InvenItem[Inven_Potion][i]->SetItem(name);
					break;
				}
			}
			break;

		case Item_Source:
			for (int i = 0; i < InvenCount; ++i)
			{
				if (strcmp(InvenItem[Inven_Source][i]->GetItemName(), "Null") == 0) //���� ��ĭ�̶��
				{
					InvenItem[Inven_Source][i]->SetItem(name);
					break;
				}
			}
			break;

		default:
			for (int i = 0; i < InvenCount; ++i)
			{
				if (strcmp(InvenItem[Inven_Equip][i]->GetItemName(), "Null") == 0) //���� ��ĭ�̶��
				{
					InvenItem[Inven_Equip][i]->SetItem(name);
					break;
				}
			}
			break;
		}
		ItemPositionSetting();
		SetCurrentInven(currentInven);
	}
}

void Inventory::DropItem()
{
}

void Inventory::SellItem()
{
}

void Inventory::CheckKey()
{
	if ((SceneManager::GetCurrentScene() == SceneType::SCENE_DUNGEON) || (SceneManager::GetCurrentScene() == SceneType::SCENE_TEST)) //������ ���
	{
		for (int i = 0; i < 10; ++i)
		{
			if (GameManager::GetKey()->isOnceKeyDown(48 + i)) //Ű���� 0~9���� ��ư ������
			{
				if (i == 0)
				{
					CharacData::SetInfomation(CH_INFOMATION::CH_HP, QuickItem[9]->GetItem().atk);
					CharacData::GetLethita()->SetCurrentState(CHARIC_STATE::STATE_POTION);
					if(strcmp( QuickItem[9]->GetItemName(), "Null") !=0) QuickItem[9]->SetKind(Item_Kind::Item_Potion);
					QuickItem[9]->SetItem("Null");
				}
				else
				{
					CharacData::SetInfomation(CH_INFOMATION::CH_HP, QuickItem[i - 1]->GetItem().atk);
					CharacData::GetLethita()->SetCurrentState(CHARIC_STATE::STATE_POTION);
					if (strcmp(QuickItem[i-1]->GetItemName(), "Null") != 0) QuickItem[9]->SetKind(Item_Kind::Item_Potion);
					QuickItem[i - 1]->SetItem("Null");
				}
			}
		}
	}
	
	
}

void Inventory::CheckMouse()
{
	//��� ����
	for (int i = 0; i < Item_Kind::Item_Potion; ++i) //��� ����
	{
		if (strcmp("Null", equipItem[i]->GetItemName()) == 0) continue; //����ĭ�� ��ĭ�� ��� ����

		switch (equipItem[i]->GetMouseState())
		{
		case Mouse_Ldown:
			equipItem[i]->SetAlpha(true);
			clickButton->SetShow(true);
			clickButton->SetItem(equipItem[i]->GetItemName());
			break;

		case Mouse_Lup:
			equipItem[i]->SetMouseState(Mouse_State::Mouse_Nomal);//���콺�� ��ַ� �����ְ�
			for (int j = 0; j < InvenCount; ++j) //��� -> �κ�
			{
				if (InvenItem[currentInven][j]->GetMouseState() == Mouse_Lup //�κ��丮 �����۵� Ȯ�����ְ�
					&& (PtInRect(&InvenItem[currentInven][j]->getRect(), GameManager::Get().getMousePoint()))) //�κ��丮 ���� ������
				{
					char* temName = equipItem[i]->GetItemName();
					equipItem[i]->SetItem(InvenItem[currentInven][j]->GetItemName());
					CharacData::SetInfomation(CH_INFOMATION::CH_ATK, equipItem[i]->GetItem().atk);
					CharacData::SetInfomation(CH_INFOMATION::CH_DEF, equipItem[i]->GetItem().def);
					CharacData::GetLethita()->ItemEquip((Item_Kind)i, equipItem[i]->GetItemName());
					InvenItem[currentInven][j]->SetItem(temName);
					equipItem[i]->SetKind((Item_Kind)i);
					return;
				}
			}
			break;
		}
	}

	//�κ�����
	for (int j = 0; j < InvenCount; ++j) //�κ� ����
	{
		if (strcmp("Null", InvenItem[currentInven][j]->GetItemName()) == 0) continue; //�κ��� ��ĭ�� ��� ����

		switch (InvenItem[currentInven][j]->GetMouseState())
		{
		case Mouse_Ldown:
			InvenItem[currentInven][j]->SetAlpha(true);
			clickButton->SetShow(true);
			clickButton->SetItem(InvenItem[currentInven][j]->GetItemName());
			break;

		case Mouse_Lup:
			InvenItem[currentInven][j]->SetMouseState(Mouse_State::Mouse_Nomal);
			if (clickButton->GetIsShow())//Ŭ�� �Ǿ� �ֵ���
			{
				for (int i = 0; i < Item_Kind::Item_Potion; ++i)//�巡�� ����
				{
					if (PtInRect(&equipItem[i]->getRect(), GameManager::Get().getMousePoint()) && (InvenItem[currentInven][j]->GetItem().kind < Item_Potion)//����Ʈ�� �� �ڸ��� �ְ�, ������ ���� ���� �ƴϰ�
						&& (i == (int)equipItem[InvenItem[currentInven][j]->GetItem().kind]->GetItem().kind))//������ ���̶� ������
					{
						char* temName = equipItem[i]->GetItemName();
						equipItem[i]->SetItem(InvenItem[currentInven][j]->GetItemName());
						CharacData::SetInfomation(CH_INFOMATION::CH_ATK, equipItem[i]->GetItem().atk);
						CharacData::SetInfomation(CH_INFOMATION::CH_DEF, equipItem[i]->GetItem().def);
						CharacData::GetLethita()->ItemEquip((Item_Kind)i, equipItem[i]->GetItemName());
						InvenItem[currentInven][j]->SetItem(temName);
						return;
					}
				}

				for (int v = 0; v < InvenCount; ++v)//�巡�� �̵�, ĭĭ ����
				{
					if (PtInRect(&InvenItem[currentInven][v]->getRect(), GameManager::Get().getMousePoint()) && (j != v)) //�ٸ�ĭ�� Ŭ��ĭ ���Ͻ�, ���� ĭ�� �ƴҽ�
					{
						char* temName = InvenItem[currentInven][j]->GetItemName();
						InvenItem[currentInven][j]->SetItem(InvenItem[currentInven][v]->GetItemName());
						InvenItem[currentInven][v]->SetItem(temName);
					}
				}

				if (currentInven != Inven_Kind::Inven_Potion) continue;
				
				for (int i = 0; i < 12; ++i)//�κ� -> ������(���Ǹ� ����)
				{
					if (PtInRect(&QuickItem[i]->getRect(), GameManager::Get().getMousePoint())) //�ٸ�ĭ�� Ŭ��ĭ ���Ͻ�, ���� ĭ�� �ƴҽ�
					{
						char* temName = InvenItem[currentInven][j]->GetItemName();
						InvenItem[currentInven][j]->SetItem(QuickItem[i]->GetItemName());
						QuickItem[i]->SetItem(temName);
					}
				}
			}
			break;

		case Mouse_Rdown:
		{
			InvenItem[currentInven][j]->SetState(Mouse_Nomal);

			if (itemShop != nullptr && itemShop->GetIsShow()) //������ ���� ���� �ִٸ�
			{
				CharacData::SetInfomation(CH_INFOMATION::CH_GOLD, (int)(InvenItem[currentInven][j]->GetItem().gold * 0.8f));
				InvenItem[currentInven][j]->SetItem("Null");
			}

			else
			{
				for (int i = 0; i < Item_Kind::Item_Potion; ++i)
				{
					if ((InvenItem[currentInven][j]->GetItem().kind < Item_Potion) //���Ǻ��� ������ ������ ����
						&& (i == (int)equipItem[InvenItem[currentInven][j]->GetItem().kind]->GetItem().kind)) //�κ� �������̶� ������°� ������
					{
						char* temName = equipItem[i]->GetItemName();
						equipItem[i]->SetItem(InvenItem[currentInven][j]->GetItemName());
						CharacData::SetInfomation(CH_INFOMATION::CH_ATK, equipItem[i]->GetItem().atk);
						CharacData::SetInfomation(CH_INFOMATION::CH_DEF, equipItem[i]->GetItem().def);
						CharacData::GetLethita()->ItemEquip((Item_Kind)i, equipItem[i]->GetItemName());
						InvenItem[currentInven][j]->SetItem(temName);
						return;
					}
				}

				if (currentInven != Inven_Kind::Inven_Potion) return;
				{
					CharacData::SetInfomation(CH_INFOMATION::CH_HP, InvenItem[currentInven][j]->GetItem().atk);
					CharacData::GetLethita()->SetCurrentState(CHARIC_STATE::STATE_POTION);
					InvenItem[currentInven][j]->SetItem("Null");
					InvenItem[currentInven][j]->SetKind(Item_Kind::Item_Potion);
				}
			}
			
		}
		break;
		}
	}

}

void Inventory::QickSlotCheckMouse()
{
	//������ ����
	//if (currentInven != Inven_Kind::Inven_Potion) return;
	for (int i = 0; i < 12; ++i)
	{
		if (strcmp("Null", QuickItem[i]->GetItemName()) == 0) continue; //����ĭ�� ��ĭ�� ��� ����

		switch (QuickItem[i]->GetMouseState())
		{
		case Mouse_Ldown:
			QuickItem[i]->SetAlpha(true);
			clickButton->SetShow(true);
			clickButton->SetItem(QuickItem[i]->GetItemName());
			break;

		case Mouse_Lup:
			QuickItem[i]->SetMouseState(Mouse_State::Mouse_Nomal);//���콺�� ��ַ� �����ְ�
			for (int j = 0; j < InvenCount; ++j) //��� -> �κ�
			{
				if (InvenItem[currentInven][j]->GetMouseState() == Mouse_Lup //�κ��丮 �����۵� Ȯ�����ְ�
					&& (PtInRect(&InvenItem[currentInven][j]->getRect(), GameManager::Get().getMousePoint())) && (currentInven == Inven_Kind::Inven_Potion)) //�κ��丮 ���� ������
				{
					char* temName = QuickItem[i]->GetItemName();
					QuickItem[i]->SetItem(InvenItem[currentInven][j]->GetItemName());
					CharacData::SetInfomation(CH_INFOMATION::CH_ATK, QuickItem[i]->GetItem().atk);
					CharacData::SetInfomation(CH_INFOMATION::CH_DEF, QuickItem[i]->GetItem().def);
					InvenItem[currentInven][j]->SetItem(temName);
					QuickItem[i]->SetKind((Item_Kind)i);
					return;
				}
			}

			for (int v = 0; v < 12; ++v)//�巡�� �̵�, ĭĭ ����
			{
				if (PtInRect(&QuickItem[v]->getRect(), GameManager::Get().getMousePoint()) && (i != v)) //�ٸ�ĭ�� Ŭ��ĭ ���Ͻ�, ���� ĭ�� �ƴҽ�
				{
					char* temName = QuickItem[i]->GetItemName();
					QuickItem[i]->SetItem(QuickItem[v]->GetItemName());
					QuickItem[v]->SetItem(temName);
					return;
				}
			}

			break;

		case Mouse_Rdown:
			CharacData::SetInfomation(CH_INFOMATION::CH_HP, QuickItem[i]->GetItem().atk);
			QuickItem[i]->SetItem("Null");
			QuickItem[i]->SetKind(Item_Kind::Item_Potion);
			break;
		}
	}
}

void Inventory::EquipItem(Item_Button* item)
{
}

void Inventory::WindowInit()
{
	InvenWindow = new UI_Image;
	InvenWindow->SetTexture("./UI/InvenWindow0.dds");
	InvenWindow->SetPosition(900, 100, 0);
	InvenWindow->SetIsMove(true);

	UI_Image* EqipBackGround = new UI_Image;
	EqipBackGround->SetTexture("./UI/InvenWindowEqip1.dds");
	EqipBackGround->SetPosition(-200, 0, 0);
	EqipBackGround->SetIsMove(false);
	InvenWindow->AddChild(EqipBackGround);

	UI_Image* EqipBackGround2 = new UI_Image;
	EqipBackGround2->SetTexture("./UI/InvenWindowEqip2.dds");
	EqipBackGround2->SetPosition(12, 70, 0);
	EqipBackGround2->SetIsMove(false);
	EqipBackGround->AddChild(EqipBackGround2);

	UI_Image* EqipBackGround3 = new UI_Image;
	EqipBackGround3->SetTexture("./UI/InvenWindowEqip3.dds");
	EqipBackGround3->SetPosition(11, 0, 0);
	EqipBackGround3->SetIsMove(false);
	EqipBackGround2->AddChild(EqipBackGround3);

	UI_Image* backGround = new UI_Image;
	backGround->SetTexture("./UI/InvenWindow2.dds");
	backGround->SetPosition(50, 50, 0);
	backGround->SetIsMove(false);
	InvenWindow->AddChild(backGround);

	Button[Inven_Kind::Inven_Equip] = new UI_Button;
	Button[Inven_Kind::Inven_Equip]->SetTexture("./UI/ButtonUp.dds", "./UI/ButtonMiddle.dds", "./UI/ButtonDown.dds");
	Button[Inven_Kind::Inven_Equip]->SetPosition(150, 26, 0);
	Button[Inven_Kind::Inven_Equip]->SetText("���");
	Button[Inven_Kind::Inven_Equip]->SetTextPosition(10, 5, 0);
	Button[Inven_Kind::Inven_Equip]->SetFont(UI_Manager::GetItemFont());
	InvenWindow->AddChild(Button[Inven_Kind::Inven_Equip]);

	Button[Inven_Kind::Inven_Potion] = new UI_Button;
	Button[Inven_Kind::Inven_Potion]->SetTexture("./UI/ButtonUp.dds", "./UI/ButtonMiddle.dds", "./UI/ButtonDown.dds");
	Button[Inven_Kind::Inven_Potion]->SetPosition(210, 26, 0);
	Button[Inven_Kind::Inven_Potion]->SetText("�Ҹ�ǰ");
	Button[Inven_Kind::Inven_Potion]->SetTextPosition(10, 5, 0);
	Button[Inven_Kind::Inven_Potion]->SetFont(UI_Manager::GetItemFont());
	InvenWindow->AddChild(Button[Inven_Kind::Inven_Potion]);

	Button[Inven_Kind::Inven_Source] = new UI_Button;
	Button[Inven_Kind::Inven_Source]->SetTexture("./UI/ButtonUp.dds", "./UI/ButtonMiddle.dds", "./UI/ButtonDown.dds");
	Button[Inven_Kind::Inven_Source]->SetText("���");
	Button[Inven_Kind::Inven_Source]->SetPosition(270, 26, 0);
	Button[Inven_Kind::Inven_Source]->SetTextPosition(10, 5, 0);
	Button[Inven_Kind::Inven_Source]->SetFont(UI_Manager::GetItemFont());
	InvenWindow->AddChild(Button[Inven_Kind::Inven_Source]);

	goldImage = new UI_Image;
	goldImage->SetTexture("./UI/ItemShop/ItemShopGold.dds");
	goldImage->SetPosition(202, 420, 0);
	goldImage->SetIsMove(false);
	InvenWindow->AddChild(goldImage);

	backGround1 = new UI_Image;
	backGround1->SetTexture("./UI/InvenWindow1.dds");
	backGround1->SetPosition(52, 50, 0);
	backGround1->SetIsMove(false);
	backGround1->SetID(0);
	InvenWindow->AddChild(backGround1);

	
	for (int i = 0; i < Inven_Kind::Inven_End; ++i)
	{
		Button[i]->OnClick = std::bind(&UI_Functions::SetInvenType, std::ref(UI_Manager::Get().func), this, (Inven_Kind)i);
	}
	

	for (int z = 0; z < Inven_End; ++z)
	{
		for (int i = 0; i < 8; ++i)
		{
			for (int j = 0; j < 6; ++j)
			{
				int sum = i * 6 + j;
				if (InvenItem[z][sum]->GetItem().name)
				{
					InvenItem[z][sum]->SetPosition((float)(j * 40 + 8 + j * 4), (float)(i * 40 + 8 + i * 4), 0);
					InvenItem[z][sum]->SetIsMove(true);
					int temp = z * 100 + sum;
					InvenItem[z][sum]->SetID(temp); //3�ڸ�, ù���ڸ� �κ� ����, 2,3�ڸ� ������ ĭ 
					backGround1->AddChild(InvenItem[z][sum]);
				}
			}
		}
	}

	equipItem[Item_HeadArmor]->SetPosition(-115, 80, 0);
	equipItem[Item_HeadArmor]->SetID(400);
	InvenWindow->AddChild(equipItem[Item_HeadArmor]);

	equipItem[Item_Weapon]->SetPosition(-164, 130, 0);
	equipItem[Item_Weapon]->SetID(401);
	InvenWindow->AddChild(equipItem[Item_Weapon]);

	equipItem[Item_UpperAromor]->SetPosition(-115, 130, 0);
	equipItem[Item_UpperAromor]->SetID(402);
	InvenWindow->AddChild(equipItem[Item_UpperAromor]);

	equipItem[Item_LowerArmor]->SetPosition(-115, 180, 0);
	equipItem[Item_LowerArmor]->SetID(403);
	InvenWindow->AddChild(equipItem[Item_LowerArmor]);
		
	equipItem[Item_HandArmor]->SetPosition(-66, 180, 0);
	equipItem[Item_HandArmor]->SetID(404);
	InvenWindow->AddChild(equipItem[Item_HandArmor]);

	equipItem[Item_FootArmor]->SetPosition(-115, 230, 0);
	equipItem[Item_FootArmor]->SetID(405);
	InvenWindow->AddChild(equipItem[Item_FootArmor]);

	//�� ����
	QuickSlot = new UI_Image;
	QuickSlot->SetTexture("./UI/QuickSlot.dds");
	QuickSlot->SetPosition(-30, -20, 0);

	for (int i = 0; i < 12; ++i)
	{
		if (i < 5)
		{
			QuickItem[i]->SetPosition((float)(i * 43 + 51), (float)26, 0);
			QuickSlot->AddChild(QuickItem[i]);
		}

		else if (i > 4 && i < 10)
		{
			QuickItem[i]->SetPosition((float)(i * 43 + 64), (float)26, 0);
			QuickSlot->AddChild(QuickItem[i]);
		}
		else
		{
			QuickItem[i]->SetPosition((float)(i * 43 + 80), (float)26, 0);
			QuickSlot->AddChild(QuickItem[i]);
		}
	}

	QuickSlot->Update();

}

void Inventory::InvenInit()
{
	for (int i = 0; i < Inven_End; ++i)
	{
		InvenItem[i].resize(InvenCount);
	}

	for (int i = 0; i < Inven_End; ++i)
	{
		for (int j = 0; j < InvenCount; ++j)
		{
			InvenItem[i][j] = new Item_Button;
			InvenItem[i][j]->SetItem("Null");
		}
	}

	equipItem.resize(Item_Potion);
	for (int i = 0; i < Item_Potion; ++i)
	{
		equipItem[i] = new Item_Button;
		equipItem[i]->SetItem("Null");
		equipItem[i]->SetKind((Item_Kind)i);
		equipItem[i]->SetIsMove(true);
	}

	QuickItem.resize(12);
	for (int i = 0; i < 12; ++i)
	{
		QuickItem[i] = new Item_Button;
		QuickItem[i]->SetItem("Null");
		QuickItem[i]->SetKind(Item_Kind::Item_Potion);
		QuickItem[i]->SetIsMove(true);
	}
}

void Inventory::SetCurrentInven(Inven_Kind x)
{
	currentInven = x;
	for (int i = 0; i < Inven_End; ++i)
	{
		if (i != x)
		{
			for (int j = 0 ;j < InvenCount; ++j)
			{
				InvenItem[i][j]->SetIsMove(false);
				InvenItem[i][j]->SetShow(false);
			}
		}

		else
		{
			for (int j = 0; j < InvenCount; ++j)
			{
				InvenItem[i][j]->SetIsMove(true);
				InvenItem[i][j]->SetShow(true);
			}
		}
	}
}

void Inventory::ItemPositionSetting()
{
	
}
