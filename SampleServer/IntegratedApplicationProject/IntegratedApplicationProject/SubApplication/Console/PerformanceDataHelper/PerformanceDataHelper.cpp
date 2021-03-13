#include "stdafx.h"
#include <Windows.h>
#include <stdio.h>
#include <conio.h>
#include <Pdh.h>
#include <PdhMsg.h>
#include "PerformanceDataHelper.h"

#pragma comment(lib, "pdh.lib")

CONST ULONG SAMPLE_INTERVAL_MS = 1000;
LPWSTR BROWSE_DIALOG_CAPTION = (LPWSTR)L"Select a counter to monitor.";

int ExecutePerformanceDataHelper(int argumentCount, char* argumentVector[])
{
	PDH_STATUS status;
	HQUERY query;
	HCOUNTER counter;
	PDH_FMT_COUNTERVALUE displayValue;
	DWORD counterType;
	SYSTEMTIME sampleTime;
	PDH_BROWSE_DLG_CONFIG browseDlgData;
	WCHAR counterPathBuffer[PDH_MAX_COUNTER_PATH];

	query = nullptr;

	status = PdhOpenQuery(NULL, NULL, &query);
	if (status != ERROR_SUCCESS)
	{
		wprintf(L"\nPdhOpenQuery failed with status 0x%x.", status);

		goto Cleanup;
	}

	ZeroMemory(&counterPathBuffer, sizeof(counterPathBuffer));
	ZeroMemory(&browseDlgData, sizeof(PDH_BROWSE_DLG_CONFIG));

	browseDlgData.bIncludeInstanceIndex = FALSE;
	browseDlgData.bSingleCounterPerAdd = TRUE;
	browseDlgData.bSingleCounterPerDialog = TRUE;
	browseDlgData.bLocalCountersOnly = FALSE;
	browseDlgData.bWildCardInstances = TRUE;
	browseDlgData.bHideDetailBox = TRUE;
	browseDlgData.bInitializePath = FALSE;
	browseDlgData.bDisableMachineSelection = FALSE;
	browseDlgData.bIncludeCostlyObjects = FALSE;
	browseDlgData.bShowObjectBrowser = FALSE;
	browseDlgData.hWndOwner = NULL;
	browseDlgData.szReturnPathBuffer = counterPathBuffer;
	browseDlgData.cchReturnPathLength = PDH_MAX_COUNTER_PATH;
	browseDlgData.pCallBack = NULL;
	browseDlgData.dwCallBackArg = 0;
	browseDlgData.CallBackStatus = ERROR_SUCCESS;
	browseDlgData.dwDefaultDetailLevel = PERF_DETAIL_WIZARD;
	browseDlgData.szDialogBoxCaption = BROWSE_DIALOG_CAPTION;

	status = PdhBrowseCounters(&browseDlgData);

	if (status != ERROR_SUCCESS)
	{
		if (status == PDH_DIALOG_CANCELLED)
		{
			wprintf(L"\nDialog canceled by user.");
		}
		else
		{
			wprintf(L"\nPdhBrowseCounters failed with status 0x%x.", status);
		}

		goto Cleanup;
	}
	else if (wcslen(counterPathBuffer) == 0)
	{
		wprintf(L"\nUser did not select any counter.");

		goto Cleanup;
	}
	else
	{
		wprintf(L"\nCounter selected : %s\n", counterPathBuffer);
	}

	status = PdhAddCounter(query, counterPathBuffer, 0, &counter);
	if (status != ERROR_SUCCESS)
	{
		wprintf(L"\nPdhAddCounter failed with status 0x%x.", status);

		goto Cleanup;
	}

	status = PdhCollectQueryData(query);
	if (status != ERROR_SUCCESS)
	{
		wprintf(L"\nPdhCollectQueryData failed with status 0x%x.", status);
	}

	while (!_kbhit())
	{
		Sleep(SAMPLE_INTERVAL_MS);

		GetLocalTime(&sampleTime);

		status = PdhCollectQueryData(query);
		if (status != ERROR_SUCCESS)
		{
			wprintf(L"\nPdhCollectQueryData failed with status 0x%x.", status);
		}

		wprintf(L"\n\"%2.2d/%2.2d%4.4d %2.2d:%2.2d:%2.2d:%3.3d\"",
			sampleTime.wMonth,
			sampleTime.wDay,
			sampleTime.wYear,
			sampleTime.wHour,
			sampleTime.wMinute,
			sampleTime.wSecond,
			sampleTime.wMilliseconds
			);

		//status = PdhGetFormattedCounterArray(counter, PDH_FMT_DOUBLE, )
		status = PdhGetFormattedCounterValue(counter, PDH_FMT_DOUBLE, &counterType, &displayValue);
		if (status != ERROR_SUCCESS)
		{
			wprintf(L"\nPdhGetFormattedCounterValue failed with status 0x%x.", status);

			goto Cleanup;
		}

		wprintf(L",\"%.20g\"", displayValue.doubleValue);
	}

Cleanup:
	if (query)
	{
		PdhCloseQuery(query);
	}
	
	return 0;
}