#include "stdafx.h"
#include "timer.h"
#include <mmsystem.h>

//윈도우가 실행중인 시간을 알려주는 API
#pragma comment(lib, "winmm.lib")

timer::timer()
{
}


timer::~timer()
{
}

HRESULT timer::init(void)
{
	//고성능 타이머를 지원하는지 여부 체크
	//만약 지원이 안되면 강제로 밀리세컨으로 맞춤(TIMEGETTIME)
	if (QueryPerformanceFrequency((LARGE_INTEGER*)&_periodFrequency))
	{
		_isHardware = true;
		QueryPerformanceCounter((LARGE_INTEGER*)&_lastTime);

		_timeScale = 1.0f / _periodFrequency;
	}
	else
	{
		//고성능 타이머를 지원하지 않을땐
		_isHardware = false;

		//강제로 밀리세컨드 형태로 맞춘다
		_lastTime = timeGetTime();
		_timeScale = 0.001f;
	}

	_frameRate = 0;
	_FPSFrameCount = 0;
	_FPSTimeElapsed = 0;
	_worldTime = 0;

	return S_OK;
}

void timer::tick(float lockFPS)
{
	if (_isHardware)
	{
		QueryPerformanceCounter((LARGE_INTEGER*)&_curTime);
	}
	else
	{
		_curTime = timeGetTime();
	}

	_timeElapsed = (_curTime - _lastTime) * _timeScale;

	if (lockFPS > 0.0f)
	{
		while (_timeElapsed < (1.0f / lockFPS))
		{
			if (_isHardware) QueryPerformanceCounter((LARGE_INTEGER*)&_curTime);
			else _curTime = timeGetTime();

			_timeElapsed = (_curTime - _lastTime) * _timeScale;
		}
	}

	_lastTime = _curTime;
	_FPSFrameCount++;
	_FPSTimeElapsed += _timeElapsed;
	_worldTime += _timeElapsed;

	//1초보다 프레임이 커지면 초기화
	if (_FPSTimeElapsed > 1.0f)
	{
		_frameRate = _FPSFrameCount;
		_FPSFrameCount = 0;
		_FPSTimeElapsed = 0.0f;
	}
}

unsigned long timer::getFrameRate(char* str) const
{
	if (str != NULL)
	{
		wsprintf(str, "FPS : %d", _frameRate);
	}

	return _frameRate;
}