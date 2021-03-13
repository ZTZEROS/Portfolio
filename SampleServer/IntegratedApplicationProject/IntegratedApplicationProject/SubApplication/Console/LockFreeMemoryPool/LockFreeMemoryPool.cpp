#include "stdafx.h"
#include "LockFreeMemoryPool.h"

LockFreePool<UINT64> POSITIVE_INTEGER_LFP;
LockFreeStack<UINT64*> POSITIVE_INTEGER_LFS;
LockFreeQueue<UINT64*> POSITIVE_INTEGER_LFQ;

LFMP_SubThread LFMP_WORK_SUB_THREAD[LFMP_WORK_SUB_THREAD_COUNT];

BOOL LFMP_SHUTDOWN;

int ExecuteLockFreeMemoryPool(int argumentCount, char* argumentVector[])
{
	//Initialize:
	{
		InitializeLockFreeMemoryPool();
	}

	//Update:
	//Render:
	{
		while (1)
		{
			if (UpdateLockFreeMemoryPool()) break;
			if (RenderLockFreeMemoryPool()) break;
		}
	}

	//Terminalize:
	{
		TerminalizeLockFreeMemoryPool();
	}

	return 0;
}

bool InitializeLockFreeMemoryPool()
{
	UINT64* positiveIntegerAddressA[LFMP_TOTAL_ALLOCATION_COUNT];

	//POSITIVE_INTEGER_LFP.Initialize();
	//POSITIVE_INTEGER_LFS.Initialize();
	//POSITIVE_INTEGER_LFQ.Initialize();

	srand(27);
	//srand(time(nullptr));

	timeBeginPeriod(1);

	TIMER.Initialize();

	LFMP_SHUTDOWN = 0;
	
	{
		FOR(i, LFMP_TOTAL_ALLOCATION_COUNT)
		{
			positiveIntegerAddressA[i] = POSITIVE_INTEGER_LFP.Allocate();

			*positiveIntegerAddressA[i] = LFMP_POSITIVE_INTEGER_INITIAL_VALUE;
			//*positiveIntegerAddressA[i] = i;
		}

		FOR(i, LFMP_TOTAL_ALLOCATION_COUNT)
		{
			POSITIVE_INTEGER_LFP.Deallocate(positiveIntegerAddressA[i]);
		}

		FOR(i, LFMP_TOTAL_ALLOCATION_COUNT)
		{
			positiveIntegerAddressA[i] = new UINT64;

			*positiveIntegerAddressA[i] = LFMP_POSITIVE_INTEGER_INITIAL_VALUE;
			//*positiveIntegerAddressA[i] = i;
		}

		FOR(i, LFMP_TOTAL_ALLOCATION_COUNT)
		{
			POSITIVE_INTEGER_LFS.Push(positiveIntegerAddressA[i]);
		}

		FOR(i, LFMP_TOTAL_ALLOCATION_COUNT)
		{
			positiveIntegerAddressA[i] = new UINT64;

			*positiveIntegerAddressA[i] = LFMP_POSITIVE_INTEGER_INITIAL_VALUE;
			//*positiveIntegerAddressA[i] = i;
		}

		FOR(i, LFMP_TOTAL_ALLOCATION_COUNT)
		{
			POSITIVE_INTEGER_LFQ.Enqueue(positiveIntegerAddressA[i]);
		}
	}
	
	FOR(i, LFMP_WORK_SUB_THREAD_COUNT)
	{
		LFMP_SubThread lfmpSubThread;

		lfmpSubThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, WorkLockFreeMemoryPool, nullptr, 0, (unsigned int*)&lfmpSubThread.IdentificationNumber);
		LFMP_WORK_SUB_THREAD[i].IdentificationNumber = lfmpSubThread.IdentificationNumber;
	}

	return 0;
}

