#pragma once

template<typename DatumType>
class Pool
{
private:
	unsigned int LastIndex;
	unsigned int CurrentCount;
	unsigned int MaximumCount;
	DatumType* PoolAddress;

public:
	Pool(unsigned int maximumCount) : LastIndex(0), CurrentCount(0), MaximumCount(maximumCount)
	{
		PoolAddress = (DatumType*)malloc(sizeof(DatumType) * MaximumCount);
		//PoolAddress = new DatumType[MaximumCount];
	}
	~Pool()
	{
		LastIndex = -1;
		CurrentCount = 0;
		MaximumCount = 0;

		free(PoolAddress);
		//delete[] PoolAddress;
	}

	DatumType Enpool(DatumType datum)
	{
		if (CurrentCount < MaximumCount)
		{
			LastIndex = ++CurrentCount - 1;
			PoolAddress[LastIndex] = datum;

			return datum;
		}
		else
		{
			DatumType nullDatum;

			memset(&nullDatum, 0, sizeof(nullDatum));

			return nullDatum;
		}
	}

	unsigned int Enpool(DatumType* datumAddress, unsigned int size)
	{
		return 0;
	}

	DatumType Expool(unsigned int index) //Depool()
	{
		DatumType extractedDatum;

		if (0 < CurrentCount && 0 <= index && index <= LastIndex)
		{
			extractedDatum = PoolAddress[index];

			PoolAddress[index] = PoolAddress[LastIndex];

			memset(&PoolAddress[LastIndex], 0, sizeof(PoolAddress[LastIndex]));

			LastIndex = --CurrentCount - 1;
			//bug
			//LastIndex = 0
			//if(-1 < LastIndex)


			return extractedDatum;
		}
		else
		{
			DatumType nullDatum;

			memset(&nullDatum, 0, sizeof(nullDatum));

			return nullDatum;
		}
	}

	unsigned int Expool(unsigned int startIndex, unsigned int size)
	{
		return 0;
	}

	unsigned int GetCurrentCount() { return CurrentCount; }
	unsigned int GetCurrentSize() { return sizeof(DatumType) * CurrentCount; }
	unsigned int GetMaximumCount() { return MaximumCount; }
	unsigned int GetMaximumSize() { return sizeof(DatumType) * MaximumCount; }

	DatumType& LastDatum() { return PoolAddress[LastIndex]; } //bug
	DatumType& Datum(unsigned int index) { return PoolAddress[index]; }

	DatumType GetLastDatum() { return PoolAddress[LastIndex]; } //bug
	DatumType GetDatum(unsigned int index) { return PoolAddress[index]; }

	DatumType& operator[](unsigned int index)
	{
		return PoolAddress[index]; //return *(PoolAddress + index);
	}
};

