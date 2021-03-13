
#include "stdafx.h"
#include "armorStoreBuy.h"
#include "inventory.h"


armorStoreBuy::armorStoreBuy()
{

}

armorStoreBuy::~armorStoreBuy()
{

}

HRESULT armorStoreBuy::init(void)
{	

	_info = new itemInfo;
	_info->init();

	_isRegisted = false;
	registItem();
	_totalItemNum = listedItem();
	_totalItemNum += 1;

	//Ŀ�� �̹��� ���
	_currentItemNum = 0;

	_cursor = IMAGEMANAGER->findImage("cursorImage");
	_cursorRc = RectMake(_vItem[_currentItemNum].itemRc.left-15, _vItem[_currentItemNum].itemRc.top+10, 50, 50);

	//���� ��Ʈ �ʱ�ȭ(vItem 0���̶� ������ ����)
	_extentRc = RectMake(_vItem[0].itemRc.left, _vItem[0].itemRc.top, 300, 400);

	_zeroVectorPointX = _vItem[0].itemRc.left;
	_zeroVectorPointY = _vItem[0].itemRc.top;

	_shopImage = IMAGEMANAGER->findImage("shopimage");
	_hellslot = IMAGEMANAGER->findImage("hellslot");


	_totalMoney = 0;

	_isPushDownC = false;

	return S_OK;
}

void armorStoreBuy::release(void)
{
}

void armorStoreBuy::update(int money)
{
	_totalMoney = money;
	/*
	���õǴ� ��Ʈ�� ���� ���õǴ� ������ ������ �ٲ��.
	����ü�� �ѹ��� �Կ���, ���� ������ ����ü �ѹ��� ������ �� ������ �����?
	*/

	if(KEYMANAGER->isOnceKeyDown(VK_DOWN))
	{
		SOUNDMANAGER->play("menuMove", 1.0f);
		//��Ʈ �����ŭ �ø���.
		if( _cursorRc.bottom >= _extentRc.bottom)
		{
			//���� 1ĭ�� �ø���.
			for(int i = 0; i < _totalItemNum - 1; i++)
			{
				if(_currentItemNum < _totalItemNum - 2)
				{
					_vItem[i].itemRc.top -= 50;
					_vItem[i].itemRc.bottom -= 50;
				}
			}
		}
		if(_currentItemNum < _totalItemNum - 2) _currentItemNum++;
	}
	if(KEYMANAGER->isOnceKeyDown(VK_UP))
	{
		SOUNDMANAGER->play("menuMove", 1.0f);
		//��Ʈ �����ŭ �ø���.
		if( _cursorRc.top < _extentRc.top + 50)
		{
			//���� 1ĭ�� �ø���.
			for(int i = 0; i < _totalItemNum; i++)
			{
				if(_currentItemNum > 0)
				{
					_vItem[i].itemRc.top += 50;
					_vItem[i].itemRc.bottom += 50;
				}
			}
		}
		if(_currentItemNum > 0) _currentItemNum--;
	}

	if(KEYMANAGER->isOnceKeyDown('X'))
	{
		SOUNDMANAGER->play("menuSelect", 1.0f);
		if(_inven->getFullItem() && _totalMoney >= 0)
		{
			if((_totalMoney - _vItem[_currentItemNum].itemHell) > 0) 
			{
				_inven->setItemVector(_vItem[_currentItemNum]);
				_totalMoney -= _vItem[_currentItemNum].itemHell;
				_vItem.erase(_vItem.begin() + _currentItemNum);
			}

			//���̻� ���� �� ���ٴ� ���� �޼��� ���!
		}
		int i = 0;
		for(_viItem = _vItem.begin(); _viItem != _vItem.end(); _viItem++)
		{
			if(i >= _totalItemNum) break;
			i++;
			_viItem->itemRc = RectMake(_zeroVectorPointX, _zeroVectorPointY + ( 50 * i ), 300, 50);
		}
		_totalItemNum = i + 1;
	}

	if(_isPushDownC) // && KEYMANAGER->isOnceKeyDown('C')) 
	{
		_inven->setInvenOff(true);
		_isPushDownC = false;
	}

	if(_isRegisted)
	{
		//_vItem.clear();
		_inven->setItemRead(true);
		//_totalItemNum = listedItem();
		_isRegisted = false;
	}

	_cursorRc = RectMake(_vItem[_currentItemNum].itemRc.left-15, _vItem[_currentItemNum].itemRc.top+10, 50, 50);
	//������ ������ �ٲ�ϱ� ����ٰ��� �ѹ� ����
	_zeroVectorPointX = _vItem[0].itemRc.left;
	_zeroVectorPointY = _vItem[0].itemRc.top - 50;

	//�� ���
	sprintf(arrHell, "%d", _totalMoney);

	//������ ������ ���� ó��(�Լ� ����� ������)
	sprintf(_itemInfo.recoveryHP, "%d", _vItem[_currentItemNum].recoveryHP);
	sprintf(_itemInfo.recoverySP, "%d", _vItem[_currentItemNum].recoverySP);
	sprintf(_itemInfo.regulaterAttack, "%d", _vItem[_currentItemNum].regulaterAttack);
	sprintf(_itemInfo.regulaterDefence, "%d", _vItem[_currentItemNum].regulaterDefence);
	sprintf(_itemInfo.regulaterHit, "%d", _vItem[_currentItemNum].regulaterHit);
	sprintf(_itemInfo.regulaterMagic, "%d", _vItem[_currentItemNum].regulaterMagic);
	sprintf(_itemInfo.regulaterSpeed, "%d", _vItem[_currentItemNum].regulaterSpeed);
	sprintf(_itemInfo.regulaterResistance, "%d", _vItem[_currentItemNum].regulaterResistance);
	sprintf(_itemInfo.itemName, "%s", _vItem[_currentItemNum].name);
	sprintf(_itemInfo.itemIndex, "%s", _vItem[_currentItemNum].itemIndex);
	//sprintf(_itemInfo.regulaterHP, " - HP�ִ�ġ: %d", _vItem[_currentItemNum].regulaterHP);
	//sprintf(_itemInfo.regulaterSP, " - SP�ִ�ġ: %d", _vItem[_currentItemNum].regulaterSP);
	
	_info->update(_itemInfo);


}

