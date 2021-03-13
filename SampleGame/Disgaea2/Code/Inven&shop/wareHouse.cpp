
#include "stdafx.h"
#include "wareHouse.h"
#include "inventory.h"

wareHouse::wareHouse()
{

}

wareHouse::~wareHouse()
{

}

HRESULT wareHouse::init(void)
{
	_info = new itemInfo;
	_info->init();

	_currentItemNum = 0;

	_isPushDownX = 0;
	_totalItemNum = 45;
	_startRenderPoint = 0;
	_cursorTurnOver = false;
	_choiceItemNum = 0;
	_choiceItem = false;

	_warehouseImage = IMAGEMANAGER->addImage("warehouseEx", "UI_IMAGE/warehouse.bmp", 320, 400, false, RGB(255, 0, 255));
	_rollimage = IMAGEMANAGER->findImage("progressbarimage");

	//아무것도 없는 상태 즉 ( IDLE )를 설정
	for(int i = 0; i < _totalItemNum; i++)
	{
		_house[i].img = IMAGEMANAGER->findImage("backitem");
		_house[i].itemClass = (itemRarity)1;
		_house[i].itemHell = NULL;
		_house[i].itemIndex = NULL;
		_house[i].itemKind = NULL;
		_house[i].itemRc = RectMake(340, 100+(50*i), 300, 50);
		_house[i].name = "( IDLE )";
		_house[i].recoveryHP = NULL;
		_house[i].recoverySP = NULL;
		_house[i].regulaterAttack = NULL;
		_house[i].regulaterDefence = NULL;
		_house[i].regulaterHit = NULL;
		_house[i].regulaterHP = NULL;
		_house[i].regulaterMagic = NULL;
		_house[i].regulaterResistance = NULL;
		_house[i].regulaterSP = NULL;
		_house[i].regulaterSpeed = NULL;
		sprintf(_house[i].str, "NULL");
	}

	_cursor = IMAGEMANAGER->findImage("cursorImage");
	_cursorRc = RectMake(_house[_currentItemNum].itemRc.left-15, _house[_currentItemNum].itemRc.top+10, 50, 50);

	//예비 커서 초기화 부분
	_backUpCursor = IMAGEMANAGER->findImage("cursorImage");
	_backUpCursorRc = RectMake(_house[_currentItemNum].itemRc.left-15, _house[_currentItemNum].itemRc.top+10, 50, 50);
	
	_extentRc = RectMake(_house[0].itemRc.left, _house[0].itemRc.top, 300, 350);

	return S_OK;
}
void wareHouse::release(void)
{

}

