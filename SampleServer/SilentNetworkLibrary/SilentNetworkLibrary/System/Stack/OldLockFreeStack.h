#pragma once

struct StackWorkRecord : public CommonWorkRecord
{

};

template<typename DatumType>
struct StackInstance //StackNode
{
	StackInstance<DatumType>* NextAddress; //NextInstanceAddress;
	DatumType Datum;
	StackInstance<DatumType>* SelfAddress;
};

template<typename DatumType>
struct StackTop
{
	StackInstance<DatumType>* Address;
	unsigned int WorkCount;
	unsigned int ThreadKey; //ThreadId = GetCurrentThreadId();
};

template<typename DatumType>
class LockFreeStack //LockFreeLinkedStack
{
private:
	unsigned int CurrentCount;
	unsigned int MaximumCount;
	bool ConstructorCalling;
	__declspec(align(16)) StackTop<DatumType> Top;

public:
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
		StackInstance<DatumType>* currentAddress;

		while (Top.Address)
		{
			currentAddress = Top.Address;

			if (!ConstructorCalling) currentAddress->Datum.~DatumType();

			Top.Address = currentAddress->NextAddress;

			free(currentAddress);
		}

		return 0;
	}

	DatumType* Push(DatumType* datumAddress)
	{
		StackInstance<DatumType>* currentAddress;
		__declspec(align(16)) StackTop<DatumType> oldTop;
		StackTop<DatumType> newTop;

		currentAddress = (StackInstance<DatumType>*)((char*)datumAddress - sizeof(StackInstance<DatumType>*));

		if (currentAddress->SelfAddress == currentAddress)
		{
			//++CurrentCount; //danger

			InterlockedIncrement(&CurrentCount);

			if (ConstructorCalling) currentAddress->Datum.~DatumType();

			do
			{
				oldTop = Top;

				currentAddress->NextAddress = oldTop.Address;

				newTop.Address = currentAddress;
				newTop.WorkCount = oldTop.WorkCount;
				++newTop.WorkCount;
			}
			while (!InterlockedCompareExchange128((LONG64*)&Top, (LONG64)newTop.WorkCount, (LONG64)newTop.Address, (LONG64*)&oldTop));
		}
		else return nullptr;


		return &(currentAddress->Datum);
	}

	DatumType* Pop()
	{
		StackInstance<DatumType>* currentAddress;
		__declspec(align(16)) StackTop<DatumType> oldTop;
		StackTop<DatumType> newTop;

		currentAddress = nullptr;

		do
		{
			oldTop = Top;

			if (oldTop.Address)
			{
				currentAddress = oldTop.Address;

				newTop.Address = oldTop.Address->NextAddress;
				newTop.WorkCount = oldTop.WorkCount;
				++newTop.WorkCount;
			}
			else break;
		}
		while (!InterlockedCompareExchange128((LONG64*)&Top, (LONG64)newTop.WorkCount, (LONG64)newTop.Address, (LONG64*)&oldTop));

		if (oldTop.Address)
		{
			//currentAddress->NextAddress = nullptr;

			//--CurrentCount; //danger

			InterlockedDecrement(&CurrentCount);

			if (ConstructorCalling) new (currentAddress) StackInstance<DatumType>;
		}
		else
		{
			currentAddress = (StackInstance<DatumType>*)malloc(sizeof(StackInstance<DatumType>));
			currentAddress->NextAddress = nullptr;
			currentAddress->SelfAddress = currentAddress;

			new (currentAddress) StackInstance<DatumType>;

			//++MaximumCount; //danger

			InterlockedIncrement(&MaximumCount);
		}

		return &(currentAddress->Datum);
	}

	unsigned int GetCurrentCount() { return CurrentCount; }
	unsigned int GetMaximumCount() { return MaximumCount; }
	DatumType* GetTop() { return &(Top.Address->Datum); }
	//StackInstance<DatumType>* GetTopInstance() { return Top.Address; }
};