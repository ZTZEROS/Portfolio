#include "stdafx.h"
#include "QueueExperiment.h"

ByteQueue BYTE_QUEUE(BUFFER_SIZE / 8);

HANDLE SUB_THREAD_HANDLE[10];
DWORD SUB_THREAD_ID[10];

HANDLE END_EVENT;

unsigned int WINAPI EnqueueExperiment(LPVOID argument);
unsigned int WINAPI DequeueExperiment(LPVOID argument);

int QueueExperiment(int argumentCount, char* argumentVector[])
{
	{
		timeBeginPeriod(1);

		SUB_THREAD_HANDLE[0] = (HANDLE)_beginthreadex(nullptr, 0, EnqueueExperiment, nullptr, 0, (unsigned int*)&SUB_THREAD_ID[0]);
		SUB_THREAD_HANDLE[1] = (HANDLE)_beginthreadex(nullptr, 0, DequeueExperiment, nullptr, 0, (unsigned int*)&SUB_THREAD_ID[1]);

		END_EVENT = CreateEvent(nullptr, 1, 0, L"END_EVENT");
	}



	WaitForMultipleObjects(2, SUB_THREAD_HANDLE, 1, INFINITE);
	


	{
		CloseHandle(END_EVENT);

		CloseHandle(SUB_THREAD_HANDLE[0]);
		CloseHandle(SUB_THREAD_HANDLE[1]);

		timeEndPeriod(1);
	}

	return 0;
}

unsigned int WINAPI EnqueueExperiment(LPVOID argument)
{
	unsigned int currentTextIndex;
	unsigned int currentTextSize;

	char inText[81];

	currentTextIndex = 0;
	currentTextSize = 0;

	srand(27);

	while (WaitForSingleObject(END_EVENT, 0) == WAIT_TIMEOUT)
	{
		currentTextSize = rand() % sizeof(inText);

		for (unsigned int i = 0; i < currentTextSize; ++i)
		{
			inText[i] = "1234567890 abcdefghijklmnopqrstuvwxyz 1234567890 abcdefghijklmnopqrstuvwxyz 12345"[(currentTextIndex + i) % sizeof(inText)];
		}

		if(BYTE_QUEUE.Enqueue(&inText[0], currentTextSize))
		{
			currentTextIndex += currentTextSize;
			currentTextIndex %= sizeof(inText);
		}
	}

	return 0;
}

unsigned int WINAPI DequeueExperiment(LPVOID argument)
{
	char peekText[81];
	char outText[81];

	unsigned int peekTextSize;

	peekTextSize = 0;

	srand(5);

	while (WaitForSingleObject(END_EVENT, 0) == WAIT_TIMEOUT)
	{
		memset(peekText, 0, sizeof(peekText));
		memset(outText, 0, sizeof(outText));

		peekTextSize = rand() % sizeof(peekText);

		if (BYTE_QUEUE.Peek(&peekText[0], peekTextSize))
		{
			if (BYTE_QUEUE.Dequeue(&outText[0], peekTextSize))
			{
				printf("%s", outText);
			}
		}

		if (memcmp(peekText, outText, peekTextSize))
		{
			SetEvent(END_EVENT);
		
			break;
		}
	}

	return 0;
}