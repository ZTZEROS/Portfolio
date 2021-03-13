#include "pch.h"
#include "MMO.h"
#include "MMO_BaseClient.h"
#include "MMO_DerivedClient.h"
#include "MMO_InnerClient.h"
#include "MMO_BaseServer.h"
#include "MMO_DerivedServer.h"

MMO_DerivedServer::MMO_DerivedServer()
{
	CurrentClientCount = 0;
	ConnectedClientCount = 0;
	DisconnectedClientCount = 0;

	//CurrentWorkCount = 0;
	//EnqueuedWorkCount = 0;
	//DequeuedWorkCount = 0;

	CurrentReceiveCount = 0;
	ReceiveCount = 0;
	ReceivedSize = 0;

	CurrentSendCount = 0;
	SendCount = 0;
	SendedSize = 0;

	CurrentGameThreadUpdateCount = 0;
	CurrentAuthorizeThreadUpdateCount = 0;

	FOR(i, sizeof(CheckPoint) / sizeof(UINT))
	{
		CheckPoint[i] = 0;
	}
}

MMO_DerivedServer::~MMO_DerivedServer()
{

}

bool MMO_DerivedServer::Initialize()
{
	{
		CurrentClientCount = 0;
		ConnectedClientCount = 0;
		DisconnectedClientCount = 0;

		//CurrentWorkCount = 0;
		//EnqueuedWorkCount = 0;
		//DequeuedWorkCount = 0;

		CurrentReceiveCount = 0;
		ReceiveCount = 0;
		ReceivedSize = 0;

		CurrentSendCount = 0;
		SendCount = 0;
		SendedSize = 0;

		CurrentGameThreadUpdateCount = 0;
		CurrentAuthorizeThreadUpdateCount = 0;

		FOR(i, sizeof(CheckPoint) / sizeof(UINT))
		{
			CheckPoint[i] = 0;
		}
	}

	//Parse configuration
	{
		FILE* jsonFile;
		GenericDocument<UTF16LE<>> jsonDocument;
		WCHAR* jsonBuffer;
		UINT jsonFileSize;

		_wfopen_s(&jsonFile, L"MMO_Server.ini", L"r, ccs=UTF-16LE");

		jsonFileSize = _filelength(_fileno(jsonFile));

		jsonBuffer = (WCHAR*)malloc(jsonFileSize);
		ZeroMemory(jsonBuffer, jsonFileSize);
		fread_s(jsonBuffer, jsonFileSize, jsonFileSize, 1, jsonFile);
		fclose(jsonFile);

		jsonDocument.Parse(jsonBuffer);

		{
			wstring ipAddress;
			UINT port;
			UINT workThreadCount;
			UINT maximumClientCount;

			ipAddress = jsonDocument[L"MMO_DerivedServer"][L"IP_ADDRESS"].GetString();
			port = jsonDocument[L"MMO_DerivedServer"][L"PORT"].GetUint();
			workThreadCount = jsonDocument[L"MMO_DerivedServer"][L"WORK_THREAD_COUNT"].GetUint();
			maximumClientCount = jsonDocument[L"MMO_DerivedServer"][L"MAXIMUM_CLIENT_COUNT"].GetUint();

			IP_ADDRESS = ipAddress;
			PORT = port;
			WORK_THREAD_COUNT = workThreadCount;
			MAXIMUM_CLIENT_COUNT = maximumClientCount;
		}

		free(jsonBuffer);
	}

	{
		BCAV.resize(MAXIMUM_CLIENT_COUNT);
		DCV.resize(MAXIMUM_CLIENT_COUNT);

		FOR(i, MAXIMUM_CLIENT_COUNT)
		{
			DCV[i].Initialize();
			DCV[i].SetBS_Address(this);
			BCAV[i] = &DCV[i];
		}

		MMO_BaseServer::Initialize();

		//MMODC_TLP.Initialize();
		//MMOW_TLP.Initialize();

		//TLDH.Initialize();
		//TLDH.Connect();

		InnerClient.Initialize();
	}

	return 0;
}

