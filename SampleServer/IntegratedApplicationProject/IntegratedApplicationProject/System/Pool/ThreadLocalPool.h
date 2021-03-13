#pragma once

/*
template<typename DatumType>
struct ThreadLocalChunk;

template <typename DatumType>
struct ThreadLocalPiece
{
	ThreadLocalChunk<DatumType>* TLC_Address;
	
	DatumType Datum;
	unsigned int PieceIndex;
};

template<typename DatumType>
struct ThreadLocalChunk
{
	ThreadLocalPool<DatumType>* TLP_Address;

	ThreadLocalPiece<DatumType> PieceA[256];
	unsigned int CurrentIndex;
	unsigned int CurrentAllocationCount;
	unsigned int CurrentDeallocationCount;
	unsigned int MaximumCount;
};
*/

template<typename DatumType>
class ThreadLocalPool
{
private:
	LockFreePool<ThreadLocalChunk<DatumType>> ChunkPool;
	DWORD TLS_Index; //ThreadLocalStorageIndex;
	
public:
	ThreadLocalPool() : ChunkPool(0, 1), TLS_Index(0)
	{
		TLS_Index = TlsAlloc();

		if (TLS_Index == TLS_OUT_OF_INDEXES) MEMORY_DUMPER.SelfCrash();
	}
	~ThreadLocalPool()
	{
		TlsFree(TLS_Index);
	}

	bool Initialize()
	{
		ChunkPool.Initialize();

		TLS_Index = TlsAlloc();

		if (TLS_Index == TLS_OUT_OF_INDEXES) MEMORY_DUMPER.SelfCrash();

		return 0;
	}

	bool Terminalize()
	{
		ChunkPool.Terminalize();

		TlsFree(TLS_Index);

		return 0;
	}

	DatumType* Allocate()
	{
		ThreadLocalChunk<DatumType>* chunkAddress;
		DatumType* datumAddress;

		chunkAddress = (ThreadLocalChunk<DatumType>*)TlsGetValue(TLS_Index);

		if (!chunkAddress)
		{
			chunkAddress = ChunkPool.Allocate();
			
			//memset(chunkAddress, 0, sizeof(*chunkAddress)); //danger

			//chunkAddress->TLP_Address = this;
			//chunkAddress->CurrentIndex = 0;
			//chunkAddress->CurrentAllocationCount = 0;
			//chunkAddress->CurrentDeallocationCount = 0;
			//chunkAddress->MaximumCount = sizeof(chunkAddress->PieceA) / sizeof(ThreadLocalPiece<DatumType>);

			chunkAddress->SetPoolAddress(this);
			
			TlsSetValue(TLS_Index, chunkAddress);
		}



		datumAddress = chunkAddress->Allocate();

		if (!datumAddress)
		{
			chunkAddress = ChunkPool.Allocate();
			
			//memset(chunkAddress, 0, sizeof(*chunkAddress)); //danger

			//chunkAddress->TLP_Address = this;
			//chunkAddress->CurrentIndex = 0;
			//chunkAddress->CurrentAllocationCount = 0;
			//chunkAddress->CurrentDeallocationCount = 0;
			//chunkAddress->MaximumCount = sizeof(chunkAddress->DatumA) / sizeof(ThreadLocalPiece<DatumType>);

			chunkAddress->SetPoolAddress(this);

			TlsSetValue(TLS_Index, chunkAddress);

			datumAddress = chunkAddress->Allocate();
		}

		if (chunkAddress->CheckAllocationCount()) TlsSetValue(TLS_Index, nullptr); //uglyCode

		return datumAddress;
	}

	DatumType* Deallocate(DatumType* datumAddress)
	{
		ThreadLocalChunk<DatumType>* chunkAddress;

		chunkAddress = ((ThreadLocalPiece<DatumType>*)((char*)datumAddress - sizeof(ThreadLocalPiece<DatumType>*)))->ChunkAddress;

		if(!chunkAddress->Deallocate(datumAddress))	ChunkPool.Deallocate(chunkAddress);
		else chunkAddress = nullptr;

		return datumAddress;
	}
};