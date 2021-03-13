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
	int _isPushDownX;//x키를 몇번을 눌렀는지 카운팅용 전역변수. 혹시나 몰라서 일단 전역
	int _choiceItemNum;//선택된 아이템 넘버가 몇번인지.

	int _totalMoney;

	//============================================
	//   커서는 일반 커서와 예비 커서를 두고
	// 그거를 함께 움직이게 하다가 x키를 누르면
	// 일반커서 고정 후 예비커서만 움직이도록 
	//         만들어 본다.
	//============================================

	//일반 커서
	image* _cursor;
	RECT _cursorRc;

	//예비 커서
	image* _backUpCursor;
	RECT _backUpCursorRc;

	//칸 랙트
	RECT _extentRc;
	image* _invenImage;

	//롤 이미지 등록
	image* _rollimage;

	bool _cursorTurnOver;
	bool _choiceItem;//만약 선택된게 빈 공간이 아니라면 아이템이 선택된 상태. 그 상태서 이 값을 true로 반환. 커서 분신술을 위한 조치

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

	//상점에서 아이템을 받아올 때 사용하는 백터
	void setItemVector(tagItem it);

	//현재 인벤 수량을 넘겨주는 겟터
	bool getFullItem(void);

	//현재 인벤 첫번째 그려진 넘버를 넘겨주는 겟터
	int getStartRenderPoint(void) { return _startRenderPoint; }
	int getChoiceItemNum(void){ return _choiceItemNum; }
	
	tagItem getInvenItemArr(int currentNum) { return _inven[currentNum]; }
	void setInvenItemArr(tagItem temp, int currentNum);

	void setPushDownX(int x){ _isPushDownX = x; }
	void setChoiceItem(bool item){ _choiceItem = item; }

	//예비용 메모리 링크
	void setWareHouseMemoryLink(wareHouse* house) { _house = house; }
	void setStatusUIMemoryLink(statusUI* status) { _status = status; }

	//읽기용 쓰기용
	void writeInvenFile(tagItem* arrayItem);
	void readInvenFile(void);

	//프로그래스 바 걍 새로 만듭니다.(절때 프로그래스바 클래스에 추가하기 귀찮아서가 아님)
	int progressbarInven(long top, long bottom);

	//인벤 종료를 위한 셋터
	void setInvenOff(bool off){ _invenOff = off; }
	//이미지 재로드를 위한 셋터
	void setItemRead(bool read){ _itemRead = read; }
	
};