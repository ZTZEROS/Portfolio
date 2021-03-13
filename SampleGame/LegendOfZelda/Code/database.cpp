#include "stdafx.h"
#include "database.h"


database::database()
{
}


database::~database()
{
}

HRESULT database::init(void)
{
//	loadDatabase("database1.txt");

	return S_OK;
}

void database::release(void)
{

}
/*
void database::loadDatabase(string name)
{
	arrElements vTemp;
	vTemp = TXTDATA->txtLoad(name.c_str());

	string str;

	int count = 0;

	for (int i = 0; i < vTemp.size(); i++)
	{
		if (vTemp[i] == "|")
		{
			elements* em = new elements;
			str = vTemp[i + 1];
			_mTotalElement.insert(pair<string, elements*>(vTemp[i + 1], em));
			if (i != 0) count += 6;
			continue;
		}

		iterElement mIter = _mTotalElement.find(str);
		if (i == count + 1)mIter->second->name = vTemp[i].c_str();
		else if (i == count + 2) mIter->second->attack = (int)atoi(vTemp[i].c_str());
		else if (i == count + 3) mIter->second->defense = (int)atoi(vTemp[i].c_str());
		else if (i == count + 4) mIter->second->speed = (int)atoi(vTemp[i].c_str());
		else if (i == count + 5) mIter->second->HP = (int)atoi(vTemp[i].c_str());
	}
	vTemp.clear();
}


void database::setElementDataAtk(string str, int atk)
{
	iterElement mIter = _mTotalElement.find(str);
	mIter->second->attack = atk;
}

void database::setElementDataDef(string str, int def)
{
	iterElement mIter = _mTotalElement.find(str);
	mIter->second->defense = def;
}

void database::setElementDataSpeed(string str, int spd)
{
	iterElement mIter = _mTotalElement.find(str);
	mIter->second->speed = spd;
}

void database::setElementDataHP(string str, int hp)
{
	iterElement mIter = _mTotalElement.find(str);
	mIter->second->HP = hp;
}*/