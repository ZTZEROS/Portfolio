#pragma once

class BitmapBackBuffer;

class BitmapImage
{
private:
	HDC MemoryDC;
	HBITMAP Bitmap;

	BITMAPFILEHEADER BitmapFileHeader;
	BITMAPINFO BitmapInfo;
	//BITMAPINFOHEADER BitmapInfoHeader;
	//RGBQUAD RGB_Quad;

	vector<int> intergerV;

	BYTE* BitmapPixelAddress;
	//BYTE* BitmapAddress;

	UINT LeftTopX;
	UINT LeftTopY;
	UINT CenterX;
	UINT CenterY;
	UINT PivotX;
	UINT PivotY;

	UINT Height;
	UINT Width;

	UINT Pitch;

	UINT ColorByteCount;

public:
	BitmapImage(HWND hWnd, LPCWSTR filePath, UINT pivotX = 0, UINT pivotY = 0, UINT colorByteCount = 4);
	BitmapImage(LPCWSTR filePath, UINT pivotX = 0, UINT pivotY = 0, UINT colorByteCount = 4);
	//BitmapImage(HDC hdc, LPCWSTR filePath, UINT width, UINT height);
	~BitmapImage();

	void AlphaBlend(BitmapImage* blendTargetBitmapImage);
	
	//void Render(HDC hdc); //DirectRender
	void Render(UINT positionX = 0, UINT positionY = 0);
	void Render(BitmapBackBuffer* bitmapBackBufferAddress, UINT positionX = 0, UINT positionY = 0);

	void Move(UINT leftTopX, UINT leftTopY) { LeftTopX = leftTopX; LeftTopY = leftTopY; }
	//void Move(UINT centerX, UINT centerY) { LeftTopX = centerX - Widht / 2; LeftTopY = centerY - Height / 2; }

	BYTE* GetBitmapPixelAddress() { return BitmapPixelAddress; }

	UINT GetLeftTopX() { return LeftTopX; }
	UINT GetLeftTopY() { return LeftTopY; }

	UINT GetHeight() { return Height; }
	UINT GetWidth() { return Width; }

	UINT GetCenterX() { return CenterX; }
	UINT GetCenterY() { return CenterY; }

	UINT GetPivotX() { return PivotX; }
	UINT GetPivotY() { return PivotY; }

	UINT GetPitch() { return Pitch; }

	UINT GetColorByteCount() { return ColorByteCount; }
};