#include "pch.h"
#include "MMO.h"
#include "MMO_InnerClient.h"

MMO_Work MMO_InnerClient::ProcessWork()
{
	MMO_Work sendingWork;
	WORD clientRequestIndex;
	BYTE serverIndex;
	BYTE datumType;
	INT datumValue;
	INT timeStamp;

	sendingWork.SessionKey = ICP.SessionKey;
	serverIndex = ICP.Index;
	timeStamp = ICP.PreviousHeartbeatTime;

	{
		//sendingWork.SessionKey = ICP.SessionKey;
		InterlockedIncrement(&CURRENT_SPQ_COUNT);
		InterlockedIncrement(&ALLOCATED_SPQ_COUNT);
		sendingWork.PacketSPQ_Address = PACKET_SPQ_TLP.Allocate();

		clientRequestIndex = MONITOR_SERVER_REQUEST_UPDATE;
		//serverIndex = ICP.Index;
		datumType = MONITOR_UPDATE_TOTAL_SERVER_CPU_USAGE;
		datumValue = CPU_PROFILER.GetTotalProcessorUsage();
		//timeStamp = ICP.PreviousHeartbeatTime;

		*sendingWork.PacketSPQ_Address << clientRequestIndex;
		*sendingWork.PacketSPQ_Address << serverIndex;
		*sendingWork.PacketSPQ_Address << datumType;
		*sendingWork.PacketSPQ_Address << datumValue;
		*sendingWork.PacketSPQ_Address << timeStamp;

		if (sendingWork.PacketSPQ_Address->PackPacket()) Unicast(sendingWork);

		if (!sendingWork.PacketSPQ_Address->DecreaseReferenceCount())
		{
			InterlockedDecrement(&CURRENT_SPQ_COUNT);
			InterlockedIncrement(&DEALLOCATED_SPQ_COUNT);
			PACKET_SPQ_TLP.Deallocate(sendingWork.PacketSPQ_Address);
		}
	}

	{
		//sendingWork.SessionKey = ICP.SessionKey;
		InterlockedIncrement(&CURRENT_SPQ_COUNT);
		InterlockedIncrement(&ALLOCATED_SPQ_COUNT);
		sendingWork.PacketSPQ_Address = PACKET_SPQ_TLP.Allocate();

		clientRequestIndex = MONITOR_SERVER_REQUEST_UPDATE;
		//serverIndex = ICP.Index;
		datumType = MONITOR_UPDATE_TOTAL_SERVER_AVAILABLE_MEMORY;
		datumValue = PDH_PROFILER.GetCurrentAvailableMByte();
		//timeStamp = ICP.PreviousHeartbeatTime;

		*sendingWork.PacketSPQ_Address << clientRequestIndex;
		*sendingWork.PacketSPQ_Address << serverIndex;
		*sendingWork.PacketSPQ_Address << datumType;
		*sendingWork.PacketSPQ_Address << datumValue;
		*sendingWork.PacketSPQ_Address << timeStamp;

		if (sendingWork.PacketSPQ_Address->PackPacket()) Unicast(sendingWork);

		if (!sendingWork.PacketSPQ_Address->DecreaseReferenceCount())
		{
			InterlockedDecrement(&CURRENT_SPQ_COUNT);
			InterlockedIncrement(&DEALLOCATED_SPQ_COUNT);
			PACKET_SPQ_TLP.Deallocate(sendingWork.PacketSPQ_Address);
		}
	}

	{
		//sendingWork.SessionKey = ICP.SessionKey;
		InterlockedIncrement(&CURRENT_SPQ_COUNT);
		InterlockedIncrement(&ALLOCATED_SPQ_COUNT);
		sendingWork.PacketSPQ_Address = PACKET_SPQ_TLP.Allocate();

		clientRequestIndex = MONITOR_SERVER_REQUEST_UPDATE;
		//serverIndex = ICP.Index;
		datumType = MONITOR_UPDATE_TOTAL_SERVER_ETHERNET_RECEIVED_SIZE;
		datumValue = PDH_PROFILER.GetCurrentReceivedByte() / (UINT)1024;
		//timeStamp = ICP.PreviousHeartbeatTime;

		*sendingWork.PacketSPQ_Address << clientRequestIndex;
		*sendingWork.PacketSPQ_Address << serverIndex;
		*sendingWork.PacketSPQ_Address << datumType;
		*sendingWork.PacketSPQ_Address << datumValue;
		*sendingWork.PacketSPQ_Address << timeStamp;

		if (sendingWork.PacketSPQ_Address->PackPacket()) Unicast(sendingWork);

		if (!sendingWork.PacketSPQ_Address->DecreaseReferenceCount())
		{
			InterlockedDecrement(&CURRENT_SPQ_COUNT);
			InterlockedIncrement(&DEALLOCATED_SPQ_COUNT);
			PACKET_SPQ_TLP.Deallocate(sendingWork.PacketSPQ_Address);
		}
	}

	{
		//sendingWork.SessionKey = ICP.SessionKey;
		InterlockedIncrement(&CURRENT_SPQ_COUNT);
		InterlockedIncrement(&ALLOCATED_SPQ_COUNT);
		sendingWork.PacketSPQ_Address = PACKET_SPQ_TLP.Allocate();

		clientRequestIndex = MONITOR_SERVER_REQUEST_UPDATE;
		//serverIndex = ICP.Index;
		datumType = MONITOR_UPDATE_TOTAL_SERVER_ETHERNET_SENDED_SIZE;
		datumValue = PDH_PROFILER.GetCurrentSendedByte() / (UINT)1024;
		//timeStamp = ICP.PreviousHeartbeatTime;

		*sendingWork.PacketSPQ_Address << clientRequestIndex;
		*sendingWork.PacketSPQ_Address << serverIndex;
		*sendingWork.PacketSPQ_Address << datumType;
		*sendingWork.PacketSPQ_Address << datumValue;
		*sendingWork.PacketSPQ_Address << timeStamp;

		if (sendingWork.PacketSPQ_Address->PackPacket()) Unicast(sendingWork);

		if (!sendingWork.PacketSPQ_Address->DecreaseReferenceCount())
		{
			InterlockedDecrement(&CURRENT_SPQ_COUNT);
			InterlockedIncrement(&DEALLOCATED_SPQ_COUNT);
			PACKET_SPQ_TLP.Deallocate(sendingWork.PacketSPQ_Address);
		}
	}

	{
		//sendingWork.SessionKey = ICP.SessionKey;
		InterlockedIncrement(&CURRENT_SPQ_COUNT);
		InterlockedIncrement(&ALLOCATED_SPQ_COUNT);
		sendingWork.PacketSPQ_Address = PACKET_SPQ_TLP.Allocate();

		clientRequestIndex = MONITOR_SERVER_REQUEST_UPDATE;
		//serverIndex = ICP.Index;
		datumType = MONITOR_UPDATE_TOTAL_SERVER_NONPAGED_MEMORY;
		datumValue = PDH_PROFILER.GetCurrentPoolNonpagedByte() / (UINT)(1024 * 1024);
		//timeStamp = ICP.PreviousHeartbeatTime;

		*sendingWork.PacketSPQ_Address << clientRequestIndex;
		*sendingWork.PacketSPQ_Address << serverIndex;
		*sendingWork.PacketSPQ_Address << datumType;
		*sendingWork.PacketSPQ_Address << datumValue;
		*sendingWork.PacketSPQ_Address << timeStamp;

		if (sendingWork.PacketSPQ_Address->PackPacket()) Unicast(sendingWork);

		if (!sendingWork.PacketSPQ_Address->DecreaseReferenceCount())
		{
			InterlockedDecrement(&CURRENT_SPQ_COUNT);
			InterlockedIncrement(&DEALLOCATED_SPQ_COUNT);
			PACKET_SPQ_TLP.Deallocate(sendingWork.PacketSPQ_Address);
		}
	}



	{
		//sendingWork.SessionKey = ICP.SessionKey;
		InterlockedIncrement(&CURRENT_SPQ_COUNT);
		InterlockedIncrement(&ALLOCATED_SPQ_COUNT);
		sendingWork.PacketSPQ_Address = PACKET_SPQ_TLP.Allocate();

		clientRequestIndex = MONITOR_SERVER_REQUEST_UPDATE;
		//serverIndex = ICP.Index;
		datumType = MONITOR_UPDATE_BATTLE_SERVER_TOTAL_CPU_USAGE;
		datumValue = CPU_PROFILER.GetKernelProcessorUsage() + CPU_PROFILER.GetUserProcessorUsage(); //?
		//timeStamp = ICP.PreviousHeartbeatTime;

		*sendingWork.PacketSPQ_Address << clientRequestIndex;
		*sendingWork.PacketSPQ_Address << serverIndex;
		*sendingWork.PacketSPQ_Address << datumType;
		*sendingWork.PacketSPQ_Address << datumValue;
		*sendingWork.PacketSPQ_Address << timeStamp;

		if (sendingWork.PacketSPQ_Address->PackPacket()) Unicast(sendingWork);

		if (!sendingWork.PacketSPQ_Address->DecreaseReferenceCount())
		{
			InterlockedDecrement(&CURRENT_SPQ_COUNT);
			InterlockedIncrement(&DEALLOCATED_SPQ_COUNT);
			PACKET_SPQ_TLP.Deallocate(sendingWork.PacketSPQ_Address);
		}
	}

	{
		//sendingWork.SessionKey = ICP.SessionKey;
		InterlockedIncrement(&CURRENT_SPQ_COUNT);
		InterlockedIncrement(&ALLOCATED_SPQ_COUNT);
		sendingWork.PacketSPQ_Address = PACKET_SPQ_TLP.Allocate();

		clientRequestIndex = MONITOR_SERVER_REQUEST_UPDATE;
		//serverIndex = ICP.Index;
		datumType = MONITOR_UPDATE_BATTLE_SERVER_MEMORY_USER_COMMIT;
		datumValue = PDH_PROFILER.GetCurrentPrivateByte();
		//timeStamp = ICP.PreviousHeartbeatTime;

		*sendingWork.PacketSPQ_Address << clientRequestIndex;
		*sendingWork.PacketSPQ_Address << serverIndex;
		*sendingWork.PacketSPQ_Address << datumType;
		*sendingWork.PacketSPQ_Address << datumValue;
		*sendingWork.PacketSPQ_Address << timeStamp;

		if (sendingWork.PacketSPQ_Address->PackPacket()) Unicast(sendingWork);

		if (!sendingWork.PacketSPQ_Address->DecreaseReferenceCount())
		{
			InterlockedDecrement(&CURRENT_SPQ_COUNT);
			InterlockedIncrement(&DEALLOCATED_SPQ_COUNT);
			PACKET_SPQ_TLP.Deallocate(sendingWork.PacketSPQ_Address);
		}
	}

	{
		//sendingWork.SessionKey = ICP.SessionKey;
		InterlockedIncrement(&CURRENT_SPQ_COUNT);
		InterlockedIncrement(&ALLOCATED_SPQ_COUNT);
		sendingWork.PacketSPQ_Address = PACKET_SPQ_TLP.Allocate();

		clientRequestIndex = MONITOR_SERVER_REQUEST_UPDATE;
		//serverIndex = ICP.Index;
		datumType = MONITOR_UPDATE_BATTLE_SERVER_PACKET_POOL_USAGE;
		datumValue = 0;
		//timeStamp = ICP.PreviousHeartbeatTime;

		*sendingWork.PacketSPQ_Address << clientRequestIndex;
		*sendingWork.PacketSPQ_Address << serverIndex;
		*sendingWork.PacketSPQ_Address << datumType;
		*sendingWork.PacketSPQ_Address << datumValue;
		*sendingWork.PacketSPQ_Address << timeStamp;

		if (sendingWork.PacketSPQ_Address->PackPacket()) Unicast(sendingWork);

		if (!sendingWork.PacketSPQ_Address->DecreaseReferenceCount())
		{
			InterlockedDecrement(&CURRENT_SPQ_COUNT);
			InterlockedIncrement(&DEALLOCATED_SPQ_COUNT);
			PACKET_SPQ_TLP.Deallocate(sendingWork.PacketSPQ_Address);
		}
	}

	{
		//sendingWork.SessionKey = ICP.SessionKey;
		InterlockedIncrement(&CURRENT_SPQ_COUNT);
		InterlockedIncrement(&ALLOCATED_SPQ_COUNT);
		sendingWork.PacketSPQ_Address = PACKET_SPQ_TLP.Allocate();

		clientRequestIndex = MONITOR_SERVER_REQUEST_UPDATE;
		//serverIndex = ICP.Index;
		datumType = MONITOR_UPDATE_BATTLE_SERVER_CURRENT_AUTHORIZE_THREAD_LOOP_COUNT;
		datumValue = 0;
		//timeStamp = ICP.PreviousHeartbeatTime;

		*sendingWork.PacketSPQ_Address << clientRequestIndex;
		*sendingWork.PacketSPQ_Address << serverIndex;
		*sendingWork.PacketSPQ_Address << datumType;
		*sendingWork.PacketSPQ_Address << datumValue;
		*sendingWork.PacketSPQ_Address << timeStamp;

		if (sendingWork.PacketSPQ_Address->PackPacket()) Unicast(sendingWork);

		if (!sendingWork.PacketSPQ_Address->DecreaseReferenceCount())
		{
			InterlockedDecrement(&CURRENT_SPQ_COUNT);
			InterlockedIncrement(&DEALLOCATED_SPQ_COUNT);
			PACKET_SPQ_TLP.Deallocate(sendingWork.PacketSPQ_Address);
		}
	}

	{
		//sendingWork.SessionKey = ICP.SessionKey;
		InterlockedIncrement(&CURRENT_SPQ_COUNT);
		InterlockedIncrement(&ALLOCATED_SPQ_COUNT);
		sendingWork.PacketSPQ_Address = PACKET_SPQ_TLP.Allocate();

		clientRequestIndex = MONITOR_SERVER_REQUEST_UPDATE;
		//serverIndex = ICP.Index;
		datumType = MONITOR_UPDATE_BATTLE_SERVER_CURRENT_GAME_THREAD_LOOP_COUNT;
		datumValue = 0;
		//timeStamp = ICP.PreviousHeartbeatTime;

		*sendingWork.PacketSPQ_Address << clientRequestIndex;
		*sendingWork.PacketSPQ_Address << serverIndex;
		*sendingWork.PacketSPQ_Address << datumType;
		*sendingWork.PacketSPQ_Address << datumValue;
		*sendingWork.PacketSPQ_Address << timeStamp;

		if (sendingWork.PacketSPQ_Address->PackPacket()) Unicast(sendingWork);

		if (!sendingWork.PacketSPQ_Address->DecreaseReferenceCount())
		{
			InterlockedDecrement(&CURRENT_SPQ_COUNT);
			InterlockedIncrement(&DEALLOCATED_SPQ_COUNT);
			PACKET_SPQ_TLP.Deallocate(sendingWork.PacketSPQ_Address);
		}
	}

	{
		//sendingWork.SessionKey = ICP.SessionKey;
		InterlockedIncrement(&CURRENT_SPQ_COUNT);
		InterlockedIncrement(&ALLOCATED_SPQ_COUNT);
		sendingWork.PacketSPQ_Address = PACKET_SPQ_TLP.Allocate();

		clientRequestIndex = MONITOR_SERVER_REQUEST_UPDATE;
		//serverIndex = ICP.Index;
		datumType = MONITOR_UPDATE_BATTLE_SERVER_TOTAL_SESSION_COUNT;
		datumValue = 0;
		//timeStamp = ICP.PreviousHeartbeatTime;

		*sendingWork.PacketSPQ_Address << clientRequestIndex;
		*sendingWork.PacketSPQ_Address << serverIndex;
		*sendingWork.PacketSPQ_Address << datumType;
		*sendingWork.PacketSPQ_Address << datumValue;
		*sendingWork.PacketSPQ_Address << timeStamp;

		if (sendingWork.PacketSPQ_Address->PackPacket()) Unicast(sendingWork);

		if (!sendingWork.PacketSPQ_Address->DecreaseReferenceCount())
		{
			InterlockedDecrement(&CURRENT_SPQ_COUNT);
			InterlockedIncrement(&DEALLOCATED_SPQ_COUNT);
			PACKET_SPQ_TLP.Deallocate(sendingWork.PacketSPQ_Address);
		}
	}

	{
		//sendingWork.SessionKey = ICP.SessionKey;
		InterlockedIncrement(&CURRENT_SPQ_COUNT);
		InterlockedIncrement(&ALLOCATED_SPQ_COUNT);
		sendingWork.PacketSPQ_Address = PACKET_SPQ_TLP.Allocate();

		clientRequestIndex = MONITOR_SERVER_REQUEST_UPDATE;
		//serverIndex = ICP.Index;
		datumType = MONITOR_UPDATE_BATTLE_SERVER_AUTHORIZING_SESSION_COUNT;
		datumValue = 0;
		//timeStamp = ICP.PreviousHeartbeatTime;

		*sendingWork.PacketSPQ_Address << clientRequestIndex;
		*sendingWork.PacketSPQ_Address << serverIndex;
		*sendingWork.PacketSPQ_Address << datumType;
		*sendingWork.PacketSPQ_Address << datumValue;
		*sendingWork.PacketSPQ_Address << timeStamp;

		if (sendingWork.PacketSPQ_Address->PackPacket()) Unicast(sendingWork);

		if (!sendingWork.PacketSPQ_Address->DecreaseReferenceCount())
		{
			InterlockedDecrement(&CURRENT_SPQ_COUNT);
			InterlockedIncrement(&DEALLOCATED_SPQ_COUNT);
			PACKET_SPQ_TLP.Deallocate(sendingWork.PacketSPQ_Address);
		}
	}

	{
		//sendingWork.SessionKey = ICP.SessionKey;
		InterlockedIncrement(&CURRENT_SPQ_COUNT);
		InterlockedIncrement(&ALLOCATED_SPQ_COUNT);
		sendingWork.PacketSPQ_Address = PACKET_SPQ_TLP.Allocate();

		clientRequestIndex = MONITOR_SERVER_REQUEST_UPDATE;
		//serverIndex = ICP.Index;
		datumType = MONITOR_UPDATE_BATTLE_SERVER_GAMING_SESSION_COUNT;
		datumValue = 0;
		//timeStamp = ICP.PreviousHeartbeatTime;

		*sendingWork.PacketSPQ_Address << clientRequestIndex;
		*sendingWork.PacketSPQ_Address << serverIndex;
		*sendingWork.PacketSPQ_Address << datumType;
		*sendingWork.PacketSPQ_Address << datumValue;
		*sendingWork.PacketSPQ_Address << timeStamp;

		if (sendingWork.PacketSPQ_Address->PackPacket()) Unicast(sendingWork);

		if (!sendingWork.PacketSPQ_Address->DecreaseReferenceCount())
		{
			InterlockedDecrement(&CURRENT_SPQ_COUNT);
			InterlockedIncrement(&DEALLOCATED_SPQ_COUNT);
			PACKET_SPQ_TLP.Deallocate(sendingWork.PacketSPQ_Address);
		}
	}

	{
		//sendingWork.SessionKey = ICP.SessionKey;
		InterlockedIncrement(&CURRENT_SPQ_COUNT);
		InterlockedIncrement(&ALLOCATED_SPQ_COUNT);
		sendingWork.PacketSPQ_Address = PACKET_SPQ_TLP.Allocate();

		clientRequestIndex = MONITOR_SERVER_REQUEST_UPDATE;
		//serverIndex = ICP.Index;
		datumType = MONITOR_UPDATE_BATTLE_SERVER_PLAYER_COUNT;
		datumValue = 0;
		//timeStamp = ICP.PreviousHeartbeatTime;

		*sendingWork.PacketSPQ_Address << clientRequestIndex;
		*sendingWork.PacketSPQ_Address << serverIndex;
		*sendingWork.PacketSPQ_Address << datumType;
		*sendingWork.PacketSPQ_Address << datumValue;
		*sendingWork.PacketSPQ_Address << timeStamp;

		if (sendingWork.PacketSPQ_Address->PackPacket()) Unicast(sendingWork);

		if (!sendingWork.PacketSPQ_Address->DecreaseReferenceCount())
		{
			InterlockedDecrement(&CURRENT_SPQ_COUNT);
			InterlockedIncrement(&DEALLOCATED_SPQ_COUNT);
			PACKET_SPQ_TLP.Deallocate(sendingWork.PacketSPQ_Address);
		}
	}

	{
		//sendingWork.SessionKey = ICP.SessionKey;
		InterlockedIncrement(&CURRENT_SPQ_COUNT);
		InterlockedIncrement(&ALLOCATED_SPQ_COUNT);
		sendingWork.PacketSPQ_Address = PACKET_SPQ_TLP.Allocate();

		clientRequestIndex = MONITOR_SERVER_REQUEST_UPDATE;
		//serverIndex = ICP.Index;
		datumType = MONITOR_UPDATE_BATTLE_SERVER_WAITING_ROOM_COUNT;
		datumValue = 0;
		//timeStamp = ICP.PreviousHeartbeatTime;

		*sendingWork.PacketSPQ_Address << clientRequestIndex;
		*sendingWork.PacketSPQ_Address << serverIndex;
		*sendingWork.PacketSPQ_Address << datumType;
		*sendingWork.PacketSPQ_Address << datumValue;
		*sendingWork.PacketSPQ_Address << timeStamp;

		if (sendingWork.PacketSPQ_Address->PackPacket()) Unicast(sendingWork);

		if (!sendingWork.PacketSPQ_Address->DecreaseReferenceCount())
		{
			InterlockedDecrement(&CURRENT_SPQ_COUNT);
			InterlockedIncrement(&DEALLOCATED_SPQ_COUNT);
			PACKET_SPQ_TLP.Deallocate(sendingWork.PacketSPQ_Address);
		}
	}

	{
		//sendingWork.SessionKey = ICP.SessionKey;
		InterlockedIncrement(&CURRENT_SPQ_COUNT);
		InterlockedIncrement(&ALLOCATED_SPQ_COUNT);
		sendingWork.PacketSPQ_Address = PACKET_SPQ_TLP.Allocate();

		clientRequestIndex = MONITOR_SERVER_REQUEST_UPDATE;
		//serverIndex = ICP.Index;
		datumType = MONITOR_UPDATE_BATTLE_SERVER_PLAYING_ROOM_COUNT;
		datumValue = 0;
		//timeStamp = ICP.PreviousHeartbeatTime;

		*sendingWork.PacketSPQ_Address << clientRequestIndex;
		*sendingWork.PacketSPQ_Address << serverIndex;
		*sendingWork.PacketSPQ_Address << datumType;
		*sendingWork.PacketSPQ_Address << datumValue;
		*sendingWork.PacketSPQ_Address << timeStamp;

		if (sendingWork.PacketSPQ_Address->PackPacket()) Unicast(sendingWork);

		if (!sendingWork.PacketSPQ_Address->DecreaseReferenceCount())
		{
			InterlockedDecrement(&CURRENT_SPQ_COUNT);
			InterlockedIncrement(&DEALLOCATED_SPQ_COUNT);
			PACKET_SPQ_TLP.Deallocate(sendingWork.PacketSPQ_Address);
		}
	}

	ICP.PreviousHeartbeatTime = time(nullptr);

	return sendingWork;
}

unsigned int MMO_InnerClient::Broadcast(MMO_Work sendingWork)
{
	if (SendPacket(ICP.SessionKey, sendingWork.PacketSPQ_Address)) PostSend(ICP.SessionKey);

	return 0;
}

unsigned int MMO_InnerClient::Multicast(MMO_Work sendingWork)
{
	if (SendPacket(ICP.SessionKey, sendingWork.PacketSPQ_Address)) PostSend(ICP.SessionKey);

	return 0;
}

unsigned int MMO_InnerClient::Unicast(MMO_Work sendingWork)
{
	if (SendPacket(ICP.SessionKey, sendingWork.PacketSPQ_Address)) PostSend(ICP.SessionKey);

	return 0;
}

unsigned int MMO_InnerClient::Singlecast(MMO_Work sendingWork)
{
	if (SendPacket(ICP.SessionKey, sendingWork.PacketSPQ_Address)) PostSend(ICP.SessionKey);

	return 0;
}
