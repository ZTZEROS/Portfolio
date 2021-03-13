#pragma once
#include "../gameNode.h"
#include "itemList.h"
#include "itemInfo.h"
#include <vector>

class wareHouse;
class statusUI;

class inventory : public gameNode
{
private:
	itemInfo* _info;
	wareHouse* _house;
	statusUI* _status;
	tagItem _inven[16];

	int _currentItemNum;
	int _totalItemNum;
	int _startRenderPoint;
	int _isPushDownX;//xŰ�� ����� �������� ī���ÿ� ��������. Ȥ�ó� ���� �ϴ� ����
	int _choiceItemNum;//���õ� ������ �ѹ��� �������.

	int _totalMoney;

	//============================================
	//   Ŀ���� �Ϲ� Ŀ���� ���� Ŀ���� �ΰ�
	// �װŸ� �Բ� �����̰� �ϴٰ� xŰ�� ������
	// �Ϲ�Ŀ�� ���� �� ����Ŀ���� �����̵��� 
	//         ����� ����.
	//============================================

	//�Ϲ� Ŀ��
	image* _cursor;
	RECT _cursorRc;

	//���� Ŀ��
	image* _backUpCursor;
	RECT _backUpCursorRc;

	//ĭ ��Ʈ
	RECT _extentRc;
	image* _invenImage;

	//�� �̹��� ���
	image* _rollimage;

	bool _cursorTurnOver;
	bool _choiceItem;//���� ���õȰ� �� ������ �ƴ϶�� �������� ���õ� ����. �� ���¼� �� ���� true�� ��ȯ. Ŀ�� �нż��� ���� ��ġ

	bool _invenOff;
	bool _itemRead;

	strItemInfo _itemInfo;

	char str[128];

public:
	inventory();
	~inventory();

	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);

	void setCursorTurnOver(bool cursorTurn){ _cursorTurnOver = cursorTurn; }
	void setCurrentItemNum(int currentNum){ _currentItemNum = currentNum; }
	//void setSelectItem(bool selectItem){ _selectItem = selectItem; }

	//�������� �������� �޾ƿ� �� ����ϴ� ����
	void setItemVector(tagItem it);

	//���� �κ� ������ �Ѱ��ִ� ����
	bool getFullItem(void);

	//���� �κ� ù��° �׷��� �ѹ��� �Ѱ��ִ� ����
	int getStartRenderPoint(void) { return _startRenderPoint; }
	int getChoiceItemNum(void){ return _choiceItemNum; }
	
	tagItem getInvenItemArr(int currentNum) { return _inven[currentNum]; }
	void setInvenItemArr(tagItem temp, int currentNum);

	void setPushDownX(int x){ _isPushDownX = x; }
	void setChoiceItem(bool item){ _choiceItem = item; }

	//����� �޸� ��ũ
	void setWareHouseMemoryLink(wareHouse* house) { _house = house; }
	void setStatusUIMemoryLink(statusUI* status) { _status = status; }

	//�б�� �����
	void writeInvenFile(tagItem* arrayItem);
	void readInvenFile(void);

	//���α׷��� �� �� ���� ����ϴ�.(���� ���α׷����� Ŭ������ �߰��ϱ� �����Ƽ��� �ƴ�)
	int progressbarInven(long top, long bottom);

	//�κ� ���Ḧ ���� ����
	void setInvenOff(bool off){ _invenOff = off; }
	//�̹��� ��ε带 ���� ����
	void setItemRead(bool read){ _itemRead = read; }
	
};