
#pragma once
#include "../gameNode.h"
#include "itemList.h"
#include "itemInfo.h"
#include <vector>

//전방선언
class inventory;

class weaponStoreBuy : public gameNode
{
private:
	image* _shopImage;
	inventory* _inven;
	itemInfo* _info;
	//실제 상점창을 위한 벡터(상점창 제한은 없는걸로 밝혀짐)
	vector<tagItem> _vItem;
	vector<tagItem>::iterator _viItem;

	//종류 등록용 구조체
	tagItem _weaponItem[8];

	//아이템 옵션 계산을 위한 선언
	itemState _is;
	
	//아이템 표시 범위를 위한 랙트
	RECT _extentRc;

	//손가락 커서를 위한 선언
	image* _cursor;
	RECT _cursorRc;

	int _currentItemNum;
	int _totalItemNum;

	//현재 0번 백터 좌표
	long _zeroVectorPointX;
	long _zeroVectorPointY;

	//아이템정보
	strItemInfo _itemInfo;

	//가지고 있는 머니 정보
	image* _hellslot;
	int _totalMoney;
	char arrHell[128]; //돈 표시

	bool _isPushDownC;
	bool _isRegisted;


public:
	weaponStoreBuy();
	~weaponStoreBuy();
	
	//계속 메모리에 상주할 애가 아니니 일단 가상으로 할당.
	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(int money);
	virtual void render(void);

	//아이템 구조체 등록용 함수
	void registItem(void);

	/*
	구조체 값의 옵션을 상세히 하여 벡터에 넣는다(렌덤)
		- 인벤에 들어갈 수량
		- 증감 스텟의 종류는 차이가 없음
		- 증감 스텟의 차이(물약 등 소모품도 똑같이 적용)
		- 증감 스텟의 차이에 따른 가격 차이(물약 등 소모품도 똑같이 적용)
		-->여기서 구조체 최종 완성.
	*/

	//제품을 진열하는 함수 <-- 여기서 모든 처리를 다 한다.
	int listedItem(void);
	tagItem itemOptionDefine(tagItem ti);
	//-->여기까지 인힛 동작.

	//진열한 후 출력한다
	/*
	인벤 사이즈 랙트를 하나 출력, 7등분 한 곳에 캐릭터 이니셜이 붙여진 캐릭터 이름 랙트를 출력.
		-->번호는 벡터 번호를 출력. 이터레이터가 끝인 경우 다시 맨 위로 돌아감.

	*/
	bool getPushDownC(void) { return _isPushDownC; }
	void setPushDownC(bool c) { _isPushDownC = c; }

	void setRegistItem(bool ri) { _isRegisted = ri; }

	//출력용 함수
	void listRender(void);

	int getTotalMoney(void) { return _totalMoney; }

	//인벤토리 메모리 링크
	void setInventoryDataLink(inventory* inven){ _inven = inven; }

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

	//'사기'에서는 넘겨주기만 하니 겟터 이외는 노필요
	vector<tagItem> getItemVector(void);
};
