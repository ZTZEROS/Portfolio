
#pragma once
#include "../gameNode.h"
#include "itemList.h"
#include "itemInfo.h"
#include <vector>

class inventory;

class wareHouse : public gameNode
{
private:
	
	itemInfo* _info;
	inventory* _inven;

	tagItem _house[50];

	int _currentItemNum;
	int _totalItemNum;

	int _startRenderPoint;
	int _choiceItemNum;

	int _isPushDownX;

	image* _cursor;
	RECT _cursorRc;

	image* _backUpCursor;
	RECT _backUpCursorRc;

	image* _warehouseImage;
	RECT _extentRc;

	//롤 이미지 등록
	image* _rollimage;

	bool _cursorTurnOver;
	bool _choiceItem;


	strItemInfo _itemInfo;

	char str[128];

public:
	wareHouse();
	~wareHouse();

	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);

	void setCursorTurnOver(bool cursorTurn) { _cursorTurnOver = cursorTurn; }

	//현재 인벤 첫번째 그려진 넘버를 넘겨주는 겟터
	int getStartRenderPoint(void) { return _startRenderPoint; }
	int getChoiceItemNum(void){ return _choiceItemNum; }

	tagItem gethouseItemArr(int currentNum) { return _house[currentNum]; }
	void sethouseItemArr(tagItem temp, int currentNum);

	void setPushDownX(int x) { _isPushDownX = x; }
	void setChoiceItem(bool item){ _choiceItem = item; }

	void setInventoryMemoryLink(inventory* inven) { _inven = inven; }
	void setCurrentItemNum(int currentNum) { _currentItemNum = currentNum; }

	//읽기용 쓰기용
	void writeWarehouseFile(tagItem* arrayItem);
	void readWarehouseFile(void);

	//프로그래스 바 걍 새로 만듭니다.(절때 프로그래스바 클래스에 추가하기 귀찮아서가 아님)
	int progressbarInven(long top, long bottom);

};
