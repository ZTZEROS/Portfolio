#include "stdafx.h"
#include "imageManager.h"


imageManager::imageManager()
{
}


imageManager::~imageManager()
{
}

HRESULT imageManager::init(void)
{
	_fadeSpeed = 0;
	_speedCount = 0;
	_alpha = 0;
	_fadeInit = false;
	_black = addImage("black", "black.bmp", 600, 1000, true, RGB(255, 0, 255), true);

	return S_OK;
}

void imageManager::release(void)
{
	deleteAll();
}

//이미지 추가
image* imageManager::addImage(string strKey, int width, int height, BOOL alpha)
{
	//먼저 동일한 키 값을 가진 이미지가 있는지 검색
	image* img = findImage(strKey);

	//만약에 이미지가 있으면~ 그 이미지를 반환
	if (img) return img;

	//여기까지 왔으면 없는거니까
	img = new image;

	//만약 제대로 이미지가 초기화 되지 않았으면 널값으로 초기화
	if (FAILED(img->init(width, height, alpha)))
	{
		SAFE_DELETE(img);

		return NULL;
	}

	//굉장히 잘 된 상황이므로 맵에 넣어줍니다 ㅋ _ㅋ
	_mImageList.insert(make_pair(strKey, img));

	return img;

}


image* imageManager::addImage(string strKey, const DWORD resID, int width, int height, bool trans, COLORREF transColor, bool alpha)
{
	//먼저 동일한 키 값을 가진 이미지가 있는지 검색
	image* img = findImage(strKey);

	//만약에 이미지가 있으면~ 그 이미지를 반환
	if (img) return img;

	//여기까지 왔으면 없는거니까
	img = new image;

	//만약 제대로 이미지가 초기화 되지 않았으면 널값으로 초기화
	if (FAILED(img->init(resID, width, height, alpha, trans, transColor)))
	{
		SAFE_DELETE(img);

		return NULL;
	}

	//굉장히 잘 된 상황이므로 맵에 넣어줍니다 ㅋ _ㅋ
	_mImageList.insert(make_pair(strKey, img));

	return img;
}

image* imageManager::addImage(string strKey, const char* fileName, int width, int height, bool trans, COLORREF transColor, bool alpha)
{
	//먼저 동일한 키 값을 가진 이미지가 있는지 검색
	image* img = findImage(strKey);

	//만약에 이미지가 있으면~ 그 이미지를 반환
	if (img) return img;

	//여기까지 왔으면 없는거니까
	img = new image;

	//만약 제대로 이미지가 초기화 되지 않았으면 널값으로 초기화
	if (FAILED(img->init(fileName, width, height, alpha, trans, transColor)))
	{
		SAFE_DELETE(img);

		return NULL;
	}

	//굉장히 잘 된 상황이므로 맵에 넣어줍니다 ㅋ _ㅋ
	_mImageList.insert(make_pair(strKey, img));

	return img;
}

image* imageManager::addImage(string strKey, const char* fileName, float x, float y, int width, int height, bool trans, COLORREF transColor, bool alpha)
{
	//먼저 동일한 키 값을 가진 이미지가 있는지 검색
	image* img = findImage(strKey);

	//만약에 이미지가 있으면~ 그 이미지를 반환
	if (img) return img;

	//여기까지 왔으면 없는거니까
	img = new image;

	//만약 제대로 이미지가 초기화 되지 않았으면 널값으로 초기화
	if (FAILED(img->init(fileName, x, y, width, height, alpha, trans, transColor)))
	{
		SAFE_DELETE(img);

		return NULL;
	}

	//굉장히 잘 된 상황이므로 맵에 넣어줍니다 ㅋ _ㅋ
	_mImageList.insert(make_pair(strKey, img));

	return img;
}


//프레임 이미지 추가
image* imageManager::addFrameImage(string strKey, const char* fileName, float x, float y, int width, int height, int frameX, int frameY, bool trans, COLORREF transColor, bool alpha)
{
	//먼저 동일한 키 값을 가진 이미지가 있는지 검색
	image* img = findImage(strKey);

	//만약에 이미지가 있으면~ 그 이미지를 반환
	if (img) return img;

	//여기까지 왔으면 없는거니까
	img = new image;

	//만약 제대로 이미지가 초기화 되지 않았으면 널값으로 초기화
	if (FAILED(img->init(fileName, x, y, width, height, frameX, frameY, alpha, trans, transColor)))
	{
		SAFE_DELETE(img);

		return NULL;
	}

	//굉장히 잘 된 상황이므로 맵에 넣어줍니다 ㅋ _ㅋ
	_mImageList.insert(make_pair(strKey, img));

	return img;
}

