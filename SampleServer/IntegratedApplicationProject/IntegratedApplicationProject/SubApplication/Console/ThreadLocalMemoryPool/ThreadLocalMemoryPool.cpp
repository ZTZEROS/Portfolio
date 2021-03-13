#include "stdafx.h"
#include "ThreadLocalMemoryPool.h"

ThreadLocalPool<UINT> POSITIVE_INTEGER_TLP;

TLMP_SubThread TLMP_WORK_SUB_THREAD[TLMP_WORK_SUB_THREAD_COUNT];

BOOL TLMP_SHUTDOWN;

int ExecuteThreadLocalMemoryPool(int argumentCount, char* argumentVector[])
{
	//Initialize:
	{
		InitializeThreadLocalMemoryPool();
	}

	//Update:
	//Render:
	{
		while (1)
		{
			if (UpdateThreadLocalMemoryPool()) break;
			if (RenderThreadLocalMemoryPool()) break;
		}
	}

	//Terminalize:
	{
		TerminalizeThreadLocalMemoryPool();
	}

	return 0;
}

bool InitializeThreadLocalMemoryPool()
{
	srand(27);
	//srand(time(nullptr));

	timeBeginPeriod(1);

	TIMER.Initialize();

	TLMP_SHUTDOWN = 0;

	FOR(i, TLMP_WORK_SUB_THREAD_COUNT)
	{
		TLMP_SubThread tlmpSubThread;

		tlmpSubThread.Handle = (HANDLE)_beginthreadex(nullptr, 0, WorkThreadLocalMemoryPool, nullptr, 0, (unsigned int*)&tlmpSubThread.IdentificationNumber);
		TLMP_WORK_SUB_THREAD[i].IdentificationNumber = tlmpSubThread.IdentificationNumber;
	}

	return 0;
}

bool UpdateThreadLocalMemoryPool()
{
	if (TIMER.CheckUpdate())
	{
		if (GetAsyncKeyState('Q') & 0x0001) return 1;
	}

	return 0;
}

bool RenderThreadLocalMemoryPool()
{
	if (TIMER.ShowCounter())
	{

	}

	return 0;
}

bool TerminalizeThreadLocalMemoryPool()
{
	HANDLE workSubThreadHandle[TLMP_WORK_SUB_THREAD_COUNT];

	timeEndPeriod(1);

	TIMER.Termimalize();

	TLMP_SHUTDOWN = 1;

	FOR(i, TLMP_WORK_SUB_THREAD_COUNT)
	{
		workSubThreadHandle[i] = TLMP_WORK_SUB_THREAD[i].Handle;
	}

	WaitForMultipleObjects(TLMP_WORK_SUB_THREAD_COUNT, workSubThreadHandle, 1, INFINITE);

	return 0;
}

unsigned int WINAPI WorkThreadLocalMemoryPool(LPVOID argument)
{
	UINT* positiveIntegerA[TLMP_ALLOCATION_COUNT];
	UINT threadKey;

	threadKey = GetCurrentThreadId();
	srand(threadKey);

	while (!TLMP_SHUTDOWN)
	{
		FOR(i, TLMP_ALLOCATION_COUNT)
		{
			positiveIntegerA[i] = POSITIVE_INTEGER_TLP.Allocate();
			(*positiveIntegerA[i]) = threadKey;
		}

		FOR(i, TLMP_ALLOCATION_COUNT)
		{
			if (*positiveIntegerA[i] != threadKey) MEMORY_DUMPER.SelfCrash();
		}

		FOR(i, TLMP_ALLOCATION_COUNT)
		{
			InterlockedIncrement(positiveIntegerA[i]);
		}

		FOR(i, TLMP_ALLOCATION_COUNT)
		{
			if (*positiveIntegerA[i] != threadKey + 1) MEMORY_DUMPER.SelfCrash();
		}

		FOR(i, TLMP_ALLOCATION_COUNT)
		{
			InterlockedDecrement(positiveIntegerA[i]);
		}

		FOR(i, TLMP_ALLOCATION_COUNT)
		{
			if (*positiveIntegerA[i] != threadKey) MEMORY_DUMPER.SelfCrash();
		}

		FOR(i, TLMP_ALLOCATION_COUNT)
		{
			POSITIVE_INTEGER_TLP.Deallocate(positiveIntegerA[i]);
		}
	}

	return 0;
}