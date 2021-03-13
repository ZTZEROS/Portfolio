#include "stdafx.h"
#include "Monitor.h"
#include "MonitorInnerServer.h"

InnerWork MonitorInnerServer::ProcessWork(InnerClient* innerClientAddress, InnerWork receivedWork)
{
	InnerWork sendingWork;
	WORD clientRequestIndex;
	WORD serverResponseIndex;

	sendingWork.SessionKey = receivedWork.SessionKey;
	InterlockedIncrement(&CURRENT_SPQ_COUNT);
	InterlockedIncrement(&ALLOCATED_SPQ_COUNT);
	sendingWork.PacketSPQ_Address = PACKET_SPQ_TLP.Allocate();

	clientRequestIndex = 0xffff;
	serverResponseIndex = 0xffff;

	if (UnpackPacketSPQ(receivedWork.PacketSPQ_Address)) *receivedWork.PacketSPQ_Address >> clientRequestIndex;
	else clientRequestIndex = 0xffff;

	switch (clientRequestIndex)
	{
	case MONITOR_SERVER_REQUEST_LOGIN:
		{
			{
				INT serverIndex;

				*receivedWork.PacketSPQ_Address >> serverIndex;



				innerClientAddress->Index = serverIndex;

				serverResponseIndex;
			}

			{

			}

			//if (PackPacketSPQ(sendingWork.PacketSPQ_Address)) Unicast(sendingWork);
		}
		break;
	case MONITOR_SERVER_REQUEST_UPDATE:
		{
			{
				BYTE serverIndex;
				BYTE datumType;
				INT datumValue;
				INT timeStamp;

				*receivedWork.PacketSPQ_Address >> serverIndex;
				*receivedWork.PacketSPQ_Address >> datumType;
				*receivedWork.PacketSPQ_Address >> datumValue;
				*receivedWork.PacketSPQ_Address >> timeStamp;



				//innerClientAddress->Index = serverIndex;
				innerClientAddress->datumType = datumType;
				innerClientAddress->datumValue = datumValue;
				innerClientAddress->timeStamp = timeStamp;

				serverResponseIndex = MONITOR_SERVER_RESPONSE_CLIENT_UPDATE;
			}

			{
				BYTE serverIndex;
				BYTE datumType;
				INT datumValue;
				INT timeStamp;

				serverIndex = innerClientAddress->Index;
				datumType = innerClientAddress->datumType;
				datumValue = innerClientAddress->datumValue;
				timeStamp = innerClientAddress->timeStamp;

				*sendingWork.PacketSPQ_Address << serverIndex;
				*sendingWork.PacketSPQ_Address << datumType;
				*sendingWork.PacketSPQ_Address << datumValue;
				*sendingWork.PacketSPQ_Address << timeStamp;
			}

			//if (PackPacketSPQ(sendingWork.PacketSPQ_Address)) Unicast(sendingWork);
		}
		break;
	default:
		{
			serverResponseIndex = 0xffff;
		}
		break;
	}

	if (!receivedWork.PacketSPQ_Address->DecreaseReferenceCount())
	{
		InterlockedDecrement(&CURRENT_SPQ_COUNT);
		InterlockedIncrement(&DEALLOCATED_SPQ_COUNT);
		PACKET_SPQ_TLP.Deallocate(receivedWork.PacketSPQ_Address);
	}

	if (!sendingWork.PacketSPQ_Address->DecreaseReferenceCount())
	{
		InterlockedDecrement(&CURRENT_SPQ_COUNT);
		InterlockedIncrement(&DEALLOCATED_SPQ_COUNT);
		PACKET_SPQ_TLP.Deallocate(sendingWork.PacketSPQ_Address);
	}

	return sendingWork;
}

unsigned int MonitorInnerServer::UnpackPacketSPQ(SerialPacketQueue<IOCP_PacketHeader>* packetSPQ_Address)
{
	IOCP_PacketHeader header;
	CHAR* frontAddress;
	BYTE checksum;

	UINT result;

	header = packetSPQ_Address->GetHeader();
	frontAddress = packetSPQ_Address->GetFrontAddress();
	checksum = 0;

	if (header.IdentificationCode == 0x77)
	{
		if (header.BodySize == packetSPQ_Address->GetCurrentSize())
		{
			header.Checksum = header.Checksum ^ header.CryptoCode;

			for (unsigned int i = 0; i < header.BodySize; ++i)
			{
				frontAddress[i] = frontAddress[i] ^ header.CryptoCode;
			}

			for (unsigned int i = 0; i < header.BodySize; ++i)
			{
				checksum += frontAddress[i];
			}

			if (header.Checksum == checksum) result = header.BodySize;
			else result = 0;
		}
		else result = 0;
	}
	else result = 0;

	return result;
}

unsigned int MonitorInnerServer::PackPacketSPQ(SerialPacketQueue<IOCP_PacketHeader>* packetSPQ_Address)
{
	IOCP_PacketHeader header;
	CHAR* frontAddress;
	BYTE checksum;

	UINT result;

	header.IdentificationCode = 0x77;
	header.BodySize = packetSPQ_Address->GetBodySize();
	header.CryptoCode = 0x27;
	header.Checksum = 0;

	frontAddress = packetSPQ_Address->GetFrontAddress();

	for (unsigned int i = 0; i < header.BodySize; ++i)
	{
		header.Checksum += frontAddress[i];
	}

	header.Checksum = header.Checksum ^ header.CryptoCode;

	for (unsigned int i = 0; i < header.BodySize; ++i)
	{
		frontAddress[i] = frontAddress[i] ^ header.CryptoCode;
	}

	packetSPQ_Address->SetHeader(header);

	result = packetSPQ_Address->GetTotalSize(); //sizeof(header) + header.BodySize;

	return result;
}

unsigned int MonitorInnerServer::Broadcast(InnerWork sendingWork)
{
	for (InnerClientAddressLI icaLI = ICAL.begin(); icaLI != ICAL.end(); ++icaLI)
	{
		if (SendPacket((*icaLI)->SessionKey, sendingWork.PacketSPQ_Address)) PostSend((*icaLI)->SessionKey);
	}

	return 0;
}

unsigned int MonitorInnerServer::Multicast(InnerWork sendingWork)
{
	for (InnerClientAddressLI icaLI = ICAL.begin(); icaLI != ICAL.end(); ++icaLI)
	{
		if (SendPacket((*icaLI)->SessionKey, sendingWork.PacketSPQ_Address)) PostSend((*icaLI)->SessionKey);
	}

	return 0;
}

unsigned int MonitorInnerServer::Unicast(InnerWork sendingWork)
{
	if (SendPacket(sendingWork.SessionKey, sendingWork.PacketSPQ_Address)) PostSend(sendingWork.SessionKey);

	return 0;
}

unsigned int MonitorInnerServer::Singlecast(InnerWork sendingWork)
{
	if (SendPacket(sendingWork.SessionKey, sendingWork.PacketSPQ_Address)) PostSend(sendingWork.SessionKey);

	return 0;
}