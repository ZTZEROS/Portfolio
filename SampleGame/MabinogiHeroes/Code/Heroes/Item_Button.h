#pragma once
#include "./UI/UI_Button.h"
#include "ItemBase.h"

class Item_Button : public UI_Button
{
public:
	Item_Button();
	~Item_Button();

	virtual void Render() override;
	virtual LRESULT InputProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
	virtual void Destroy();

	void SetItem(char* fileName);
	void SetTexture(char* fileName);
	void SetTexture(char* fileName, char* path);
	inline void SetIsMove(bool move) { isMove = move; }
	inline void SetState(Mouse_State _state) { state = _state; }
	inline void SetAlpha(bool alpha) { isAlpha = alpha; }
	inline void SetKind(Item_Kind kind) { _item.kind = kind; }
	inline void SetMouseState(Mouse_State _state) { state = _state; }
	inline void SetIsUp(bool x) { isUp = x; }

	inline stItem GetItem() { return _item; }
	inline LPDIRECT3DTEXTURE9 GetTexture() { return texture; }
	inline Mouse_State GetMouseState() { return state; }
	inline char* GetItemName() { return ItemName; }
	inline bool GetIsUp() { return isUp; }

	std::function<void()> OnClick = nullptr;


protected:
	Mouse_State state = Mouse_State::Mouse_Nomal;
	LPDIRECT3DTEXTURE9 textureItem;
	stItem _item;
	POINT prevMouse;
	char* ItemName;
	bool isDrag = false;
	bool isMove = false;
	bool isAlpha = false;
	bool isUp = false;//마우스가 렉트 위에 있는가?
};

