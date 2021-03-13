#include "stdafx.h"
#include "Timer.h"

Timer TIMER;

Timer::Timer()
{
	FrameRate = 60;
	CurrentLoopCount = 0;
	CurrentUpdateCount = 0;
	CurrentRenderCount = 0;

	QueryPerformanceFrequency(&PerformanceFrequency);

	FramePeriod.QuadPart = PerformanceFrequency.QuadPart / FrameRate;
	Error.QuadPart = PerformanceFrequency.QuadPart - FramePeriod.QuadPart * FrameRate;
	FrameDelay.QuadPart = 0;

	timeBeginPeriod(1);
}

Timer::~Timer()
{
	timeEndPeriod(1);
}

void Timer::Initialize()
{
	QueryPerformanceCounter(&CurrentPerformanceCounter);
	PreviousPerformanceCounter = CurrentPerformanceCounter;
	PostPerformanceCounter.QuadPart = CurrentPerformanceCounter.QuadPart + PerformanceFrequency.QuadPart;
}

void Timer::InitializeByDolphin()
{
	//QueryPerformanceCounter(&InitialPerformanceCounter);
	QueryPerformanceCounter(&GetTime);
	PostPerformanceCounter.QuadPart = GetTime.QuadPart + PerformanceFrequency.QuadPart;
	//PreviousPerformanceCounter.QuadPart = CurrentPerformanceCounter.QuadPart;
	
	CorrectValue = PerformanceFrequency.QuadPart - (FramePeriod.QuadPart * FrameRate);
	CorrectValue_Accumlate = CorrectValue;
	
	Before.QuadPart = GetTime.QuadPart;
	Accumlate.QuadPart = 0;
}


void Timer::Termimalize()
{
}

BOOL Timer::CheckUpdate()
{
	LARGE_INTEGER currentElapsedCounter;

	QueryPerformanceCounter(&CurrentPerformanceCounter);

	currentElapsedCounter.QuadPart = (CurrentPerformanceCounter.QuadPart - PreviousPerformanceCounter.QuadPart);

	++CurrentLoopCount;

	if (FramePeriod.QuadPart <= currentElapsedCounter.QuadPart)
	{
		PreviousPerformanceCounter.QuadPart += FramePeriod.QuadPart;
		++CurrentUpdateCount;

		--CurrentLoopCount;

		return 1;
	}

	return 0;
}

BOOL Timer::CheckRender() //uncorrected part
{
	LARGE_INTEGER currentElapsedCounter;

	if (FramePeriod.QuadPart <= FrameDelay.QuadPart)
	{
		FrameDelay.QuadPart -= FramePeriod.QuadPart;

		return 0;
	}
	else
	{
		++CurrentRenderCount;

		if (FramePeriod.QuadPart < currentElapsedCounter.QuadPart)
		{
			FrameDelay.QuadPart += currentElapsedCounter.QuadPart - FramePeriod.QuadPart;
		}
		else
		{
			//do
			//{
			//	QueryPerformanceCounter(&currentPerformanceCounter);
			//}
			//while (currentPerformanceCounter.QuadPart < CurrentPerformanceCounter.QuadPart + (FramePeriod.QuadPart - currentElapsedCounter.QuadPart));

			Sleep(1000 * (FramePeriod.QuadPart - currentElapsedCounter.QuadPart) / PerformanceFrequency.QuadPart);
		}

		CurrentPerformanceCounter.QuadPart += (FramePeriod.QuadPart - currentElapsedCounter.QuadPart);

		return 1;
	}

	return 0;
}

BOOL Timer::ShowCounter()
{
	//if (PerformanceFrequency.QuadPart <= OneSecond.QuadPart)
	if (PostPerformanceCounter.QuadPart <= CurrentPerformanceCounter.QuadPart)
	{
		//WCHAR mainWindowTitle[256];

		//wsprintf(mainWindowTitle, L"%s : %d, %d", MAIN_WINDOW_TITLE, CurrentUpdateCount, CurrentRenderCount);

		//SetWindowText(MAIN_WINDOW_HANDLE, mainWindowTitle);

		printf("LoopCount : %u, UpdateCount : %u, RenderCount : %u" CRLF, CurrentLoopCount, CurrentUpdateCount, CurrentRenderCount);

		CurrentLoopCount = 0;
		CurrentUpdateCount = 0;
		CurrentRenderCount = 0;
		
		//OneSecond.QuadPart -= PerformanceFrequency.QuadPart;
		PostPerformanceCounter.QuadPart += PerformanceFrequency.QuadPart;

		return 1;
	}

	return 0;
}

