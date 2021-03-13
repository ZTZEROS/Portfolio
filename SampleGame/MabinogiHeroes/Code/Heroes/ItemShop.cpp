#include "stdafx.h"
#include "ItemShop.h"
#include "Inventory.h"


ItemShop::ItemShop()
{
}


ItemShop::~ItemShop()
{
}

void ItemShop::Initilize()
{
	ShopWindow = new UI_Image;
	ShopWindow->SetTexture("./UI/ItemShop/ItemShop0.dds");
	ShopWindow->SetPosition(200, 100, 0);
	ShopWindow->SetIsMove(false);

	UI_Image* ShopWindow1 = new UI_Image;
	ShopWindow1->SetTexture("./UI/ItemShop/ItemShop1.dds");
	ShopWindow1->SetPosition(17, 50, 0);
	ShopWindow1->SetIsMove(false);
	ShopWindow->AddChild(ShopWindow1);

	UI_Image* ShopWindow2 = new UI_Image;
	ShopWindow2->SetTexture("./UI/ItemShop/ItemShop2.dds");
	ShopWindow2->SetPosition(18, 26, 0);
	ShopWindow2->SetIsMove(false);
	
	ShopWindow->AddChild(ShopWindow2);

	UI_Image* ShopWindow3 = new UI_Image;
	ShopWindow3->SetTexture("./UI/ItemShop/ItemShop3.dds");
	ShopWindow3->SetPosition(17, 50, 0); 
	ShopWindow3->SetIsMove(false);
	ShopWindow1->AddChild(ShopWindow3);

	UI_Button* BuyButton = new UI_Button;
	BuyButton->SetTexture("./UI/ButtonUp.dds", "./UI/ButtonMiddle.dds", "./UI/ButtonDown.dds");
	BuyButton->SetPosition(180, 400, 0);
	BuyButton->SetScale(1.0);
	BuyButton->SetText("구매");
	BuyButton->OnClick = std::bind(&UI_Functions::BuyButton, std::ref(UI_Manager::Get().func), this);
	BuyButton->SetTextPosition(15, 7, 0);
	BuyButton->SetFont(UI_Manager::GetItemFont());
	ShopWindow->AddChild(BuyButton);
	 
	for (auto i = vItems.begin(); i != vItems.end(); ++i)
	{
		ShopWindow3->AddChild(*i);
	}
	clickItem = new UI_Image;
	clickItem->SetTexture("./UI/ItemShop/ItemClick.dds");
	clickItem->SetPosition(vItems[0]->position.x - 4, vItems[0]->position.y - 6, 0);
	ShopWindow3->AddChild(clickItem);
	
	ShopWindow->Update();
}

void ItemShop::ItemInit()
{
	vItems.resize(10);
	for (int i = 0; i < 10; ++i)
	{
		if (i < 5)
		{
			vItems[i] = new Item_Button;
			vItems[i]->SetPosition(0, (float)10 + i * 50, 0);
			vItems[i]->SetItem("Null");
			vItems[i]->SetIsMove(true);
		}
	
		else
		{
			vItems[i] = new Item_Button;
			vItems[i]->SetPosition(200, (float)10 + (i-5) * 50, 0);
			vItems[i]->SetItem("Null");
			vItems[i]->SetIsMove(true);
		}
	}
}

void ItemShop::Destroy()
{
	SAFE_DELETE(ShopWindow);
}

void ItemShop::AddItem(char * name)
{
	vItems[k]->SetItem(name);
	++k;
}

void ItemShop::Render()
{
	if (!isShow) return;
	ShopWindow->Render();

	for (int i = 0; i < 10; ++i)
	{
		if (strcmp(vItems[i]->GetItemName(), "Null") != 0 && PtInRect(&vItems[i]->getRect(), GameManager::Get().getMousePoint())) //마우스가 렉트 안에 있으면
		{
			UI_Image* ti = new UI_Image;
			ti->SetScale(2);
			ti->SetTexture("./UI/ItemToolTip.dds");
			RECT temp = vItems[i]->getRect();
			temp.left += (int)vItems[i]->GetSize().x;
			temp.right += (int)vItems[i]->GetSize().x;
			temp.top += (int)vItems[i]->GetSize().y;
			temp.bottom += (int)vItems[i]->GetSize().y;
			char text[1024];
			ti->SetPosition((float)temp.left - 20, (float)temp.top - 20, 0);
			ti->Update();
			ti->Render();
			sprintf_s(text, "\n\n이름 : %s\n\n공력력 : %d\n\n방어력 : %d\n\n레벨제한 : %d\n\n가격 : %d", vItems[i]->GetItemName(),
				vItems[i]->GetItem().atk, vItems[i]->GetItem().def, vItems[i]->GetItem().level, vItems[i]->GetItem().gold);
			UI_Manager::GetItemFont()->DrawTextA(NULL, text, strlen(text), &temp, DT_TOP | DT_LEFT | DT_NOCLIP, D3DCOLOR_XRGB(255, 255, 255));

		}
	}
}

void ItemShop::Update()
{
//	ShopWindow->Update();
//	CheckMouse();
}

HRESULT ItemShop::InputProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (!isShow) return 0;
	ShopWindow->InputProc(hWnd, message, wParam, lParam);

	switch (message)
	{
	case WM_LBUTTONDOWN:
	{
		CheckMouse();
		ShopWindow->Update();
		
	}
	break;
	case WM_LBUTTONUP:
	{
	}
	break;

	case WM_RBUTTONDOWN:
	{

	}
	break;
	case WM_NCRBUTTONUP:
	{

	}
	break;
	case WM_MOUSEMOVE:
	{
	}
	break;
	}
	return 0;
}

void ItemShop::CheckMouse()
{
	if (!isShow) return;

	for (int i = 0; i < 10; ++i) //장비 관리
	{
		if (strcmp("Null", vItems[i]->GetItemName()) == 0) continue; //장착칸이 빈칸일 경우 무시

		switch (vItems[i]->GetMouseState())
		{
		case Mouse_Ldown:
			vItems[i]->SetIsUp(true);
			clickItem->SetPosition(vItems[i]->position.x-4, vItems[i]->position.y-7, 0);
			for (int j = 0; j < 10; ++j)
			{
				if(i!=j) vItems[j]->SetIsUp(false);
			}
			break;
		}
	}
}

