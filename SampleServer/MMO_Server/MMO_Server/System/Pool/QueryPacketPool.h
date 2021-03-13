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

//��� ������Ŷ�� ������ �� �ִ� ������Ŷ ����ü�� ���ؼ� �� ����ü�� ����ϵ��� �ٽ� ����