#pragma once
#include "gameNode.h"

struct tagStat
{
	string sName;
	string sClass;
	int lv;
	int	hp;
	int	sp;
	int	currentHp;
	int	currentSp;
	int	atk;
	int	def;
	int	inte;
	int	res;
	int	hit;
	int	spd;
	int exp;
	int next;
};

enum TOWN_CHAR
{ 
	CHAR_PRIER,
	CHAR_FENRICH,
	CHAR_AXEL,
	//CHAR_ADELL,
	CHAR_END
};

class UIstat : public gameNode
{
private:
	image* ui;
	image* charFace;
	tagStat charStat;
	tagStat charsStat[CHAR_END];
	char str[128];

public:
	UIstat();
	~UIstat();

	HRESULT init();
	void release(void);
	void update(void);
	void render(int x, int y);

	void setCharStat(tagStat stat) { charStat = stat; }
	void setCharStatIndex(int num) { charStat = charsStat[num]; }

	void setCurrectHP(int num, int currentHp){ charsStat[num].currentHp = currentHp; }
	void setCurrectSP(int num, int currentSp){ charsStat[num].currentSp = currentSp; }



	tagStat getCharStatIndex(int num)	{ return charsStat[num]; }

	void statSave();
	void statLoad();
};