bool MMO_DerivedServer::Update()
{
	if (TIMER.CheckUpdate())
	{
		//CPU_PROFILER.Update();
		//PDH_PROFILER.Update();
		//
		//InnerClient.Update();
	}

	{
		if (GetAsyncKeyState(VK_F1)) THREAD_CONTEXT_SWITCH_ON = !THREAD_CONTEXT_SWITCH_ON;
		if (GetAsyncKeyState(VK_F2)) INDICATOR_ON = !INDICATOR_ON;
		if (GetAsyncKeyState(VK_F4)) return 1;
	}

	{
		//MMO_BaseServer::Update();

		//CPU_PROFILER.Update();
		//PDH_PROFILER.Update();

		//InnerClient.Update();
	}

	Sleep(10);

	return 0;
}

bool MMO_DerivedServer::Render()
{
	if (TIMER.ShowCounter()) //CheckRender()
	{
		CPU_PROFILER.Update();
		PDH_PROFILER.Update();

		InnerClient.Update();

		printf(
			"====== MMO_DerivedServer =====" CRALF
			"CurrentClientCount : %u" CRALF
			"ConnectedClientCount : %u" CRALF
			"DisconnectedClientCount : %u" CRALF
			CRALF
			//"CurrentWorkCount : %u" CRALF
			//"EnqueuedWorkCount : %u" CRALF
			//"DequeuedWorkCount : %u" CRALF
			//CRALF
			"CurrentReciveCount : %u" CRALF
			"ReceiveCount : %u" CRALF
			"ReceivedSize : %u" CRALF
			CRALF
			"CurrentSendCount : %u" CRALF
			"SendCount : %u" CRALF
			"SendedSize : %u" CRALF
			CRALF
			"CURRENT_SPQ_COUNT : %u" CRALF
			"ALLOCATED_SPQ_COUNT : %u" CRALF
			"DEALLOCATED_SPQ_COUNT : %u" CRALF
			CRALF
			"CURRENT_SPQ_CHUNK_COUNT : %u" CRALF
			"MAXIMUM_SPQ_CHUNK_COUNT : %u" CRALF
			"===== ===== =====" CRALF
			CRALF
			,
			CurrentClientCount,
			ConnectedClientCount,
			DisconnectedClientCount,

			//CurrentWorkCount,
			//EnqueuedWorkCount,
			//DequeuedWorkCount,

			CurrentReceiveCount,
			ReceiveCount,
			ReceivedSize,

			CurrentSendCount,
			SendCount,
			SendedSize,

			CURRENT_SPQ_COUNT,
			ALLOCATED_SPQ_COUNT,
			DEALLOCATED_SPQ_COUNT,

			CURRENT_SPQ_CHUNK_COUNT,
			MAXIMUM_SPQ_CHUNK_COUNT
			);

		InterlockedExchange(&CurrentReceiveCount, 0);
		InterlockedExchange(&CurrentSendCount, 0);

		if (INDICATOR_ON)
		{
			printf(
				"CheckPoint[0] : %u" CRALF
				"CheckPoint[1] : %u" CRALF
				"CheckPoint[2] : %u" CRALF
				"CheckPoint[3] : %u" CRALF
				"CheckPoint[4] : %u" CRALF
				"CheckPoint[5] : %u" CRALF
				"CheckPoint[6] : %u" CRALF
				"CheckPoint[7] : %u" CRALF
				"CheckPoint[8] : %u" CRALF
				"CheckPoint[9] : %u" CRALF
				"CheckPoint[10] : %u" CRALF
				"CheckPoint[11] : %u" CRALF
				CRALF
				,
				CheckPoint[0],
				CheckPoint[1],
				CheckPoint[2],
				CheckPoint[3],
				CheckPoint[4],
				CheckPoint[5],
				CheckPoint[6],
				CheckPoint[7],
				CheckPoint[8],
				CheckPoint[9],
				CheckPoint[10],
				CheckPoint[11]
				);
		}

		InnerClient.Render();
	}

	{
		//MMO_BaseServer::Render();

		//InnerClient.Render();
	}

	return 0;
}

bool MMO_DerivedServer::Terminalize()
{
	{
		MMO_BaseServer::Terminalize();
	}

	{
		//TLDH.Disconnect();
		//TLDH.Terminalize();
	}

	{
		InnerClient.Terminalize();
	}

	return 0;
}