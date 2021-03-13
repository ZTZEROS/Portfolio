#pragma once
#include "../gameNode.h"
#include "Cursors.h"

class saveLoad : public gameNode
{
private:
	image* Image;

	Cursors Cursor;

public:
	HRESULT Initialize();
	void Release();
	void Update();
	void Render();

	Cursors GetCursor() { return Cursor; }
	float GetCursorCenterX() { return Cursor.GetCenterX(); }
	float GetCursorCenterY() { return Cursor.GetCenterY(); }

	void SetCursor(Cursors cursor) { Cursor = cursor; }
	void SetCursorCenterX(float centerX) { Cursor.SetCenterX(centerX); }
	void SetCursorCenterY(float centerX) { Cursor.SetCenterY(centerX); }
	
	saveLoad();
	~saveLoad();
};