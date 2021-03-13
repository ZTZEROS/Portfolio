#include "pch.h"
#include "MMO.h"
#include "MMO_BaseClient.h"
#include "MMO_DerivedClient.h"

MMO_DerivedClient::MMO_DerivedClient()
{

}

MMO_DerivedClient::~MMO_DerivedClient()
{

}

bool MMO_DerivedClient::Initialize()
{
	MMO_BaseClient::Initialize();

	return 0;
}

bool MMO_DerivedClient::Update()
{
	return 0;
}

bool MMO_DerivedClient::Render()
{
	return 0;
}

bool MMO_DerivedClient::Terminalize()
{
	MMO_DerivedClient::Terminalize();

	return 0;
}