#include "pch.h"
#include "MMO.h"
#include "MMO_BaseClient.h"
#include "MMO_DerivedClient.h"

unsigned int MMO_DerivedClient::OnAT_ClientJoin()
{


	return 0;
}

unsigned int MMO_DerivedClient::OnAT_ClientReceive()
{
	SerialPacketQueue<IOCP_PacketHeader>* receivedPacketSPQ_Address;
	SerialPacketQueue<IOCP_PacketHeader>* sendingPacketSPQ_Address;
	WORD clientRequestIndex;
	WORD serverResponseIndex;
	
	clientRequestIndex = 0xffff;
	serverResponseIndex = 0xffff;

	if (CS.ReceiveQ.GetCurrentCount())
	{
		receivedPacketSPQ_Address = CS.ReceiveQ.Dequeue();
		sendingPacketSPQ_Address = SPQ_TLP_Allocate();

		if (receivedPacketSPQ_Address->UnpackPacket()) *receivedPacketSPQ_Address >> clientRequestIndex;
		else clientRequestIndex = 0xffff;

		switch (clientRequestIndex)
		{
		case MMO_CLIENT_REQUEST_AUTHORIZE_LOGIN:
			{
				{
					INT64 accountKey;
					CHAR playerKey[64];
					INT version;

					*receivedPacketSPQ_Address >> accountKey;

					FOR(i, sizeof(playerKey) / sizeof(CHAR))
					{
						*receivedPacketSPQ_Address >> playerKey[i];
					}

					*receivedPacketSPQ_Address >> version;



					AccountKey = accountKey;

					FOR(i, sizeof(PlayerKey) / sizeof(CHAR))
					{
						PlayerKey[i] = playerKey[i];
					}

					Version = version;

					serverResponseIndex = MMO_CLIENT_RESPONSE_AUTHORIZE_LOGIN;
				}

				{
					BYTE status;
					INT64 accountKey;

					status = MMO_SERVER_LOGIN_STATUS_SUCCESS;

					accountKey = AccountKey;

					*sendingPacketSPQ_Address << serverResponseIndex;
					*sendingPacketSPQ_Address << status;
					*sendingPacketSPQ_Address << accountKey;

					sendingPacketSPQ_Address->PackPacket();

					if (SendPacket(sendingPacketSPQ_Address))
					{
						//PostSend();

						CS.FromAuthorizeToGame = 1;
					}
					else CS.Disconnecting = 1;
				}
			}
			break;
		default:
			{

			}
			break;
		}

		SPQ_TLP_Deallocate(receivedPacketSPQ_Address);
		SPQ_TLP_Deallocate(sendingPacketSPQ_Address);
	}

	return 0;
}

unsigned int MMO_DerivedClient::OnAT_ClientLeave()
{


	return 0;
}

unsigned int MMO_DerivedClient::OnGT_ClientJoin()
{


	return 0;
}

unsigned int MMO_DerivedClient::OnGT_ClientReceive()
{
	SerialPacketQueue<IOCP_PacketHeader>* receivedPacketSPQ_Address;
	SerialPacketQueue<IOCP_PacketHeader>* sendingPacketSPQ_Address;
	WORD clientRequestIndex;
	WORD serverResponseIndex;

	FOR(i, MMO_SERVER_CLIENT_RECEIVE_LOOP_COUNT)
	{
		receivedPacketSPQ_Address = CS.ReceiveQ.Dequeue();

		if (receivedPacketSPQ_Address)
		{
			if (receivedPacketSPQ_Address->UnpackPacket()) *receivedPacketSPQ_Address >> clientRequestIndex;
			else clientRequestIndex = 0xffff;

			switch (clientRequestIndex)
			{
			case MMO_CLIENT_REQUEST_GAME_ECHO:
				{
					{
						INT64 accountKey;
						LONGLONG sendTick;

						*receivedPacketSPQ_Address >> accountKey;
						*receivedPacketSPQ_Address >> sendTick;

						AccountKey = accountKey;
						CurrentTick = sendTick;
					
						serverResponseIndex = MMO_CLIENT_RESPONSE_GAME_ECHO;

						sendingPacketSPQ_Address = receivedPacketSPQ_Address;
					}
					
					{
						INT64 accountKey;
						LONGLONG sendTick;

						sendingPacketSPQ_Address->Initialize();
					
						accountKey = AccountKey;
						sendTick = CurrentTick;

						*sendingPacketSPQ_Address << serverResponseIndex;
						*sendingPacketSPQ_Address << accountKey;
						*sendingPacketSPQ_Address << sendTick;

						sendingPacketSPQ_Address->PackPacket();

						if (SendPacket(sendingPacketSPQ_Address))
						{
							//PostSend();
						}
						else CS.Disconnecting = 1;

						SPQ_TLP_Deallocate(sendingPacketSPQ_Address);
					}
				}
				break;
			case MMO_CLIENT_REQUEST_GAME_HEARTBEAT:
				{

				}
				break;
			default:
				{

				}
				break;
			}
		}
		else break;
	}

	return 0;
}

unsigned int MMO_DerivedClient::OnGT_ClientLeave()
{
	return 0;
}

unsigned int MMO_DerivedClient::OnGT_ClientDisconnect()
{
	return 0;
}