bool UpdateLockFreeMemoryPool()
{
	//UINT64* positiveIntegerA_Address;
	//UINT64* positiveIntegerB_Address;

	//if (GetAsyncKeyState('P') & 0x0001)
	//{
	//	positiveIntegerA_Address = POSITIVE_INTEGER_A_LFS.Pop();
	//	positiveIntegerB_Address = POSITIVE_INTEGER_B_LFS.Pop();
	//
	//	printf("[A_LFS.Pop] : %x, %u / %u" CRALF, *positiveIntegerA_Address, POSITIVE_INTEGER_A_LFS.GetCurrentCount(), POSITIVE_INTEGER_A_LFS.GetMaximumCount());
	//	printf("[B_LFS.Pop] : %x, %u / %u" CRALF, *positiveIntegerB_Address, POSITIVE_INTEGER_B_LFS.GetCurrentCount(), POSITIVE_INTEGER_B_LFS.GetMaximumCount());
	//
	//	*positiveIntegerA_Address = LFMP_POSITIVE_INTEGER_INITIAL_VALUE;
	//	*positiveIntegerB_Address = LFMP_POSITIVE_INTEGER_INITIAL_VALUE;
	//	
	//	POSITIVE_INTEGER_B_LFS.Push(positiveIntegerA_Address);
	//	POSITIVE_INTEGER_A_LFS.Push(positiveIntegerB_Address);
	//	
	//	printf("[A_LFS.Push] : %x, %u / %u" CRALF, *positiveIntegerA_Address, POSITIVE_INTEGER_A_LFS.GetCurrentCount(), POSITIVE_INTEGER_A_LFS.GetMaximumCount());
	//	printf("[B_LFS.Push] : %x, %u / %u" CRALF, *positiveIntegerB_Address, POSITIVE_INTEGER_B_LFS.GetCurrentCount(), POSITIVE_INTEGER_B_LFS.GetMaximumCount());
	//}

	if (TIMER.CheckUpdate())
	{
		if (GetAsyncKeyState('Q') & 0x0001) return 1;

		//printf("CurrentCount : %u, MaximumCount %u" CRALF, POSITIVE_INTEGER_LFS.GetCurrentCount(), POSITIVE_INTEGER_LFS.GetMaximumCount());
		//printf("CurrentCount : %u, MaximumCount %u" CRALF, POSITIVE_INTEGER_LFP.GetCurrentCount(), POSITIVE_INTEGER_LFP.GetMaximumCount());
		//printf(CRALF);
	}

	return 0;
}

bool RenderLockFreeMemoryPool()
{
	if(TIMER.ShowCounter())
	{
		printf("CurrentCount : %u, MaximumCount %u" CRALF, POSITIVE_INTEGER_LFP.GetCurrentCount(), POSITIVE_INTEGER_LFP.GetMaximumCount());
		printf("CurrentCount : %u, MaximumCount %u" CRALF, POSITIVE_INTEGER_LFS.GetCurrentCount(), POSITIVE_INTEGER_LFS.GetMaximumCount());
		printf("CurrentCount : %u, MaximumCount %u" CRALF, POSITIVE_INTEGER_LFQ.GetCurrentCount(), POSITIVE_INTEGER_LFQ.GetMaximumCount());
		printf(CRALF);
	}

	return 0;
}

bool TerminalizeLockFreeMemoryPool()
{
	HANDLE workSubThreadHandle[LFMP_WORK_SUB_THREAD_COUNT];

	timeEndPeriod(1);

	TIMER.Termimalize();

	LFMP_SHUTDOWN = 1;

	FOR(i, LFMP_WORK_SUB_THREAD_COUNT)
	{
		workSubThreadHandle[i] = LFMP_WORK_SUB_THREAD[i].Handle;
	}

	WaitForMultipleObjects(LFMP_WORK_SUB_THREAD_COUNT, workSubThreadHandle, 1, INFINITE);

	//POSITIVE_INTEGER_LFP.Terminalize();
	//POSITIVE_INTEGER_LFS.Terminalize();
	//POSITIVE_INTEGER_LFQ.Terminalize();

	return 0;
}

