#pragma once
#include "UI_Base.h"
class UI_Image :
	public UI_Base
{
public:
	UI_Image();
	virtual ~UI_Image();

	virtual void Render() override;
	virtual LRESULT InputProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;

	void SetTexture(char* fileName);
	void SetTexture(char* fileName, char* path);

	inline LPDIRECT3DTEXTURE9 GetTexture() { return texture; }

	inline void SetIsMove(bool move) { isMove = move; }
protected:
	LPDIRECT3DTEXTURE9 texture = nullptr;
	POINT prevMouse;
	bool isDrag = false;
	bool isMove = true;
};

