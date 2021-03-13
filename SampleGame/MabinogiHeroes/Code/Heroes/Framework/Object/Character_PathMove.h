#pragma once
#include "Character_MouseMove.h"
#include "PathFinder.h"

class Character_PathMove :
	public Character_MouseMove
{
public:
	Character_PathMove();
	virtual ~Character_PathMove();

	virtual LRESULT InputProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
	inline void SetPathFinder(PathFinder* _pathFinder){ pathFinder = _pathFinder; }

protected:
	PathFinder* pathFinder = nullptr;
};

