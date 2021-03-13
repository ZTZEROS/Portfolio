
#include "stdafx.h"
#include "weaponStoreBuy.h"
#include "inventory.h"

weaponStoreBuy::weaponStoreBuy()
{

}

weaponStoreBuy::~weaponStoreBuy()
{

}

HRESULT weaponStoreBuy::init(void)
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

void weaponStoreBuy::release(void)
{
}

void weaponStoreBuy::update(int money)
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

void weaponStoreBuy::render(void)
{
	IMAGEMANAGER->findImage("storebg2")->render(getIMemDC(), 0, 0);
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

void weaponStoreBuy::registItem(void)
{
	//����
	_weaponItem[0].name = "�丣�� �и�ġ";
	_weaponItem[0].img = IMAGEMANAGER->findImage("axeType");
	_weaponItem[0].itemKind = "weapon";
	_weaponItem[0].itemIndex = "����ġ�� ������ ĥ ���� �𸥴�.";
	_weaponItem[0].recoveryHP = 0;
	_weaponItem[0].recoverySP = 0;
	_weaponItem[0].regulaterAttack = 20;
	_weaponItem[0].regulaterDefence = 40;
	_weaponItem[0].regulaterSpeed = 0;
	_weaponItem[0].regulaterHit = 0;
	_weaponItem[0].regulaterHP = 0;
	_weaponItem[0].regulaterSP = 0;
	_weaponItem[0].regulaterMagic = 0;
	_weaponItem[0].regulaterResistance = 0;
	_weaponItem[0].itemHell = 5000;


	//Ȱ
	_weaponItem[1].name = "���ν��� �峭�� Ȱ";
	_weaponItem[1].img = IMAGEMANAGER->findImage("bowType");
	_weaponItem[1].itemKind = "weapon";
	_weaponItem[1].itemIndex = "������ �а��ٴ� ������ ��Ʈ��� ȭ�� 2��(�ö�ƽ��, ���ǰ) �߰�";
	_weaponItem[1].recoveryHP = 0;
	_weaponItem[1].recoverySP = 0;
	_weaponItem[1].regulaterAttack = 10;
	_weaponItem[1].regulaterDefence = 0;
	_weaponItem[1].regulaterSpeed = 0;
	_weaponItem[1].regulaterHit = 20;
	_weaponItem[1].regulaterHP = 0;
	_weaponItem[1].regulaterSP = 0;
	_weaponItem[1].regulaterMagic = 5;
	_weaponItem[1].regulaterResistance = 0;
	_weaponItem[1].itemHell = 1000;

	//��
	_weaponItem[2].name = "�۷�17";
	_weaponItem[2].img = IMAGEMANAGER->findImage("gunType");
	_weaponItem[2].itemKind = "weapon";
	_weaponItem[2].itemIndex = "�����ڰ� � �� ������ �μ��� ��Ҵ� BBź�� ����. �ִ� 35��";
	_weaponItem[2].recoveryHP = 0;
	_weaponItem[2].recoverySP = 0;
	_weaponItem[2].regulaterAttack = 20;
	_weaponItem[2].regulaterDefence = 0;
	_weaponItem[2].regulaterSpeed = 12;
	_weaponItem[2].regulaterHit = 15;
	_weaponItem[2].regulaterHP = 0;
	_weaponItem[2].regulaterSP = 0;
	_weaponItem[2].regulaterMagic = 0;
	_weaponItem[2].regulaterResistance = 0;
	_weaponItem[2].itemHell = 500;

	//��Ʋ��
	_weaponItem[3].name = "������ ���尩";
	_weaponItem[3].img = IMAGEMANAGER->findImage("guntletType");
	_weaponItem[3].itemKind = "weapon";
	_weaponItem[3].itemIndex = "��ģ���� �ĸ¾Ƶ� �ٽ� �Ͼ �� �ִٴ� ����ȭ�� �߶��� ������ �尩";
	_weaponItem[3].recoveryHP = 0;
	_weaponItem[3].recoverySP = 0;
	_weaponItem[3].regulaterAttack = 0;
	_weaponItem[3].regulaterDefence = 900;
	_weaponItem[3].regulaterSpeed = 0;
	_weaponItem[3].regulaterHit = 0;
	_weaponItem[3].regulaterHP = 0;
	_weaponItem[3].regulaterSP = 0;
	_weaponItem[3].regulaterMagic = 0;
	_weaponItem[3].regulaterResistance = 0;
	_weaponItem[3].itemHell = 5300;

	//����尩
	_weaponItem[4].name = "����Ÿ���� ��";
	_weaponItem[4].img = IMAGEMANAGER->findImage("nikuType");
	_weaponItem[4].itemKind = "weapon";
	_weaponItem[4].itemIndex = "��ǰ�� �̻��� ������ �ʿ��Ѱ�?";
	_weaponItem[4].recoveryHP = 0;
	_weaponItem[4].recoverySP = 0;
	_weaponItem[4].regulaterAttack = 700;
	_weaponItem[4].regulaterDefence = 0;
	_weaponItem[4].regulaterSpeed = 0;
	_weaponItem[4].regulaterHit = 0;
	_weaponItem[4].regulaterHP = 0;
	_weaponItem[4].regulaterSP = 0;
	_weaponItem[4].regulaterMagic = 0;
	_weaponItem[4].regulaterResistance = 0;
	_weaponItem[4].itemHell = 10000;

	//���Ǿ�
	_weaponItem[5].name = "���Ǻ�";
	_weaponItem[5].img = IMAGEMANAGER->findImage("spearType");
	_weaponItem[5].itemKind = "weapon";
	_weaponItem[5].itemIndex = "���Ǻ��̶� ����� ȿ�ڼ��̶� �д´�. ���̰� �þ�� ���򼼴� ��Ʈ��ǰ";
	_weaponItem[5].recoveryHP = 0;
	_weaponItem[5].recoverySP = 0;
	_weaponItem[5].regulaterAttack = 100;
	_weaponItem[5].regulaterDefence = 50;
	_weaponItem[5].regulaterSpeed = 0;
	_weaponItem[5].regulaterHit = 0;
	_weaponItem[5].regulaterHP = 0;
	_weaponItem[5].regulaterSP = 0;
	_weaponItem[5].regulaterMagic = 0;
	_weaponItem[5].regulaterResistance = 0;
	_weaponItem[5].itemHell = 1200;

	//������
	_weaponItem[6].name = "�������� ������";
	_weaponItem[6].img = IMAGEMANAGER->findImage("steapType");
	_weaponItem[6].itemKind = "weapon";
	_weaponItem[6].itemIndex = "� ���������� ������ �̾��ð��� �ٿ� ������ٴ� ��°� ���� �깰.";
	_weaponItem[6].recoveryHP = 0;
	_weaponItem[6].recoverySP = 0;
	_weaponItem[6].regulaterAttack = 10;
	_weaponItem[6].regulaterDefence = 5;
	_weaponItem[6].regulaterSpeed = 0;
	_weaponItem[6].regulaterHit = 0;
	_weaponItem[6].regulaterHP = 0;
	_weaponItem[6].regulaterSP = 0;
	_weaponItem[6].regulaterMagic = 50;
	_weaponItem[6].regulaterResistance = 40;
	_weaponItem[6].itemHell = 5000;

	//Į
	_weaponItem[7].name = "�������� ������";
	_weaponItem[7].img = IMAGEMANAGER->findImage("swordType");
	_weaponItem[7].itemKind = "weapon";
	_weaponItem[7].itemIndex = "�������� ���� ���� ���� �� �ִ� ������ LED ���� ������(ȿ�������)";
	_weaponItem[7].recoveryHP = 0;
	_weaponItem[7].recoverySP = 0;
	_weaponItem[7].regulaterAttack = 30;
	_weaponItem[7].regulaterDefence = 10;
	_weaponItem[7].regulaterSpeed = 0;
	_weaponItem[7].regulaterHit = 50;
	_weaponItem[7].regulaterHP = 0;
	_weaponItem[7].regulaterSP = 0;
	_weaponItem[7].regulaterMagic = 0;
	_weaponItem[7].regulaterResistance = 0;
	_weaponItem[7].itemHell = 1000;

}

//������ �ɷ�ġ ������ �Լ�
tagItem weaponStoreBuy::itemOptionDefine(tagItem ti)
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
int weaponStoreBuy::listedItem(void)
{
	//���� ��ϵ� �����ۺ��� ������������ ���Ϳ� �ε�ȴ�.
	//-->�ݺ����� ������ 1�� � 2�� � ������ �ε��Ѵ�.

	int totalListedItemNum = 0;


	//�� ����
	for(int i = 0; i < 8; i++)
	{
		for(int j = 0; j < RND->getFromIntTo(0, 8); j++)
		{
			totalListedItemNum++;
			_weaponItem[i].itemRc = RectMake(50, 90 + ( 50 * totalListedItemNum ), 300, 50);
			_vItem.push_back(itemOptionDefine(_weaponItem[i]));
		}
	}

	return totalListedItemNum;

}

void weaponStoreBuy::listRender(void)
{
	//����Ʈ�� ���� �������� ����Ѵ�.

	for(_viItem = _vItem.begin(); _viItem != _vItem.end(); ++_viItem)
	{
		if(_viItem->itemRc.top >= _extentRc.top && _viItem->itemRc.bottom <= _extentRc.bottom)
		{
			//Rectangle(getMemDC(), _viItem->itemRc.left, _viItem->itemRc.top, _viItem->itemRc.right, _viItem->itemRc.bottom);
			//_viItem->img->render(getMemDC(), _viItem->itemRc.left+30, _viItem->itemRc.top);
			TextOut(getIMemDC(), _viItem->itemRc.left+90, _viItem->itemRc.top, _viItem->name, lstrlen(_viItem->name));
			TextOut(getIMemDC(), _viItem->itemRc.left+230, _viItem->itemRc.top, _viItem->str, lstrlen(_viItem->str));
		}
	}
}

