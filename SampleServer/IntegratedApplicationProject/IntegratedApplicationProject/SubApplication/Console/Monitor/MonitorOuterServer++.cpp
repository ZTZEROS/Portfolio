#include "stdafx.h"
#include "Monitor.h"
#include "MonitorOuterServer.h"

MonitorWork MonitorOuterServer::ProcessWork(MonitorClient* monitorClientAddress, MonitorWork receivedWork)
{
	MonitorWork sendingWork;
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

				{
					//BYTE serverIndex;
					//BYTE datumType;
					//INT datumValue;
					//INT timeStamp;

					*receivedWork.PacketSPQ_Address >> serverIndex;
					*receivedWork.PacketSPQ_Address >> datumType;
					*receivedWork.PacketSPQ_Address >> datumValue;
					*receivedWork.PacketSPQ_Address >> timeStamp;

					serverResponseIndex = MONITOR_SERVER_RESPONSE_CLIENT_UPDATE;
				}

				{
					//BYTE serverIndex;
					//BYTE datumType;
					//INT datumValue;
					//INT timeStamp;

					*sendingWork.PacketSPQ_Address << serverResponseIndex;
					*sendingWork.PacketSPQ_Address << serverIndex;
					*sendingWork.PacketSPQ_Address << datumType;
					*sendingWork.PacketSPQ_Address << datumValue;
					*sendingWork.PacketSPQ_Address << timeStamp;
				}
			}

			if (PackPacketSPQ(sendingWork.PacketSPQ_Address)) Broadcast(sendingWork);
		}
		break;
	case MONITOR_CLIENT_REQUEST_LOGIN:
		{
			{
				CHAR monitorClientKey[32];

				FOR(i, sizeof(monitorClientKey) / sizeof(CHAR))
				{
					*receivedWork.PacketSPQ_Address >> monitorClientAddress->MonitorClientKey[i];
				}
				
				serverResponseIndex = MONITOR_SERVER_RESPONSE_CLIENT_LOGIN;
			}

			{
				string loginKey;
				BYTE status;

				loginKey.assign(LoginKey.begin(), LoginKey.end());

				status = 1;

				FOR(i, sizeof(monitorClientAddress->MonitorClientKey) / sizeof(CHAR))
				{
					if (monitorClientAddress->MonitorClientKey[i] != loginKey[i])
					{
						status = 0;

						Disconnect(monitorClientAddress->SessionKey);

						break;
					}
				}

				*sendingWork.PacketSPQ_Address << serverResponseIndex;
				*sendingWork.PacketSPQ_Address << status;
			}

			if (PackPacketSPQ(sendingWork.PacketSPQ_Address)) Unicast(sendingWork);
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

unsigned int MonitorOuterServer::UnpackPacketSPQ(SerialPacketQueue<IOCP_PacketHeader>* packetSPQ_Address)
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

unsigned int MonitorOuterServer::PackPacketSPQ(SerialPacketQueue<IOCP_PacketHeader>* packetSPQ_Address)
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

unsigned int MonitorOuterServer::Broadcast(MonitorWork sendingWork)
{
	for (MonitorClientAddressLI mcaLI = MCAL.begin(); mcaLI != MCAL.end(); ++mcaLI)
	{
		if (SendPacket((*mcaLI)->SessionKey, sendingWork.PacketSPQ_Address)) PostSend((*mcaLI)->SessionKey);
	}

	return 0;
}

unsigned int MonitorOuterServer::Multicast(MonitorWork sendingWork)
{
	for (MonitorClientAddressLI mcaLI = MCAL.begin(); mcaLI != MCAL.end(); ++mcaLI)
	{
		if (SendPacket((*mcaLI)->SessionKey, sendingWork.PacketSPQ_Address)) PostSend((*mcaLI)->SessionKey);
	}

	return 0;
}

unsigned int MonitorOuterServer::Unicast(MonitorWork sendingWork)
{
	if (SendPacket(sendingWork.SessionKey, sendingWork.PacketSPQ_Address)) PostSend(sendingWork.SessionKey);

	return 0;
}

unsigned int MonitorOuterServer::Singlecast(MonitorWork sendingWork)
{
	if (SendPacket(sendingWork.SessionKey, sendingWork.PacketSPQ_Address)) PostSend(sendingWork.SessionKey);

	return 0;
}