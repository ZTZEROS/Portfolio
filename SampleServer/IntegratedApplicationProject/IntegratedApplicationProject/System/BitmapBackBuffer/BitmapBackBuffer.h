#pragma once

class BitmapImage;

class BitmapBackBuffer
{
private:
	BITMAPINFO BitmapInfo;
	BYTE* BufferAddress; //BitmapPixelAddress

	HWND TargetWindowHandle;
	HDC TargetDC_Handle; //TargetDeviceContext

	BitmapImage* MapAddress;

	unsigned int Width;
	unsigned int Height;
	unsigned int Pitch;
	unsigned int ColorByteCount;
	unsigned int BufferSize;

public:
	BitmapBackBuffer();
	~BitmapBackBuffer();

	void Initialize(HWND targetWindowHandle, BitmapImage* mapAddress, unsigned int colorByteCount = 4);
	void Initialize(HWND targetWindowHandle, unsigned int Width, unsigned int Height, unsigned int colorByteCount = 4);
	void Terminalize();

	void Render();
	void Render(BitmapImage* bitmapImage, INT positionX = 0, INT positionY = 0);
	void Render(BYTE* bitmapPixelAddress, INT positionX, INT positionY, UINT pivotX, UINT pivotY , UINT width, UINT height, UINT pitch, UINT colorByteCount = 4);

	void FlipBuffer();

	BITMAPINFO GetBitmapInfo(){ return BitmapInfo; }

	BYTE* GetBufferAddress() { return BufferAddress; }
	unsigned int GetHeight() { return Height; }
	unsigned int GetWidth() { return Width; }
	unsigned int GetPitch() { return Pitch; }
	
	void SetTargetDC_Handle(HWND targetWindowHandle) { TargetDC_Handle = GetDC(targetWindowHandle); }
};

extern BitmapBackBuffer BITMAP_BACK_BUFFER;