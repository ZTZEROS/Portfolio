#include "stdafx.h"
#include "UI_Button.h"


UI_Button::UI_Button()
{
	ZeroMemory(textures, sizeof(LPDIRECT3DTEXTURE9)*ButtonState::NUM_OF_BUTTON_STATE);
}


UI_Button::~UI_Button()
{
	ZeroMemory(textures, sizeof(LPDIRECT3DTEXTURE9)*ButtonState::NUM_OF_BUTTON_STATE);
}

void UI_Button::Render()
{
	if (isShow != true)
	{
		return;
	}

	RECT rc = { 0, 0, (int)(size.x / Scale._11), (int)(size.y / Scale._22 )};

	UI_Manager::GetSprite()->SetTransform(&world);

	UI_Manager::GetSprite()->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
	
	if (texture)
	{
		UI_Manager::GetSprite()->Draw(texture,	&rc,&D3DXVECTOR3(0, 0, 0),	&D3DXVECTOR3(0, 0, 0),	D3DCOLOR_XRGB(255, 255, 255));
	}
	if (textures[state])
	{
		UI_Manager::GetSprite()->Draw(textures[state], &rc, &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 0, 0), D3DCOLOR_XRGB(255, 255, 255));
	}
	
	UI_Manager::GetSprite()->End();
	
	if (text != nullptr && font != nullptr)
	{
		RECT temp = { (int)(world._41+ textPostion.x), (int)(world._42 + textPostion.y), (int)(world._41 + textPostion.x + 1),(int)(world._41 + textPostion.y + 1) };
		char tempT[256];
		sprintf_s(tempT, "%s", text);
		font->DrawTextA(NULL, tempT, strlen(tempT), &temp, DT_TOP | DT_LEFT | DT_NOCLIP, D3DCOLOR_XRGB(255, 255, 255));
	}

	UI_Base::Render();
}

LRESULT UI_Button::InputProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (isShow != true)
	{
		return -1;
	}

	LRESULT result = 0;
	result = UI_Base::InputProc(hWnd, message, wParam, lParam);
	if (result == 0)
	{
		RECT buttonRect = {
			(int)world._41, (int)world._42,
			(int)(world._41 + size.x),
			(int)(world._42 + size.y)
		};
		RECT rect = {
			(int)world._41, (int)world._42,
			(int)(world._41 + size.x),
			(int)(world._42 + size.y)
		};


		switch (message)
		{
		case WM_LBUTTONDOWN:
		{
			POINT mousePoint;
			mousePoint.x = GET_X_LPARAM(lParam);
			mousePoint.y = GET_Y_LPARAM(lParam);

			if (mousePoint.x > buttonRect.left
				&& mousePoint.x < buttonRect.right
				&& mousePoint.y > buttonRect.top
				&& mousePoint.y < buttonRect.bottom)
			{
				state = ButtonState::STATE_PRESS;
				result = WM_LBUTTONDOWN;
			}
		}
		break;
		case WM_LBUTTONUP:
		{
			POINT mousePoint;
			mousePoint.x = GET_X_LPARAM(lParam);
			mousePoint.y = GET_Y_LPARAM(lParam);

			if (mousePoint.x > buttonRect.left
				&& mousePoint.x < buttonRect.right
				&& mousePoint.y > buttonRect.top
				&& mousePoint.y < buttonRect.bottom)
			{
				state = ButtonState::STATE_OVER;
				if (OnClick != nullptr)
				{
					OnClick();
				}
				result = WM_LBUTTONUP;
			}
			else
			{
				state = ButtonState::STATE_NORMAL;
			}


		}
		break;
		case WM_MOUSEMOVE:
		{
			POINT mousePoint;
			mousePoint.x = GET_X_LPARAM(lParam);
			mousePoint.y = GET_Y_LPARAM(lParam);

			if (mousePoint.x > buttonRect.left
				&& mousePoint.x < buttonRect.right
				&& mousePoint.y > buttonRect.top
				&& mousePoint.y < buttonRect.bottom)
			{
				if (state == ButtonState::STATE_NORMAL)
				{
					state = ButtonState::STATE_OVER;
				}
			}
			else
			{
				if (state == ButtonState::STATE_OVER)
				{
					state = ButtonState::STATE_NORMAL;
				}
			}
		}
		break;
		}
	}

	return result;
}

void UI_Button::SetTexture(char* normalName, char* overName, char* pressName)
{
	D3DXIMAGE_INFO imageInfo;
	textures[ButtonState::STATE_NORMAL] =
		TextureManager::GetTexture(
			normalName, &imageInfo);
	size.x = (float)imageInfo.Width;
	size.y = (float)imageInfo.Height;
	textures[ButtonState::STATE_OVER] =
		TextureManager::GetTexture(overName);
	textures[ButtonState::STATE_PRESS] =
		TextureManager::GetTexture(pressName);
}

void UI_Button::SetTexture(char * fileName, char * path)
{
	D3DXIMAGE_INFO imageInfo;
	texture = TextureManager::GetTexturePath(path, fileName,&imageInfo);
	size.x = (float)imageInfo.Width;
	size.y = (float)imageInfo.Height;
//	rect = RectMake(position.x, position.y, size.x, size.y);
}
