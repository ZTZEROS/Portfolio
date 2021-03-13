#include "stdafx.h"
#include "image.h"

//pragma == �����Ϸ� ���ñ�
#pragma comment(lib, "msimg32.lib")

//��� �̴ϼȶ�����~
image::image()
	: _imageInfo(NULL),
	_fileName(NULL),
	_trans(false),
	_transColor(RGB(0, 0, 0))
{
}


image::~image()
{
}

//�� ��Ʈ�� �̹��� ���� �� ���
HRESULT image::init(int width, int height, BOOL alpha)
{
	if (_imageInfo != NULL) release();

	HDC hdc = GetDC(_hWnd);

	//�̹��� ���� ����
	_imageInfo = new IMAGE_INFO;
	_imageInfo->loadType = LOAD_EMPTY;
	_imageInfo->resID = 0;
	_imageInfo->hMemDC = CreateCompatibleDC(hdc);
	_imageInfo->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, width, height);
	_imageInfo->hObit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
	_imageInfo->x = 0;
	_imageInfo->y = 0;
	_imageInfo->width = width;
	_imageInfo->height = height;

	_fileName = NULL;

	_trans = FALSE;
	_transColor = RGB(0, 0, 0);

	//���࿡ �̹������ҽ� �����°� ����������~
	if (_imageInfo->hBit == 0)
	{
		//�������ش�
		release();

		return E_FAIL;
	}
	if (alpha)
	{
		//���� ���� �ɼ�~
		_blendFunc.BlendFlags = 0;
		_blendFunc.AlphaFormat = 0;
		_blendFunc.BlendOp = AC_SRC_OVER;

		//���� ���� ����� ���� �̹��� ����
		_blendImage = new IMAGE_INFO;
		_blendImage->loadType = LOAD_EMPTY;
		_blendImage->resID = 0;
		_blendImage->hMemDC = CreateCompatibleDC(hdc);
		_blendImage->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, WINSIZEX, WINSIZEY);
		_blendImage->hObit = (HBITMAP)SelectObject(_blendImage->hMemDC, _blendImage->hBit);
		_blendImage->width = WINSIZEX;
		_blendImage->height = WINSIZEY;
	}


	ReleaseDC(_hWnd, hdc);


	return S_OK;
}

//���ҽ��κ��� �о�鿩�� �� ���
HRESULT image::init(const DWORD resID, int width, int height, BOOL alpha,
	BOOL trans, COLORREF transColor)
{

	if (_imageInfo != NULL) release();

	HDC hdc = GetDC(_hWnd);

	//�̹��� ���� ����
	_imageInfo = new IMAGE_INFO;
	_imageInfo->loadType = LOAD_RESOURCE;
	_imageInfo->resID = 0;
	_imageInfo->hMemDC = CreateCompatibleDC(hdc);
	_imageInfo->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, width, height);
	_imageInfo->hObit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
	_imageInfo->x = 0;
	_imageInfo->y = 0;
	_imageInfo->width = width;
	_imageInfo->height = height;

	_fileName = NULL;

	_trans = FALSE;
	_transColor = RGB(0, 0, 0);

	//���࿡ �̹������ҽ� �����°� ����������~
	if (_imageInfo->hBit == 0)
	{
		//�������ش�
		release();

		return E_FAIL;
	}
	if (alpha)
	{
		//���� ���� �ɼ�~
		_blendFunc.BlendFlags = 0;
		_blendFunc.AlphaFormat = 0;
		_blendFunc.BlendOp = AC_SRC_OVER;

		//���� ���� ����� ���� �̹��� ����
		_blendImage = new IMAGE_INFO;
		_blendImage->loadType = LOAD_EMPTY;
		_blendImage->resID = 0;
		_blendImage->hMemDC = CreateCompatibleDC(hdc);
		_blendImage->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, WINSIZEX, WINSIZEY);
		_blendImage->hObit = (HBITMAP)SelectObject(_blendImage->hMemDC, _blendImage->hBit);
		_blendImage->width = WINSIZEX;
		_blendImage->height = WINSIZEY;
	}



	ReleaseDC(_hWnd, hdc);

	return S_OK;
}


