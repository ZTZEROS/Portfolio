#include "stdafx.h"
#include "BitmapBackBuffer.h"

UINT TRANSPARENT_COLOR;
BitmapBackBuffer BITMAP_BACK_BUFFER;

BitmapBackBuffer::BitmapBackBuffer() {}
BitmapBackBuffer::~BitmapBackBuffer() {}

void BitmapBackBuffer::Initialize(HWND targetWindowHandle, BitmapImage* mapAddress, unsigned int colorByteCount)
{
	unsigned int width;
	unsigned int height;

	TargetWindowHandle = targetWindowHandle;
	TargetDC_Handle = GetDC(targetWindowHandle);

	width = mapAddress->GetWidth();
	height = mapAddress->GetHeight();

	BitmapInfo.bmiHeader.biSize = 40;
	BitmapInfo.bmiHeader.biWidth = width;
	BitmapInfo.bmiHeader.biHeight = height;
	BitmapInfo.bmiHeader.biPlanes = 1;
	BitmapInfo.bmiHeader.biBitCount = colorByteCount * 8;
	//BitmapInfo.bmiHeader.biCompression;
	BitmapInfo.bmiHeader.biSizeImage = width * height * colorByteCount;
	//BitmapInfo.bmiHeader.biXPelsPerMeter;
	//BitmapInfo.bmiHeader.biYPelsPerMeter;
	//BitmapInfo.bmiHeader.biClrUsed;
	//BitmapInfo.bmiHeader.biClrImportant;


	//BitmapInfo.bmiColors[0];

	BufferAddress = (BYTE*)malloc(width * height * colorByteCount);

	Width = width;
	Height = height;
	Pitch = ((width * colorByteCount) + 0x03) & ~0x03;
	ColorByteCount = colorByteCount;

	TRANSPARENT_COLOR = 0x00ffffff; //0x00ff00ff
}

void BitmapBackBuffer::Initialize(HWND targetWindowHandle, unsigned int width, unsigned int height, unsigned int colorByteCount)
{
	TargetWindowHandle = targetWindowHandle;
	TargetDC_Handle = GetDC(targetWindowHandle);

	BitmapInfo.bmiHeader.biSize = 40;
	BitmapInfo.bmiHeader.biWidth = width;
	BitmapInfo.bmiHeader.biHeight = height;
	BitmapInfo.bmiHeader.biPlanes = 1;
	BitmapInfo.bmiHeader.biBitCount = colorByteCount * 8;
	//BitmapInfo.bmiHeader.biCompression;
	BitmapInfo.bmiHeader.biSizeImage = width * height * colorByteCount;
	//BitmapInfo.bmiHeader.biXPelsPerMeter;
	//BitmapInfo.bmiHeader.biYPelsPerMeter;
	//BitmapInfo.bmiHeader.biClrUsed;
	//BitmapInfo.bmiHeader.biClrImportant;


	//BitmapInfo.bmiColors[0];

	BufferAddress = (BYTE*)malloc(width * height * colorByteCount);

	Width = width;
	Height = height;
	Pitch = ((width * colorByteCount) + 0x03) & ~0x03;
	ColorByteCount = colorByteCount;

	TRANSPARENT_COLOR = 0x00ffffff; //0x00ff00ff
}

void BitmapBackBuffer::Terminalize()
{
	free(BufferAddress);
	
	ReleaseDC(TargetWindowHandle, TargetDC_Handle);
}

void BitmapBackBuffer::Render()
{
	SetDIBitsToDevice(TargetDC_Handle, 0, 0, Width, Height, 0, 0, 0, Height, BufferAddress, &BitmapInfo, DIB_RGB_COLORS);
}

