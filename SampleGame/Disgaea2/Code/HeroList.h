#pragma once
#include "gameNode.h"


struct tagHeroInList
{
	RECT rc; //���ĺ��̹��� ���� ��ǥ �����ϱ����� ���� ��Ʈ!
	int numbering; //���ɲ����� �ϴ� ������ �ѹ�������
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