BOOL Timer::CheckElapsedTime() //CheckUpdateAndRender()
{
	LARGE_INTEGER currentElapsedCounter;
	LARGE_INTEGER currentPerformanceCounter;

	PreviousPerformanceCounter = CurrentPerformanceCounter;
	QueryPerformanceCounter(&CurrentPerformanceCounter);

	//OneSecond.QuadPart += currentElapsedCounter.QuadPart;
	currentElapsedCounter.QuadPart = (CurrentPerformanceCounter.QuadPart - PreviousPerformanceCounter.QuadPart);

	//if (PerformanceFrequency.QuadPart <= OneSecond.QuadPart)
	if (PostPerformanceCounter.QuadPart <= CurrentPerformanceCounter.QuadPart)
	{
		//WCHAR mainWindowTitle[256];

		//wsprintf(mainWindowTitle, L"%s : %d, %d", MAIN_WINDOW_TITLE, CurrentUpdateCount, CurrentRenderCount);

		//SetWindowText(MAIN_WINDOW_HANDLE, mainWindowTitle);

		CurrentUpdateCount = 0;
		CurrentRenderCount = 0;
		//OneSecond.QuadPart -= PerformanceFrequency.QuadPart;
		PostPerformanceCounter.QuadPart += PerformanceFrequency.QuadPart;
	}

	++CurrentUpdateCount;

	if (FramePeriod.QuadPart <= FrameDelay.QuadPart)
	{
		FrameDelay.QuadPart -= FramePeriod.QuadPart;

		return 0;
	}
	else
	{
		++CurrentRenderCount;

		if (FramePeriod.QuadPart <= currentElapsedCounter.QuadPart)
		{
			FrameDelay.QuadPart += currentElapsedCounter.QuadPart - FramePeriod.QuadPart;
		}
		else
		{
			//do
			//{
			//	QueryPerformanceCounter(&currentPerformanceCounter);
			//}
			//while (currentPerformanceCounter.QuadPart < CurrentPerformanceCounter.QuadPart + (FramePeriod.QuadPart - currentElapsedCounter.QuadPart));

			Sleep(1000 * (FramePeriod.QuadPart - currentElapsedCounter.QuadPart) / PerformanceFrequency.QuadPart);
		}

		CurrentPerformanceCounter.QuadPart += (FramePeriod.QuadPart - currentElapsedCounter.QuadPart);

		return 1;
	}
}

BOOL Timer::CheckElapsedTimeByDolphin(UINT millisecondCriterialTime)
{
	LARGE_INTEGER Delta;
	LARGE_INTEGER SleepValue;
	ULONGLONG uiSleep;
	int iCorrectValue;
	LONGLONG llFrameRate;
	bool bRender;

	//Before = CurrentPerformanceCounter;

	QueryPerformanceCounter(&GetTime);

	if (0 < CorrectValue_Accumlate)
	{
		iCorrectValue = 1;
		--CorrectValue_Accumlate;
	}
	else
	{
		iCorrectValue = 0;
	}

	if(PostPerformanceCounter.QuadPart <= GetTime.QuadPart)
	{
		WCHAR mainWindowTitle[256];
		
		//wsprintf(mainWindowTitle, L"%s : %d, %d", MAIN_WINDOW_TITLE, CurrentUpdateCount, CurrentRenderCount);

		//SetWindowText(MAIN_WINDOW_HANDLE, mainWindowTitle);

		CurrentUpdateCount = 0;
		CurrentRenderCount = 0;
		PostPerformanceCounter.QuadPart += PerformanceFrequency.QuadPart;
		CorrectValue_Accumlate = CorrectValue;
	}

	llFrameRate = FramePeriod.QuadPart + iCorrectValue;

	Delta.QuadPart = (GetTime.QuadPart - Before.QuadPart);
	Accumlate.QuadPart += Delta.QuadPart;
	Before.QuadPart += Delta.QuadPart;

	if (llFrameRate <= Accumlate.QuadPart)
	{
		Accumlate.QuadPart -= llFrameRate;
		if (llFrameRate <= Accumlate.QuadPart)
		{
			bRender = false;
		}
		else
		{
			bRender = true;
		}
	}
	else
	{
		bRender = true;
		SleepValue.QuadPart = llFrameRate - Accumlate.QuadPart;
		uiSleep = ((SleepValue.QuadPart * 1000) / PerformanceFrequency.QuadPart);
		uiSleep += 1;
		Before.QuadPart += SleepValue.QuadPart;
		Accumlate.QuadPart = 0;
		Sleep(uiSleep);
	}

	return bRender;
}