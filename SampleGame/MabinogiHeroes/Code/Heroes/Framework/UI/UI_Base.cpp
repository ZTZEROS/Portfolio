#include "stdafx.h"
#include "UI_Base.h"


UI_Base::UI_Base()
{
	D3DXMatrixIdentity(&world);
	D3DXMatrixIdentity(&Scale);
	D3DXMatrixIdentity(&Translation);
}


UI_Base::~UI_Base()
{
}

void UI_Base::Destroy()
{
	parent = nullptr;

	for (auto iter = childArray.begin(); iter != childArray.end(); ++iter)
	{
		SAFE_DELETE(*iter);
	}
	childArray.clear();
}

void UI_Base::Update()
{
	if (isShow != true)
	{
		return;
	}

	D3DXMatrixIdentity(&world);
	world = world * Scale * Translation;

	if (parent)
	{
		world *= parent->GetWorld();
	}

	for (auto iter = childArray.cbegin(); iter != childArray.cend(); ++iter)
	{
		(*iter)->Update();
	}
}

void UI_Base::Render()
{
	if (isShow != true)
	{
		return;
	}

	for (auto iter = childArray.cbegin(); iter != childArray.cend(); ++iter)
	{
		(*iter)->Render();
	}
}

void UI_Base::AddChild(UI_Base* child)
{
	if (child)
	{
		child->SetParent(this);
		childArray.push_back(child);
	}
}

LRESULT UI_Base::InputProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	if (isShow != true)
	{
		return 0;
	}

	LRESULT result = 0;
	for (auto iter = childArray.cbegin(); iter != childArray.cend(); ++iter)
	{
		result = (*iter)->InputProc(hWnd, message, wParam, lParam);
		if (result != 0)
		{
			break;
		}
	}
	return result;
}

UI_Base* UI_Base::GetChildByID(int childID)
{
	UI_Base* child = nullptr;

	if (childID < (int)childArray.size() && childID >= 0)
	{
		child = childArray[childID];
	}

	//for ( auto iter = childArray.cbegin(); iter != childArray.cend(); ++iter )
	//{
	//	if ( ( *iter )->GetID() == childID )
	//	{
	//		child = ( *iter );
	//		break;
	//	}
	//}

	return child;
}

void UI_Base::SetChildUI(int ID, UI_Base change)
{
	UI_Base* child = nullptr;

	if (ID < (int)childArray.size() && ID >= 0)
	{
		childArray[ID] = &change;
	}

}

void UI_Base::SetRect()
{
	if (parent)
	{
		rect = RectMake((int)(parent->position.x + position.x), (int)(parent->position.y + position.y), (int)size.x, (int)size.y);
	}
	else
	{
		rect = RectMake((int)position.x, (int)position.y, (int)size.x, (int)size.y);
	}
}
