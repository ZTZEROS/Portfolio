#pragma once
#include "animation.h"
//============================================
// 2015. 11. 05 ## image Class ##
//============================================

class image
{
public:
	enum IMAGE_LOAD_KIND
	{
		LOAD_RESOURCE,		//리소스 파일, 
		LOAD_FILE,			//파일
		LOAD_EMPTY,
		LOAD_END
	};

	typedef struct tagImageInfo
	{
		DWORD		resID;
		HDC			hMemDC;
		HBITMAP		hBit;
		HBITMAP		hObit;
		int			x;
		int			y;
		int			width;
		int			height;
		int			currentFrameX;	//현재 프레임X(번째)
		int			currentFrameY;	//현재 프레임Y(번째)
		int			maxFrameX;		//최대프레임 
		int			maxFrameY;
		int			frameWidth;		//프레임 가로 크기
		int			frameHeight;	//프레임 세로 크기
		BYTE		loadType;

		tagImageInfo()
		{
			resID = 0;
			hMemDC = NULL;
			hBit = NULL;
			hObit = NULL;
			x = 0;
			y = 0;
			width = 0;
			height = 0;
			currentFrameX = 0;
			currentFrameY = 0;
			maxFrameX = 0;
			maxFrameY = 0;
			frameWidth = 0;
			frameHeight = 0;
			loadType = LOAD_RESOURCE;
		}

	}IMAGE_INFO, *LPIMAGE_INFO;

private:
	LPIMAGE_INFO	_imageInfo;		//이미지 구조체
	TCHAR*			_fileName;		//파일 이름 담을 캐릭터포인터
	BOOL			_trans;			//특정 픽셀을 제거할거냐 여부
	COLORREF		_transColor;	//제거한다면 어떤 픽셀을 제거할꺼냐
	BOOL			_alpha;
	BLENDFUNCTION	_blendFunc;		//알파블렌드를 사용하기 위한 정보
	LPIMAGE_INFO	_blendImage;	//알파블렌드를 사용하기 위한 이미지 정보

public:
	image();
	~image();

	//빈 비트맵 이미지 만들어서 쓸 경우
	HRESULT init(int width, int height, BOOL alpha);

	//리소스로부터 읽어들여서 쓸 경우
	HRESULT init(const DWORD resID, int width, int height, BOOL alpha,
		BOOL trans = FALSE, COLORREF transColor = FALSE);

	//파일로부터 읽어들여서 쓸 경우
	HRESULT init(const char* fileName, int width, int height, BOOL alpha,
		BOOL trans = FALSE, COLORREF transColor = FALSE);

	HRESULT init(const char* fileName, float x, float y, int width, int height, BOOL alpha,
		BOOL trans = FALSE, COLORREF transColor = FALSE);

	//이미지 + 프레임 파일 초기화
	HRESULT init(const char* fileName, float x, float y, int width, int height, int frameX, int frameY, BOOL alpha, BOOL trans = FALSE, COLORREF transColor = RGB(255, 0, 255));

	HRESULT init(const char* fileName, int width, int height, int frameX, int frameY, BOOL alpha, BOOL trans = FALSE, COLORREF transColor = RGB(255, 0, 255));

	void release(void);

	void setTransColor(BOOL trans, COLORREF transColor);

	void aniRender(HDC hdc, int destX, int destY, animation* ani);

	//이미지 렌더해줄 함수

	void render(HDC hdc);
	//					 그려줄 x좌표 그려줄 y좌표
	void render(HDC hdc, int destX, int destY);
	//					뿌릴좌표, 가져올 곳의 LEFT, TOP, 거기서부터의 가로,세로크기
	void render(HDC hdc, int destX, int destY,
		int sourX, int sourY,
		int sourWidth, int sourHeight);


	void frameRender(HDC hdc, int destX, int destY);
	void frameRender(HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY);
	void alphaFrameRender(HDC hdc, int destX, int destY, BYTE alpha);
	void alphaFrameRender(HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY, BYTE alpha);
	//뤂 렌더
	void loopRender(HDC hdc, const LPRECT drawArea, int offSetX, int offSetY);

	void alphaRender(HDC hdc, BYTE alpha);
	void alphaRender(HDC hdc, int destX, int destY, BYTE alpha);
	void alphaRender(HDC hdc, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight, BYTE alpha);

	//DC얻어올 함수
	inline HDC getMemDC(void) { return _imageInfo->hMemDC; }

	//====================================
	// 이미지 관련 함수들 getter, setter
	//====================================


	//이미지 좌표 X
	inline void setX(float x) { _imageInfo->x = x; }
	inline float getX() { return _imageInfo->x; }

	//이미지 좌표 Y
	inline void setY(float y) { _imageInfo->y = y; }
	inline float getY() { return _imageInfo->y; }

	//이미지 중점좌표
	inline void setCenter(float x, float y)
	{
		_imageInfo->x = x - (_imageInfo->width / 2);
		_imageInfo->y = y - (_imageInfo->height / 2);
	}

	//중점 X
	inline float getCenterX(void)
	{
		return _imageInfo->maxFrameX <= 0 ?
			_imageInfo->x + (_imageInfo->width / 2) :
			_imageInfo->x + (_imageInfo->frameWidth / 2);
	}

	//중점 Y
	inline float getCenterY(void)
	{
		return _imageInfo->maxFrameY <= 0 ?
			_imageInfo->y + (_imageInfo->height / 2) :
			_imageInfo->y + (_imageInfo->frameHeight / 2);
	}

	//가로 해상도
	inline int getWidth(void) { return _imageInfo->width; }

	//세로 해상도
	inline int getHeight(void) { return _imageInfo->height; }

	//프레임 관련
	inline void setFrameX(int frameX)
	{
		_imageInfo->currentFrameX = frameX;
		if (frameX > _imageInfo->maxFrameX) _imageInfo->currentFrameX = _imageInfo->maxFrameX;
	}

	inline void setFrameY(int frameY)
	{
		_imageInfo->currentFrameY = frameY;
		if (frameY > _imageInfo->maxFrameY) _imageInfo->currentFrameY = _imageInfo->maxFrameY;
	}

	inline int getMaxFrameX(void) { return _imageInfo->maxFrameX; }
	inline int getMaxFrameY(void) { return _imageInfo->maxFrameY; }

	inline int getFrameX(void) { return _imageInfo->currentFrameX; }
	inline int getFrameY(void) { return _imageInfo->currentFrameY; }

	inline int getFrameWidth(void) { return _imageInfo->frameWidth; }
	inline int getFrameHeight(void) { return _imageInfo->frameHeight; }
};

