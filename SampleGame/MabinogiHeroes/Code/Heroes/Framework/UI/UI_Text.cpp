#include "stdafx.h"
#include "UI_Text.h"


UI_Text::UI_Text()
{
}


UI_Text::~UI_Text()
{
}

void UI_Text::Render()
{
	if (isShow != true)
	{
		return;
	}

	if (font != nullptr)
	{
		RECT rc = { (int)world._41, (int)world._42, (int)(world._41 + size.x), (int)(world._42 + size.y) };

		font->DrawTextA(NULL, text.c_str(), text.length(), &rc, textFormat, textColor);
	}

	UI_Base::Render();
}