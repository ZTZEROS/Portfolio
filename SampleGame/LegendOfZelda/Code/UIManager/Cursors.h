#pragma once
#include "../gameNode.h"

struct CursorStructure
{
	image* Image;
	float CenterX, CenterY;
	int CurrentFrameX, CurrentFrameY;
};

class Cursors : public gameNode
{
private:
	CursorStructure Cursor;

public:
	HRESULT Initialize();
	void Release();
	void Update();
	void Render();

	image* GetImage() { return Cursor.Image; }
	float GetCenterX() { return Cursor.CenterX; }
	float GetCenterY() { return Cursor.CenterY; }
	int GetCurrentFrameX() { return Cursor.CurrentFrameX; }
	int GetCurrentFrameY() { return Cursor.CurrentFrameY; }

	void SetImage(image* image) { Cursor.Image = image; }
	void SetCenterX(float centerX) { Cursor.CenterX = centerX; }
	void SetCenterY(float centerY) { Cursor.CenterY = centerY; }
	void SetCurrentFrameX(int currentFrameX) { Cursor.CurrentFrameX = currentFrameX; }
	void SetCurrentFrameY(int currentFrameY) { Cursor.CurrentFrameY = currentFrameY; }
	
	Cursors();
	~Cursors();
};

