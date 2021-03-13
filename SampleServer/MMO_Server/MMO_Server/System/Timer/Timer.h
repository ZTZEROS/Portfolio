#pragma once

class Timer
{
public:
	UINT FrameRate;
	UINT CurrentLoopCount;
	UINT CurrentUpdateCount;
	UINT CurrentRenderCount;

	LARGE_INTEGER PerformanceFrequency;

	LARGE_INTEGER PreviousPerformanceCounter;
	LARGE_INTEGER CurrentPerformanceCounter;
	LARGE_INTEGER PostPerformanceCounter;
	
	LARGE_INTEGER FramePeriod;
	LARGE_INTEGER FrameDelay;
	LARGE_INTEGER Error;

	//ByDolphin
	LARGE_INTEGER GetTime;
	LARGE_INTEGER Accumlate;
	LARGE_INTEGER Before;
	ULONGLONG CorrectValue;
	ULONGLONG CorrectValue_Accumlate;
	//ByDolphin

public:
	Timer();
	~Timer();

	bool Initialize();
	bool Update();
	bool Render();
	bool Termimalize();

	BOOL CheckUpdate();
	BOOL CheckRender();

	BOOL ShowCounter();

	BOOL CheckElapsedTime();

	void InitializeByDolphin();
	BOOL CheckElapsedTimeByDolphin(UINT millisecondCriterialTime = 20);


	UINT GetFrameRate() { return FrameRate; }
	UINT GetCurrentUpdateCount() { return CurrentUpdateCount; }
	UINT GetCurrentRenderCount() { return CurrentRenderCount; }
	LARGE_INTEGER GetPerformanceFrequency() { return PerformanceFrequency; }
	LARGE_INTEGER GetFramePeriod() { return FramePeriod; }
	LARGE_INTEGER GetFrameDelay() { return FrameDelay; }
	LARGE_INTEGER GetError() { return Error; }



	void SetFrameRate(UINT frameRate) {	FrameRate = frameRate; FramePeriod.QuadPart = PerformanceFrequency.QuadPart / FrameRate; Error.QuadPart = PerformanceFrequency.QuadPart - FramePeriod.QuadPart * FrameRate;	}
};

extern Timer TIMER;