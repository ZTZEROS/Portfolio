#pragma once

enum HTTP_LOCAL_CONSTANT
{
	HTTP_SERVER_PORT = 80
};

struct HTTP_Server : public CommonSession
{

};

struct HTTP_Client : public CommonSession
{

};

#pragma pack(push, 1)

struct HTTP_PacketHeader
{

};

struct HTTP_PacketBody
{

};

struct HTTP_PacketTrailer
{

};

struct HTTP_Packet
{
	HTTP_PacketHeader Header;
	HTTP_PacketBody Body;
	HTTP_PacketTrailer Trailer;
};

#pragma pack(pop)