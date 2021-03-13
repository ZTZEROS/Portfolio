#pragma once
#include "Character_Collision.h"
class Character_MouseMove :
	public Character_Collision
{
public:
	Character_MouseMove();
	virtual ~Character_MouseMove();

	virtual LRESULT InputProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;

	inline void SetGrid(Grid* _grid){ grid = _grid; }

protected:
	Grid* grid = nullptr;
};

