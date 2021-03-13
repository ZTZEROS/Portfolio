#pragma once
#include <functional>
#include "UI_Base.h"

enum ButtonState
{
	STATE_NORMAL,
	STATE_OVER,
	STATE_PRESS,
	NUM_OF_BUTTON_STATE
};

class UI_Button :
	public UI_Base
{
public:
	UI_Button();
	virtual ~UI_Button();

	virtual void Render() override;
	virtual LRESULT InputProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;

	void SetTexture(char* normalName, char* overName, char* pressName);
	void SetTexture(char* fileName, char* path);
	inline void SetState(ButtonState _state) { state = _state; }
	inline void SetText(char* _text) { text = _text; }
	inline void SetTextPosition(float x, float y, float z) { textPostion.x = x, textPostion.y = y, textPostion.z = z; }
	inline void SetFont(LPD3DXFONT x) { font = x; }

	inline char* GetText() { return text; }


	std::function<void()> OnClick = nullptr;

protected:
	char* text =nullptr;
	ButtonState state = ButtonState::STATE_NORMAL;
	LPDIRECT3DTEXTURE9 textures[NUM_OF_BUTTON_STATE];
	LPDIRECT3DTEXTURE9 texture = nullptr;
	LPD3DXFONT font = nullptr;

	D3DXVECTOR3 textPostion = D3DXVECTOR3(0,0,0);
};

