#pragma once
#include "gameNode.h"
#include "progressBar.h"
#include <vector>

enum LOADING_KIND
{
	LOADING_KIND_IMAGE_00,
	LOADING_KIND_IMAGE_01,
	LOADING_KIND_IMAGE_02,
	LOADING_KIND_IMAGE_03,
	LOADING_KIND_FRAMEIMAGE_00,
	LOADING_KIND_FRAMEIMAGE_01,
	LOADING_KIND_SOUND_00,
	LOADING_KIND_END
};

//�̹��� ���ҽ� ����ü
struct tagImageResource
{
	string keyName;
	const char* fileName;
	int x, y;
	int width, height;
	int frameX, frameY;
	bool trans;
	COLORREF transColor;
};

class loadItem
{
private:
	LOADING_KIND _kind;
	tagImageResource _imageResource;

public:
	HRESULT initForImage(string keyName, int width, int height);
	HRESULT initForImage(string keyName, const char* fileName, int x, int y, int width, int height, bool trans, COLORREF transColor);
	HRESULT initForImage(string keyName, const char* fileName, int width, int height, bool trans, COLORREF transColor);
	HRESULT initForFrameImage(string keyName, const char* fileName, int x, int y, int width, int height, int frameX, int frameY, bool trans, COLORREF transColor);
	HRESULT initForFrameImage(string keyName, const char* fileName, int width, int height, int frameX, int frameY, bool trans, COLORREF transColor);

	LOADING_KIND getLoadingKind() { return _kind; }
	tagImageResource getImageResource() { return _imageResource; }

	loadItem();
	~loadItem();

};

class loading : public gameNode
{
private:
	typedef vector<loadItem*> arrLoadItem;
	typedef vector<loadItem*>::iterator arrLoadItemIter;

private:
	arrLoadItem _vLoadItem;
	image* _backGround;
	progressBar* _loadingBar;

	int _current;

public:
	HRESULT init();
	void release();
	void update();
	void render();

	void loadImage(string keyName, int width, int height);
	void loadImage(string keyName, const char* fileName, int x, int y, int width, int height, bool trans, COLORREF transColor);
	void loadImage(string keyName, const char* fileName, int width, int height, bool trans, COLORREF transColor);

	void loadFrameImage(string keyName, const char* fileName, int x, int y, int width, int height, int frameX, int frameY, bool trans, COLORREF transColor);
	void loadFrameImage(string keyName, const char* fileName, int width, int height, int frameX, int frameY, bool trans, COLORREF transColor);

	BOOL loadNext();

	vector<loadItem*> getLoadItem() { return _vLoadItem; }

	loading();
	~loading();
};

