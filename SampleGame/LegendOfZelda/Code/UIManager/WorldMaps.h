#pragma once
#include "../gameNode.h"
#include "Cursors.h"

class WorldMaps : public gameNode
{
private:
	image* Image;
	bool Show;
	bool ZoomIn;

	int LeftTopScrollX;
	int LeftTopScrollY;

	Cursors Cursor;

	int UK, DK, LK, RK; //UpKey, DownKey, LeftKey, RightKey;

public:
	HRESULT Initialize();
	void Release();
	void Update();
	void Render();

	bool GetShow() { return Show; }
	bool GetZoomIn() { return ZoomIn; }
	int GetImageWidth() { return Image->getWidth(); }
	int GetImageHeight() { return Image->getHeight(); }
	int GetLeftTopScrollX() { return LeftTopScrollX; }
	int GetLeftTopScrollY() { return LeftTopScrollY; }
	Cursors GetCursor() { return Cursor; }
	float GetCursorCenterX() { return Cursor.GetCenterX(); }
	float GetCursorCenterY() { return Cursor.GetCenterY(); }

	void SetShow(bool show) { Show = show; }
	void SetZoomIn(bool zoomIn) { ZoomIn = zoomIn; }
	void SetLeftTopScrollX(int leftTopScrollX) { LeftTopScrollX = leftTopScrollX; }
	void SetLeftTopScrollY(int leftTopScrollY) { LeftTopScrollY = leftTopScrollY; }
	void SetCursor(Cursors cursor) { Cursor = cursor; }
	void SetCursorCenterX(float centerX) { Cursor.SetCenterX(centerX); }
	void SetCursorCenterY(float centerX) { Cursor.SetCenterY(centerX); }

	WorldMaps();
	~WorldMaps();
};

