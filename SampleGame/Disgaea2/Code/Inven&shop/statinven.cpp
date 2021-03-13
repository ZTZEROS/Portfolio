
#include "stdafx.h"
#include "statinven.h"
#include "statusUI.h"
#include "itemInfo.h"

statinven::statinven()
{

}

statinven::~statinven()
{

}

HRESULT statinven::init(void)
{
	_info = new itemInfo;
	_info->init();

	_currentItemNum = 0;
	_choiceItemNum = 0;
	_isPushDownX = 0;
	_totalItemNum = 16;

	_totalMoney = 15000;

	_startRenderPoint = 0;
	//Ŀ�� ������ -> Item ���̵� status ���̵� �κ��� ������ �׻� �κ��� ������
	_cursorTurnOver = true;
	_choiceItem = false; //�������� ���� �������� ���� ����
	_itemRead = false;

	//���� �б�-> ������ ���� ���� �װɷ� �а� ������ ���� ���� ���� Idle�� �ʱ�ȭ
	readInvenFile();

	_invenImage = IMAGEMANAGER->addImage("inven", "UI_IMAGE/inven.bmp", 320, 400, false, RGB(255, 0, 255));

	//�� �̹��� ���
	_rollimage = IMAGEMANAGER->findImage("progressbarimage");

	//�Ϲ� Ŀ�� �ʱ�ȭ �κ�
	_cursor = IMAGEMANAGER->findImage("cursorImage");
	_cursorRc = RectMake(_inven[_currentItemNum].itemRc.left-15, _inven[_currentItemNum].itemRc.top+10, 50, 50);
	
	//���� Ŀ�� �ʱ�ȭ �κ�
	_backUpCursor = IMAGEMANAGER->findImage("cursorImage");
	_backUpCursorRc = RectMake(_inven[_currentItemNum].itemRc.left-15, _inven[_currentItemNum].itemRc.top+10, 50, 50);

	//������ ��Ʈ Ʋ�� ���� ����(�̰ɷ� �浹ó�� �ؼ� �����̵带 �ѱ��)
	_extentRc = RectMake(_inven[0].itemRc.left, _inven[0].itemRc.top, 300, 350);

	_invenOff = false;

	return S_OK;
}
void statinven::release(void)
{
}
void statinven::update(void)
{
	if(_itemRead)
	{
		readInvenFile();
		_itemRead = false;
	}

	if(_cursorTurnOver && KEYMANAGER->isOnceKeyDown(VK_DOWN))
	{
		//��Ʈ �����ŭ �ø���.
		if(_backUpCursorRc.bottom >= _extentRc.bottom)
		{
			//���� 1ĭ�� �ø���.
			for(int i = 0; i < _totalItemNum; i++)
			{
				if(_currentItemNum < 15)
				{
					_inven[i].itemRc.top -= 50;
					_inven[i].itemRc.bottom -= 50;
				}
			}
			if(_currentItemNum < 15) _startRenderPoint++;
		}
		if(_currentItemNum < 15) _currentItemNum++;
	}

	if(_cursorTurnOver && KEYMANAGER->isOnceKeyDown(VK_UP))
	{
		//��Ʈ �����ŭ �ø���.
		if(_backUpCursorRc.top < _extentRc.top + 50)
		{
			//���� 1ĭ�� �ø���.
			for(int i = 0; i < _totalItemNum; i++)
			{
				if(_currentItemNum > 0)
				{
					_inven[i].itemRc.top += 50;
					_inven[i].itemRc.bottom += 50;
				}
			}
			if(_currentItemNum > 0) _startRenderPoint--;
		}
		if(_currentItemNum > 0) _currentItemNum--;
	}

	if(_cursorTurnOver && KEYMANAGER->isOnceKeyDown(VK_RIGHT))
	{
		if(_isPushDownX == 1)
		{

		}

		else
		{

		}
	}

	if(_cursorTurnOver && KEYMANAGER->isOnceKeyDown('X'))
	{
		if(_isPushDownX >= 1 && !_choiceItem)
		{
			tagItem temp;
			
			_isPushDownX = 1;
		}
		if(_isPushDownX < 1 && _choiceItem)
		{
			_choiceItem = false;
			_choiceItemNum = 0;
			_isPushDownX = 0;
		}

		if(_isPushDownX >= 1 && _choiceItem)
		{
			//���� ������� �̹� �������� ������ ���̹Ƿ�,
			tagItem temp;

			_choiceItem = false;
			_choiceItemNum = 0;
			_isPushDownX = 0;
		}

		else if(!_choiceItem && _inven[_currentItemNum].name != "( IDLE )")
		{
			//�������� ������ ��
			_choiceItem = true;
			_choiceItemNum = _currentItemNum;
			_isPushDownX = 1;
		}
	}

	//���ÿ��� ����Ŀ���� �Ϲ�Ŀ���� �Բ� �����δ�.
	if(!_choiceItem) _cursorRc = RectMake(_inven[_currentItemNum].itemRc.left-15, _inven[_currentItemNum].itemRc.top+10, 50, 50);
	else _cursorRc = RectMake(_inven[_choiceItemNum].itemRc.left-15, _inven[_choiceItemNum].itemRc.top+10, 50, 50);
	
	_backUpCursorRc = RectMake(_inven[_currentItemNum].itemRc.left-15, _inven[_currentItemNum].itemRc.top+10, 50, 50);

	sprintf(str, "%d/%d", _currentItemNum + 1, _totalItemNum);

	//���� ����
	//if(KEYMANAGER->isOnceKeyDown('C'))
	if(_invenOff)
	{
		for(int i = 0; i < _totalItemNum; i++)
		{
			_inven[i].itemRc = RectMake(100, 100+(50*i), 300, 50);
		}
		writeInvenFile(_inven);
	}
	//������ ������ ���� ó��(�Լ� ����� ������)
	sprintf(_itemInfo.recoveryHP, "%d", _inven[_currentItemNum].recoveryHP);
	sprintf(_itemInfo.recoverySP, "%d", _inven[_currentItemNum].recoverySP);
	sprintf(_itemInfo.regulaterAttack, "%d", _inven[_currentItemNum].regulaterAttack);
	sprintf(_itemInfo.regulaterDefence, "%d", _inven[_currentItemNum].regulaterDefence);
	sprintf(_itemInfo.regulaterHit, "%d", _inven[_currentItemNum].regulaterHit);
	sprintf(_itemInfo.regulaterMagic, "%d", _inven[_currentItemNum].regulaterMagic);
	sprintf(_itemInfo.regulaterSpeed, "%d", _inven[_currentItemNum].regulaterSpeed);
	sprintf(_itemInfo.regulaterResistance, "%d", _inven[_currentItemNum].regulaterResistance);
	sprintf(_itemInfo.itemName, "%s", _inven[_currentItemNum].name);
	sprintf(_itemInfo.itemIndex, "%s", _inven[_currentItemNum].itemIndex);
	//sprintf(_itemInfo.regulaterHP, " - HP�ִ�ġ: %d", _vItem[_currentItemNum].regulaterHP);
	//sprintf(_itemInfo.regulaterSP, " - SP�ִ�ġ: %d", _vItem[_currentItemNum].regulaterSP);

	_info->update(_itemInfo);

}
void statinven::render(void)
{
	IMAGEMANAGER->findImage("inven")->render(getIMemDC(), _extentRc.left, _extentRc.top - 45);
	IMAGEMANAGER->findImage("progressbarimage")->render(getIMemDC(), _extentRc.right + 1, progressbarInven(_extentRc.top, _extentRc.bottom));

	HFONT font, oldfont;
	font = CreateFont(35, 0, 0, 0, 1000, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH, TEXT("���� ���"));
	oldfont = (HFONT)SelectObject(getIMemDC(), font);
	SetTextColor(getIMemDC(), RGB(255, 255, 255));
	SetBkMode(getIMemDC(), TRANSPARENT);
	for(int i = 0; i < _totalItemNum; i++)
	{
		if(_inven[i].itemRc.top >= _extentRc.top && _inven[i].itemRc.bottom <= _extentRc.bottom)
		{
			//Rectangle(getMemDC(), _inven[i].itemRc.left, _inven[i].itemRc.top, _inven[i].itemRc.right, _inven[i].itemRc.bottom);
			//_inven[i].img->render(getMemDC(), _inven[i].itemRc.left+30, _inven[i].itemRc.top);
			TextOut(getIMemDC(), _inven[i].itemRc.left+90, _inven[i].itemRc.top+5, _inven[i].name, lstrlen(_inven[i].name));
			//TextOut(getIMemDC(), _inven[i].itemRc.left+230, _inven[i].itemRc.top+20, _inven[i].str, lstrlen(_inven[i].str));
		}
	}
	TextOut(getIMemDC(), _extentRc.left + 250, _extentRc.top-40, str, lstrlen(str));
	SelectObject(getIMemDC(), oldfont);
	DeleteObject(font);

	if(_cursorTurnOver)
	{
		if(_cursorRc.top >= _extentRc.top && _cursorRc.bottom <= _extentRc.bottom + 50)
		{
			IMAGEMANAGER->findImage("cursorImage")->render(getIMemDC(), _cursorRc.left, _cursorRc.top);
		}
		IMAGEMANAGER->findImage("cursorImage")->render(getIMemDC(), _backUpCursorRc.left, _backUpCursorRc.top);
	}
	if(_cursorTurnOver && _inven[_currentItemNum].name != "( IDLE )") _info->render();

}
void statinven::setItemVector(tagItem it)
{
	//���� �������� 16�ڸ� �˻� �� ���� ���� NULL�� ���� ����ȴ�.
	for(int i = 0; i < _totalItemNum; i++)
	{
		if(_inven[i].name == "( IDLE )")
		{
			_inven[i].img = it.img;
			_inven[i].itemClass = it.itemClass;
			_inven[i].itemHell = it.itemHell;
			_inven[i].itemIndex = it.itemIndex;
			_inven[i].itemKind = it.itemKind;
			_inven[i].itemRc = RectMake(10, 100+(50*i), 300, 50);
			_inven[i].name = it.name;
			_inven[i].recoveryHP = it.recoveryHP;
			_inven[i].recoverySP = it.recoverySP;
			_inven[i].regulaterAttack = it.regulaterAttack;
			_inven[i].regulaterDefence = it.regulaterDefence;
			_inven[i].regulaterHit = it.regulaterHit;
			_inven[i].regulaterHP = it.regulaterHP;
			_inven[i].regulaterMagic = it.regulaterMagic;
			_inven[i].regulaterResistance = it.regulaterResistance;
			_inven[i].regulaterSP = it.regulaterSP;
			_inven[i].regulaterSpeed = it.regulaterSpeed;
			sprintf(_inven[i].str, "NULL");
			break;
		}
	}
}

