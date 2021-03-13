#pragma once

struct PoolWorkHistory : public CommonWorkHistory
{

};

template<typename DatumType>
struct PoolInstance //PoolNode
{
	PoolInstance<DatumType>* NextAddress; //NextInstanceAddress;
	DatumType Datum;
	PoolInstance<DatumType>* SelfAddress; //SelfInstanceAddress;
};

template<typename DatumType>
struct PoolTop
{
	PoolInstance<DatumType>* Address;
	unsigned int WorkCount;
	unsigned int ThreadKey; //ThreadId = GetCurrentThreadId();
};

template<typename DatumType>
class LockFreePool //LockFreeLinkedStackMemoryPool //LockFreeLinkedStackInstancePool 
{
private:
	unsigned int CurrentCount;
	unsigned int MaximumCount;
	bool ConstructorCalling;

	__declspec(align(16)) PoolTop<DatumType> Top;

public:
	LockFreePool() : CurrentCount(0), MaximumCount(0), ConstructorCalling(0)
	{
		Top.Address = nullptr;
		Top.WorkCount = 0;
		Top.ThreadKey = 0;
	}
	explicit LockFreePool(unsigned int maximumCount, bool constructorCalling) : CurrentCount(0), MaximumCount(maximumCount), ConstructorCalling(constructorCalling)
	{
		if (0 < MaximumCount)
		{
			PoolInstance<DatumType>* currentAddress;
			
			Top.Address = nullptr;
			Top.WorkCount = 0;
			Top.ThreadKey = 0;

			for (unsigned int i = 0; i < MaximumCount; ++i)
			{
				currentAddress = (PoolInstance<DatumType>*)malloc(sizeof(PoolInstance<DatumType>));

				currentAddress->NextAddress = Top.Address;
				currentAddress->SelfAddress = currentAddress;

				Top.Address = currentAddress;

				if (!ConstructorCalling) new (currentAddress) PoolInstance<DatumType>;

				++CurrentCount;
			}
		}
	}
	~LockFreePool()
	{
		PoolInstance<DatumType>* currentAddress;

		while (Top.Address)
		{
			currentAddress = Top.Address;

			if(!ConstructorCalling) currentAddress->Datum.~DatumType();

			Top.Address = currentAddress->NextAddress;

			free(currentAddress);
		}
	}

	bool Initialize()
	{
		CurrentCount = 0;
		MaximumCount = 0;
		ConstructorCalling = 1;

		Top.Address = nullptr;
		Top.WorkCount = 0;
		Top.ThreadKey = 0;

		return 0;
	}

	bool Terminalize()
	{
		PoolInstance<DatumType>* currentAddress;

		while (Top.Address)
		{
			currentAddress = Top.Address;

			if (!ConstructorCalling) currentAddress->Datum.~DatumType();

			Top.Address = currentAddress->NextAddress;

			free(currentAddress);
		}

		return 0;
	}

