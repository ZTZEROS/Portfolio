#pragma once

class PDH_Profiler
{
private:
	HQUERY PDH_QueryHandle;
	WCHAR CounterPath[PDH_MAX_COUNTER_PATH];

	PDH_HCOUNTER NetworkReceivedByteHandle;
	PDH_HCOUNTER NetworkSendedByteHandle;

	double NetworkReceivedByteCount;
	double NetworkSendedByteCount;

public:
	PDH_Profiler();
	virtual ~PDH_Profiler();

	bool Initialize();
	bool Update();
	bool Render();
	bool Terminalize();
};

extern PDH_Profiler PDH_PROFILER;