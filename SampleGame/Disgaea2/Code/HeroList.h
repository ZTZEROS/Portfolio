#pragma once
#include "gameNode.h"


struct tagHeroInList
{
	RECT rc; //알파벳이미지 렌더 좌표 쉽게하기위해 만든 렉트!
	int numbering; //사용될꺼같아 일단 만들어둔 넘버링변수
	bool Out;
};

class HeroList : public gameNode
{
private:
	vector<tagHeroInList> _vHeroList;
	image* _heroListCursor;
	RECT _rcHand;
private:
	int _numberOfHeroes;
	int _cursorPositionY;
	int _cursorPlace;
	int _selectedOne;
public:
	HRESULT init(int numberOfHeroes, int x, int y);
	void release();
	void update();
	void render(int x, int y);
	void removeList(int index, int x, int y);
public:
	vector<tagHeroInList> getVHeroList() { return _vHeroList; }
	int getNumberOfHeroes() { return _numberOfHeroes; }
	int getCursorPlace() { return _cursorPlace; }
	int getSelectedOne() { return _selectedOne;}
	void SetOut(int index, bool out) { _vHeroList[index].Out = out; }
	HeroList();
	~HeroList();
};

