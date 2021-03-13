#pragma once
#include "../../gameNode.h"

enum ITEMTYPE
{
	EQUIPITEM,		//���� ������(����, �ҵ�)
	SUBITEM,		//���� ������(�θ޶�, ����...)
	DUNGEONITEM,	//���� ������(������Ű, ����...)
	UIITEM,			//UI ǥ�� ������(��ź, ����, ��Ʈ)
	GOLDITEM		//Gold ������
};

struct tagItem
{
	ITEMTYPE _Itype;			//������ Ÿ��
	image* _itemimg;			//�κ� ������ �̹���
	image* _dropimg;			//��� ������ �̹���
	image* _nameimg;			//������ �̸� �̹���
	const char* _Iname;			//������ �̸�
	const char* _IExplain;
	int _Iprice;				//������ ����
	bool _equip;				//������ ���뿩��
	int Num;					//������ ����
	float _leftTopX, _leftTopY;
	float CreationPointX, CreationPointY;	//������ ��ġ
	float CenterX, CenterY;
	float LoopX, LoopY;
	RECT _itemRc;				//������ ��Ʈ
};

class items : public gameNode
{
protected:
	tagItem* _Item;

	bool Talking;
	int Timer;

public:
	virtual HRESULT init(int x, int y);
	void release(void);
	void update(void);
	virtual void render(int leftTopX, int leftTopY);

	bool getEquip() { return _Item->_equip; }
	tagItem* getItem() { return _Item; }					//������ ������
	bool GetTalking() { return Talking; }
	
	const char* GetItemName() { return _Item->_Iname; }
	int GetItemNum() { return _Item->Num; }
	ITEMTYPE GetItemType() { return _Item->_Itype; }
	image* GetInvenImage() { return _Item->_itemimg; }
	image* GetDropImage() { return _Item->_dropimg; }
	image* GetNameImage() { return _Item->_nameimg; }
	float GetLeftTopX() { return _Item->_leftTopX; }
	float GetLeftTopY() { return _Item->_leftTopY; }
	float GetCenterX() { return _Item->CenterX; }
	float GetCenterY() { return _Item->CenterY; }
	float GetLoopX() { return _Item->LoopX; }
	float GetLoopY() { return _Item->LoopY; }

	RECT GetVirtualItem() { return _Item->_itemRc; }

	void setEquip(bool _equip) { _Item->_equip = _equip; }	//������ ���� ������
	void SetItemNum(int num) { _Item->Num = num; }
	void SetLeftTopX(float leftTopX) { _Item->_leftTopX = leftTopX; }
	void SetLeftTopY(float leftTopY) { _Item->_leftTopX = leftTopY; }
	void SetCenterX(float centerX) { _Item->CenterX = centerX; }
	void SetCenterY(float centerY) { _Item->CenterY = centerY; }
	void SetLoopX(float loopX) { _Item->LoopX = loopX; }
	void SetLoopY(float loopY) { _Item->LoopY = loopY; }
	void SetTalking(bool paused) { Talking = paused; }
	
	void AquisitionEffect();

	items();
	~items();
};

