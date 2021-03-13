#pragma once
#include "../gameNode.h"
#include "Cursors.h"

class DungeonMaps : public gameNode
{
private:
	float LeftTopX;
	float LeftTopY;
	
	bool Show;

	Cursors Cursor;

public:
	HRESULT Initialize();
	void Release();
	void Update();
	void Render();

	bool GetShow() { return Show; }
	float GetLeftTopX() { return LeftTopX; }
	float GetLeftTopY() { return LeftTopY; }
	Cursors GetCursor() { return Cursor; }
	float GetCursorCenterX() { return Cursor.GetCenterX(); }
	float GetCursorCenterY() { return Cursor.GetCenterY(); }

	void SetShow(bool show) { Show = show; }
	void SetLeftTopX(float leftTopX) { LeftTopX = leftTopX; }
	void SetLeftTopY(float leftTopY) { LeftTopY = leftTopY; }
	void SetCursor(Cursors cursor) { Cursor = cursor; }
	void SetCursorCenterX(float centerX) { Cursor.SetCenterX(centerX); }
	void SetCursorCenterY(float centerX) { Cursor.SetCenterY(centerX); }

	DungeonMaps();
	~DungeonMaps();
};