void armorStoreBuy::render(void)
{

	IMAGEMANAGER->findImage("storebg1")->render(getIMemDC(), 0, 0);
	IMAGEMANAGER->findImage("shopimage")->render(getIMemDC(), _extentRc.left, _extentRc.top - 50);
	IMAGEMANAGER->findImage("hellslot")->render(getIMemDC(), 50, 590);
	//Rectangle(getMemDC(), _extentRc.left, _extentRc.top, _extentRc.right, _extentRc.bottom);
	HFONT font, oldfont;
	font = CreateFont(35, 0, 0, 0, 1000, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH, TEXT("���� ���"));
	oldfont = (HFONT)SelectObject(getIMemDC(), font);
	SetTextColor(getIMemDC(), RGB(255, 255, 255));
	SetBkMode(getIMemDC(), TRANSPARENT);
	TextOut(getIMemDC(), 250 - (unitNum(_totalMoney) * 14), 595, arrHell, lstrlen(arrHell));
	listRender();
	SelectObject(getIMemDC(), oldfont);
	DeleteObject(font);
	IMAGEMANAGER->findImage("cursorImage")->render(getIMemDC(), _cursorRc.left, _cursorRc.top);
	_info->render();


}

void armorStoreBuy::registItem(void)
{
	//�ƹķ� <--������ �������� �⺻�Ƹ�
	_armorItem[0].name = "�ƹķ�";
	_armorItem[0].img = IMAGEMANAGER->findImage("necklessType");
	_armorItem[0].itemKind = "armor";
	_armorItem[0].itemIndex = "���� ������ �� �����, ����ǰƼ�� ����.";
	_armorItem[0].recoveryHP = 0;
	_armorItem[0].recoverySP = 0;
	_armorItem[0].regulaterAttack = 0;
	_armorItem[0].regulaterDefence = 40;
	_armorItem[0].regulaterSpeed = 0;
	_armorItem[0].regulaterHit = 0;
	_armorItem[0].regulaterHP = 0;
	_armorItem[0].regulaterSP = 0;
	_armorItem[0].regulaterMagic = 0;
	_armorItem[0].regulaterResistance = 0;
	_armorItem[0].itemHell = 20;


	//��������
	_armorItem[1].name = "��������";
	_armorItem[1].img = IMAGEMANAGER->findImage("armorType");
	_armorItem[1].itemKind = "armor";
	_armorItem[1].itemIndex = "�߱��� ���� �Ŵ����� ����ϴ�, ���⸸ �ص� ������ �ٷ� �� ��";
	_armorItem[1].recoveryHP = 0;
	_armorItem[1].recoverySP = 0;
	_armorItem[1].regulaterAttack = 0;
	_armorItem[1].regulaterDefence = 100;
	_armorItem[1].regulaterSpeed = 0;
	_armorItem[1].regulaterHit = 0;
	_armorItem[1].regulaterHP = 0;
	_armorItem[1].regulaterSP = 0;
	_armorItem[1].regulaterMagic = 0;
	_armorItem[1].regulaterResistance = 0;
	_armorItem[1].itemHell = 50;

	//��ź����
	_armorItem[2].name = "��ź����";
	_armorItem[2].img = IMAGEMANAGER->findImage("armorType");
	_armorItem[2].itemKind = "armor";
	_armorItem[2].itemIndex = "������ ������ ���� ��û�� «�� ������ �������.";
	_armorItem[2].recoveryHP = 0;
	_armorItem[2].recoverySP = 0;
	_armorItem[2].regulaterAttack = 0;
	_armorItem[2].regulaterDefence = 150;
	_armorItem[2].regulaterSpeed = 0;
	_armorItem[2].regulaterHit = 0;
	_armorItem[2].regulaterHP = 0;
	_armorItem[2].regulaterSP = 0;
	_armorItem[2].regulaterMagic = 0;
	_armorItem[2].regulaterResistance = 0;
	_armorItem[2].itemHell = 200;

	//�Ź�
	_armorItem[3].name = "����Ű";
	_armorItem[3].img = IMAGEMANAGER->findImage("shoseType");
	_armorItem[3].itemKind = "armor";
	_armorItem[3].itemIndex = "80��� ������ �ҳ���� ������ ������ �ߴ� �� �ΰ��� ���ΰ�.";
	_armorItem[3].recoveryHP = 0;
	_armorItem[3].recoverySP = 0;
	_armorItem[3].regulaterAttack = 0;
	_armorItem[3].regulaterDefence = 20;
	_armorItem[3].regulaterSpeed = 1;
	_armorItem[3].regulaterHit = 0;
	_armorItem[3].regulaterHP = 0;
	_armorItem[3].regulaterSP = 0;
	_armorItem[3].regulaterMagic = 0;
	_armorItem[3].regulaterResistance = 0;
	_armorItem[3].itemHell = 150;

	//�������
	_armorItem[4].name = "�ݹ���";
	_armorItem[4].img = IMAGEMANAGER->findImage("ringType");
	_armorItem[4].itemKind = "armor";
	_armorItem[4].itemIndex = "����� ��¡. ������ �̻��� ������ ���� �ִ�.";
	_armorItem[4].recoveryHP = 0;
	_armorItem[4].recoverySP = 0;
	_armorItem[4].regulaterAttack = 0;
	_armorItem[4].regulaterDefence = 40;
	_armorItem[4].regulaterSpeed = 0;
	_armorItem[4].regulaterHit = 0;
	_armorItem[4].regulaterHP = 0;
	_armorItem[4].regulaterSP = 0;
	_armorItem[4].regulaterMagic = 0;
	_armorItem[4].regulaterResistance = 0;
	_armorItem[4].itemHell = 250;

	//�Ҵ�Ʈ
	_armorItem[5].name = "�Ҵ�Ʈ";
	_armorItem[5].img = IMAGEMANAGER->findImage("necklessType");
	_armorItem[5].itemKind = "armor";
	_armorItem[5].itemIndex = "�̱⿡�� ���� ���� �� �ִ� �ö�ƽ�� �����.";
	_armorItem[5].recoveryHP = 0;
	_armorItem[5].recoverySP = 0;
	_armorItem[5].regulaterAttack = 0;
	_armorItem[5].regulaterDefence = 50;
	_armorItem[5].regulaterSpeed = 0;
	_armorItem[5].regulaterHit = 0;
	_armorItem[5].regulaterHP = 0;
	_armorItem[5].regulaterSP = 0;
	_armorItem[5].regulaterMagic = 0;
	_armorItem[5].regulaterResistance = 0;
	_armorItem[5].itemHell = 300;

	//�ô� �� ��
	_consumeItem[0].name = "�ô� �� ��";
	_consumeItem[0].img = IMAGEMANAGER->findImage("chuingType");
	_consumeItem[0].itemKind = "consumeItemHP";
	_consumeItem[0].itemIndex = "���� Ű���� ������ HP�� ȸ���ȴ�. �� ������ ��쿡�� ��ȿ��";
	_consumeItem[0].recoveryHP = 10;
	_consumeItem[0].recoverySP = 0;
	_consumeItem[0].regulaterAttack = 0;
	_consumeItem[0].regulaterDefence = 0;
	_consumeItem[0].regulaterSpeed = 0;
	_consumeItem[0].regulaterHit = 0;
	_consumeItem[0].regulaterHP = 0;
	_consumeItem[0].regulaterSP = 0;
	_consumeItem[0].regulaterMagic = 0;
	_consumeItem[0].regulaterResistance = 0;
	_consumeItem[0].itemHell = 10;

	//���� ��
	_consumeItem[1].name = "���� ��";
	_consumeItem[1].img = IMAGEMANAGER->findImage("gumType");
	_consumeItem[1].itemKind = "consumeItemHP";
	_consumeItem[1].itemIndex = "���̵��� ���� ���� �� �ִ� �Ϲ� ��. ������ HP�� ȸ���ȴ�.";
	_consumeItem[1].recoveryHP = 30;
	_consumeItem[1].recoverySP = 0;
	_consumeItem[1].regulaterAttack = 0;
	_consumeItem[1].regulaterDefence = 0;
	_consumeItem[1].regulaterSpeed = 0;
	_consumeItem[1].regulaterHit = 0;
	_consumeItem[1].regulaterHP = 0;
	_consumeItem[1].regulaterSP = 0;
	_consumeItem[1].regulaterMagic = 0;
	_consumeItem[1].regulaterResistance = 0;
	_consumeItem[1].itemHell = 70;

	//�ֻ�� ��
	_consumeItem[2].name = "�ֻ�� ��";
	_consumeItem[2].img = IMAGEMANAGER->findImage("gumType");
	_consumeItem[2].itemKind = "consumeItemHP";
	_consumeItem[2].itemIndex = "��갡 ÷���Ǿ� HP ȸ������ �ٸ� �͵�� �񱳰� ���� �ʴ´�.";
	_consumeItem[2].recoveryHP = 100;
	_consumeItem[2].recoverySP = 0;
	_consumeItem[2].regulaterAttack = 0;
	_consumeItem[2].regulaterDefence = 0;
	_consumeItem[2].regulaterSpeed = 0;
	_consumeItem[2].regulaterHit = 0;
	_consumeItem[2].regulaterHP = 0;
	_consumeItem[2].regulaterSP = 0;
	_consumeItem[2].regulaterMagic = 0;
	_consumeItem[2].regulaterResistance = 0;
	_consumeItem[2].itemHell = 200;

	//�����佺
	_consumeItem[3].name = "�����佺";
	_consumeItem[3].img = IMAGEMANAGER->findImage("candyType");
	_consumeItem[3].itemKind = "consumeItemSP";
	_consumeItem[3].itemIndex = "���ֱ���. ��� ������ ���� �׺��� ��� �Դ� ��. SP �ҷ� ȸ��";
	_consumeItem[3].recoveryHP = 0;
	_consumeItem[3].recoverySP = 20;
	_consumeItem[3].regulaterAttack = 0;
	_consumeItem[3].regulaterDefence = 0;
	_consumeItem[3].regulaterSpeed = 0;
	_consumeItem[3].regulaterHit = 0;
	_consumeItem[3].regulaterHP = 0;
	_consumeItem[3].regulaterSP = 0;
	_consumeItem[3].regulaterMagic = 0;
	_consumeItem[3].regulaterResistance = 0;
	_consumeItem[3].itemHell = 10;

	//���䷹��
	_consumeItem[4].name = "���䷹��";
	_consumeItem[4].img = IMAGEMANAGER->findImage("canType");
	_consumeItem[4].itemKind = "consumeItemSP";
	_consumeItem[4].itemIndex = "������ �帵ũ�� ���� ź ���� ���� �ֹ��� ������ �����. SP ȸ��";
	_consumeItem[4].recoveryHP = 0;
	_consumeItem[4].recoverySP = 70;
	_consumeItem[4].regulaterAttack = 0;
	_consumeItem[4].regulaterDefence = 0;
	_consumeItem[4].regulaterSpeed = 0;
	_consumeItem[4].regulaterHit = 0;
	_consumeItem[4].regulaterHP = 0;
	_consumeItem[4].regulaterSP = 0;
	_consumeItem[4].regulaterMagic = 0;
	_consumeItem[4].regulaterResistance = 0;
	_consumeItem[4].itemHell = 100;

	//�ӽ��ӽ�
	_consumeItem[5].name = "�ӽ��ӽ�";
	_consumeItem[5].img = IMAGEMANAGER->findImage("mersleType");
	_consumeItem[5].itemKind = "consumeItemHP";
	_consumeItem[5].itemIndex = "�Ǻ����� �ñ�ġ���� ��Ȱ�� �ϴ� ������";
	_consumeItem[5].recoveryHP = 0;
	_consumeItem[5].recoverySP = 0;
	_consumeItem[5].regulaterAttack = 5;
	_consumeItem[5].regulaterDefence = 0;
	_consumeItem[5].regulaterSpeed = 0;
	_consumeItem[5].regulaterHit = 0;
	_consumeItem[5].regulaterHP = 0;
	_consumeItem[5].regulaterSP = 0;
	_consumeItem[5].regulaterMagic = 0;
	_consumeItem[5].regulaterResistance = 0;
	_consumeItem[5].itemHell = 150;
}

