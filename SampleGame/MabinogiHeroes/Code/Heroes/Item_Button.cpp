#include "stdafx.h"
#include "Item_Button.h"

Item_Button::Item_Button()
{
	textureItem = nullptr;
}


Item_Button::~Item_Button()
{
}

void Item_Button::Render()
{
	if (isShow != true)
	{
		return;
	}

	rect = { (int)world._41, (int)world._42, (int)(world._41 + size.x), (int)(world._42 + size.y) };
	RECT rc = { 0, 0, (int)size.x, (int)size.y };
	if (isAlpha)
	{
		UI_Manager::GetSprite()->SetTransform(&world);
		UI_Manager::GetSprite()->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
		UI_Manager::GetSprite()->Draw(textureItem, &rc, &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 0, 0), D3DCOLOR_ARGB(125, 255, 255, 255));
		UI_Manager::GetSprite()->End();
	}
	else if(!isAlpha)
	{
		UI_Manager::GetSprite()->SetTransform(&world);
		UI_Manager::GetSprite()->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
		UI_Manager::GetSprite()->Draw(textureItem, &rc, &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 0, 0), D3DCOLOR_ARGB(255,255, 255, 255));
		UI_Manager::GetSprite()->End();
		//UI_Base::Render();
	}
//	Rectangle(GetDC(HWnd[0]), rect.left, rect.top, rect.right, rect.bottom);
}

LRESULT Item_Button::InputProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (!isMove) return 0;
	if (isShow != true)
	{
		return 0;
	}

	LRESULT result = 0;
	result = UI_Base::InputProc(hWnd, message, wParam, lParam);
	
	if (result == 0)
	{
		 rect = { (int)world._41, (int)world._42, (int)(world._41 + size.x), (int)(world._42 + size.y) };
		switch (message)
		{
		case WM_LBUTTONDOWN:
		{
			{
				prevMouse.x = GameManager::Get().getMousePoint().x;
				prevMouse.y = GameManager::Get().getMousePoint().y;

				if (prevMouse.x > rect.left && prevMouse.x < rect.right && prevMouse.y > rect.top && prevMouse.y < rect.bottom) //마우스를 누르면
				{
					state = Mouse_State::Mouse_Ldown;
					result = WM_LBUTTONDOWN;
					isAlpha = true;
				}
			}
		}
		break;
		case WM_LBUTTONUP:
		{
			POINT mousePoint;
			mousePoint.x = GET_X_LPARAM(lParam);
			mousePoint.y = GET_Y_LPARAM(lParam);
			if (state == Mouse_Ldown)
			{
				state = Mouse_State::Mouse_Lup;
			}
			if (mousePoint.x > rect.left && mousePoint.x < rect.right && mousePoint.y > rect.top && mousePoint.y < rect.bottom) //마우스를 때면
			{
				state = Mouse_State::Mouse_Lup;
			//	result = WM_LBUTTONUP;
			}
			
			isAlpha = false;
		}
		break;

		case WM_RBUTTONDOWN:
		{
			POINT mousePoint;
			mousePoint.x = GET_X_LPARAM(lParam);
			mousePoint.y = GET_Y_LPARAM(lParam);
			if (mousePoint.x > rect.left && mousePoint.x < rect.right && mousePoint.y > rect.top && mousePoint.y < rect.bottom) //우클릭 하면
			{
				state = Mouse_State::Mouse_Rdown;
				result = WM_RBUTTONDOWN;
			}
		}
		break;
		case WM_RBUTTONUP:
		{
			POINT mousePoint;
			mousePoint.x = GET_X_LPARAM(lParam);
			mousePoint.y = GET_Y_LPARAM(lParam);

			if (mousePoint.x > rect.left && mousePoint.x < rect.right && mousePoint.y > rect.top && mousePoint.y < rect.bottom) //우클릭 때면
			{
				state = Mouse_State::Mouse_Rup;
			
			}
			else
			{
				state = Mouse_State::Mouse_Nomal;
			}
		}

		case WM_MOUSEMOVE:
		{
			POINT mousePoint;
			mousePoint.x = GET_X_LPARAM(lParam);
			mousePoint.y = GET_Y_LPARAM(lParam);
		}
		break;
		}
	}
	return result;
}

void Item_Button::Destroy()
{
	SAFE_RELEASE(textureItem);
}

void Item_Button::SetItem(char * fileName)
{
	stItem temp;
	ItemName = fileName;
	temp = ItemData::Get().findItem(fileName);
	if (temp.name)
	{
		SAFE_RELEASE(textureItem);
		stItem returnTemp(temp.name, temp.kind, temp.atk, temp.def, temp.gold, temp.level);
		D3DXIMAGE_INFO imageInfo;
		_item = temp;
		char path[256] = "../ResourceData/Texture/Item/Inven/";
	//	strcat_s(path, returnTemp.name);
		textureItem = TextureManager::GetTexturePathSolo(path, _item.name, &imageInfo);
		
		size.x = (float)imageInfo.Width;
		size.y = (float)imageInfo.Height;
		//SAFE_RELEASE(texture);
	}
}

void Item_Button::SetTexture(char * fileName)
{
	D3DXIMAGE_INFO imageInfo;
	textureItem = TextureManager::GetTexture(fileName, &imageInfo);
	size.x = (float)imageInfo.Width;
	size.y = (float)imageInfo.Height;
//	rect = RectMake(position.x, position.y, size.x, size.y);
}

void Item_Button::SetTexture(char * fileName, char * path)
{
	D3DXIMAGE_INFO imageInfo;
	textureItem = TextureManager::GetTexturePath(path, fileName, &imageInfo);
	size.x = (float)imageInfo.Width;
	size.y = (float)imageInfo.Height;
//	rect = RectMake(position.x, position.y, size.x, size.y);
}