image* imageManager::addFrameImage(string strKey, const char* fileName, int width, int height, int frameX, int frameY, bool trans, COLORREF transColor, bool alpha)
{
	//먼저 동일한 키 값을 가진 이미지가 있는지 검색
	image* img = findImage(strKey);

	//만약에 이미지가 있으면~ 그 이미지를 반환
	if (img) return img;

	//여기까지 왔으면 없는거니까
	img = new image;

	//만약 제대로 이미지가 초기화 되지 않았으면 널값으로 초기화
	if (FAILED(img->init(fileName, width, height, frameX, frameY, alpha, trans, transColor)))
	{
		SAFE_DELETE(img);

		return NULL;
	}

	//굉장히 잘 된 상황이므로 맵에 넣어줍니다 ㅋ _ㅋ
	_mImageList.insert(make_pair(strKey, img));

	return img;
}


//이미지 찾는 함수
image* imageManager::findImage(string strKey)
{
	mapImageIter key = _mImageList.find(strKey);

	if (key != _mImageList.end())
	{
		return key->second;
	}

	return NULL;
}


BOOL imageManager::deleteImage(string strKey)
{
	//해당 키 값이 존재하는지 검사한다
	mapImageIter key = _mImageList.find(strKey);

	if (key != _mImageList.end())
	{
		key->second->release();
		SAFE_DELETE(key->second);
		_mImageList.erase(key);

		return true;
	}

	return false;

}

BOOL imageManager::deleteAll(void)
{
	mapImageIter iter = _mImageList.begin();

	for (; iter != _mImageList.end();)
	{
		if (iter->second != NULL)
		{
			SAFE_DELETE(iter->second);
			iter = _mImageList.erase(iter);
		}
		else
		{
			++iter;
		}
	}

	_mImageList.clear();

	return TRUE;
}

//렌더 함수
void imageManager::render(string strKey, HDC hdc)
{
	image* img = findImage(strKey);
	if (img) img->render(hdc);
}

void imageManager::render(string strKey, HDC hdc, int destX, int destY)
{
	image* img = findImage(strKey);
	if (img) img->render(hdc, destX, destY);
}

void imageManager::render(string strKey, HDC hdc, int destX, int destY,
	int sourX, int sourY, int sourWidth, int sourHeight)
{
	image* img = findImage(strKey);
	if (img) img->render(hdc, destX, destY, sourX, sourY,
		sourWidth, sourHeight);
}

void imageManager::frameRender(string strKey, HDC hdc, int destX, int destY)
{
	image* img = findImage(strKey);
	if (img) img->frameRender(hdc, destX, destY);
}

void imageManager::frameRender(string strKey, HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY)
{
	image* img = findImage(strKey);
	if (img) img->frameRender(hdc, destX, destY, currentFrameX, currentFrameY);
}

//뤂 렌더
void imageManager::loopRender(string strKey, HDC hdc, const LPRECT drawArea, int offSetX, int offSetY)
{
	image* img = findImage(strKey);
	if (img) img->loopRender(hdc, drawArea, offSetX, offSetY);
}

void imageManager::alphaRender(string strKey, HDC hdc, BYTE alpha)
{
	image* img = findImage(strKey);
	if (img) img->alphaRender(hdc, alpha);
}

void imageManager::alphaRender(string strKey, HDC hdc, int destX, int destY, BYTE alpha)
{
	image* img = findImage(strKey);
	if (img) img->alphaRender(hdc, destX, destY, alpha);
}

void imageManager::alphaRender(string strKey, HDC hdc, int destX, int destY,
	int sourX, int sourY, int sourWidth, int sourHeight, BYTE alpha)
{
	image* img = findImage(strKey);
	if (img) img->alphaRender(hdc, destX, destY, sourX, sourY, sourWidth, sourHeight, alpha);
}

void imageManager::alphaFrameRender(string strKey, HDC hdc, int destX, int destY, BYTE alpha)
{
	image* img = findImage(strKey);
	if (img) img->alphaFrameRender(hdc, destX, destY, alpha);
}
void imageManager::alphaFrameRender(string strKey, HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY, BYTE alpha)
{
	image* img = findImage(strKey);
	if (img) img->alphaFrameRender(hdc, destX, destY, currentFrameX, currentFrameY, alpha);
}

bool imageManager::fadeIn(float speed, HDC hdc)
{
	if (!_fadeInit)
	{
		_alpha = 255;
		_fadeSpeed = speed;
		_speedCount = 0;
		_fadeInit = true;
	}

	_speedCount += TIMEMANAGER->getElapsedTime();
	if (_speedCount >= _fadeSpeed / 100)
	{
		_speedCount = 0;
		_alpha -= 5;
	}

	_black->alphaRender(hdc, _alpha);

	if (_alpha == 0)
	{
		_fadeInit = false;
		return true;
	}

	return false;
}

bool imageManager::fadeOut(float speed, HDC hdc)
{
	if (!_fadeInit)
	{
		_alpha = 0;
		_fadeSpeed = speed;
		_speedCount = 0;
		_fadeInit = true;
	}

	_speedCount += TIMEMANAGER->getElapsedTime();
	if (_speedCount >= _fadeSpeed / 100)
	{
		_speedCount = 0;
		_alpha += 5;
	}

	_black->alphaRender(hdc, 0, 0, _alpha);

	if (_alpha == 255)
	{
		_fadeInit = false;
		return true;
	}

	return false;
}