//������ �ɷ�ġ ������ �Լ�
tagItem armorStoreBuy::itemOptionDefine(tagItem ti)
{
	ti.itemClass = (itemRarity)RND->getFromIntTo(1, 5); // 1~5������ ����� �������� ���δ�.
		
	//�÷��� ��ġ ����
	int loop = RND->getFromIntTo(0, 2); //�� �� �÷��� ����. ������

	//������ ������ ���ϱ� ���� �⺻ ������(��ü �ɷ�ġ�� �� �� ��.)
	int oldStateNum = ti.recoveryHP + ti.recoverySP + ti.regulaterAttack + ti.regulaterDefence + ti.regulaterHit + ti.regulaterHP +
		ti.regulaterSP + ti.regulaterMagic + ti.regulaterResistance + ti.regulaterSpeed;

	//������ ��޿� ���� ��ġ ����
	if(loop > 0)
	{
		for(int i = 0; i <= loop; i++)
		{
			_is = (itemState)RND->getFromIntTo(0, 10);
			switch(_is)
			{
				case STATE_RECOVERY_HP:
					if(ti.itemKind == "consumeItemHP") ti.recoveryHP = ti.recoveryHP + ti.itemClass*(RND->getFromIntTo(1, 5) - i);
					break;
				case STATE_RECOVERY_SP:
					if(ti.itemKind == "consumeItemSP") ti.recoverySP = ti.recoverySP + ti.itemClass*(RND->getFromIntTo(1, 5) - i);
					break;
				case STATE_REGULATER_HP:
					if(ti.itemKind != "consumeItemHP" && ti.itemKind != "consumeItemSP") ti.regulaterHP = ti.regulaterHP + ti.itemClass*(RND->getFromIntTo(1, 5) - i);
					break;
				case STATE_REGULATER_SP:
					if(ti.itemKind != "consumeItemHP" && ti.itemKind != "consumeItemSP") ti.regulaterSP = ti.regulaterSP + ti.itemClass*(RND->getFromIntTo(1, 5) - i);
					break;
				case STATE_REGULATER_ATK:
					if(ti.itemKind != "consumeItemHP" && ti.itemKind != "consumeItemSP") ti.regulaterAttack = ti.regulaterAttack + ti.itemClass*(RND->getFromIntTo(1, 5) - i);
					break;
				case STATE_REGULATER_DEF:
					if(ti.itemKind != "consumeItemHP" && ti.itemKind != "consumeItemSP") ti.regulaterDefence = ti.regulaterDefence + ti.itemClass*(RND->getFromIntTo(1, 5) - i);
					break;
				case STATE_REGULATER_HIT:
					if(ti.itemKind != "consumeItemHP" && ti.itemKind != "consumeItemSP") ti.regulaterHit = ti.regulaterHit + ti.itemClass*(RND->getFromIntTo(1, 5) - i);
					break;
				case STATE_REGULATER_SPEED:
					if(ti.itemKind != "consumeItemHP" && ti.itemKind != "consumeItemSP") ti.regulaterSpeed = ti.regulaterSpeed + ti.itemClass*(RND->getFromIntTo(1, 5) - i);
					break;
				case STATE_REGULATER_MAGIC:
					if(ti.itemKind != "consumeItemHP" && ti.itemKind != "consumeItemSP") ti.regulaterMagic = ti.regulaterMagic + ti.itemClass*(RND->getFromIntTo(1, 5) - i);
					break;
				case STATE_REGULATER_RES:
					if(ti.itemKind != "consumeItemHP" && ti.itemKind != "consumeItemSP") ti.regulaterResistance = ti.regulaterResistance + ti.itemClass*(RND->getFromIntTo(1, 5) - i);
					break;
				default:
					break;
			}
		}
	}
	//���� �� �������ͽ� ��
	int nowStateNum = ti.recoveryHP + ti.recoverySP + ti.regulaterAttack + ti.regulaterDefence + ti.regulaterHit + ti.regulaterHP +
		ti.regulaterSP + ti.regulaterMagic + ti.regulaterResistance + ti.regulaterSpeed;
	
	//������ ���մϴ�.(�⺻ �ݾ�+ ((���� �� �������ͽ� ��ġ - ���� �� �������ͽ� ��ġ) / 2))
	ti.itemHell = ti.itemHell + ((nowStateNum - oldStateNum) / 2);

	wsprintf(ti.str, "                 HL           %d", ti.itemHell);
	
	return ti;
}

