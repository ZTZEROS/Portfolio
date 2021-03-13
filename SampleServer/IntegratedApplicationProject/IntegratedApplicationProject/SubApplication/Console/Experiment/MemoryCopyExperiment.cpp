#include "stdafx.h"
#include "MemoryCopyExperiment.h"

int MemoryCopyExperiment(int argumentCount, char* argumentVector[])
{
	char a[BUFFER_SIZE];
	char b[BUFFER_SIZE];

	LARGE_INTEGER performanceFrequency;
	LARGE_INTEGER previousPerformanceCounter;
	LARGE_INTEGER currentPerformanceCounter;

	memset(a, 0, sizeof(a));
	memset(b, 0, sizeof(b));

	timeBeginPeriod(1);

	//QueryPerformanceFrequency(&performanceFrequency);
	
	//while (1)
	{
		{
			QueryPerformanceCounter(&currentPerformanceCounter);

			memcpy(a, b, sizeof(b));

			previousPerformanceCounter = currentPerformanceCounter;
			QueryPerformanceCounter(&currentPerformanceCounter);

			printf("case 1 : %lld" CRALF, currentPerformanceCounter.QuadPart - previousPerformanceCounter.QuadPart);
		}

		{
			QueryPerformanceCounter(&currentPerformanceCounter);

			memcpy(a, b, sizeof(b));

			previousPerformanceCounter = currentPerformanceCounter;
			QueryPerformanceCounter(&currentPerformanceCounter);

			printf("case 1 : %lld" CRALF, currentPerformanceCounter.QuadPart - previousPerformanceCounter.QuadPart);
		}

		{
			QueryPerformanceCounter(&currentPerformanceCounter);

			memcpy(&a[0], &b[0], sizeof(b) / 2);
			memcpy(&a[BUFFER_SIZE * 1/2], &b[BUFFER_SIZE * 1/2], sizeof(b) / 2);

			previousPerformanceCounter = currentPerformanceCounter;
			QueryPerformanceCounter(&currentPerformanceCounter);

			printf("case 2 : %lld" CRALF, currentPerformanceCounter.QuadPart - previousPerformanceCounter.QuadPart);
		}

		{
			QueryPerformanceCounter(&currentPerformanceCounter);

			memcpy(&a[0], &b[0], sizeof(b) / 4);
			memcpy(&a[BUFFER_SIZE * 1 / 4], &b[BUFFER_SIZE * 1/4], sizeof(b) / 4);
			memcpy(&a[BUFFER_SIZE * 2 / 4], &b[BUFFER_SIZE * 2/4], sizeof(b) / 4);
			memcpy(&a[BUFFER_SIZE * 3 / 4], &b[BUFFER_SIZE * 3/4], sizeof(b) / 4);

			previousPerformanceCounter = currentPerformanceCounter;
			QueryPerformanceCounter(&currentPerformanceCounter);

			printf("case 4 : %lld" CRALF, currentPerformanceCounter.QuadPart - previousPerformanceCounter.QuadPart);
		}

		{
			QueryPerformanceCounter(&currentPerformanceCounter);

			memcpy(&a[0], &b[0], sizeof(b) / 8);
			memcpy(&a[BUFFER_SIZE * 1/8], &b[BUFFER_SIZE * 1/8], sizeof(b) / 8);
			memcpy(&a[BUFFER_SIZE * 2/8], &b[BUFFER_SIZE * 2/8], sizeof(b) / 8);
			memcpy(&a[BUFFER_SIZE * 3/8], &b[BUFFER_SIZE * 3/8], sizeof(b) / 8);
			memcpy(&a[BUFFER_SIZE * 4/8], &b[BUFFER_SIZE * 4/8], sizeof(b) / 8);
			memcpy(&a[BUFFER_SIZE * 5/8], &b[BUFFER_SIZE * 5/8], sizeof(b) / 8);
			memcpy(&a[BUFFER_SIZE * 6/8], &b[BUFFER_SIZE * 6/8], sizeof(b) / 8);
			memcpy(&a[BUFFER_SIZE * 7/8], &b[BUFFER_SIZE * 7/8], sizeof(b) / 8);

			previousPerformanceCounter = currentPerformanceCounter;
			QueryPerformanceCounter(&currentPerformanceCounter);

			printf("case 8 : %lld" CRALF, currentPerformanceCounter.QuadPart - previousPerformanceCounter.QuadPart);
		}

		{
			QueryPerformanceCounter(&currentPerformanceCounter);

			memcpy(&a[0], &b[0], sizeof(b) / 16);
			memcpy(&a[BUFFER_SIZE * 1 / 16], &b[BUFFER_SIZE * 1 / 16], sizeof(b) / 16);
			memcpy(&a[BUFFER_SIZE * 2 / 16], &b[BUFFER_SIZE * 2 / 16], sizeof(b) / 16);
			memcpy(&a[BUFFER_SIZE * 3 / 16], &b[BUFFER_SIZE * 3 / 16], sizeof(b) / 16);
			memcpy(&a[BUFFER_SIZE * 4 / 16], &b[BUFFER_SIZE * 4 / 16], sizeof(b) / 16);
			memcpy(&a[BUFFER_SIZE * 5 / 16], &b[BUFFER_SIZE * 5 / 16], sizeof(b) / 16);
			memcpy(&a[BUFFER_SIZE * 6 / 16], &b[BUFFER_SIZE * 6 / 16], sizeof(b) / 16);
			memcpy(&a[BUFFER_SIZE * 7 / 16], &b[BUFFER_SIZE * 7 / 16], sizeof(b) / 16);
			memcpy(&a[BUFFER_SIZE * 8 / 16], &b[BUFFER_SIZE * 8 / 16], sizeof(b) / 16);
			memcpy(&a[BUFFER_SIZE * 9 / 16], &b[BUFFER_SIZE * 9 / 16], sizeof(b) / 16);
			memcpy(&a[BUFFER_SIZE * 10 / 16], &b[BUFFER_SIZE * 10 / 16], sizeof(b) / 16);
			memcpy(&a[BUFFER_SIZE * 11 / 16], &b[BUFFER_SIZE * 11 / 16], sizeof(b) / 16);
			memcpy(&a[BUFFER_SIZE * 12 / 16], &b[BUFFER_SIZE * 12 / 16], sizeof(b) / 16);
			memcpy(&a[BUFFER_SIZE * 13 / 16], &b[BUFFER_SIZE * 13 / 16], sizeof(b) / 16);
			memcpy(&a[BUFFER_SIZE * 14 / 16], &b[BUFFER_SIZE * 14 / 16], sizeof(b) / 16);
			memcpy(&a[BUFFER_SIZE * 15 / 16], &b[BUFFER_SIZE * 15 / 16], sizeof(b) / 16);

			previousPerformanceCounter = currentPerformanceCounter;
			QueryPerformanceCounter(&currentPerformanceCounter);

			printf("case 16 : %lld" CRALF, currentPerformanceCounter.QuadPart - previousPerformanceCounter.QuadPart);
		}

		{
			QueryPerformanceCounter(&currentPerformanceCounter);

			for (unsigned int i = 0; i < BUFFER_SIZE / BUFFER_UNIT_SIZE; ++i)
			{
				memcpy(&a[i], &b[i], BUFFER_UNIT_SIZE);
			}

			previousPerformanceCounter = currentPerformanceCounter;
			QueryPerformanceCounter(&currentPerformanceCounter);

			printf("case for 8 : %lld" CRALF, currentPerformanceCounter.QuadPart - previousPerformanceCounter.QuadPart);
		}

		{
			QueryPerformanceCounter(&currentPerformanceCounter);

			for (unsigned int i = 0; i < BUFFER_SIZE; ++i)
			{
				memcpy(&a[i], &b[i], 1);
			}

			previousPerformanceCounter = currentPerformanceCounter;
			QueryPerformanceCounter(&currentPerformanceCounter);

			printf("case for 1 : %lld" CRALF, currentPerformanceCounter.QuadPart - previousPerformanceCounter.QuadPart);
		}

		{
			QueryPerformanceCounter(&currentPerformanceCounter);

			for (unsigned int i = 0; i < BUFFER_SIZE; ++i)
			{
				a[i] = b[i];
			}

			previousPerformanceCounter = currentPerformanceCounter;
			QueryPerformanceCounter(&currentPerformanceCounter);

			printf("case for substitution : %lld" CRALF, currentPerformanceCounter.QuadPart - previousPerformanceCounter.QuadPart);
		}

		getchar();
		system("cls");
	}

	timeEndPeriod(1);

	return 0;
}