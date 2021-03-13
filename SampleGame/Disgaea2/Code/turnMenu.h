#pragma once
#include "gameNode.h"

//�ϸ޴����� � �޴����� �˷��� �̳�!
enum TURN_MENUS
{
	TURN_NONE,
	EXECUTE,
	END_TURN,
	CHARACTER_LIST,
	BONUS,
	HELP,
	GIVEUP,
	SETTING
};
//� �޴��� ���������� ������ �̳�!
enum MENUS_IN_TURNMENU
{
	RENDER_TURNMENU,
	RENDER_CHARACTER_LIST
};
//ĳ���͸���Ʈ���� �̸��� ��ĭ�� ����ü��!
struct tagCharacterInList
{
	RECT rc; //���ĺ��̹��� ���� ��ǥ �����ϱ����� ���� ��Ʈ!
	int numbering; //���ɲ����� �ϴ� ������ �ѹ�������
};
class turnMenu : public gameNode
{
private:
	vector<tagCharacterInList> _vCharacterList;
private:
	image* _turnMenuCursorImage;
	MENUS_IN_TURNMENU _menusInTurnMenu;
	int _turnMenus; //�ϸ޴��� �Ҹ޴��� �̳ѿ� 
	int _numberOfCharacter; //ĳ���� �� �޾ƿ� ����
	int _cursorPositionY; //�ϸ޴������� �հ���Ŀ��Y��
	int _charChoicePositionY; //ĳ���͸���Ʈ������ �հ���Ŀ��Y��
	int _handCursorPlace; // ĳ���͸���Ʈ���� �հ���Ŀ�� ���� ������ ����
public:
	HRESULT init();
	void release();
	void update(int numberOfCharacters, int x, int y);
	void render(int x, int y);
public:
	//���� ����
	vector<tagCharacterInList> getCharacterList(void) { return _vCharacterList; }
	int getTurnMenus(void) { return _turnMenus; }
	int getCursorPlace() { return _handCursorPlace; }
	void setTurnMenus(int turnMenu) { _turnMenus = turnMenu; }
	MENUS_IN_TURNMENU getMenusInTurnMenu(void){ return _menusInTurnMenu; }
	void setMenusInTurnMenu(MENUS_IN_TURNMENU menusInTrunMenu) { _menusInTurnMenu = menusInTrunMenu; }
	turnMenu();
	~turnMenu();
};

