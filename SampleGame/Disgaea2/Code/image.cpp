#include "stdafx.h"
#include "image.h"

//pragma == 컴파일러 지시기
#pragma comment(lib, "msimg32.lib")

//멤버 이니셜라이즈~
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

//빈 비트맵 이미지 만들어서 쓸 경우
HRESULT image::init(int width, int height, BOOL alpha)
{
	if (_imageInfo != NULL) release();

	HDC hdc = GetDC(_hWnd);

	//이미지 정보 생성
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

	//만약에 이미지리소스 얻어오는걸 실패했으면~
	if (_imageInfo->hBit == 0)
	{
		//해제해준다
		release();

		return E_FAIL;
	}
	if (alpha)
	{
		//알파 블렌드 옵션~
		_blendFunc.BlendFlags = 0;
		_blendFunc.AlphaFormat = 0;
		_blendFunc.BlendOp = AC_SRC_OVER;

		//알파 블렌드 사용을 위한 이미지 설정
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

//리소스로부터 읽어들여서 쓸 경우
HRESULT image::init(const DWORD resID, int width, int height, BOOL alpha,
	BOOL trans, COLORREF transColor)
{

	if (_imageInfo != NULL) release();

	HDC hdc = GetDC(_hWnd);

	//이미지 정보 생성
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

	//만약에 이미지리소스 얻어오는걸 실패했으면~
	if (_imageInfo->hBit == 0)
	{
		//해제해준다
		release();

		return E_FAIL;
	}
	if (alpha)
	{
		//알파 블렌드 옵션~
		_blendFunc.BlendFlags = 0;
		_blendFunc.AlphaFormat = 0;
		_blendFunc.BlendOp = AC_SRC_OVER;

		//알파 블렌드 사용을 위한 이미지 설정
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


//파일로부터 읽어들여서 쓸 경우
HRESULT image::init(const char* fileName, int width, int height, BOOL alpha,
	BOOL trans, COLORREF transColor)
{
	if (fileName == NULL) return E_FAIL;

	if (_imageInfo != NULL) release();

	HDC hdc = GetDC(_hWnd);

	//이미지 정보 생성
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

	//파일 경로 이름을 복사해서 가져온다
	int len = strlen(fileName);

	_fileName = new TCHAR[len + 1];
	strcpy_s(_fileName, len + 1, fileName);


	_trans = trans;
	_transColor = transColor;

	//만약에 이미지리소스 얻어오는걸 실패했으면~
	if (_imageInfo->hBit == 0)
	{
		//해제해준다
		release();

		return E_FAIL;
	}
	if (alpha)
	{
		//알파 블렌드 옵션~
		_blendFunc.BlendFlags = 0;
		_blendFunc.AlphaFormat = 0;
		_blendFunc.BlendOp = AC_SRC_OVER;

		//알파 블렌드 사용을 위한 이미지 설정
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

	//이미지 정보 생성
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

	//파일 경로 이름을 복사해서 가져온다
	int len = strlen(fileName);

	_fileName = new char[len + 1];
	strcpy_s(_fileName, len + 1, fileName);


	_trans = trans;
	_transColor = transColor;

	//만약에 이미지리소스 얻어오는걸 실패했으면~
	if (_imageInfo->hBit == 0)
	{
		//해제해준다
		release();

		return E_FAIL;
	}
	if (alpha)
	{
		//알파 블렌드 옵션~
		_blendFunc.BlendFlags = 0;
		_blendFunc.AlphaFormat = 0;
		_blendFunc.BlendOp = AC_SRC_OVER;

		//알파 블렌드 사용을 위한 이미지 설정
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

	//이미지 정보 생성
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

	//파일 경로 이름을 복사해서 가져온다
	int len = strlen(fileName);

	_fileName = new char[len + 1];
	strcpy_s(_fileName, len + 1, fileName);


	_trans = trans;
	_transColor = transColor;

	//만약에 이미지리소스 얻어오는걸 실패했으면~
	if (_imageInfo->hBit == 0)
	{
		//해제해준다
		release();

		return E_FAIL;
	}
	if (alpha)
	{
		//알파 블렌드 옵션~
		_blendFunc.BlendFlags = 0;
		_blendFunc.AlphaFormat = 0;
		_blendFunc.BlendOp = AC_SRC_OVER;

		//알파 블렌드 사용을 위한 이미지 설정
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

	//이미지 정보 생성
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

	//파일 경로 이름을 복사해서 가져온다
	int len = strlen(fileName);

	_fileName = new char[len + 1];
	strcpy_s(_fileName, len + 1, fileName);


	_trans = trans;
	_transColor = transColor;

	//만약에 이미지리소스 얻어오는걸 실패했으면~
	if (_imageInfo->hBit == 0)
	{
		//해제해준다
		release();

		return E_FAIL;
	}
	if (alpha)
	{
		//알파 블렌드 옵션~
		_blendFunc.BlendFlags = 0;
		_blendFunc.AlphaFormat = 0;
		_blendFunc.BlendOp = AC_SRC_OVER;

		//알파 블렌드 사용을 위한 이미지 설정
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
		//이미지 삭제
		SelectObject(_imageInfo->hMemDC, _imageInfo->hObit);
		DeleteObject(_imageInfo->hBit);
		DeleteDC(_imageInfo->hMemDC);

		//알파블렌드 사용 이미지 삭제
		SelectObject(_blendImage->hMemDC, _blendImage->hObit);
		DeleteObject(_blendImage->hBit);
		DeleteDC(_blendImage->hMemDC);

		//포인터도 삭제
		SAFE_DELETE(_imageInfo);
		SAFE_DELETE(_fileName);
		SAFE_DELETE(_blendImage);

		//투명값도 초기화 해줍시다
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
	//이미지를 본격적으로 그려주는건 얘다!
	if (_trans)
	{
		GdiTransparentBlt(hdc,	//복사될 장소 DC
			_imageInfo->x,				//복사될 좌표 시작점 X
			_imageInfo->y,				//복사될 좌표 시작점 Y
			_imageInfo->width,	//복사될 크기 width
			_imageInfo->height,	//복사될 크기 height
			_imageInfo->hMemDC,	//복사대상 DC
			0, 0,				//복사될 시작지점
			_imageInfo->width,	//복사될 영역지정 width
			_imageInfo->height,	//복사될 영역지정 height
			_transColor);		//복사에서 제외될 색상
	}
	else
	{
		//DC영역간의 고속복사
		BitBlt(hdc, _imageInfo->x, _imageInfo->y,
			_imageInfo->width, _imageInfo->height,
			_imageInfo->hMemDC, 0, 0, SRCCOPY);
	}

}

//이미지 렌더해줄 함수
//					 그려줄 x좌표 그려줄 y좌표
void image::render(HDC hdc, int destX, int destY)
{
	if (_trans)
	{
		GdiTransparentBlt(hdc,	//복사될 장소의 DC
			destX,				//복사될 좌표 시작점 X
			destY,				//복사될 좌표 시작점 Y
			_imageInfo->width,	//복사될 크기 width
			_imageInfo->height,	//복사될 크기 height
			_imageInfo->hMemDC,	//복사대상 DC
			0, 0,				//복사될 시작지점(보통 0, 0)
			_imageInfo->width,	//복사될 영역지정 width
			_imageInfo->height,	//복사될 영역지정 height
			_transColor);		//복사에서 제외될 색상
	}
	else
	{
		//DC영역간의 고속복사 함수
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
		GdiTransparentBlt(hdc,	//복사될 장소의 DC
			destX,				//복사될 좌표 시작점 X
			destY,				//복사될 좌표 시작점 Y
			sourWidth,			//복사될 크기 width
			sourHeight,			//복사될 크기 height
			_imageInfo->hMemDC,	//복사대상 DC
			sourX, sourY,		//복사될 시작지점(보통 0, 0)
			sourWidth,			//복사될 영역지정 width
			sourHeight,			//복사될 영역지정 height
			_transColor);		//복사에서 제외될 색상
	}
	else
	{
		//DC영역간의 고속복사 함수
		BitBlt(hdc, destX, destY,
			sourWidth, sourHeight,
			_imageInfo->hMemDC, sourX, sourY, SRCCOPY);
	}
}

void image::frameRender(HDC hdc, int destX, int destY)
{
	if (_trans)
	{
		GdiTransparentBlt(hdc,				//복사될 장소의 DC
			destX,							//복사될 좌표 시작점 X
			destY,							//복사될 좌표 시작점 Y
			_imageInfo->frameWidth,			//복사될 크기 width
			_imageInfo->frameHeight,		//복사될 크기 height
			_imageInfo->hMemDC,				//복사대상 DC
			_imageInfo->currentFrameX * _imageInfo->frameWidth,
			_imageInfo->currentFrameY * _imageInfo->frameHeight,		//복사될 시작지점(보통 0, 0)
			_imageInfo->frameWidth,			//복사될 영역지정 width
			_imageInfo->frameHeight,		//복사될 영역지정 height
			_transColor);					//복사에서 제외될 색상
	}
	else
	{
		//DC영역간의 고속복사 함수
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

		GdiTransparentBlt(hdc,				//복사될 장소의 DC
			destX,							//복사될 좌표 시작점 X
			destY,							//복사될 좌표 시작점 Y
			_imageInfo->frameWidth,			//복사될 크기 width
			_imageInfo->frameHeight,		//복사될 크기 height
			_imageInfo->hMemDC,				//복사대상 DC
			currentFrameX * _imageInfo->frameWidth,
			currentFrameY * _imageInfo->frameHeight,		//복사될 시작지점(보통 0, 0)
			_imageInfo->frameWidth,			//복사될 영역지정 width
			_imageInfo->frameHeight,		//복사될 영역지정 height
			_transColor);					//복사에서 제외될 색상
	}
	else
	{
		//DC영역간의 고속복사 함수
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
	//offSet값이 음수일 경우 다시 재보정
	if (offSetX < 0) offSetX = _imageInfo->width + (offSetX % _imageInfo->width);
	if (offSetY < 0) offSetY = _imageInfo->height + (offSetY % _imageInfo->height);

	//그려지는 소스의 영역(이미지) 셋팅
	RECT rcSour;
	int sourWidth;
	int sourHeight;

	//그려지는 영역
	RECT rcDest;

	int drawAreaX = drawArea->left;
	int drawAreaY = drawArea->top;
	int drawAreaW = drawArea->right - drawArea->left;
	int drawAreaH = drawArea->bottom - drawArea->top;

	//세로 먼저
	for (int y = 0; y < drawAreaH; y += sourHeight)
	{
		//이미지 소스의 높이 계산
		rcSour.top = (y + offSetY) % _imageInfo->height;
		rcSour.bottom = _imageInfo->height;
		sourHeight = rcSour.bottom - rcSour.top;

		//소스의 영역이 드로우 에어리어(그려주는 영역) 밖으로 나가면
		if (y + sourHeight > drawAreaH)
		{
			//넘어간 그림의 값만큼 밑으로 보낸다
			rcSour.bottom -= (y + sourHeight) - drawAreaH;
			sourHeight = rcSour.bottom - rcSour.top;
		}

		rcDest.top = y + drawAreaY;
		rcDest.bottom = rcDest.top + sourHeight;

		//가로 루프는 안에다가!
		for (int x = 0; x < drawAreaW; x += sourWidth)
		{
			//소스의 영역 가로 계산
			rcSour.left = (x + offSetX) % _imageInfo->width;
			rcSour.right = _imageInfo->width;
			sourWidth = rcSour.right - rcSour.left;

			if (x + sourWidth > drawAreaW)
			{
				rcSour.right -= (x + sourWidth) - drawAreaW;
				sourWidth = rcSour.right - rcSour.left;
			}

			//그려지는 영역
			rcDest.left = x + drawAreaX;
			rcDest.right = rcDest.left + sourWidth;

			//그려준다
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
		//DC영역간의 고속복사 함수
		BitBlt(_blendImage->hMemDC, 0, 0,
			_imageInfo->width, _imageInfo->height,
			hdc, (int)_imageInfo->x, (int)_imageInfo->y, SRCCOPY);

		GdiTransparentBlt(_blendImage->hMemDC,	//복사될 장소의 DC
			0,									//복사될 좌표 시작점 X
			0,									//복사될 좌표 시작점 Y
			_imageInfo->width,					//복사될 크기 width
			_imageInfo->height,					//복사될 크기 height
			_imageInfo->hMemDC,					//복사대상 DC
			0, 0,								//복사될 시작지점(보통 0, 0)
			_imageInfo->width,					//복사될 영역지정 width
			_imageInfo->height,					//복사될 영역지정 height
			_transColor);						//복사에서 제외될 색상

		AlphaBlend(hdc, (int)_imageInfo->x, (int)_imageInfo->y, _imageInfo->width, _imageInfo->height,
			_blendImage->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height, _blendFunc);
	}
	else
	{
		//DC영역간의 고속복사 함수
		AlphaBlend(hdc, (int)_imageInfo->x, (int)_imageInfo->y, _imageInfo->width, _imageInfo->height,
			_imageInfo->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height, _blendFunc);
	}
}

void image::alphaRender(HDC hdc, int destX, int destY, BYTE alpha)
{
	_blendFunc.SourceConstantAlpha = alpha;

	if (_trans)
	{
		//DC영역간의 고속복사 함수
		BitBlt(_blendImage->hMemDC, 0, 0,
			_imageInfo->width, _imageInfo->height,
			hdc, destX, destY, SRCCOPY);

		GdiTransparentBlt(_blendImage->hMemDC,	//복사될 장소의 DC
			0,									//복사될 좌표 시작점 X
			0,									//복사될 좌표 시작점 Y
			_imageInfo->width,					//복사될 크기 width
			_imageInfo->height,					//복사될 크기 height
			_imageInfo->hMemDC,					//복사대상 DC
			0, 0,								//복사될 시작지점(보통 0, 0)
			_imageInfo->width,					//복사될 영역지정 width
			_imageInfo->height,					//복사될 영역지정 height
			_transColor);						//복사에서 제외될 색상

		AlphaBlend(hdc, destX, destY, _imageInfo->width, _imageInfo->height,
			_blendImage->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height, _blendFunc);
	}
	else
	{
		//DC영역간의 고속복사 함수
		AlphaBlend(hdc, destX, destY, _imageInfo->width, _imageInfo->height,
			_imageInfo->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height, _blendFunc);
	}
}

void image::alphaRender(HDC hdc, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight, BYTE alpha)
{
	_blendFunc.SourceConstantAlpha = alpha;

	if (_trans)
	{
		//DC영역간의 고속복사 함수
		BitBlt(_blendImage->hMemDC, 0, 0,
			_imageInfo->width, _imageInfo->height,
			hdc, destX, destY, SRCCOPY);

		GdiTransparentBlt(_blendImage->hMemDC,	//복사될 장소의 DC
			0,									//복사될 좌표 시작점 X
			0,									//복사될 좌표 시작점 Y
			sourWidth,							//복사될 크기 width
			sourHeight,							//복사될 크기 height
			_imageInfo->hMemDC,					//복사대상 DC
			sourX, sourY,						//복사될 시작지점(보통 0, 0)
			sourWidth,							//복사될 영역지정 width
			sourHeight,							//복사될 영역지정 height
			_transColor);						//복사에서 제외될 색상

		AlphaBlend(hdc, destX, destY, sourWidth, sourHeight,
			_blendImage->hMemDC, 0, 0, sourWidth, sourHeight, _blendFunc);
	}
	else
	{
		//DC영역간의 고속복사 함수
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
		//DC영역간의 고속복사 함수
		BitBlt(_blendImage->hMemDC, 0, 0,
			_imageInfo->width, _imageInfo->height,
			hdc, destX, destY, SRCCOPY);

		GdiTransparentBlt(_blendImage->hMemDC,	//복사될 장소의 DC
			0,									//복사될 좌표 시작점 X
			0,									//복사될 좌표 시작점 Y
			_imageInfo->frameWidth,					//복사될 크기 width
			_imageInfo->frameHeight,					//복사될 크기 height
			_imageInfo->hMemDC,					//복사대상 DC
			currentFrameX * _imageInfo->frameWidth,
			currentFrameY * _imageInfo->frameHeight,
			_imageInfo->frameWidth,					//복사될 영역지정 width
			_imageInfo->frameHeight,					//복사될 영역지정 height
			_transColor);						//복사에서 제외될 색상

		AlphaBlend(hdc, destX, destY, _imageInfo->width, _imageInfo->height,
			_blendImage->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height, _blendFunc);
	}
	else
	{
		//DC영역간의 고속복사 함수
		AlphaBlend(hdc, destX, destY, _imageInfo->width, _imageInfo->height,
			_imageInfo->hMemDC, 0, 0, _imageInfo->width, _imageInfo->height, _blendFunc);
	}
}

void image::aniRender(HDC hdc, int destX, int destY, animation* ani)
{
	render(hdc, destX, destY, ani->getFramePos().x, ani->getFramePos().y, ani->getFrameWidth(), ani->getFrameHeight());
}