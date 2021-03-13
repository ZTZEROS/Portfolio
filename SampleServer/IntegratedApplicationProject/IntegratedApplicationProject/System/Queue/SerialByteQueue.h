#pragma once

class SerialByteQueue
{
private:
	unsigned int HeaderSize;

	unsigned int FrontIndex;
	unsigned int CurrentSize; //BodySize;
	unsigned int MaximumSize;

	char Queue[SERIAL_BYTE_QUEUE_SIZE]; //ByteA

public:
	SerialByteQueue();
	~SerialByteQueue();

	template<typename DatumType>
	unsigned int operator<<(DatumType datum)
	{
		unsigned int datumSize;

		datumSize = sizeof(datum);

		if (datumSize <= MaximumSize - CurrentSize)
		{
			*(DatumType*)&(Queue[FrontIndex + CurrentSize]) = datum;

			CurrentSize += datumSize;

			return datumSize;
		}
		else return 0;
	}

	template<typename DatumType>
	unsigned int operator>>(DatumType& datum)
	{
		unsigned int datumSize;

		datumSize = sizeof(datum);

		if (datumSize <= CurrentSize)
		{
			datum = *(DatumType*)&(Queue[FrontIndex]);

			FrontIndex += datumSize;
			CurrentSize -= datumSize;

			return datumSize;
		}
		else return 0;
	}



	void IncreaseCurrentSize(unsigned int increasingSize) { CurrentSize += increasingSize; }
	void DecreaseCurrentSize(unsigned int decresingSize) { CurrentSize -= decresingSize; }



	unsigned int GetHeaderSize() { return HeaderSize; }

	unsigned int GetFrontIndex() { return FrontIndex; }
	unsigned int GetCurrentSize() { return CurrentSize; }
	unsigned int GetBodySize() { return CurrentSize; } //unsigned int GetBodySize() { return BodySize; }
	unsigned int GetrNextIndex() { return FrontIndex + CurrentSize; } //RearIndex + 1
	unsigned int GetMaximumSize() { return MaximumSize; }

	unsigned int GetTotalSize() { return HeaderSize + CurrentSize; }
	
	char* GetQueueAddress() { return &Queue[0]; }
	char* GetFrontAddress() { return &Queue[FrontIndex]; }
	char* GetNextAddress() { return &Queue[FrontIndex + CurrentSize]; }

	template<typename DatumType>
	DatumType GetHeader()
	{
		DatumType header;

		header = *(DatumType*)&Queue[0];

		return header;
	}
	


	template<typename DatumType>
	void SetHeader(DatumType datum)
	{
		unsigned int headerSize;

		headerSize = sizeof(datum);
		
		HeaderSize = headerSize;
		FrontIndex = headerSize;
		MaximumSize = sizeof(Queue) - headerSize;

		*(DatumType*)&Queue[0] = datum;
	}


	
	/*
	template<typename DatumType>
	unsigned int operator<<(DatumType datum)
	{

	try
	{
	if (sizeof(DatumType) <= MaximumSize - CurrentSize)
	{
	*(DatumType*)&(Queue[FrontIndex + CurrentSize]) = datum;

	CurrentSize += sizeof(DatumType);
	}
	else
	{
	return 0;

	Exception exception;

	//#FrontIndex;
	FrontIndex;
	//#CurrentSize;
	CurrentSize;
	//#MaxSize;
	//MaxSize;
	//typeid(DatumType).name;
	sizeof(DatumType);
	__DATE__;
	__TIME__;

	//EXCEPTION_MANAGER.AddException(exception);

	throw exception;
	}

	return sizeof(DatumType);
	}
	catch (Exception exception)
	{
	return 0;
	}

	}

	template<typename DatumType>
	unsigned int operator>>(DatumType& datum)
	{
		try
		{
			if (sizeof(DatumType) <= CurrentSize)
			{
				datum = *(DatumType*)&(Queue[FrontIndex]);
				FrontIndex += sizeof(DatumType);

				CurrentSize -= sizeof(DatumType);

				if (CurrentSize == 0) FrontIndex = HeaderSize; //if (CurrentSize < 1) FrontIndex = 0;
			}
			else
			{
				return 0;

				Exception exception;

				//#FrontIndex;
				FrontIndex;
				//#CurrentSize;
				CurrentSize;
				//#MaxSize;
				//MaxSize;
				//typeid(DatumType).name;
				sizeof(DatumType);
				__DATE__;
				__TIME__;

				//EXCEPTION_MANAGER.AddException(exception);

				throw exception;
			}

			return sizeof(DatumType);
		}
		catch (Exception exception)
		{
			return 0;
		}
		
	}
	*/

	/*
	unsigned int operator<<(char datum)
	{
		try
		{
			*(char*)&(Queue[FrontIndex + CurrentSize]) = datum;
		}
		catch (...)
		{

		}
	}

	unsigned int operator<<(unsigned char datum)
	{
		try
		{
			*(unsigned char*)&(Queue[FrontIndex + CurrentSize]) = datum;
		}
		catch (...)
		{

		}
	}

	unsigned int operator<<(short int datum)
	{
		try
		{
			*(short int*)&(Queue[FrontIndex + CurrentSize]) = datum;
		}
		catch (...)
		{

		}
	}

	unsigned int operator<<(unsigned short int datum)
	{
		try
		{
			*(unsigned short int*)&(Queue[FrontIndex + CurrentSize]) = datum;
		}
		catch (...)
		{

		}
	}

	unsigned int operator<<(long int datum)
	{
		try
		{
			*(short int*)&(Queue[FrontIndex + CurrentSize]) = datum;
		}
		catch (...)
		{

		}
	}

	unsigned int operator<<(unsigned long int datum)
	{
		try
		{
			*(unsigned long int*)&(Queue[FrontIndex + CurrentSize]) = datum;
		}
		catch (...)
		{

		}
	}

	unsigned int operator<<(long long int datum)
	{
		try
		{
			*(long int*)&(Queue[FrontIndex + CurrentSize]) = datum;
		}
		catch (...)
		{

		}
	}

	unsigned int operator<<(unsigned long long int datum)
	{
		try
		{
			*(unsigned long long int*)&(Queue[FrontIndex + CurrentSize]) = datum;
		}
		catch (...)
		{

		}
	}

	unsigned int operator<<(float datum)
	{
		try
		{
			*(float*)&(Queue[FrontIndex + CurrentSize]) = datum;
		}
		catch (...)
		{

		}
	}

	unsigned int operator<<(double datum)
	{
		try
		{
			*(double*)&(Queue[FrontIndex + CurrentSize]) = datum;
		}
		catch (...)
		{

		}
	}
	*/
};

