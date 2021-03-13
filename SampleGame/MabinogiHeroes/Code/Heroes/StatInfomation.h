#pragma once
#include "./UI/UI_Image.h"
#include "./UI/UI_Text.h"

class StatInfomation
{
public:
	StatInfomation();
	~StatInfomation();

	void Initilize();
	void Destroy();
	void Render();
	void Update();

	void CheckKey();
	void CheckMouse();

	LRESULT InputProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	UI_Image* StatWidnow;
	UI_Text* ChInfomation[8];

	bool isShow = false;
};

