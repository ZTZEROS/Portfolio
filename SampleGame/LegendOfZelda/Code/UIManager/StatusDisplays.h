#pragma once
#include "../gameNode.h"

class StatusDisplays : public gameNode
{
private:
	bool Show;
	bool Talking;

	int _currentHp;
	int _maxHp;

	float LeftTopX, LeftTopY;

	int SelectedSubWeapon;

public:
	HRESULT Initialize();
	void Release();
	void Update();
	void Render();

	bool GetShow() { return Show; }
	float GetLeftTopY() { return LeftTopY; }

	void Set_currentHp(int currentHp) { _currentHp = currentHp; }
	void Set_maxHp(int maxHp) { _maxHp = maxHp; }
	void SetTalking(bool talking) { Talking = talking; }
	void SetShow(bool show) { Show = show; }
	void SetLeftTopY(float leftTopY) { LeftTopY = leftTopY; }
	void SetSelectedSubWeapon(int selectedSubWeapon) { SelectedSubWeapon = selectedSubWeapon; }

	StatusDisplays();
	~StatusDisplays();
};

