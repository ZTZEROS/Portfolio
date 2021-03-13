#pragma once

class QueryPacketPool //QueuryAddressQueue //QueryAddressPool
{
private:
	volatile unsigned int FrontIndex;
	volatile unsigned int CurrentCount; //CurrentSize
	volatile unsigned int NextIndex; //(FrontIndex + CurrentSize % MaximumSize) == RearIndex + 1
	unsigned int MaximumCount; //MaximumSize
	bool ConstructorCalling;
	void* Queue[BUFFER_SIZE]; 

	void** QueueAddress; 

	bool Locked;

	SRWLOCK EnqueueLock;
	SRWLOCK DequeueLock;

	//bool Enqueuing;
	//bool Dequeuing;

public:
	QueryPacketPool();
	explicit QueryPacketPool(unsigned int maximumCount, bool constructorCalling);
	~QueryPacketPool();

	bool Initialize();
	bool Terminalze();

	void* Allocate(); //Dequeue();
	void* Deallocate(); //Enqueue();
};

//모든 쿼리패킷을 포괄할 수 있는 쿼리패킷 구조체를 정해서 그 구조체를 사용하도록 다시 설계