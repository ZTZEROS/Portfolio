#pragma once
#include "singletonBase.h"
#include <vector>
#include <map>

class elements
{
public:
/*	string name;
	int attack;
	int defense;
	float speed;
	int HP;
*/
	elements() {};
	~elements() {};
};
class database : public singletonBase<database>
{
private:
	typedef vector<string> arrElements;
	typedef vector<string>::iterator iterElements;
	typedef map<string, elements*> arrElement;
	typedef map<string, elements*>::iterator iterElement;

private:
	arrElement _mTotalElement;
public:
	HRESULT init(void);
	void release(void);

	//void loadDatabase(string name);

	elements* getElementData(string str) { return _mTotalElement.find(str)->second; }
	/*
	void setElementDataAtk(string str, int atk);
	void setElementDataDef(string str, int def);
	void setElementDataSpeed(string str, int spd);
	void setElementDataHP(string str, int hp);
	*/
	database();
	~database();
};

