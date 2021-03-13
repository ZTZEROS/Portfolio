#pragma once

template<typename PacketHeaderType>
class SerialPacketQueue
{
private:
	unsigned int HeaderSize;
	unsigned int BodySize;

	unsigned int FrontIndex;
	unsigned int CurrentSize;
	unsigned int MaximumSize;

	unsigned int ReferenceCount;

	char Queue[SERIAL_PACKET_QUEUE_SIZE]; //ByteA

public:
	SerialPacketQueue() : HeaderSize(0), BodySize(0), FrontIndex(0), CurrentSize(0), MaximumSize(0), ReferenceCount(0)
	{
		HeaderSize = sizeof(PacketHeaderType);
		//BodySize = 0;

		FrontIndex = HeaderSize;
		//CurrentSize = 0;
		MaximumSize = sizeof(Queue) - HeaderSize;

		memset(Queue, 0, sizeof(Queue));

		++ReferenceCount;
	}
	~SerialPacketQueue()
	{

	}

	template<typename DatumType>
	unsigned int Enqueue(DatumType datum)
	{
		unsigned int datumSize;

		datumSize = sizeof(datum);

		if (datumSize <= MaximumSize - CurrentSize)
		//if (datumSize <= MaximumSize - BodySize)
		{
			*(DatumType*)&(Queue[FrontIndex + CurrentSize]) = datum;

			BodySize += datumSize;

			CurrentSize += datumSize;

			return datumSize;
		}
		else return 0;
	}

	template<typename DatumType>
	unsigned int Dequeue(DatumType& datum)
	{
		unsigned int datumSize;

		datumSize = sizeof(datum);

		if (datumSize <= CurrentSize)
		//if (datumSize <= BodySize)
		{
			datum = *(DatumType*)&(Queue[FrontIndex]);

			BodySize += datumSize;

			FrontIndex += datumSize;
			CurrentSize -= datumSize;

			return datumSize;
		}
		else return 0;
	}

	/*
	unsigned int Encrypt()
	{
		return 0;
	}
	
	unsigned int Decrypt()
	{
		return 0;
	}

	unsigned int RSA_Encrypt()
	{
		return 0;
	}
	
	unsigned int RSA_Decrypt()
	{
		return 0;
	}

	unsigned int XorEncrypt(char xorCode)
	{
		unsigned int frontIndex;
		unsigned int currentSize;

		frontIndex = FrontIndex;
		currentSize = CurrentSize;

		for (unsigned int i = 0; i < currentSize; ++i)
		{
			Queue[frontIndex + i] = Queue[frontIndex + i] ^ xorCode;
		}

		return xorCode;
	}

	unsigned int XorDecrypt(char xorCode)
	{
		unsigned int frontIndex;
		unsigned int currentSize;

		frontIndex = FrontIndex;
		currentSize = CurrentSize;

		for (unsigned int i = 0; i < currentSize; ++i)
		{
			Queue[frontIndex + i] = Queue[frontIndex + i] ^ xorCode;
		}

		return xorCode;
	}

	unsigned int CalculateChecksum()
	{
		unsigned char checksum;
		unsigned int frontIndex;
		unsigned int currentSize;

		checksum = 0;
		frontIndex = FrontIndex;
		currentSize = CurrentSize;

		for (unsigned int i = 0; i < currentSize; ++i)
		{
			checksum += Queue[frontIndex + i];
		}

		return checksum;
	}
	*/

	template<typename DatumType>
	unsigned int operator<<(DatumType datum)
	{
		unsigned int datumSize;

		datumSize = sizeof(datum);

		if (datumSize <= MaximumSize - CurrentSize)
		//if (datumSize <= MaximumSize - BodySize)
		{
			*(DatumType*)&(Queue[FrontIndex + CurrentSize]) = datum;

			BodySize += datumSize;

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
		//if (datumSize <= BodySize)
		{
			datum = *(DatumType*)&(Queue[FrontIndex]);

			BodySize -= datumSize;

			FrontIndex += datumSize;
			CurrentSize -= datumSize;

			return datumSize;
		}
		else return 0;
	}



	unsigned int IncreaseBodySize(unsigned int increasingSize) { BodySize += increasingSize;  CurrentSize += increasingSize; return BodySize; }
	unsigned int IncreaseCurrentSize(unsigned int increasingSize) { CurrentSize += increasingSize; BodySize += increasingSize; return CurrentSize; }
	unsigned int IncreaseReferenceCount() { return InterlockedIncrement(&ReferenceCount); }

	unsigned int DecreaseBodySize(unsigned int decreasingSize) { BodySize -= decreasingSize; CurrentSize -= decreasingSize; }
	unsigned int DecreaseCurrentSize(unsigned int decreasingSize) { CurrentSize -= decreasingSize; BodySize -= decreasingSize; }
	unsigned int DecreaseReferenceCount() { return InterlockedDecrement(&ReferenceCount); }



	unsigned int GetHeaderSize() { return HeaderSize; }
	unsigned int GetBodySize() { return BodySize; }

	unsigned int GetFrontIndex() { return FrontIndex; }
	unsigned int GetCurrentSize() { return CurrentSize; }
	unsigned int GetNextIndex() { return FrontIndex + CurrentSize; } //RearIndex + 1
	unsigned int GetMaximumSize() { return MaximumSize; }

	unsigned int GetTotalSize() { return HeaderSize + CurrentSize; }

	unsigned int GetReferenceCount() { return ReferenceCount; }

	char* GetHeaderAddress() { return &Queue[0]; }
	char* GetBodyAddress() { return &Queue[HeaderSize]; }

	char* GetQueueAddress() { return &Queue[0]; }
	char* GetFrontAddress() { return &Queue[FrontIndex]; }
	char* GetNextAddress() { return &Queue[FrontIndex + CurrentSize]; }

	PacketHeaderType GetHeader()
	{
		PacketHeaderType header;

		header = *(PacketHeaderType*)&Queue[0];

		return header;
	}



	void SetHeader(PacketHeaderType header)
	{
		unsigned int headerSize;

		headerSize = sizeof(header);

		HeaderSize = headerSize;
		FrontIndex = headerSize;
		MaximumSize = sizeof(Queue) - headerSize;

		*(PacketHeaderType*)&Queue[0] = header;
	}
};