//���Ϸκ��� �о�鿩�� �� ���
HRESULT image::init(const char* fileName, int width, int height, BOOL alpha,
	BOOL trans, COLORREF transColor)
{
	if (fileName == NULL) return E_FAIL;

	if (_imageInfo != NULL) release();

	HDC hdc = GetDC(_hWnd);

	//�̹��� ���� ����
	_imageInfo = new IMAGE_INFO;
	_imageInfo->loadType = LOAD_FILE;
	_imageInfo->resID = 0;
	_imageInfo->hMemDC = CreateCompatibleDC(hdc);
	_imageInfo->hBit = (HBITMAP)LoadImage(_hInstance, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
	_imageInfo->hObit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
	_imageInfo->x = 0;
	_imageInfo->y = 0;
	_imageInfo->width = width;
	_imageInfo->height = height;

	//���� ��� �̸��� �����ؼ� �����´�
	int len = strlen(fileName);

	_fileName = new TCHAR[len + 1];
	strcpy_s(_fileName, len + 1, fileName);


	_trans = trans;
	_transColor = transColor;

	//���࿡ �̹������ҽ� �����°� ����������~
	if (_imageInfo->hBit == 0)
	{
		//�������ش�
		release();

		return E_FAIL;
	}
	if (alpha)
	{
		//���� ���� �ɼ�~
		_blendFunc.BlendFlags = 0;
		_blendFunc.AlphaFormat = 0;
		_blendFunc.BlendOp = AC_SRC_OVER;

		//���� ���� ����� ���� �̹��� ����
		_blendImage = new IMAGE_INFO;
		_blendImage->loadType = LOAD_EMPTY;
		_blendImage->resID = 0;
		_blendImage->hMemDC = CreateCompatibleDC(hdc);
		_blendImage->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, WINSIZEX, WINSIZEY);
		_blendImage->hObit = (HBITMAP)SelectObject(_blendImage->hMemDC, _blendImage->hBit);
		_blendImage->width = WINSIZEX;
		_blendImage->height = WINSIZEY;
	}
	ReleaseDC(_hWnd, hdc);


	return S_OK;
}

HRESULT image::init(const char* fileName, float x, float y, int width, int height, BOOL alpha,
	BOOL trans, COLORREF transColor)
{

	if (fileName == NULL) return E_FAIL;

	if (_imageInfo != NULL) release();

	HDC hdc = GetDC(_hWnd);

	//�̹��� ���� ����
	_imageInfo = new IMAGE_INFO;
	_imageInfo->loadType = LOAD_FILE;
	_imageInfo->resID = 0;
	_imageInfo->hMemDC = CreateCompatibleDC(hdc);
	_imageInfo->hBit = (HBITMAP)LoadImage(_hInstance, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
	_imageInfo->hObit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
	_imageInfo->x = x - (width / 2);
	_imageInfo->y = y - (height / 2);
	_imageInfo->width = width;
	_imageInfo->height = height;

	//���� ��� �̸��� �����ؼ� �����´�
	int len = strlen(fileName);

	_fileName = new char[len + 1];
	strcpy_s(_fileName, len + 1, fileName);


	_trans = trans;
	_transColor = transColor;

	//���࿡ �̹������ҽ� �����°� ����������~
	if (_imageInfo->hBit == 0)
	{
		//�������ش�
		release();

		return E_FAIL;
	}
	if (alpha)
	{
		//���� ���� �ɼ�~
		_blendFunc.BlendFlags = 0;
		_blendFunc.AlphaFormat = 0;
		_blendFunc.BlendOp = AC_SRC_OVER;

		//���� ���� ����� ���� �̹��� ����
		_blendImage = new IMAGE_INFO;
		_blendImage->loadType = LOAD_EMPTY;
		_blendImage->resID = 0;
		_blendImage->hMemDC = CreateCompatibleDC(hdc);
		_blendImage->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, WINSIZEX, WINSIZEY);
		_blendImage->hObit = (HBITMAP)SelectObject(_blendImage->hMemDC, _blendImage->hBit);
		_blendImage->width = WINSIZEX;
		_blendImage->height = WINSIZEY;
	}
	ReleaseDC(_hWnd, hdc);

	return S_OK;
}

