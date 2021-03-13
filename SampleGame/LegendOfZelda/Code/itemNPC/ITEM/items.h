#pragma once
#include "../../gameNode.h"

enum ITEMTYPE
{
	EQUIPITEM,		//착용 아이템(방패, 소드)
	SUBITEM,		//서브 아이템(부메랑, 랜턴...)
	DUNGEONITEM,	//던전 아이템(마스터키, 지도...)
	UIITEM,			//UI 표시 아이템(폭탄, 열쇠, 하트)
	GOLDITEM		//Gold 아이템
};

struct tagItem
{
	ITEMTYPE _Itype;			//아이템 타입
	image* _itemimg;			//인벤 아이템 이미지
	image* _dropimg;			//드랍 아이템 이미지
	image* _nameimg;			//아이템 이름 이미지
	const char* _Iname;			//아이템 이름
	const char* _IExplain;
	int _Iprice;				//아이템 가격
	bool _equip;				//아이템 착용여부
	int Num;					//아이템 개수
	float _leftTopX, _leftTopY;
	float CreationPointX, CreationPointY;	//아이템 위치
	float CenterX, CenterY;
	float LoopX, LoopY;
	RECT _itemRc;				//아이템 렉트
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
	tagItem* getItem() { return _Item; }					//아이템 접근자
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

	void setEquip(bool _equip) { _Item->_equip = _equip; }	//아이템 착용 설정자
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

