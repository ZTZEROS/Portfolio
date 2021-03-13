#include "pch.h"
#include "MMO.h"
#include "MMO_BaseClient.h"
#include "MMO_BaseServer.h"

unsigned int MMO_BaseClient::ReceivePacket(SerialPacketQueue<IOCP_PacketHeader>* receivedPacketSPQ_Address)
{
	IOCP_PacketHeader receivedPacketHeader;

	UINT result;

	//if(1 < InterlockedIncrement(&CS.IO_Count))
	{
		//if(CS.Connected && CS.Key == sessionKey)
		if(CS.Connected)
		{
			result = CS.ReceiveBQ.Peek((char*)&receivedPacketHeader, sizeof(receivedPacketHeader));
			if (sizeof(receivedPacketHeader) == result)
			{
				result = CS.ReceiveBQ.Peek(receivedPacketSPQ_Address->GetQueueAddress(), sizeof(receivedPacketHeader) + receivedPacketHeader.BodySize);
				if (sizeof(receivedPacketHeader) + receivedPacketHeader.BodySize == result)
				{
					receivedPacketSPQ_Address->SetHeader(receivedPacketHeader);
					receivedPacketSPQ_Address->IncreaseCurrentSize(receivedPacketHeader.BodySize);

					receivedPacketSPQ_Address->IncreaseReferenceCount();

					if (CS.ReceiveQ.Enqueue(receivedPacketSPQ_Address) == receivedPacketSPQ_Address)
					{
						CS.ReceiveBQ.DecreaseCurrentSize(receivedPacketHeader.BodySize);
						CS.ReceiveBQ.DecreaseCurrentSize(sizeof(receivedPacketHeader));
					}
					else
					{
						receivedPacketSPQ_Address->DecreaseReferenceCount();

						result = 0;
					}
				}
				else result = 0;
			}
			else result = 0;
		}
		//else result = 0;
	}
	//else result = 0;

	//InterlockedDecrement(&CS.IO_Count);

	return result;
}

unsigned int MMO_BaseClient::SendPacket(SerialPacketQueue<IOCP_PacketHeader>* sendingPacketSPQ_Address)
{
	IOCP_PacketHeader sendingPacketHeader;
	UINT sessionIndex;

	UINT result;

	if (1 < InterlockedIncrement(&CS.IO_Count))
	{
		//if (CS.Connected && CS.Key == sessionKey)
		if(CS.Connected)
		{
			result = sendingPacketSPQ_Address->GetHeaderSize();
			if (sizeof(sendingPacketHeader) == result)
			{
				sendingPacketHeader = sendingPacketSPQ_Address->GetHeader();

				result = sendingPacketSPQ_Address->GetTotalSize();
				if (sizeof(sendingPacketHeader) + sendingPacketHeader.BodySize == result)
				{
					sendingPacketSPQ_Address->IncreaseReferenceCount();

					if (CS.SendQ.Enqueue(sendingPacketSPQ_Address) == sendingPacketSPQ_Address);
					else
					{
						sendingPacketSPQ_Address->DecreaseReferenceCount();

						result = 0;
					}
				}
				else result = 0;
			}
			else result = 0;
		}
		else result = 0;

		{
			//if(!CS.ReceiveCount && !CS.SendCount)
			if (!InterlockedDecrement(&CS.IO_Count))
			{
				//if (InterlockedExchange(&CS.Connected, 0) && AWAN_CSV[sessionIndex].Key == sessionKey)
				if (InterlockedExchange(&CS.Connected, 0))
				{
					CS.Disconnecting = 1;

					//PostQueuedCompletionStatus(DISCONNECT_CP, NULL, CSV[sessionIndex].Key, &CSV[sessionIndex].OverlappedSend);
				}

				//PostDisconnect(AWAN_CSV[sessionIndex].Key);
				//Disconnect(AWAN_CSV[sessionIndex].Key);
			}
		}
	}
	else //result = 0;
	{
		InterlockedDecrement(&CS.IO_Count);

		result = 0;
	}

	return result;
}

SerialPacketQueue<IOCP_PacketHeader>* MMO_BaseClient::SPQ_TLP_Allocate()
{
	SerialPacketQueue<IOCP_PacketHeader>* serialPacketQueueAddress;

	serialPacketQueueAddress = BS_Address->SPQ_TLP_Allocate();

	return serialPacketQueueAddress;
}

unsigned int MMO_BaseClient::SPQ_TLP_Deallocate(SerialPacketQueue<IOCP_PacketHeader>* serialPacketQueueAddress)
{
	UINT result;

	result = BS_Address->SPQ_TLP_Deallocate(serialPacketQueueAddress);

	return result;
}