#pragma once

template<typename DatumType>
class ThreadLocalChunk;

template<typename DatumType>
class ThreadLocalPool;

template <typename DatumType>
struct ThreadLocalPiece
{
	ThreadLocalChunk<DatumType>* ChunkAddress;
	
	DatumType Datum;
	unsigned int Index; //PieceIndex;
	//unsigned int ReferenceCount;
};

template<typename DatumType>
class ThreadLocalChunk
{
private:
	ThreadLocalPool<DatumType>* PoolAddress;
	
	ThreadLocalPiece<DatumType> PieceA[256];
	unsigned int CurrentIndex;
	unsigned int CurrentAllocationCount;
	unsigned int CurrentDeallocationCount;
	unsigned int MaximumCount;

public:
	ThreadLocalChunk() : PoolAddress(nullptr), CurrentIndex(0), CurrentAllocationCount(0), CurrentDeallocationCount(0), MaximumCount(0)
	{
		//PoolAddress = nullptr;
		
		//memset(&PieceA, 0, sizeof(PieceA)); //danger

		//CurrentIndex = 0;
		//CurrentAllocationCount = 0;
		//CurrentDeallocationCount = 0;
		MaximumCount = sizeof(PieceA) / sizeof(ThreadLocalPiece<DatumType>);

		for (unsigned int i = 0; i < MaximumCount; ++i)
		{
			PieceA[i].ChunkAddress = this;
			PieceA[i].Index = i;
			//ReferenceCount = 0;
		}
	}
	~ThreadLocalChunk()
	{
		CurrentIndex = 0;
		CurrentAllocationCount = 0;
		CurrentDeallocationCount = 0;
	}

	bool Initialize()
	{
		PoolAddress = nullptr;

		//memset(&DatumA, 0, sizeof(DatumA)); //danger

		CurrentIndex = 0;
		CurrentAllocationCount = 0;
		CurrentDeallocationCount = 0;
		MaximumCount = sizeof(PieceA) / sizeof(ThreadLocalPiece<DatumType>);

		for (unsigned int i = 0; i < MaximumCount; ++i)
		{
			PieceA[i].ChunkAddress = this;
			PieceA[i].Index = i;
			//ReferenceCount = 0;
		}

		return 0;
	}

	bool Terminalize()
	{
		CurrentIndex = 0;
		CurrentAllocationCount = 0;
		CurrentDeallocationCount = 0;

		return 0;
	}

	DatumType* Allocate()
	{
		DatumType* currentDatumAddress;

		if (CurrentAllocationCount < MaximumCount)
		{
			PieceA[CurrentIndex].ChunkAddress = this;
			//new (&PieceA[CurrentIndex].Datum) DatumType;
			PieceA[CurrentIndex].Index = CurrentIndex;
			//++PieceA[CurrentIndex].ReferenceCount;

			currentDatumAddress = &PieceA[CurrentIndex].Datum;

			++CurrentIndex;
			++CurrentAllocationCount;

			//InterlockedIncrement(&CurrentIndex);
			//InterlockedIncrement(&CurrentAllocationCount);
		}
		else currentDatumAddress = nullptr;

		return currentDatumAddress; //&DatumA[CurrentIndex++];
	}

	DatumType* Deallocate(DatumType* datumAddress)
	{
		DatumType* currentDatumAddress;

		if (CurrentDeallocationCount < MaximumCount)
		{
			//datumAddress->~DatumType();

			if (InterlockedIncrement(&CurrentDeallocationCount) < MaximumCount) currentDatumAddress = datumAddress;
			else currentDatumAddress = nullptr;
		}
		else currentDatumAddress = nullptr;

		return currentDatumAddress;
	}

	void SetPoolAddress(ThreadLocalPool<DatumType>* poolAddress) { PoolAddress = poolAddress; }

	unsigned int GetAllocationCount() { return CurrentAllocationCount; }
	unsigned int GetDeallocationCount() { return CurrentDeallocationCount; }
	bool CheckAllocationCount() { return CurrentAllocationCount == MaximumCount ? 1 : 0; }
};