//���⼭ ������ ������ ���� ����
int armorStoreBuy::listedItem(void)
{
	//���� ��ϵ� �����ۺ��� ������������ ���Ϳ� �ε�ȴ�.
	//-->�ݺ����� ������ 1�� � 2�� � ������ �ε��Ѵ�.

	int totalListedItemNum = 0;

	//�� ����
	for(int i = 0; i < 6; i++)
	{
		for(int j = 0; j < RND->getFromIntTo(0, 6); j++)
		{
			totalListedItemNum++;
			_armorItem[i].itemRc = RectMake(50, 90 + ( 50 * totalListedItemNum ), 300, 50);
			_vItem.push_back(itemOptionDefine(_armorItem[i]));
		}
	}

	//�Ҹ�ǰ 
	for(int k = 0; k < 6; k++)
	{
		for(int l = 0; l < RND->getFromIntTo(0, 6); l++)
		{
			totalListedItemNum++;
			_consumeItem[k].itemRc = RectMake(50, 90 + ( 50 * totalListedItemNum ), 300, 50);
			_vItem.push_back(itemOptionDefine(_consumeItem[k]));
		}
	}
	return totalListedItemNum;

}

void armorStoreBuy::listRender(void)
{
	//����Ʈ�� ���� �������� ����Ѵ�.

	for(_viItem = _vItem.begin(); _viItem != _vItem.end(); ++_viItem)
	{
		if(_viItem->itemRc.top >= _extentRc.top && _viItem->itemRc.bottom <= _extentRc.bottom)
		{
			//Rectangle(getIMemDC(), _viItem->itemRc.left, _viItem->itemRc.top, _viItem->itemRc.right, _viItem->itemRc.bottom);
			//_viItem->img->render(getIMemDC(), _viItem->itemRc.left+30, _viItem->itemRc.top);
			TextOut(getIMemDC(), _viItem->itemRc.left+90, _viItem->itemRc.top, _viItem->name, lstrlen(_viItem->name));
			TextOut(getIMemDC(), _viItem->itemRc.left+230, _viItem->itemRc.top, _viItem->str, lstrlen(_viItem->str));
		}
	}
}
