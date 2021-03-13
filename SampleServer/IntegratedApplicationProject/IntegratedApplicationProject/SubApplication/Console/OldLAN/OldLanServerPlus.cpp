#include "stdafx.h"
#include "LanServer.h"

bool InitializeLanServer()
{
	if(DERIVED_LAN_SERVER.Initialize()) return 1;

	return 0;
}

bool UpdateLanServer()
{
	if(DERIVED_LAN_SERVER.Update()) return 1;

	return 0;
}

bool RenderLanServer()
{
	//if(DERIVED_LAN_SERVER.Render()) return 1;

	return 0;
}

bool TerminalizeLanServer()
{
	if(DERIVED_LAN_SERVER.Terminalize()) return 1;

	return 0;
}