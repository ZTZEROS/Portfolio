#pragma once
#include "gameNode.h"

//턴메뉴에서 어떤 메뉴인지 알려줄 이넘!
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
//어떤 메뉴를 렌더해줄지 도와줄 이넘!
enum MENUS_IN_TURNMENU
{
	RENDER_TURNMENU,
	RENDER_CHARACTER_LIST
};
//캐릭터리스트에서 이름들어갈 한칸을 구조체로!
struct tagCharacterInList
{
	RECT rc; //알파벳이미지 렌더 좌표 쉽게하기위해 만든 렉트!
	int numbering; //사용될꺼같아 일단 만들어둔 넘버링변수
};
class turnMenu : public gameNode
{
private:
	vector<tagCharacterInList> _vCharacterList;
private:
	image* _turnMenuCursorImage;
	MENUS_IN_TURNMENU _menusInTurnMenu;
	int _turnMenus; //턴메뉴의 소메뉴들 이넘용 
	int _numberOfCharacter; //캐릭터 수 받아올 변수
	int _cursorPositionY; //턴메뉴에서의 손가락커서Y축
	int _charChoicePositionY; //캐릭터리스트에서의 손가락커서Y축
	int _handCursorPlace; // 캐릭터리스트에서 손가락커서 범위 조정용 변수
public:
	HRESULT init();
	void release();
	void update(int numberOfCharacters, int x, int y);
	void render(int x, int y);
public:
	//겟터 셋터
	vector<tagCharacterInList> getCharacterList(void) { return _vCharacterList; }
	int getTurnMenus(void) { return _turnMenus; }
	int getCursorPlace() { return _handCursorPlace; }
	void setTurnMenus(int turnMenu) { _turnMenus = turnMenu; }
	MENUS_IN_TURNMENU getMenusInTurnMenu(void){ return _menusInTurnMenu; }
	void setMenusInTurnMenu(MENUS_IN_TURNMENU menusInTrunMenu) { _menusInTurnMenu = menusInTrunMenu; }
	turnMenu();
	~turnMenu();
};