HRESULT image::init(const char* fileName, float x, float y, int width, int height, int frameX, int frameY, BOOL alpha, BOOL trans, COLORREF transColor)
{
	if (fileName == NULL) return E_FAIL;

	if (_imageInfo != NULL) release();

	HDC hdc = GetDC(_hWnd);

	//�̹��� ���� ����
	_imageInfo = new IMAGE_INFO;
	_imageInfo->loadType = LOAD_FILE;
	_imageInfo->resID = 0;
	_imageInfo->hMemDC = CreateCompatibleDC(hdc);
	_imageInfo->hBit = (HBITMAP)LoadImage(_hInstance, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
	_imageInfo->hObit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
	_imageInfo->x = x - (width / 2);
	_imageInfo->y = y - (height / 2);
	_imageInfo->width = width;
	_imageInfo->height = height;
	_imageInfo->currentFrameX = 0;
	_imageInfo->currentFrameY = 0;
	_imageInfo->frameWidth = width / frameX;
	_imageInfo->frameHeight = height / frameY;
	_imageInfo->maxFrameX = frameX - 1;
	_imageInfo->maxFrameY = frameY - 1;

	//���� ��� �̸��� �����ؼ� �����´�
	int len = strlen(fileName);

	_fileName = new char[len + 1];
	strcpy_s(_fileName, len + 1, fileName);


	_trans = trans;
	_transColor = transColor;

	//���࿡ �̹������ҽ� �����°� ����������~
	if (_imageInfo->hBit == 0)
	{
		//�������ش�
		release();

		return E_FAIL;
	}
	if (alpha)
	{
		//���� ���� �ɼ�~
		_blendFunc.BlendFlags = 0;
		_blendFunc.AlphaFormat = 0;
		_blendFunc.BlendOp = AC_SRC_OVER;

		//���� ���� ����� ���� �̹��� ����
		_blendImage = new IMAGE_INFO;
		_blendImage->loadType = LOAD_EMPTY;
		_blendImage->resID = 0;
		_blendImage->hMemDC = CreateCompatibleDC(hdc);
		_blendImage->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, WINSIZEX, WINSIZEY);
		_blendImage->hObit = (HBITMAP)SelectObject(_blendImage->hMemDC, _blendImage->hBit);
		_blendImage->width = WINSIZEX;
		_blendImage->height = WINSIZEY;
	}
	ReleaseDC(_hWnd, hdc);

	return S_OK;
}

