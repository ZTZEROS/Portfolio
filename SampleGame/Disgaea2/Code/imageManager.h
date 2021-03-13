#pragma once
#include "singletonBase.h"
#include "image.h"
#include <map>

class imageManager : public singletonBase<imageManager>
{
public:
	typedef map<string, image*> mapImageList;
	typedef map<string, image*>::iterator mapImageIter;

private:
	mapImageList _mImageList;

	image* _black;
	float _fadeSpeed;
	float _speedCount;
	int _alpha;
	bool _fadeInit;

public:
	HRESULT init(void);
	void release(void);

	//이미지 추가
	image* addImage(string strKey, int width, int height, BOOL alpha);

	image* addImage(string strKey, const DWORD resID, int width, int height, bool trans, COLORREF transColor, bool alpha = false);
	image* addImage(string strKey, const char* fileName, int width, int height, bool trans, COLORREF transColor, bool alpha = false);
	image* addImage(string strKey, const char* fileName, float x, float y, int width, int height, bool trans, COLORREF transColor, bool alpha = false);

	//프레임 이미지 추가
	image* addFrameImage(string strKey, const char* fileName, float x, float y, int width, int height, int frameX, int frameY, bool trans, COLORREF transColor, bool alpha = false);
	image* addFrameImage(string strKey, const char* fileName, int width, int height, int frameX, int frameY, bool trans, COLORREF transColor, bool alpha = false);

	//이미지 찾는 함수
	image* findImage(string strKey);

	BOOL deleteImage(string strKey);

	BOOL deleteAll(void);

	//렌더 함수
	void render(string strKey, HDC hdc);
	void render(string strKey, HDC hdc, int destX, int destY);
	void render(string strKey, HDC hdc, int destX, int destY,
		int sourX, int sourY, int sourWidth, int sourHeight);

	void frameRender(string strKey, HDC hdc, int destX, int destY);
	void frameRender(string strKey, HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY);

	//뤂 렌더
	void loopRender(string strKey, HDC hdc, const LPRECT drawArea, int offSetX, int offSetY);

	void alphaRender(string strKey, HDC hdc, BYTE alpha);
	void alphaRender(string strKey, HDC hdc, int destX, int destY, BYTE alpha);
	void alphaRender(string strKey, HDC hdc, int destX, int destY,
		int sourX, int sourY, int sourWidth, int sourHeight, BYTE alpha);

	void alphaFrameRender(string strKey, HDC hdc, int destX, int destY, BYTE alpha);
	void alphaFrameRender(string strKey, HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY, BYTE alpha);

	// 페이드 인, 페이드 아웃
	bool fadeIn(float speed, HDC hdc);
	bool fadeOut(float speed, HDC hdc);


	imageManager();
	~imageManager();
};

