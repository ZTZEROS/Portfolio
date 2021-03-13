#pragma once

enum THREAD_LOCAL_MEMORY_POOL_LOCAL_CONSTANT
{
	TLMP_ALLOCATION_COUNT = 512,
	TLMP_WORK_SUB_THREAD_COUNT = 3,
	TLMP_TOTAL_ALLOCATION_COUNT = TLMP_ALLOCATION_COUNT * TLMP_WORK_SUB_THREAD_COUNT,
	TLMP_SLEEP_COUNT = 50,
	TLMP_POSITIVE_INTEGER_INITIAL_VALUE = 0x0000000055555555,
};

struct TLMP_SubThread : public CommonSubThread
{

};

extern ThreadLocalPool<UINT> POSITIVE_INTEGER_TLP;

extern TLMP_SubThread TLMP_WORK_SUB_THREAD[TLMP_WORK_SUB_THREAD_COUNT];

extern BOOL TLMP_SHUTDOWN;

int ExecuteThreadLocalMemoryPool(int argumentCount, char* argumentVector[]);

bool InitializeThreadLocalMemoryPool();

bool UpdateThreadLocalMemoryPool();
bool RenderThreadLocalMemoryPool();

bool TerminalizeThreadLocalMemoryPool();

unsigned int WINAPI WorkThreadLocalMemoryPool(LPVOID argument);