bool statinven::getFullItem(void)
{
	for(int i = 0; i < _totalItemNum; i++)
	{
		if(_inven[i].name == "( IDLE )")
		{
			return true;
		}
	}
	return false;
}

void statinven::writeInvenFile(tagItem* arrayItem)
{
	//����� ��� ������, ������ ��ư�� ����� ������ ��ư�� �־ ���� ��Ë� 1���а� ������ 1������ ���
	HANDLE file;
	DWORD write;

	file = CreateFile("inventory.inv", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	WriteFile(file, arrayItem, sizeof(tagItem) * 16, &write, NULL);

	CloseHandle(file);
}

void statinven::readInvenFile(void)
{
	//�� ÷�� �б�
	HANDLE file;
	DWORD read;
	tagItem strItem[16];

	int isInFile; 

	file = CreateFile("inventory.inv", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	isInFile = ReadFile(file, strItem, sizeof(tagItem) * 16, &read, NULL);

	if(isInFile == 0)
	{
		for(int i = 0; i < _totalItemNum; i++)
		{
			_inven[i].img = IMAGEMANAGER->findImage("backitem");
			_inven[i].itemClass = (itemRarity)1;
			_inven[i].itemHell = NULL;
			_inven[i].itemIndex = NULL;
			_inven[i].itemKind = NULL;
			_inven[i].itemRc = RectMake(10, 100+(50*i), 300, 50);
			_inven[i].name = "( IDLE )";
			_inven[i].recoveryHP = NULL;
			_inven[i].recoverySP = NULL;
			_inven[i].regulaterAttack = NULL;
			_inven[i].regulaterDefence = NULL;
			_inven[i].regulaterHit = NULL;
			_inven[i].regulaterHP = NULL;
			_inven[i].regulaterMagic = NULL;
			_inven[i].regulaterResistance = NULL;
			_inven[i].regulaterSP = NULL;
			_inven[i].regulaterSpeed = NULL;
			sprintf(_inven[i].str, "NULL");
		}
	}
	else
	{
		for(int j = 0; j < 16; j++)
		{
			_inven[j] = strItem[j];
			_inven[j].itemRc = RectMake(10, 100+(50*j), 300, 50);
		}	
	}

	CloseHandle(file);

}


void statinven::setInvenItemArr(tagItem temp, int currentNum)
{
	RECT rcTemp;
	rcTemp = _inven[currentNum].itemRc;

	_inven[currentNum] = temp;
	_inven[currentNum].itemRc = rcTemp;
}

int statinven::progressbarInven(long top, long bottom)
{
	//top��ǥ(����)�� �����ͼ� currentItemNum�� ������ ���� �´� ����ŭ �ڷ� �����ش�.
	int currentTopPoint;
	currentTopPoint = top - 7 + ((( bottom - top) / _totalItemNum) * _currentItemNum);
	
	return currentTopPoint;
}