void wareHouse::update(void)
{

	if(_cursorTurnOver && KEYMANAGER->isOnceKeyDown(VK_DOWN))
	{
		//랙트 사이즈만큼 올린다.
		if( _backUpCursorRc.bottom >= _extentRc.bottom)
		{
			//전부 1칸씪 올린다.
			for(int i = 0; i < _totalItemNum; i++)
			{
				if(_currentItemNum < _totalItemNum - 1)
				{
					_house[i].itemRc.top -= 50;
					_house[i].itemRc.bottom -= 50;
				}
			}
			if(_currentItemNum < _totalItemNum - 1) _startRenderPoint++;
		}
		if(_currentItemNum < _totalItemNum - 1) _currentItemNum++;
	}

	if(_cursorTurnOver && KEYMANAGER->isOnceKeyDown(VK_UP))
	{
		//랙트 사이즈만큼 올린다.
		if( _backUpCursorRc.top < _extentRc.top + 50)
		{
			//전부 1칸씪 올린다.
			for(int i = 0; i < _totalItemNum; i++)
			{
				if(_currentItemNum > 0)
				{
					_house[i].itemRc.top += 50;
					_house[i].itemRc.bottom += 50;
				}
			}
			if(_currentItemNum > 0) _startRenderPoint--;
		}
		if(_currentItemNum > 0) _currentItemNum--;
	}

	if(_cursorTurnOver && KEYMANAGER->isOnceKeyDown(VK_LEFT))
	{
		if(_isPushDownX == 1)
		{
			tagItem temp;
			temp = _inven->getInvenItemArr(_inven->getStartRenderPoint() + (_currentItemNum - _startRenderPoint));
			_inven->setPushDownX(_isPushDownX);

			_inven->setCurrentItemNum(_inven->getStartRenderPoint() + (_currentItemNum - _startRenderPoint));
			_cursorTurnOver = false;
			_inven->setCursorTurnOver(true);
		}

		else
		{
			_inven->setCurrentItemNum(_inven->getStartRenderPoint() + (_currentItemNum - _startRenderPoint));
			_cursorTurnOver = false;
			_inven->setCursorTurnOver(true);
		}
	}

	if(_cursorTurnOver && KEYMANAGER->isOnceKeyDown('X'))
	{
		if(_isPushDownX >= 1 && !_choiceItem)
		{
			tagItem temp;
			temp = _inven->getInvenItemArr(_inven->getChoiceItemNum());
			_inven->setInvenItemArr(_house[_currentItemNum], _inven->getChoiceItemNum());
			temp.itemRc = _house[_currentItemNum].itemRc;

			_house[_currentItemNum] = temp;

			_inven->setPushDownX(0);
			_inven->setChoiceItem(false);
			_choiceItem = true;
			_choiceItemNum = _currentItemNum;
			
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
			//여길 들왔으면 이미 아이템을 선택한 것이므로,
			tagItem temp;
			temp = _house[_currentItemNum];
			_house[_currentItemNum] = _house[_choiceItemNum];
			_house[_choiceItemNum] = temp;

			temp.itemRc = _house[_currentItemNum].itemRc;
			_house[_currentItemNum].itemRc = _house[_choiceItemNum].itemRc;
			_house[_choiceItemNum].itemRc = temp.itemRc;

			_choiceItem = false;
			_choiceItemNum = 0;
			_isPushDownX = 0;
		}

		else if(!_choiceItem && _house[_currentItemNum].name != "( IDLE )")
		{
			//아이템을 선택한 것
			_choiceItem = true;
			_choiceItemNum = _currentItemNum;
			_isPushDownX = 1;
		}
	}

	//아이템 인포를 위한 처리(함수 만들기 귀찮다)
	sprintf(_itemInfo.recoveryHP, "%d", _house[_currentItemNum].recoveryHP);
	sprintf(_itemInfo.recoverySP, "%d", _house[_currentItemNum].recoverySP);
	sprintf(_itemInfo.regulaterAttack, "%d", _house[_currentItemNum].regulaterAttack);
	sprintf(_itemInfo.regulaterDefence, "%d", _house[_currentItemNum].regulaterDefence);
	sprintf(_itemInfo.regulaterHit, "%d", _house[_currentItemNum].regulaterHit);
	sprintf(_itemInfo.regulaterMagic, "%d", _house[_currentItemNum].regulaterMagic);
	sprintf(_itemInfo.regulaterSpeed, "%d", _house[_currentItemNum].regulaterSpeed);
	sprintf(_itemInfo.regulaterResistance, "%d", _house[_currentItemNum].regulaterResistance);
	sprintf(_itemInfo.itemName, "%s", _house[_currentItemNum].name);
	sprintf(_itemInfo.itemIndex, "%s", _house[_currentItemNum].itemIndex);
	//sprintf(_itemInfo.regulaterHP, " - HP최대치: %d", _vItem[_currentItemNum].regulaterHP);
	//sprintf(_itemInfo.regulaterSP, " - SP최대치: %d", _vItem[_currentItemNum].regulaterSP);

	_info->update(_itemInfo);

	if(!_choiceItem) _cursorRc = RectMake(_house[_currentItemNum].itemRc.left-15, _house[_currentItemNum].itemRc.top+10, 50, 50);
	else _cursorRc = RectMake(_house[_choiceItemNum].itemRc.left-15, _house[_choiceItemNum].itemRc.top+10, 50, 50);
	
	_backUpCursorRc = RectMake(_house[_currentItemNum].itemRc.left-15, _house[_currentItemNum].itemRc.top+10, 50, 50);
	sprintf(str, "%d/%d", _currentItemNum + 1, _totalItemNum);
}

