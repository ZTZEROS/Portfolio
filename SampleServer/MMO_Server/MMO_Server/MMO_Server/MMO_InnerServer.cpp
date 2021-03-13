#include "pch.h"
#include "MMO.h"
#include "MMO_InnerServer.h"

MMO_InnerServer::MMO_InnerServer()
{
	CurrentClientCount = 0;
	ConnectedClientCount = 0;
	DisconnectedClientCount = 0;

	CurrentWorkCount = 0;
	EnqueuedWorkCount = 0;
	DequeuedWorkCount = 0;

	ReceiveCount = 0;
	ReceivedSize = 0;
	SendCount = 0;
	SendedSize = 0;

	FOR(i, sizeof(CheckPoint) / sizeof(UINT))
	{
		CheckPoint[i] = 0;
	}
}

MMO_InnerServer::~MMO_InnerServer() {}

bool MMO_InnerServer::Initialize()
{
	{
		CurrentClientCount = 0;
		ConnectedClientCount = 0;
		DisconnectedClientCount = 0;

		CurrentWorkCount = 0;
		EnqueuedWorkCount = 0;
		DequeuedWorkCount = 0;

		ReceiveCount = 0;
		ReceivedSize = 0;
		SendCount = 0;
		SendedSize = 0;

		FOR(i, sizeof(CheckPoint) / sizeof(UINT))
		{
			CheckPoint[i] = 0;
		}
	}

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

			ipAddress = jsonDocument[L"MMO_InnerClient"][L"IP_Address"].GetString();
			port = jsonDocument[L"MMO_InnerClient"][L"Port"].GetUint();
			workThreadCount = jsonDocument[L"MMO_InnerClient"][L"WorkThreadCount"].GetUint();
			maximumClientCount = jsonDocument[L"MMO_InnerClient"][L"MaximumClientCount"].GetUint();

			Start(ipAddress.c_str(), port, workThreadCount, 1, maximumClientCount); //IOCP_Server::Initialize();
		}

		free(jsonBuffer);
	}

	return 0;
}

bool MMO_InnerServer::Update()
{
	//if (TIMER.CheckUpdate())
	{
		{
			if (GetAsyncKeyState(VK_F1)) THREAD_CONTEXT_SWITCH_ON = !THREAD_CONTEXT_SWITCH_ON;
			if (GetAsyncKeyState(VK_F3)) INDICATOR_ON = !INDICATOR_ON;
			if (GetAsyncKeyState(VK_F4)) return 1;
		}
	}

	return 0;
}

bool MMO_InnerServer::Render()
{
	//if (TIMER.ShowCounter()) //CheckRender()
	{
		printf(
			"====== MMO_InnerServer =====" CRALF
			"CurrentClientCount : %u" CRALF
			"ConnectedClientCount : %u" CRALF
			"DisconnectedClientCount : %u" CRALF
			CRALF
			"CurrentWorkCount : %u" CRALF
			"EnqueuedWorkCount : %u" CRALF
			"DequeuedWorkCount : %u" CRALF
			CRALF
			"ReceiveCount : %u" CRALF
			"ReceivedSize : %u" CRALF
			"SendCount : %u" CRALF
			"SendedSize : %u" CRALF
			CRALF
			"CURRENT_SPQ_COUNT : %u" CRALF
			"ALLOCATED_SPQ_COUNT : %u" CRALF
			"DEALLOCATED_SPQ_COUNT : %u" CRALF
			"===== ===== =====" CRALF
			CRALF
			,
			CurrentClientCount,
			ConnectedClientCount,
			DisconnectedClientCount,

			CurrentWorkCount,
			EnqueuedWorkCount,
			DequeuedWorkCount,

			ReceiveCount,
			ReceivedSize,
			SendCount,
			SendedSize,

			CURRENT_SPQ_COUNT,
			ALLOCATED_SPQ_COUNT,
			DEALLOCATED_SPQ_COUNT
		);

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
	}

	return 0;
}

bool MMO_InnerServer::Terminalize()
{
	{
		Stop(); //IOCP_Server::Terminalize();
	}

	{

	}

	return 0;
}