#include "pch.h"
#include "SerialByteQueue.h"

SerialByteQueue::SerialByteQueue() : HeaderSize(0), FrontIndex(0), CurrentSize(0), MaximumSize(0)
{
	MaximumSize = sizeof(Queue);

	memset(Queue, 0, sizeof(Queue));
}

SerialByteQueue::~SerialByteQueue() {}