	DatumType* Allocate()
	{
		//old non-thread safe code
		//PoolInstance<DatumType>* currentAddress;
		//
		//if (Locked) AquireSRWLockExclusive(&PoolLock);
		//
		//if (TopAddress)
		//{
		//	currentAddress = TopAddress;
		//	//currentAddress->NextAddress = nullptr;
		//	//currentAddress->SelfAddress = currentAddress;
		//
		//	TopAddress = TopAddress->NextAddress;
		//
		//	if (ConstructorCalling) new (currentAddress) PoolInstance<DatumType>;
		//
		//	--CurrentCount;
		//}
		//else
		//{
		//	currentAddress = (PoolInstance<DatumType>*)malloc(sizeof(PoolInstance<DatumType>));
		//	currentAddress->NextAddress = nullptr;
		//	currentAddress->SelfAddress = currentAddress;
		//
		//	new (currentAddress) PoolInstance<DatumType>;
		//
		//	++MaximumCount;
		//}
		//
		//if (Locked) ReleaseSRWLockExclusive(&PoolLock);

		PoolInstance<DatumType>* currentAddress;
		__declspec(align(16)) PoolTop<DatumType> oldTop;
		PoolTop<DatumType> newTop;

		currentAddress = nullptr;

		do
		{
			oldTop.WorkCount = Top.WorkCount;
			oldTop.Address = Top.Address;

			if (oldTop.Address)
			{
				currentAddress = oldTop.Address;

				newTop.WorkCount = oldTop.WorkCount;
				newTop.Address = oldTop.Address;

				++newTop.WorkCount;
				newTop.Address = newTop.Address->NextAddress;
			}
			else break;
		}
		while (!InterlockedCompareExchange128((LONG64*)&Top, (LONG64)newTop.WorkCount, (LONG64)newTop.Address, (LONG64*)&oldTop));

		if (oldTop.Address) //if(currentAddress)
		{
			//currentAddress->NextAddress = nullptr;

			InterlockedDecrement(&CurrentCount);

			if (ConstructorCalling)
			{
				new (currentAddress) PoolInstance<DatumType>;
			}
		}
		else
		{
			currentAddress = (PoolInstance<DatumType>*)malloc(sizeof(PoolInstance<DatumType>));
			currentAddress->NextAddress = nullptr;
			currentAddress->SelfAddress = currentAddress;

			new (currentAddress) PoolInstance<DatumType>;

			InterlockedIncrement(&MaximumCount);
		}

		return &(currentAddress->Datum);
	}

	DatumType* Deallocate(DatumType* datumAddress)
	{
		//old non-thread safe code
		//PoolInstance<DatumType>* currentAddress;
		//
		//if (Locked) AquireSRWLockExclusive(&PoolLock);
		//
		//currentAddress = (PoolInstance<DatumType>*)((char*)datumAddress - sizeof(PoolInstance<DatumType>*));
		//
		//if (currentAddress->SelfAddress == currentAddress) //&& (currentAddress->nextAddress == nullptr)
		//{
		//	if (ConstructorCalling) currentAddress->Instance.~DatumType();
		//
		//	currentAddress->NextAddress = TopAddress;
		//	//currentAddress->SelfAddress = currentAddress;
		//	
		//	TopAddress = currentAddress;
		//
		//	++CurrentCount;
		//
		//	if (Locked) ReleaseSRWLockExclusive(&PoolLock);
		//
		//	return &(TopAddress->Instance);
		//}
		//else
		//{
		//	if (Locked) ReleaseSRWLockExclusive(&PoolLock);
		//
		//	return nullptr;
		//}

		PoolInstance<DatumType>* currentAddress;
		__declspec(align(16)) PoolTop<DatumType> oldTop;
		PoolTop<DatumType> newTop;

		currentAddress = (PoolInstance<DatumType>*)((char*)datumAddress - sizeof(PoolInstance<DatumType>*));

		if (currentAddress->SelfAddress == currentAddress)
		{
			InterlockedIncrement(&CurrentCount);

			if (ConstructorCalling) currentAddress->Datum.~DatumType();

			do
			{
				oldTop.WorkCount = Top.WorkCount;
				oldTop.Address = Top.Address;

				currentAddress->NextAddress = oldTop.Address;

				newTop.WorkCount = oldTop.WorkCount;

				++newTop.WorkCount;
				newTop.Address = currentAddress;
			}
			while (InterlockedCompareExchange64((LONG64*)&Top.Address, (LONG64)newTop.Address, (LONG64)oldTop.Address) != (LONG64)oldTop.Address);
			//while (!InterlockedCompareExchange128((LONG64*)&Top, (LONG64)newTop.WorkCount, (LONG64)newTop.Address, (LONG64*)&oldTop));
		}
		else return nullptr;

		return &(currentAddress->Datum); //danger, need to correct
	}

	unsigned int GetCurrentCount() { return CurrentCount; }
	unsigned int GetMaximumCount() { return MaximumCount; }

	PoolTop<DatumType> GetPoolTop() { return Top; }
	PoolInstance<DatumType> GetTopInstnce() { return *(Top.Address); }
	PoolInstance<DatumType>* GetTopAddress() { return Top.Address; }
	DatumType GetTopDatum() { return Top.Address->Datum; }
};