
#pragma once
#include "../gameNode.h"
#include "armorStoreBuy.h"
#include "armorStoreSale.h"
#include "inventory.h"
#include "itemList.h"

enum choosebox
{
	CHOOSE_BUY = 1,
	CHOOSE_SALE,
	DEFAULT
};

class armorStoreView : public gameNode
{
private:
	armorStoreBuy* _asb;
	armorStoreSale* _ass;
	inventory* _inven;

	//선택 화면
	RECT _chooseBuy;
	RECT _chooseSale;

	//커서 설정
	RECT _cursorRc;
	image* _cursorImage;

	int _currentMoney;

	//X키를 눌렀을 때 키막기용 및 상점 띄우기용 불값들
	bool isPushButtonX;
	choosebox isChooseBox;

public:
	armorStoreView();
	~armorStoreView();

	HRESULT init(void);
	void release(void);
	void update(int money);
	void render(void);

	int getCurrentMoney(void) { return _currentMoney; }

	void setIsPushButtonX(bool x) { isPushButtonX = x; }
};

