//QueuryAddressQueue.cpp //QueryAddressPool.cpp
#include "stdafx.h"
#include "QueryPacketPool.h"

QueryPacketPool::QueryPacketPool()
{
	FrontIndex = 0;
	CurrentCount = 0;
	NextIndex = 0;
	MaximumCount = sizeof(Queue); //sizeof(Queue) / sizeof(CommonQueryPacket*);
	ConstructorCalling = 0;
	
	memset(Queue, 0, sizeof(Queue));

	QueueAddress = Queue;

	Locked = 0;

	InitializeSRWLock(&EnqueueLock);
	InitializeSRWLock(&DequeueLock);
}

QueryPacketPool::QueryPacketPool(unsigned int maximumCount, bool constructorCalling)
{
	FrontIndex = 0;
	CurrentCount = 0;
	NextIndex = 0;
	MaximumCount = maximumCount;
	ConstructorCalling = constructorCalling;

	memset(Queue, 0, sizeof(Queue));

	QueueAddress = (void**)malloc(sizeof(void*) * MaximumCount);
	//QueueAddress = (CommonQueryPacket**)malloc(sizeof(CommonQueryPacket*) * MaximumCount);
	//QueueAddress = Queue;

	Locked = 0;

	InitializeSRWLock(&EnqueueLock);
	InitializeSRWLock(&DequeueLock);
}

QueryPacketPool::~QueryPacketPool()
{
	if (CurrentCount != MaximumCount)
	{
		//Crash(); //Memoryleak
	}

	for (unsigned int i = 0; i < MaximumCount; ++i)
	{
		free(QueueAddress[i]);
	}

	if (QueueAddress != Queue) free(QueueAddress);
}

void* QueryPacketPool::Allocate()
{

}

void* QueryPacketPool::Deallocate()
{

}