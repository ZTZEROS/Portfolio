#include "stdafx.h"
#include "UI_Image.h"


UI_Image::UI_Image()
{
}


UI_Image::~UI_Image()
{
	texture = nullptr;
}


void UI_Image::Render()
{
	if (isShow != true)
	{
		return;
	}

	RECT rc = { 0, 0, (int)size.x, (int)size.y };

	UI_Manager::GetSprite()->SetTransform(&world);
	UI_Manager::GetSprite()->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
	UI_Manager::GetSprite()->Draw(
		texture,
		&rc,
		&D3DXVECTOR3(0, 0, 0),
		&D3DXVECTOR3(0, 0, 0),
		D3DCOLOR_XRGB(255, 255, 255));
	UI_Manager::GetSprite()->End();
	UI_Base::Render();

}

LRESULT UI_Image::InputProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//if (!isMove) return 0;
	if (isShow != true)
	{
		return 0;
	}

	LRESULT result = 0;
	result = UI_Base::InputProc(hWnd, message, wParam, lParam);
	if (result == 0)
	{
		RECT rect = { (int)world._41, (int)world._42, (int)(world._41 + size.x), (int)(world._42 + size.y) };
		switch (message)
		{
		case WM_LBUTTONDOWN:
		{
			if (isMove == true)
			{
				POINT mousePoint;
				mousePoint.x = GameManager::Get().getMousePoint().x;
				mousePoint.y = GameManager::Get().getMousePoint().y;
				if (mousePoint.x > rect.left && mousePoint.x < rect.right && mousePoint.y > rect.top && mousePoint.y < rect.bottom)
				{
					//눌러졌다. 드래그 시작
					prevMouse.x = GameManager::Get().getMousePoint().x;
					prevMouse.y = GameManager::Get().getMousePoint().y;
					isDrag = true;

					result = WM_LBUTTONDOWN;
				}
			}
		}
		break;
		case WM_LBUTTONUP:
		{
			isDrag = false;
		}
		break;
		case WM_RBUTTONDOWN:
		{
			/*POINT mousePoint;
			mousePoint.x = GET_X_LPARAM(lParam);
			mousePoint.y = GET_Y_LPARAM(lParam);
			if (mousePoint.x > rect.left && mousePoint.x < rect.right
				&& mousePoint.y > rect.top && mousePoint.y < rect.bottom)
			{
				result = WM_RBUTTONDOWN;
			}*/
		}
		break;
		case WM_MOUSEMOVE:
		{
			if (isMove && isDrag)
			{
				POINT mousePoint;
				mousePoint.x = GameManager::Get().getMousePoint().x;
				mousePoint.y = GameManager::Get().getMousePoint().y;

				this->SetPosition(this->position.x + (float)(mousePoint.x - prevMouse.x), this->position.y + (float)(mousePoint.y - prevMouse.y), 0);
				prevMouse.x = mousePoint.x;
				prevMouse.y = mousePoint.y;
			}
		}
		break;
		}

	}
	return result;
}

void UI_Image::SetTexture(char* fileName)
{
	D3DXIMAGE_INFO imageInfo;
	texture = TextureManager::GetTexture(fileName, &imageInfo);
	size.x = (float)imageInfo.Width;
	size.y = (float)imageInfo.Height;
	//rect = RectMake(position.x, position.y, size.x, size.y);

}

void UI_Image::SetTexture(char* fileName, char* path)
{
	D3DXIMAGE_INFO imageInfo;
	texture = TextureManager::GetTexturePath(path, fileName,&imageInfo);
	size.x = (float)imageInfo.Width;
	size.y = (float)imageInfo.Height;
	//rect = RectMake(position.x, position.y, size.x, size.y);

}