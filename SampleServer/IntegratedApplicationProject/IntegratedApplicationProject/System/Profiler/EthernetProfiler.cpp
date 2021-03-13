#include "stdafx.h"
#include "EthernetProfiler.h"

EthernetProfiler::EthernetProfiler()
{

}
EthernetProfiler::~EthernetProfiler()
{

}

bool EthernetProfiler::Initialize()
{
	int currentCount;
	bool error;
	WCHAR* currentName;
	WCHAR* counterName;
	WCHAR* interfaceName;
	DWORD counterSize;
	DWORD interfaceSize;
	WCHAR query[1024];
	HQUERY queryHandle;
	PDH_HCOUNTER networkReceiveByteCounterHandle;
	PDH_HCOUNTER networkSendByteCounterHandle;
	double networkReceiveByte;
	double networkSendByte;

	PdhOpenQuery(nullptr, NULL, &queryHandle);

	counterName = nullptr;
	interfaceName = nullptr;

	counterSize = 0;
	interfaceSize = 0;

	PdhEnumObjectItems(nullptr, nullptr, L"Network Interface", counterName, &counterSize, interfaceName, &interfaceSize, PERF_DETAIL_WIZARD, 0);

	counterName = new WCHAR[counterSize];
	interfaceName = new WCHAR[interfaceSize];

	if (PdhEnumObjectItems(nullptr, nullptr, L"Network Interface", counterName, &counterSize, interfaceName, &interfaceSize, PERF_DETAIL_WIZARD, 0) != ERROR_SUCCESS)
	{
		delete[] counterName;
		delete[] interfaceName;

		return 1;
	}

	currentCount = 0;
	currentName = interfaceName;

	while (currentName != L"\0" && currentCount < 8)
	{
		query[0] = L'\0';

		StringCbPrintf(query, sizeof(query), L"\\NetWork Interface(%s)\\Bytes Received/sec", currentName);
		PdhAddCounter(queryHandle, query, NULL, &networkReceiveByteCounterHandle);

		query[0] = L'\0';

		StringCbPrintf(query, sizeof(query), L"\\NetWork Interface(%s)\\Bytes Sent/sec", currentName);
		PdhAddCounter(queryHandle, query, NULL, &networkSendByteCounterHandle);

		currentName += wcslen(currentName) + 1;
		++currentCount;
	}

	PdhCollectQueryData(query);

	for (unsigned int i = 0; i < 8; ++i)
	{
		PDH_FMT_COUNTERVALUE counterValue;

		PdhGetFormattedCounterValue(networkReceiveByteCounterHandle, PDH_FMT_DOUBLE, NULL, &counterValue);
		networkReceiveByte += counterValue.doubleValue;

		PdhGetFormattedCounterValue(networkSendByteCounterHandle, PDH_FMT_DOUBLE, NULL, &counterValue);
		networkSendByte += counterValue.doubleValue;
	}

	return 0;
}

bool EthernetProfiler::Update()
{
	return 0;
}

bool EthernetProfiler::Render()
{
	return 0;
}

bool EthernetProfiler::Terminalize()
{
	return 0;
}