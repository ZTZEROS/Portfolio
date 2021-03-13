#include "stdafx.h"
#include "../stdafx.h"
#include "boss.h"

boss::boss()
{
}


boss::~boss()
{
}

HRESULT boss::init(void)
{

	return S_OK;
}

void boss::release(void)
{

}

//void boss::update(void)
//{
//	move();
//}

void boss::move(void)
{
	_moveCount++;
}