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

//�̹��� �߰�
image* imageManager::addImage(string strKey, int width, int height, BOOL alpha)
{
	//���� ������ Ű ���� ���� �̹����� �ִ��� �˻�
	image* img = findImage(strKey);

	//���࿡ �̹����� ������~ �� �̹����� ��ȯ
	if (img) return img;

	//������� ������ ���°Ŵϱ�
	img = new image;

	//���� ����� �̹����� �ʱ�ȭ ���� �ʾ����� �ΰ����� �ʱ�ȭ
	if (FAILED(img->init(width, height, alpha)))
	{
		SAFE_DELETE(img);

		return NULL;
	}

	//������ �� �� ��Ȳ�̹Ƿ� �ʿ� �־��ݴϴ� �� _��
	_mImageList.insert(make_pair(strKey, img));

	return img;

}


image* imageManager::addImage(string strKey, const DWORD resID, int width, int height, bool trans, COLORREF transColor, bool alpha)
{
	//���� ������ Ű ���� ���� �̹����� �ִ��� �˻�
	image* img = findImage(strKey);

	//���࿡ �̹����� ������~ �� �̹����� ��ȯ
	if (img) return img;

	//������� ������ ���°Ŵϱ�
	img = new image;

	//���� ����� �̹����� �ʱ�ȭ ���� �ʾ����� �ΰ����� �ʱ�ȭ
	if (FAILED(img->init(resID, width, height, alpha, trans, transColor)))
	{
		SAFE_DELETE(img);

		return NULL;
	}

	//������ �� �� ��Ȳ�̹Ƿ� �ʿ� �־��ݴϴ� �� _��
	_mImageList.insert(make_pair(strKey, img));

	return img;
}

image* imageManager::addImage(string strKey, const char* fileName, int width, int height, bool trans, COLORREF transColor, bool alpha)
{
	//���� ������ Ű ���� ���� �̹����� �ִ��� �˻�
	image* img = findImage(strKey);

	//���࿡ �̹����� ������~ �� �̹����� ��ȯ
	if (img) return img;

	//������� ������ ���°Ŵϱ�
	img = new image;

	//���� ����� �̹����� �ʱ�ȭ ���� �ʾ����� �ΰ����� �ʱ�ȭ
	if (FAILED(img->init(fileName, width, height, alpha, trans, transColor)))
	{
		SAFE_DELETE(img);

		return NULL;
	}

	//������ �� �� ��Ȳ�̹Ƿ� �ʿ� �־��ݴϴ� �� _��
	_mImageList.insert(make_pair(strKey, img));

	return img;
}

image* imageManager::addImage(string strKey, const char* fileName, float x, float y, int width, int height, bool trans, COLORREF transColor, bool alpha)
{
	//���� ������ Ű ���� ���� �̹����� �ִ��� �˻�
	image* img = findImage(strKey);

	//���࿡ �̹����� ������~ �� �̹����� ��ȯ
	if (img) return img;

	//������� ������ ���°Ŵϱ�
	img = new image;

	//���� ����� �̹����� �ʱ�ȭ ���� �ʾ����� �ΰ����� �ʱ�ȭ
	if (FAILED(img->init(fileName, x, y, width, height, alpha, trans, transColor)))
	{
		SAFE_DELETE(img);

		return NULL;
	}

	//������ �� �� ��Ȳ�̹Ƿ� �ʿ� �־��ݴϴ� �� _��
	_mImageList.insert(make_pair(strKey, img));

	return img;
}


//������ �̹��� �߰�
image* imageManager::addFrameImage(string strKey, const char* fileName, float x, float y, int width, int height, int frameX, int frameY, bool trans, COLORREF transColor, bool alpha)
{
	//���� ������ Ű ���� ���� �̹����� �ִ��� �˻�
	image* img = findImage(strKey);

	//���࿡ �̹����� ������~ �� �̹����� ��ȯ
	if (img) return img;

	//������� ������ ���°Ŵϱ�
	img = new image;

	//���� ����� �̹����� �ʱ�ȭ ���� �ʾ����� �ΰ����� �ʱ�ȭ
	if (FAILED(img->init(fileName, x, y, width, height, frameX, frameY, alpha, trans, transColor)))
	{
		SAFE_DELETE(img);

		return NULL;
	}

	//������ �� �� ��Ȳ�̹Ƿ� �ʿ� �־��ݴϴ� �� _��
	_mImageList.insert(make_pair(strKey, img));

	return img;
}

image* imageManager::addFrameImage(string strKey, const char* fileName, int width, int height, int frameX, int frameY, bool trans, COLORREF transColor, bool alpha)
{
	//���� ������ Ű ���� ���� �̹����� �ִ��� �˻�
	image* img = findImage(strKey);

	//���࿡ �̹����� ������~ �� �̹����� ��ȯ
	if (img) return img;

	//������� ������ ���°Ŵϱ�
	img = new image;

	//���� ����� �̹����� �ʱ�ȭ ���� �ʾ����� �ΰ����� �ʱ�ȭ
	if (FAILED(img->init(fileName, width, height, frameX, frameY, alpha, trans, transColor)))
	{
		SAFE_DELETE(img);

		return NULL;
	}

	//������ �� �� ��Ȳ�̹Ƿ� �ʿ� �־��ݴϴ� �� _��
	_mImageList.insert(make_pair(strKey, img));

	return img;
}


//�̹��� ã�� �Լ�
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
	//�ش� Ű ���� �����ϴ��� �˻��Ѵ�
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

//���� �Լ�
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

//�� ����
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