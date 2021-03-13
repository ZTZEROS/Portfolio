
#pragma once
#include "../gameNode.h"
#include "itemList.h"
#include "itemInfo.h"

class armorStoreSale : public gameNode
{
private:
	tagItem _inStoreinven[16];

	itemInfo* _info;
	int _currentItemNum;
	int _totalItemNum;

	bool _isPushDownC;
	bool _fileLode;

	image* _hellslot;
	int _totalMoney;
	char arrHell[128]; //돈 표시

	//일반 커서
	image* _cursor;
	RECT _cursorRc;

	//칸 랙트
	RECT _extentRc;
	image* _shopImage;

	//롤 이미지 등록
	image* _rollimage;

	bool _cursorTurnOver;
	bool _choiceItem;
	bool _isPushDownX;

	strItemInfo _itemInfo;

	char str[128];
	
public:
	armorStoreSale();
	~armorStoreSale();

	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(int money);
	virtual void render(void);

	void readInStoreInvenFile(void);
	void writeInStoreInvenFile(tagItem* arrayItem);
	int progressbarSaleInven(long top, long bottom);

	bool getPushDownC(void) { return _isPushDownC; }
	void setPushDownC(bool c) { _isPushDownC = c; }

	int getTotalMoney(void) { return _totalMoney; }

	void setFileLode(bool lode) { _fileLode = lode; }

	//돈 표시를 위한 숫자 자릿수 알려주는 함수
	int unitNum(int num)
	{
		int count = 0;
		while(true)
		{
			if(num/10 <= 0) break;
			num = num / 10;
			count++;
		}

		return count;
	}

};
