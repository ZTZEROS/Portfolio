#include "stdafx.h"
#include "BitmapImage.h"

BitmapImage::BitmapImage(HWND hWnd, LPCWSTR filePath, UINT pivotX, UINT pivotY, UINT colorByteCount)
{
	HANDLE fileHandle;
	unsigned int bitmapPixelSize;
	
	fileHandle = CreateFile(filePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);



	ReadFile(fileHandle, &BitmapFileHeader, sizeof(BitmapFileHeader), nullptr, NULL);
	ReadFile(fileHandle, &(BitmapInfo.bmiHeader), sizeof(BitmapInfo.bmiHeader), nullptr, NULL);
	
	if(BitmapInfo.bmiHeader.biSize > 40 || BitmapInfo.bmiHeader.biClrUsed > 0) ReadFile(fileHandle, &(BitmapInfo.bmiColors), sizeof(BitmapInfo.bmiColors), nullptr, NULL);

	bitmapPixelSize = ((BitmapInfo.bmiHeader.biWidth * colorByteCount + 0x03) & 0xfffffffc) * BitmapInfo.bmiHeader.biHeight;

	BitmapPixelAddress = (BYTE*)malloc(bitmapPixelSize);

	//SetFilePointer(fileHandle, 0, nullptr, FILE_BEGIN);
	
	ReadFile(fileHandle, BitmapPixelAddress, bitmapPixelSize, nullptr, NULL);



	CloseHandle(fileHandle);

	HDC hdc;
	hdc = GetDC(hWnd);

	LeftTopX = 0;
	LeftTopY = 0;
	
	PivotX = pivotX;
	PivotY = pivotY;

	Height = BitmapInfo.bmiHeader.biHeight;
	Width = BitmapInfo.bmiHeader.biWidth;
	
	CenterX = Height / 2;
	CenterY = Width / 2;
	
	Pitch = (BitmapInfo.bmiHeader.biWidth * colorByteCount + 0x03) & 0xfffffffc;
	ColorByteCount = colorByteCount;

	
	MemoryDC = CreateCompatibleDC(hdc);
	//Bitmap = CreateCompatibleBitmap(hdc, Width, Height);
	//Bitmap = LoadImage();
	
	Bitmap = CreateDIBitmap(hdc, &(BitmapInfo.bmiHeader), CBM_INIT, BitmapPixelAddress, &BitmapInfo, DIB_RGB_COLORS);
	SelectObject(MemoryDC, Bitmap);
}

BitmapImage::BitmapImage(LPCWSTR filePath, UINT pivotX, UINT pivotY, UINT colorByteCount)
{
	HANDLE fileHandle;
	unsigned int bitmapPixelSize;

	fileHandle = CreateFile(filePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);



	ReadFile(fileHandle, &BitmapFileHeader, sizeof(BitmapFileHeader), nullptr, NULL);
	ReadFile(fileHandle, &(BitmapInfo.bmiHeader), sizeof(BitmapInfo.bmiHeader), nullptr, NULL);

	if (BitmapInfo.bmiHeader.biSize > 40 || BitmapInfo.bmiHeader.biClrUsed > 0) ReadFile(fileHandle, &(BitmapInfo.bmiColors), sizeof(BitmapInfo.bmiColors), nullptr, NULL);

	bitmapPixelSize = ((BitmapInfo.bmiHeader.biWidth * colorByteCount + 0x03) & 0xfffffffc) * BitmapInfo.bmiHeader.biHeight;

	BitmapPixelAddress = (BYTE*)malloc(bitmapPixelSize);

	//SetFilePointer(fileHandle, 0, nullptr, FILE_BEGIN);

	ReadFile(fileHandle, BitmapPixelAddress, bitmapPixelSize, nullptr, NULL);



	CloseHandle(fileHandle);

	LeftTopX = 0;
	LeftTopY = 0;

	PivotX = pivotX;
	PivotY = pivotY;

	Height = BitmapInfo.bmiHeader.biHeight;
	Width = BitmapInfo.bmiHeader.biWidth;

	CenterX = Height / 2;
	CenterY = Width / 2;

	Pitch = (BitmapInfo.bmiHeader.biWidth * colorByteCount + 0x03) & 0xfffffffc;
	ColorByteCount = colorByteCount;
}

/*
BitmapImage::BitmapImage(HDC hdc, LPCWSTR filePath, UINT width, UINT height)
{
	Width = width; 
	Height = height;

	MemoryDC = CreateCompatibleDC(hdc);
	Bitmap = CreateCompatibleBitmap(hdc, Width, Height);
	SelectObject(MemoryDC, Bitmap);
}
*/

BitmapImage::~BitmapImage()
{
	DeleteObject(Bitmap);
	DeleteObject(MemoryDC);

	free(BitmapPixelAddress);
}

void BitmapImage::AlphaBlend(BitmapImage* blendTargetBitmapImage)
{
	for (unsigned int i = 0; i < Height; ++i)
	{
		for (unsigned int j = 0; j < Width; ++j)
		{
			BitmapPixelAddress[i * Pitch + j] = ((BitmapPixelAddress[i * Pitch + j] >> 1) & 0x7fffffff) + ((blendTargetBitmapImage->BitmapPixelAddress[i * Pitch + j] >> 1) & 0x7fffffff);
		}
	}
}

/*
void BitmapImage::Render(HDC hdc)
{
//PatBlt(hdc, 0, 0, 640, 480, BLACKNESS);

//SetDIBits(MemoryDC, Bitmap, 0, BitmapInfo.bmiHeader.biHeight, BitmapPixelAddress, &BitmapInfo, DIB_RGB_COLORS );
SetDIBitsToDevice(hdc, 0, 0, Width, Height, 0, 0, 0, BitmapInfo.bmiHeader.biHeight, BitmapPixelAddress, &BitmapInfo, DIB_RGB_COLORS);

//BitBlt(hdc, LeftTopX, LeftTopY, Width, Height, MemoryDC, 0, 0, SRCCOPY);
//StretchBlt(hdc, LeftTopX, LeftTopY, Width, Height, MemoryDC, 0, 0, Width, Height, SRCCOPY);
}

void BitmapImage::Render(BitmapBackBuffer* bitmapBackBufferAddress)
{
bitmapBackBufferAddress->Render(this);
}
*/

void BitmapImage::Render(UINT positionX, UINT positionY)
{
	BITMAP_BACK_BUFFER.Render(BitmapPixelAddress, positionX, positionY, PivotX, PivotY, Width, Height, Pitch);
}

void BitmapImage::Render(BitmapBackBuffer* bitmapBackBufferAddress, UINT positionX, UINT positionY)
{
	bitmapBackBufferAddress->Render(BitmapPixelAddress, positionX, positionY, PivotX, PivotY, Width, Height, Pitch);
}