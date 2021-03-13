
#pragma once
#include "../gameNode.h"
#include "itemList.h"
#include "itemInfo.h"
#include <vector>

//���漱��
class inventory;

class weaponStoreBuy : public gameNode
{
private:
	image* _shopImage;
	inventory* _inven;
	itemInfo* _info;
	//���� ����â�� ���� ����(����â ������ ���°ɷ� ������)
	vector<tagItem> _vItem;
	vector<tagItem>::iterator _viItem;

	//���� ��Ͽ� ����ü
	tagItem _weaponItem[8];

	//������ �ɼ� ����� ���� ����
	itemState _is;
	
	//������ ǥ�� ������ ���� ��Ʈ
	RECT _extentRc;

	//�հ��� Ŀ���� ���� ����
	image* _cursor;
	RECT _cursorRc;

	int _currentItemNum;
	int _totalItemNum;

	//���� 0�� ���� ��ǥ
	long _zeroVectorPointX;
	long _zeroVectorPointY;

	//����������
	strItemInfo _itemInfo;

	//������ �ִ� �Ӵ� ����
	image* _hellslot;
	int _totalMoney;
	char arrHell[128]; //�� ǥ��

	bool _isPushDownC;
	bool _isRegisted;


public:
	weaponStoreBuy();
	~weaponStoreBuy();
	
	//��� �޸𸮿� ������ �ְ� �ƴϴ� �ϴ� �������� �Ҵ�.
	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(int money);
	virtual void render(void);

	//������ ����ü ��Ͽ� �Լ�
	void registItem(void);

	/*
	����ü ���� �ɼ��� ���� �Ͽ� ���Ϳ� �ִ´�(����)
		- �κ��� �� ����
		- ���� ������ ������ ���̰� ����
		- ���� ������ ����(���� �� �Ҹ�ǰ�� �Ȱ��� ����)
		- ���� ������ ���̿� ���� ���� ����(���� �� �Ҹ�ǰ�� �Ȱ��� ����)
		-->���⼭ ����ü ���� �ϼ�.
	*/

	//��ǰ�� �����ϴ� �Լ� <-- ���⼭ ��� ó���� �� �Ѵ�.
	int listedItem(void);
	tagItem itemOptionDefine(tagItem ti);
	//-->������� ���� ����.

	//������ �� ����Ѵ�
	/*
	�κ� ������ ��Ʈ�� �ϳ� ���, 7��� �� ���� ĳ���� �̴ϼ��� �ٿ��� ĳ���� �̸� ��Ʈ�� ���.
		-->��ȣ�� ���� ��ȣ�� ���. ���ͷ����Ͱ� ���� ��� �ٽ� �� ���� ���ư�.

	*/
	bool getPushDownC(void) { return _isPushDownC; }
	void setPushDownC(bool c) { _isPushDownC = c; }

	void setRegistItem(bool ri) { _isRegisted = ri; }

	//��¿� �Լ�
	void listRender(void);

	int getTotalMoney(void) { return _totalMoney; }

	//�κ��丮 �޸� ��ũ
	void setInventoryDataLink(inventory* inven){ _inven = inven; }

	//�� ǥ�ø� ���� ���� �ڸ��� �˷��ִ� �Լ�
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

	//'���'������ �Ѱ��ֱ⸸ �ϴ� ���� �ܴ̿� ���ʿ�
	vector<tagItem> getItemVector(void);
};