unsigned int WINAPI WorkLockFreeMemoryPool(LPVOID argument)
{
	UINT64* positiveIntegerAddressA[LFMP_ALLOCATION_COUNT];
	UINT threadKey;

	threadKey = GetCurrentThreadId();
	srand(GetCurrentThreadId());

	while (!LFMP_SHUTDOWN)
	{
		{
			if(1)
			{
				FOR(i, LFMP_ALLOCATION_COUNT)
				{
					positiveIntegerAddressA[i] = POSITIVE_INTEGER_LFP.Allocate();
				}
			
				FOR(i, LFMP_ALLOCATION_COUNT)
				{
					if (*positiveIntegerAddressA[i] != LFMP_POSITIVE_INTEGER_INITIAL_VALUE) MEMORY_DUMPER.SelfCrash();
				}
			
				FOR(i, LFMP_ALLOCATION_COUNT)
				{
					InterlockedIncrement(positiveIntegerAddressA[i]);
				}
			
				FOR(i, LFMP_ALLOCATION_COUNT)
				{
					if (*positiveIntegerAddressA[i] != LFMP_POSITIVE_INTEGER_INITIAL_VALUE + 1) MEMORY_DUMPER.SelfCrash();
				}
			
				FOR(i, LFMP_ALLOCATION_COUNT)
				{
					InterlockedDecrement(positiveIntegerAddressA[i]);
				}
			
				FOR(i, LFMP_ALLOCATION_COUNT)
				{
					if (*positiveIntegerAddressA[i] != LFMP_POSITIVE_INTEGER_INITIAL_VALUE) MEMORY_DUMPER.SelfCrash();
				}
			
				FOR(i, LFMP_ALLOCATION_COUNT)
				{
					POSITIVE_INTEGER_LFP.Deallocate(positiveIntegerAddressA[i]);
				}
			}

			if (1)
			{
				FOR(i, LFMP_ALLOCATION_COUNT)
				{
					positiveIntegerAddressA[i] = POSITIVE_INTEGER_LFS.Pop();
				}

				FOR(i, LFMP_ALLOCATION_COUNT)
				{
					if (*positiveIntegerAddressA[i] != LFMP_POSITIVE_INTEGER_INITIAL_VALUE) MEMORY_DUMPER.SelfCrash();
				}

				FOR(i, LFMP_ALLOCATION_COUNT)
				{
					InterlockedIncrement(positiveIntegerAddressA[i]);
				}

				FOR(i, LFMP_ALLOCATION_COUNT)
				{
					if (*positiveIntegerAddressA[i] != LFMP_POSITIVE_INTEGER_INITIAL_VALUE + 1) MEMORY_DUMPER.SelfCrash();
				}

				FOR(i, LFMP_ALLOCATION_COUNT)
				{
					InterlockedDecrement(positiveIntegerAddressA[i]);
				}

				FOR(i, LFMP_ALLOCATION_COUNT)
				{
					if (*positiveIntegerAddressA[i] != LFMP_POSITIVE_INTEGER_INITIAL_VALUE) MEMORY_DUMPER.SelfCrash();
				}

				FOR(i, LFMP_ALLOCATION_COUNT)
				{
					POSITIVE_INTEGER_LFS.Push(positiveIntegerAddressA[i]);
				}
			}

			if (1)
			{
				FOR(i, LFMP_ALLOCATION_COUNT)
				{
					positiveIntegerAddressA[i] = POSITIVE_INTEGER_LFQ.Dequeue();
					*positiveIntegerAddressA[i] = threadKey;
				}

				FOR(i, LFMP_ALLOCATION_COUNT)
				{
					//if (*positiveIntegerAddressA[i] != LFMP_POSITIVE_INTEGER_INITIAL_VALUE) MEMORY_DUMPER.SelfCrash();
					if (*positiveIntegerAddressA[i] != threadKey) MEMORY_DUMPER.SelfCrash();
				}
				
				FOR(i, LFMP_ALLOCATION_COUNT)
				{
					InterlockedIncrement(positiveIntegerAddressA[i]);
				}
				
				FOR(i, LFMP_ALLOCATION_COUNT)
				{
					//if (*positiveIntegerAddressA[i] != LFMP_POSITIVE_INTEGER_INITIAL_VALUE + 1) MEMORY_DUMPER.SelfCrash();
					if (*positiveIntegerAddressA[i] != threadKey + 1) MEMORY_DUMPER.SelfCrash();
				}
				
				FOR(i, LFMP_ALLOCATION_COUNT)
				{
					InterlockedDecrement(positiveIntegerAddressA[i]);
				}
				
				FOR(i, LFMP_ALLOCATION_COUNT)
				{
					//if (*positiveIntegerAddressA[i] != LFMP_POSITIVE_INTEGER_INITIAL_VALUE) MEMORY_DUMPER.SelfCrash();
					if (*positiveIntegerAddressA[i] != threadKey) MEMORY_DUMPER.SelfCrash();
				}

				FOR(i, LFMP_ALLOCATION_COUNT)
				{
					POSITIVE_INTEGER_LFQ.Enqueue(positiveIntegerAddressA[i]);
				}
			}
		}

		//{
		//	positiveIntegerAddress = POSITIVE_INTEGER_LFS.Pop();
		//
		//	if (*positiveIntegerAddress == LFMP_POSITIVE_INTEGER_INITIAL_VALUE)
		//	{
		//		InterlockedIncrement(positiveIntegerAddress);
		//
		//		Sleep(LFMP_SLEEP_COUNT);
		//
		//		if (*positiveIntegerAddress == LFMP_POSITIVE_INTEGER_INITIAL_VALUE + 1)
		//		{
		//			InterlockedDecrement(positiveIntegerAddress);
		//
		//			Sleep(LFMP_SLEEP_COUNT);
		//
		//			if (*positiveIntegerAddress == LFMP_POSITIVE_INTEGER_INITIAL_VALUE) POSITIVE_INTEGER_LFS.Push(positiveIntegerAddress);
		//			else MEMORY_DUMPER.ForcedCrash();
		//		}
		//		else MEMORY_DUMPER.ForcedCrash();
		//	}
		//	else MEMORY_DUMPER.ForcedCrash();
		//}
	}

	return 0;
}