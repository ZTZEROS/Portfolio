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
		LOAD_RESOURCE,		//���ҽ� ����, 
		LOAD_FILE,			//����
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
		int			currentFrameX;	//���� ������X(��°)
		int			currentFrameY;	//���� ������Y(��°)
		int			maxFrameX;		//�ִ������� 
		int			maxFrameY;
		int			frameWidth;		//������ ���� ũ��
		int			frameHeight;	//������ ���� ũ��
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
	LPIMAGE_INFO	_imageInfo;		//�̹��� ����ü
	TCHAR*			_fileName;		//���� �̸� ���� ĳ����������
	BOOL			_trans;			//Ư�� �ȼ��� �����Ұų� ����
	COLORREF		_transColor;	//�����Ѵٸ� � �ȼ��� �����Ҳ���
	BOOL			_alpha;
	BLENDFUNCTION	_blendFunc;		//���ĺ��带 ����ϱ� ���� ����
	LPIMAGE_INFO	_blendImage;	//���ĺ��带 ����ϱ� ���� �̹��� ����

public:
	image();
	~image();

	//�� ��Ʈ�� �̹��� ���� �� ���
	HRESULT init(int width, int height, BOOL alpha);

	//���ҽ��κ��� �о�鿩�� �� ���
	HRESULT init(const DWORD resID, int width, int height, BOOL alpha,
		BOOL trans = FALSE, COLORREF transColor = FALSE);

	//���Ϸκ��� �о�鿩�� �� ���
	HRESULT init(const char* fileName, int width, int height, BOOL alpha,
		BOOL trans = FALSE, COLORREF transColor = FALSE);

	HRESULT init(const char* fileName, float x, float y, int width, int height, BOOL alpha,
		BOOL trans = FALSE, COLORREF transColor = FALSE);

	//�̹��� + ������ ���� �ʱ�ȭ
	HRESULT init(const char* fileName, float x, float y, int width, int height, int frameX, int frameY, BOOL alpha, BOOL trans = FALSE, COLORREF transColor = RGB(255, 0, 255));

	HRESULT init(const char* fileName, int width, int height, int frameX, int frameY, BOOL alpha, BOOL trans = FALSE, COLORREF transColor = RGB(255, 0, 255));

	void release(void);

	void setTransColor(BOOL trans, COLORREF transColor);

	void aniRender(HDC hdc, int destX, int destY, animation* ani);

	//�̹��� �������� �Լ�

	void render(HDC hdc);
	//					 �׷��� x��ǥ �׷��� y��ǥ
	void render(HDC hdc, int destX, int destY);
	//					�Ѹ���ǥ, ������ ���� LEFT, TOP, �ű⼭������ ����,����ũ��
	void render(HDC hdc, int destX, int destY,
		int sourX, int sourY,
		int sourWidth, int sourHeight);


	void frameRender(HDC hdc, int destX, int destY);
	void frameRender(HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY);
	void alphaFrameRender(HDC hdc, int destX, int destY, BYTE alpha);
	void alphaFrameRender(HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY, BYTE alpha);
	//�� ����
	void loopRender(HDC hdc, const LPRECT drawArea, int offSetX, int offSetY);

	void alphaRender(HDC hdc, BYTE alpha);
	void alphaRender(HDC hdc, int destX, int destY, BYTE alpha);
	void alphaRender(HDC hdc, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight, BYTE alpha);

	//DC���� �Լ�
	inline HDC getMemDC(void) { return _imageInfo->hMemDC; }

	//====================================
	// �̹��� ���� �Լ��� getter, setter
	//====================================


	//�̹��� ��ǥ X
	inline void setX(float x) { _imageInfo->x = x; }
	inline float getX() { return _imageInfo->x; }

	//�̹��� ��ǥ Y
	inline void setY(float y) { _imageInfo->y = y; }
	inline float getY() { return _imageInfo->y; }

	//�̹��� ������ǥ
	inline void setCenter(float x, float y)
	{
		_imageInfo->x = x - (_imageInfo->width / 2);
		_imageInfo->y = y - (_imageInfo->height / 2);
	}

	//���� X
	inline float getCenterX(void)
	{
		return _imageInfo->maxFrameX <= 0 ?
			_imageInfo->x + (_imageInfo->width / 2) :
			_imageInfo->x + (_imageInfo->frameWidth / 2);
	}

	//���� Y
	inline float getCenterY(void)
	{
		return _imageInfo->maxFrameY <= 0 ?
			_imageInfo->y + (_imageInfo->height / 2) :
			_imageInfo->y + (_imageInfo->frameHeight / 2);
	}

	//���� �ػ�
	inline int getWidth(void) { return _imageInfo->width; }

	//���� �ػ�
	inline int getHeight(void) { return _imageInfo->height; }

	//������ ����
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

