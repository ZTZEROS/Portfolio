#pragma once
#include "UI_Base.h"
class UI_Text :
	public UI_Base
{
public:
	UI_Text();
	virtual ~UI_Text();

	virtual void Render() override;

	inline void SetText(std::string _text) { text = _text; }
	inline void SetTextFormat(DWORD format) { textFormat = format; }
	inline void SetTextColor(D3DCOLOR color) { textColor = color; }
	inline void SetFont(LPD3DXFONT _font) { font = _font; }

protected:
	std::string text;
	DWORD textFormat = (DT_TOP | DT_LEFT);
	D3DCOLOR textColor = D3DCOLOR_XRGB(255, 255, 255);

	LPD3DXFONT font = nullptr;
};