void wareHouse::render(void)
{
	IMAGEMANAGER->findImage("warehouseEx")->render(getIMemDC(), _extentRc.left, _extentRc.top - 45);
	IMAGEMANAGER->findImage("progressbarimage")->render(getIMemDC(), _extentRc.right + 1, progressbarInven(_extentRc.top, _extentRc.bottom));
	HFONT font, oldfont;
	font = CreateFont(35, 0, 0, 0, 1000, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH, TEXT("맑은 고딕"));
	oldfont = (HFONT)SelectObject(getIMemDC(), font);
	SetTextColor(getIMemDC(), RGB(255, 255, 255));
	SetBkMode(getIMemDC(), TRANSPARENT);
	for(int i = 0; i < 50; i++)
	{
		if(_house[i].itemRc.top >= _extentRc.top && _house[i].itemRc.bottom <= _extentRc.bottom)
		{
			//Rectangle(getMemDC(), _house[i].itemRc.left, _house[i].itemRc.top, _house[i].itemRc.right, _house[i].itemRc.bottom);
			//_house[i].img->render(getMemDC(), _house[i].itemRc.left+30, _house[i].itemRc.top);
			TextOut(getIMemDC(), _house[i].itemRc.left+90, _house[i].itemRc.top+8, _house[i].name, lstrlen(_house[i].name));
			//TextOut(getMemDC(), _inven[i].itemRc.left+230, _inven[i].itemRc.top+20, _inven[i].str, lstrlen(_inven[i].str));
		}
	}

	TextOut(getIMemDC(), _extentRc.left+240, _extentRc.top-40, str, lstrlen(str));
	
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
	if(_cursorTurnOver && _house[_currentItemNum].name != "( IDLE )") _info->render();
}

void wareHouse::sethouseItemArr(tagItem temp, int currentNum)
{
	RECT rcTemp;
	rcTemp = _house[currentNum].itemRc;

	_house[currentNum] = temp;
	_house[currentNum].itemRc = rcTemp;
}

void wareHouse::writeWarehouseFile(tagItem* arrayItem)
{
	//현재는 계속 쓰지만, 나가는 버튼이 생기면 나가는 버튼에 넣어서 상점 들올떄 1번읽고 나갈때 1번쓰는 방식
	HANDLE file;
	DWORD write;

	file = CreateFile("warehouse.inv", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	WriteFile(file, arrayItem, sizeof(tagItem) * 50, &write, NULL);

	CloseHandle(file);
}

void wareHouse::readWarehouseFile(void)
{
	//맨 첨에 읽기
	HANDLE file;
	DWORD read;
	tagItem strItem[50];

	int isInFile; 

	file = CreateFile("warehouse.inv", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	isInFile = ReadFile(file, strItem, sizeof(tagItem) * 50, &read, NULL);

	if(isInFile == 0)
	{
		for(int i = 0; i < _totalItemNum; i++)
		{
			_house[i].img = IMAGEMANAGER->findImage("backitem");
			_house[i].itemClass = (itemRarity)1;
			_house[i].itemHell = NULL;
			_house[i].itemIndex = NULL;
			_house[i].itemKind = NULL;
			_house[i].itemRc = RectMake(340, 100+(50*i), 300, 50);
			_house[i].name = "( IDLE )";
			_house[i].recoveryHP = NULL;
			_house[i].recoverySP = NULL;
			_house[i].regulaterAttack = NULL;
			_house[i].regulaterDefence = NULL;
			_house[i].regulaterHit = NULL;
			_house[i].regulaterHP = NULL;
			_house[i].regulaterMagic = NULL;
			_house[i].regulaterResistance = NULL;
			_house[i].regulaterSP = NULL;
			_house[i].regulaterSpeed = NULL;
			sprintf(_house[i].str, "NULL");
		}
	}
	else
	{
		for(int j = 0; j < 50; j++)
		{
			_house[j] = strItem[j];
			_house[j].itemRc = RectMake(340, 100+(50*j), 300, 50);
		}
		//난중에 랙트 재정의
	}

	CloseHandle(file);

}


int wareHouse::progressbarInven(long top, long bottom)
{
	//top좌표(끝점)를 가져와서 currentItemNum이 증가할 수록 맞는 점만큼 뒤로 더해준다.
	int currentTopPoint;
	currentTopPoint = top - 7 + ((( bottom - top) / _totalItemNum) * _currentItemNum);
	
	return currentTopPoint;
}