void BitmapBackBuffer::Render(BitmapImage* bitmapImage, INT positionX, INT positionY)
{
	int leftTopX;
	int leftTopY;
	UINT pivotX;
	UINT pivotY;
	UINT width;
	UINT height;
	
	UINT pitch;
	UINT colorByteCount;

	BYTE* bitmapPixelAddress;

	int iStart;
	int iEnd;
	int jStart;
	int jEnd;

	width = bitmapImage->GetWidth();
	height = bitmapImage->GetHeight();

	pivotX = bitmapImage->GetPivotX();
	pivotY = bitmapImage->GetPivotY();

	pitch = bitmapImage->GetPitch();

	colorByteCount = bitmapImage->GetColorByteCount();

	bitmapPixelAddress = bitmapImage->GetBitmapPixelAddress();

	leftTopX = positionX - pivotX;
	leftTopY = positionY - pivotY;

	/*
	for (unsigned int i = 0; i < height; ++i)
	{
		for (unsigned int j = 0; j < width * colorByteCount; ++j)
		{
			BufferAddress[(leftTopY + i) * Pitch + (leftTopX + j)] = bitmapPixelAddress[(height - i -1) * pitch + j];
		}
	}
	*/

	if (leftTopY < 0)
	{
		iEnd = height + leftTopY;
		leftTopY = 0;
	}
	else iEnd = height;
	
	if (MapAddress->GetHeight() < leftTopY + height)
	{
		iStart = leftTopY + height - MapAddress->GetHeight();
	}
	else iStart = 0;

	if (leftTopX < 0)
	{
		jStart = -1 * leftTopX;
		leftTopX = 0;
	}
	else jStart = 0;

	if (MapAddress->GetWidth() < leftTopX + width)
	{
		jEnd = width + MapAddress->GetWidth() - (leftTopX + width);
	}
	else jEnd = width;

	for (int i = 0; i + iStart < iEnd; ++i)
	{
		DWORD* pixelAddress;

		pixelAddress = (DWORD*)(bitmapPixelAddress + (iEnd - i - 1) * pitch);

		for (int j = 0; j + jStart < jEnd; ++j)
		{
			if ((*(pixelAddress + j + jStart) & TRANSPARENT_COLOR) != TRANSPARENT_COLOR) *((DWORD*)(BufferAddress + (leftTopY + i) * Pitch) + leftTopX + j) = *(pixelAddress + j + jStart);
		}
	}
}

void BitmapBackBuffer::Render(BYTE* bitmapPixelAddress, INT positionX, INT positionY, UINT pivotX, UINT pivotY, UINT width, UINT height, UINT pitch, UINT colorByteCount)
{
	int leftTopX;
	int leftTopY;

	int iStart;
	int iEnd;
	int jStart;
	int jEnd;

	leftTopX = positionX - pivotX;
	leftTopY = positionY - pivotY;

	/*
	for (unsigned int i = 0; i < height; ++i)
	{
		for (unsigned int j = 0; j < width * colorByteCount; ++j)
		{
			BufferAddress[(leftTopY + i) * Pitch + (leftTopX + j)] = bitmapPixelAddress[(height - i -1) * pitch + j];
		}
	}
	*/

	if (leftTopY < 0)
	{
		iEnd = height + leftTopY;
		leftTopY = 0;
	}
	else iEnd = height;

	if (MapAddress->GetHeight() < leftTopY + height)
	{
		iStart = leftTopY + height - MapAddress->GetHeight();
	}
	else iStart = 0;

	if (leftTopX < 0)
	{
		jStart = -1 * leftTopX;
		leftTopX = 0;
	}
	else jStart = 0;

	if (MapAddress->GetWidth() < leftTopX + width)
	{
		jEnd = width + MapAddress->GetWidth() - (leftTopX + width);
	}
	else jEnd = width;

	for (int i = 0; i + iStart < iEnd; ++i)
	{
		DWORD* pixelAddress;

		pixelAddress = (DWORD*)(bitmapPixelAddress + (iEnd - i - 1) * pitch);

		for (int j = 0; j + jStart < jEnd; ++j)
		{
			if ((*(pixelAddress + j + jStart) & TRANSPARENT_COLOR) != TRANSPARENT_COLOR) *((DWORD*)(BufferAddress + (leftTopY + i) * Pitch) + leftTopX + j) = *(pixelAddress + j + jStart);
		}
	}
}

void BitmapBackBuffer::FlipBuffer()
{
	//PatBlt(TargetDC_Handle, 0, 0, Width, Height, BLACKNESS);
	//StretchDIBits(TargetDC_Handle, 0, 0, Width, Height, 0, 0, Width, Height, BufferAddress, &BitmapInfo, DIB_RGB_COLORS, SRCCOPY);
	
	//StretchDIBits(TargetDC_Handle, 0, Height, Width, -1 * Height, 0, 0, 640, 480, BufferAddress, &BitmapInfo, DIB_RGB_COLORS, SRCCOPY);
	StretchDIBits(TargetDC_Handle, 0, Height, Width, -1 * Height, 0, 0, Width, Height, BufferAddress, &BitmapInfo, DIB_RGB_COLORS, SRCCOPY);
}