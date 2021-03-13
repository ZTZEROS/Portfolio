
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

	//�� �̹��� ���
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

	//���� �κ� ù��° �׷��� �ѹ��� �Ѱ��ִ� ����
	int getStartRenderPoint(void) { return _startRenderPoint; }
	int getChoiceItemNum(void){ return _choiceItemNum; }

	tagItem gethouseItemArr(int currentNum) { return _house[currentNum]; }
	void sethouseItemArr(tagItem temp, int currentNum);

	void setPushDownX(int x) { _isPushDownX = x; }
	void setChoiceItem(bool item){ _choiceItem = item; }

	void setInventoryMemoryLink(inventory* inven) { _inven = inven; }
	void setCurrentItemNum(int currentNum) { _currentItemNum = currentNum; }

	//�б�� �����
	void writeWarehouseFile(tagItem* arrayItem);
	void readWarehouseFile(void);

	//���α׷��� �� �� ���� ����ϴ�.(���� ���α׷����� Ŭ������ �߰��ϱ� �����Ƽ��� �ƴ�)
	int progressbarInven(long top, long bottom);

};
