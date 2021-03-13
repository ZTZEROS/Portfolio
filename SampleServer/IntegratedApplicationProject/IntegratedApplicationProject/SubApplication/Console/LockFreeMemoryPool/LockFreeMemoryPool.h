#pragma once

enum LOCK_FREE_MEMORY_POOL_LOCAL_CONSTANT
{
	LFMP_ALLOCATION_COUNT = 3,
	LFMP_WORK_SUB_THREAD_COUNT = 3,
	LFMP_TOTAL_ALLOCATION_COUNT = LFMP_ALLOCATION_COUNT * LFMP_WORK_SUB_THREAD_COUNT,
	LFMP_SLEEP_COUNT = 50,
	LFMP_POSITIVE_INTEGER_INITIAL_VALUE = 0x0000000055555555,
};

struct LFMP_SubThread : public CommonSubThread
{

};

extern LockFreePool<UINT64> POSITIVE_INTEGER_LFP;
extern LockFreeStack<UINT64*> POSITIVE_INTEGER_LFS;
extern LockFreeQueue<UINT64*> POSITIVE_INTEGER_LFQ;

extern LFMP_SubThread LFMP_WORK_SUB_THREAD[LFMP_WORK_SUB_THREAD_COUNT];

extern BOOL LFMP_SHUTDOWN;

int ExecuteLockFreeMemoryPool(int argumentCount, char* argumentVector[]);

bool InitializeLockFreeMemoryPool();

bool UpdateLockFreeMemoryPool();
bool RenderLockFreeMemoryPool();

bool TerminalizeLockFreeMemoryPool();

unsigned int WINAPI WorkLockFreeMemoryPool(LPVOID argument);