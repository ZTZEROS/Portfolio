
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

	//���� ȭ��
	RECT _chooseBuy;
	RECT _chooseSale;

	//Ŀ�� ����
	RECT _cursorRc;
	image* _cursorImage;

	int _currentMoney;

	//XŰ�� ������ �� Ű����� �� ���� ����� �Ұ���
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

