#pragma once
#include <vector>

//=======================================
// ## animation ## 2015.12.02 ##
//=======================================

typedef void(*CALLBACKFUNCTION)(void);
typedef void(*CALLBACKFUNCTION_PARAMETER)(void*);

class animation
{
public:
	//������ ������ ���Ƿ� ������ �����ϰ� �д�
	typedef vector<POINT> _vFrameList;
	typedef vector<int> _vPlayList;

private:
	int _frameNum;

	_vFrameList _frameList;
	_vPlayList _playList;

	int _frameWidth;
	int _frameHeight;

	BOOL _loop;

	float _frameUpdateSec;
	float _elapsedSec;

	DWORD _nowPlayIdx;
	BOOL _play;

	void* _obj;
	CALLBACKFUNCTION _callbackFunction;
	CALLBACKFUNCTION_PARAMETER _callbackFunctionParameter;

public:

	animation();
	~animation();

	HRESULT init(int totalW, int totalH, int frameW, int frameH);
	void release(void);

	//Default �ִϸ��̼�
	void setDefPlayFrame(BOOL reverse = FALSE, BOOL loop = FALSE);
	void setDefPlayFrame(BOOL reverse, BOOL loop, CALLBACKFUNCTION cbFunction);
	void setDefPlayFrame(BOOL reverse, BOOL loop, CALLBACKFUNCTION_PARAMETER cbFunction, void* obj);

	//�迭�� ������ �ִϸ��̼�
	void setPlayFrame(int* playArr, int arrLen, BOOL loop = FALSE);
	void setPlayFrame(int* playArr, int arrLen, BOOL loop, CALLBACKFUNCTION cbFunction);
	void setPlayFrame(int* playArr, int arrLen, BOOL loop, CALLBACKFUNCTION_PARAMETER cbFunction, void* obj);

	//���۰� �� ������ �ִ� �ִϸ��̼�
	void setPlayFrame(int start, int end, BOOL reverse = FALSE, BOOL loop = FALSE);
	void setPlayFrame(int start, int end, BOOL reverse, BOOL loop, CALLBACKFUNCTION cbFunction);
	void setPlayFrame(int start, int end, BOOL reverse, BOOL loop, CALLBACKFUNCTION_PARAMETER cbFunction, void* obj);


	void setFPS(int framePerSec);

	void frameUpdate(float elapsedTime);

	void start(void);
	void stop(void);
	void pause(void);
	void resume(void);

	inline BOOL isPlay(void) { return _play; }

	inline POINT getFramePos(void) { return _frameList[_playList[_nowPlayIdx]]; }
	inline int getNowPlayIndex(void) { return _nowPlayIdx; }
	inline int getFrameWidth(void) { return _frameWidth; }

	inline int getFrameHeight(void) { return _frameHeight; }
};