HRESULT image::init(const char* fileName, int width, int height, int frameX, int frameY, BOOL alpha, BOOL trans, COLORREF transColor)
{
	if (fileName == NULL) return E_FAIL;

	if (_imageInfo != NULL) release();

	HDC hdc = GetDC(_hWnd);

	//�̹��� ���� ����
	_imageInfo = new IMAGE_INFO;
	_imageInfo->loadType = LOAD_FILE;
	_imageInfo->resID = 0;
	_imageInfo->hMemDC = CreateCompatibleDC(hdc);
	_imageInfo->hBit = (HBITMAP)LoadImage(_hInstance, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
	_imageInfo->hObit = (HBITMAP)SelectObject(_imageInfo->hMemDC, _imageInfo->hBit);
	_imageInfo->width = width;
	_imageInfo->height = height;
	_imageInfo->frameWidth = width / frameX;
	_imageInfo->frameHeight = height / frameY;
	_imageInfo->maxFrameX = frameX - 1;
	_imageInfo->maxFrameY = frameY - 1;

	//���� ��� �̸��� �����ؼ� �����´�
	int len = strlen(fileName);

	_fileName = new char[len + 1];
	strcpy_s(_fileName, len + 1, fileName);


	_trans = trans;
	_transColor = transColor;

	//���࿡ �̹������ҽ� �����°� ����������~
	if (_imageInfo->hBit == 0)
	{
		//�������ش�
		release();

		return E_FAIL;
	}
	if (alpha)
	{
		//���� ���� �ɼ�~
		_blendFunc.BlendFlags = 0;
		_blendFunc.AlphaFormat = 0;
		_blendFunc.BlendOp = AC_SRC_OVER;

		//���� ���� ����� ���� �̹��� ����
		_blendImage = new IMAGE_INFO;
		_blendImage->loadType = LOAD_EMPTY;
		_blendImage->resID = 0;
		_blendImage->hMemDC = CreateCompatibleDC(hdc);
		_blendImage->hBit = (HBITMAP)CreateCompatibleBitmap(hdc, WINSIZEX, WINSIZEY);
		_blendImage->hObit = (HBITMAP)SelectObject(_blendImage->hMemDC, _blendImage->hBit);
		_blendImage->width = WINSIZEX;
		_blendImage->height = WINSIZEY;
	}
	ReleaseDC(_hWnd, hdc);


	return S_OK;
}

void image::release(void)
{
	if (_imageInfo)
	{
		//�̹��� ����
		SelectObject(_imageInfo->hMemDC, _imageInfo->hObit);
		DeleteObject(_imageInfo->hBit);
		DeleteDC(_imageInfo->hMemDC);

		//���ĺ��� ��� �̹��� ����
		SelectObject(_blendImage->hMemDC, _blendImage->hObit);
		DeleteObject(_blendImage->hBit);
		DeleteDC(_blendImage->hMemDC);

		//�����͵� ����
		SAFE_DELETE(_imageInfo);
		SAFE_DELETE(_fileName);
		SAFE_DELETE(_blendImage);

		//������ �ʱ�ȭ ���ݽô�
		_trans = false;
		_transColor = RGB(0, 0, 0);

	}

}


void image::setTransColor(BOOL trans, COLORREF transColor)
{
	_trans = trans;
	_transColor = transColor;

}

void image::render(HDC hdc)
{
	//�̹����� ���������� �׷��ִ°� ���!
	if (_trans)
	{
		GdiTransparentBlt(hdc,	//����� ��� DC
			_imageInfo->x,				//����� ��ǥ ������ X
			_imageInfo->y,				//����� ��ǥ ������ Y
			_imageInfo->width,	//����� ũ�� width
			_imageInfo->height,	//����� ũ�� height
			_imageInfo->hMemDC,	//������ DC
			0, 0,				//����� ��������
			_imageInfo->width,	//����� �������� width
			_imageInfo->height,	//����� �������� height
			_transColor);		//���翡�� ���ܵ� ����
	}
	else
	{
		//DC�������� ��Ӻ���
		BitBlt(hdc, _imageInfo->x, _imageInfo->y,
			_imageInfo->width, _imageInfo->height,
			_imageInfo->hMemDC, 0, 0, SRCCOPY);
	}

}

//�̹��� �������� �Լ�
//					 �׷��� x��ǥ �׷��� y��ǥ
void image::render(HDC hdc, int destX, int destY)
{
	if (_trans)
	{
		GdiTransparentBlt(hdc,	//����� ����� DC
			destX,				//����� ��ǥ ������ X
			destY,				//����� ��ǥ ������ Y
			_imageInfo->width,	//����� ũ�� width
			_imageInfo->height,	//����� ũ�� height
			_imageInfo->hMemDC,	//������ DC
			0, 0,				//����� ��������(���� 0, 0)
			_imageInfo->width,	//����� �������� width
			_imageInfo->height,	//����� �������� height
			_transColor);		//���翡�� ���ܵ� ����
	}
	else
	{
		//DC�������� ��Ӻ��� �Լ�
		BitBlt(hdc, destX, destY,
			_imageInfo->width, _imageInfo->height,
			_imageInfo->hMemDC, 0, 0, SRCCOPY);
	}
}

void image::render(HDC hdc, int destX, int destY,
	int sourX, int sourY,
	int sourWidth, int sourHeight)
{
	if (_trans)
	{
		GdiTransparentBlt(hdc,	//����� ����� DC
			destX,				//����� ��ǥ ������ X
			destY,				//����� ��ǥ ������ Y
			sourWidth,			//����� ũ�� width
			sourHeight,			//����� ũ�� height
			_imageInfo->hMemDC,	//������ DC
			sourX, sourY,		//����� ��������(���� 0, 0)
			sourWidth,			//����� �������� width
			sourHeight,			//����� �������� height
			_transColor);		//���翡�� ���ܵ� ����
	}
	else
	{
		//DC�������� ��Ӻ��� �Լ�
		BitBlt(hdc, destX, destY,
			sourWidth, sourHeight,
			_imageInfo->hMemDC, sourX, sourY, SRCCOPY);
	}
}

void image::frameRender(HDC hdc, int destX, int destY)
{
	if (_trans)
	{
		GdiTransparentBlt(hdc,				//����� ����� DC
			destX,							//����� ��ǥ ������ X
			destY,							//����� ��ǥ ������ Y
			_imageInfo->frameWidth,			//����� ũ�� width
			_imageInfo->frameHeight,		//����� ũ�� height
			_imageInfo->hMemDC,				//������ DC
			_imageInfo->currentFrameX * _imageInfo->frameWidth,
			_imageInfo->currentFrameY * _imageInfo->frameHeight,		//����� ��������(���� 0, 0)
			_imageInfo->frameWidth,			//����� �������� width
			_imageInfo->frameHeight,		//����� �������� height
			_transColor);					//���翡�� ���ܵ� ����
	}
	else
	{
		//DC�������� ��Ӻ��� �Լ�
		BitBlt(hdc, destX, destY,
			_imageInfo->frameWidth, _imageInfo->frameHeight,
			_imageInfo->hMemDC,
			_imageInfo->currentFrameX * _imageInfo->frameWidth,
			_imageInfo->currentFrameY * _imageInfo->frameHeight,
			SRCCOPY);
	}
}

void image::frameRender(HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY)
{

	_imageInfo->currentFrameX = currentFrameX;
	_imageInfo->currentFrameY = currentFrameY;

	if (_trans)
	{

		GdiTransparentBlt(hdc,				//����� ����� DC
			destX,							//����� ��ǥ ������ X
			destY,							//����� ��ǥ ������ Y
			_imageInfo->frameWidth,			//����� ũ�� width
			_imageInfo->frameHeight,		//����� ũ�� height
			_imageInfo->hMemDC,				//������ DC
			currentFrameX * _imageInfo->frameWidth,
			currentFrameY * _imageInfo->frameHeight,		//����� ��������(���� 0, 0)
			_imageInfo->frameWidth,			//����� �������� width
			_imageInfo->frameHeight,		//����� �������� height
			_transColor);					//���翡�� ���ܵ� ����
	}
	else
	{
		//DC�������� ��Ӻ��� �Լ�
		BitBlt(hdc, destX, destY,
			_imageInfo->frameWidth, _imageInfo->frameHeight,
			_imageInfo->hMemDC,
			currentFrameX * _imageInfo->frameWidth,
			currentFrameY * _imageInfo->frameHeight,
			SRCCOPY);
	}
}

void image::loopRender(HDC hdc, const LPRECT drawArea, int offSetX, int offSetY)
{
	//offSet���� ������ ��� �ٽ� �纸��
	if (offSetX < 0) offSetX = _imageInfo->width + (offSetX % _imageInfo->width);
	if (offSetY < 0) offSetY = _imageInfo->height + (offSetY % _imageInfo->height);

	//�׷����� �ҽ��� ����(�̹���) ����
	RECT rcSour;
	int sourWidth;
	int sourHeight;

	//�׷����� ����
	RECT rcDest;

	int drawAreaX = drawArea->left;
	int drawAreaY = drawArea->top;
	int drawAreaW = drawArea->right - drawArea->left;
	int drawAreaH = drawArea->bottom - drawArea->top;

	//���� ����
	for (int y = 0; y < drawAreaH; y += sourHeight)
	{
		//�̹��� �ҽ��� ���� ���
		rcSour.top = (y + offSetY) % _imageInfo->height;
		rcSour.bottom = _imageInfo->height;
		sourHeight = rcSour.bottom - rcSour.top;

		//�ҽ��� ������ ��ο� �����(�׷��ִ� ����) ������ ������
		if (y + sourHeight > drawAreaH)
		{
			//�Ѿ �׸��� ����ŭ ������ ������
			rcSour.bottom -= (y + sourHeight) - drawAreaH;
			sourHeight = rcSour.bottom - rcSour.top;
		}

		rcDest.top = y + drawAreaY;
		rcDest.bottom = rcDest.top + sourHeight;

		//���� ������ �ȿ��ٰ�!
		for (int x = 0; x < drawAreaW; x += sourWidth)
		{
			//�ҽ��� ���� ���� ���
			rcSour.left = (x + offSetX) % _imageInfo->width;
			rcSour.right = _imageInfo->width;
			sourWidth = rcSour.right - rcSour.left;

			if (x + sourWidth > drawAreaW)
			{
				rcSour.right -= (x + sourWidth) - drawAreaW;
				sourWidth = rcSour.right - rcSour.left;
			}

			//�׷����� ����
			rcDest.left = x + drawAreaX;
			rcDest.right = rcDest.left + sourWidth;

			//�׷��ش�
			render(hdc, rcDest.left, rcDest.top,
				rcSour.left, rcSour.top,
				rcSour.right - rcSour.left,
				rcSour.bottom - rcSour.top);
		}
	}
}

void image::alphaRender(HDC hdc, BYTE alpha)
{
	_blendFunc.SourceConstantAlpha = alpha;

	if (_trans)
	{
		//DC�������� ��Ӻ��� �Լ�
		BitBlt(_blendImage->hMemDC, 0, 0,
			_imageInfo->width, _imageInfo->height,
			hdc, (int)_imageInfo->x, (int)_imageInfo->y, SRCCOPY);

		GdiTransparentBlt(_blendImage->hMemDC,	//����� ����� DC
			0,									//����� ��ǥ ������ X
			0,									//����� ��ǥ ������ Y
			_imageInfo->width,					//����� ũ�� width
			_imageInfo->height,					//����� ũ�� height
			_imageInfo->hMemDC,					//������ DC
			0, 0,								//����� ��������(���� 0, 0)
			_imageInfo->width,					//����� �������� width
			_imageInfo->height,					//����� �������� height
			_transColor);						//���翡�� ���ܵ� ����

		AlphaBlend(hdc, (int)_imageInfo->x, (int)_imageInfo->y, _imageInfo->width, _imageInfo->height,
			_blendImage->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height, _blendFunc);
	}
	else
	{
		//DC�������� ��Ӻ��� �Լ�
		AlphaBlend(hdc, (int)_imageInfo->x, (int)_imageInfo->y, _imageInfo->width, _imageInfo->height,
			_imageInfo->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height, _blendFunc);
	}
}

void image::alphaRender(HDC hdc, int destX, int destY, BYTE alpha)
{
	_blendFunc.SourceConstantAlpha = alpha;

	if (_trans)
	{
		//DC�������� ��Ӻ��� �Լ�
		BitBlt(_blendImage->hMemDC, 0, 0,
			_imageInfo->width, _imageInfo->height,
			hdc, destX, destY, SRCCOPY);

		GdiTransparentBlt(_blendImage->hMemDC,	//����� ����� DC
			0,									//����� ��ǥ ������ X
			0,									//����� ��ǥ ������ Y
			_imageInfo->width,					//����� ũ�� width
			_imageInfo->height,					//����� ũ�� height
			_imageInfo->hMemDC,					//������ DC
			0, 0,								//����� ��������(���� 0, 0)
			_imageInfo->width,					//����� �������� width
			_imageInfo->height,					//����� �������� height
			_transColor);						//���翡�� ���ܵ� ����

		AlphaBlend(hdc, destX, destY, _imageInfo->width, _imageInfo->height,
			_blendImage->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height, _blendFunc);
	}
	else
	{
		//DC�������� ��Ӻ��� �Լ�
		AlphaBlend(hdc, destX, destY, _imageInfo->width, _imageInfo->height,
			_imageInfo->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height, _blendFunc);
	}
}

void image::alphaRender(HDC hdc, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight, BYTE alpha)
{
	_blendFunc.SourceConstantAlpha = alpha;

	if (_trans)
	{
		//DC�������� ��Ӻ��� �Լ�
		BitBlt(_blendImage->hMemDC, 0, 0,
			_imageInfo->width, _imageInfo->height,
			hdc, destX, destY, SRCCOPY);

		GdiTransparentBlt(_blendImage->hMemDC,	//����� ����� DC
			0,									//����� ��ǥ ������ X
			0,									//����� ��ǥ ������ Y
			sourWidth,							//����� ũ�� width
			sourHeight,							//����� ũ�� height
			_imageInfo->hMemDC,					//������ DC
			sourX, sourY,						//����� ��������(���� 0, 0)
			sourWidth,							//����� �������� width
			sourHeight,							//����� �������� height
			_transColor);						//���翡�� ���ܵ� ����

		AlphaBlend(hdc, destX, destY, sourWidth, sourHeight,
			_blendImage->hMemDC, 0, 0, sourWidth, sourHeight, _blendFunc);
	}
	else
	{
		//DC�������� ��Ӻ��� �Լ�
		AlphaBlend(hdc, destX, destY, sourWidth, sourHeight,
			_imageInfo->hMemDC, 0, 0, sourWidth, sourHeight, _blendFunc);
	}
}

void image::alphaFrameRender(HDC hdc, int destX, int destY, BYTE alpha)
{
	_blendFunc.SourceConstantAlpha = alpha;

	if (_trans)
	{
		BitBlt(_blendImage->hMemDC, 0, 0,
			_imageInfo->width, _imageInfo->height,
			hdc, destX, destY, SRCCOPY);

		GdiTransparentBlt(_blendImage->hMemDC,
			0,
			0,
			_imageInfo->frameWidth,
			_imageInfo->frameHeight,
			_imageInfo->hMemDC,
			_imageInfo->currentFrameX * _imageInfo->frameWidth,
			_imageInfo->currentFrameY * _imageInfo->frameHeight,
			_imageInfo->frameWidth,
			_imageInfo->frameHeight,
			_transColor);

		AlphaBlend(hdc, destX, destY, _imageInfo->width, _imageInfo->height,
			_blendImage->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height, _blendFunc);
	}

	else
	{
		AlphaBlend(hdc, destX, destY, _imageInfo->width, _imageInfo->height,
			_blendImage->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height, _blendFunc);
	}
}


void image::alphaFrameRender(HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY, BYTE alpha)
{
	_blendFunc.SourceConstantAlpha = alpha;

	if (_trans)
	{
		//DC�������� ��Ӻ��� �Լ�
		BitBlt(_blendImage->hMemDC, 0, 0,
			_imageInfo->width, _imageInfo->height,
			hdc, destX, destY, SRCCOPY);

		GdiTransparentBlt(_blendImage->hMemDC,	//����� ����� DC
			0,									//����� ��ǥ ������ X
			0,									//����� ��ǥ ������ Y
			_imageInfo->frameWidth,					//����� ũ�� width
			_imageInfo->frameHeight,					//����� ũ�� height
			_imageInfo->hMemDC,					//������ DC
			currentFrameX * _imageInfo->frameWidth,
			currentFrameY * _imageInfo->frameHeight,
			_imageInfo->frameWidth,					//����� �������� width
			_imageInfo->frameHeight,					//����� �������� height
			_transColor);						//���翡�� ���ܵ� ����

		AlphaBlend(hdc, destX, destY, _imageInfo->width, _imageInfo->height,
			_blendImage->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height, _blendFunc);
	}
	else
	{
		//DC�������� ��Ӻ��� �Լ�
		AlphaBlend(hdc, destX, destY, _imageInfo->width, _imageInfo->height,
			_imageInfo->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height, _blendFunc);
	}
}

void image::aniRender(HDC hdc, int destX, int destY, animation* ani)
{
	render(hdc, destX, destY, ani->getFramePos().x, ani->getFramePos().y, ani->getFrameWidth(), ani->getFrameHeight());
}