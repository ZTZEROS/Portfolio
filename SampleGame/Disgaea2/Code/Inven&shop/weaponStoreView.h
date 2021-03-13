
#pragma once
#include "../gameNode.h"
#include "weaponStoreBuy.h"
#include "weaponStoreSale.h"
#include "inventory.h"
#include "itemList.h"


enum weaponchoosebox
{
	CHOOSE_WEAPON_BUY = 1,
	CHOOSE_WEAPON_SALE,
	WEAPON_DEFAULT
};


class weaponStoreView : public gameNode 
{
private:
	weaponStoreBuy* _wsb;
	weaponStoreSale* _wss;
	inventory* _inven;

	int _currentMoney;

	//선택 화면
	RECT _chooseBuy;
	RECT _chooseSale;

	//커서 설정
	RECT _cursorRc;
	image* _cursorImage;

	//X키를 눌렀을 때 키막기용 및 상점 띄우기용 불값들
	bool isPushButtonX;
	weaponchoosebox isChooseBox;

public:
	weaponStoreView();
	~weaponStoreView();

	HRESULT init(void);
	void release(void);
	void update(int money);
	void render(void);

	int getCurrentMoney(void) { return _currentMoney; }

	void setIsPushButtonX(bool x) { isPushButtonX = x; }
};
