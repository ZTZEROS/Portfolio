
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
	//커서 시작점 -> Item 탭이든 status 탭이든 인벤이 나오면 항상 인벤이 기준점
	_cursorTurnOver = true;
	_choiceItem = false; //아이템을 아직 선택하지 않은 상태
	_itemRead = false;

	//파일 읽기-> 파일이 있을 때는 그걸로 읽고 파일이 없을 때는 전부 Idle로 초기화
	readInvenFile();

	_invenImage = IMAGEMANAGER->addImage("inven", "UI_IMAGE/inven.bmp", 320, 400, false, RGB(255, 0, 255));

	//롤 이미지 등록
	_rollimage = IMAGEMANAGER->findImage("progressbarimage");

	//일반 커서 초기화 부분
	_cursor = IMAGEMANAGER->findImage("cursorImage");
	_cursorRc = RectMake(_inven[_currentItemNum].itemRc.left-15, _inven[_currentItemNum].itemRc.top+10, 50, 50);
	
	//예비 커서 초기화 부분
	_backUpCursor = IMAGEMANAGER->findImage("cursorImage");
	_backUpCursorRc = RectMake(_inven[_currentItemNum].itemRc.left-15, _inven[_currentItemNum].itemRc.top+10, 50, 50);

	//움직일 랙트 틀에 대한 정의(이걸로 충돌처리 해서 슬라이드를 넘긴다)
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
		//랙트 사이즈만큼 올린다.
		if(_backUpCursorRc.bottom >= _extentRc.bottom)
		{
			//전부 1칸씩 올린다.
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
		//랙트 사이즈만큼 올린다.
		if(_backUpCursorRc.top < _extentRc.top + 50)
		{
			//전부 1칸씪 올린다.
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
			//여길 들왔으면 이미 아이템을 선택한 것이므로,
			tagItem temp;

			_choiceItem = false;
			_choiceItemNum = 0;
			_isPushDownX = 0;
		}

		else if(!_choiceItem && _inven[_currentItemNum].name != "( IDLE )")
		{
			//아이템을 선택한 것
			_choiceItem = true;
			_choiceItemNum = _currentItemNum;
			_isPushDownX = 1;
		}
	}

	//평상시에는 예비커서랑 일반커서가 함께 움직인다.
	if(!_choiceItem) _cursorRc = RectMake(_inven[_currentItemNum].itemRc.left-15, _inven[_currentItemNum].itemRc.top+10, 50, 50);
	else _cursorRc = RectMake(_inven[_choiceItemNum].itemRc.left-15, _inven[_choiceItemNum].itemRc.top+10, 50, 50);
	
	_backUpCursorRc = RectMake(_inven[_currentItemNum].itemRc.left-15, _inven[_currentItemNum].itemRc.top+10, 50, 50);

	sprintf(str, "%d/%d", _currentItemNum + 1, _totalItemNum);

	//파일 저장
	//if(KEYMANAGER->isOnceKeyDown('C'))
	if(_invenOff)
	{
		for(int i = 0; i < _totalItemNum; i++)
		{
			_inven[i].itemRc = RectMake(100, 100+(50*i), 300, 50);
		}
		writeInvenFile(_inven);
	}
	//아이템 인포를 위한 처리(함수 만들기 귀찮다)
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
	//sprintf(_itemInfo.regulaterHP, " - HP최대치: %d", _vItem[_currentItemNum].regulaterHP);
	//sprintf(_itemInfo.regulaterSP, " - SP최대치: %d", _vItem[_currentItemNum].regulaterSP);

	_info->update(_itemInfo);

}
void statinven::render(void)
{
	IMAGEMANAGER->findImage("inven")->render(getIMemDC(), _extentRc.left, _extentRc.top - 45);
	IMAGEMANAGER->findImage("progressbarimage")->render(getIMemDC(), _extentRc.right + 1, progressbarInven(_extentRc.top, _extentRc.bottom));

	HFONT font, oldfont;
	font = CreateFont(35, 0, 0, 0, 1000, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH, TEXT("맑은 고딕"));
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
	//들어온 아이템은 16자리 검사 후 가장 상위 NULL로 가서 복사된다.
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
	//현재는 계속 쓰지만, 나가는 버튼이 생기면 나가는 버튼에 넣어서 상점 들올떄 1번읽고 나갈때 1번쓰는 방식
	HANDLE file;
	DWORD write;

	file = CreateFile("inventory.inv", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	WriteFile(file, arrayItem, sizeof(tagItem) * 16, &write, NULL);

	CloseHandle(file);
}

void statinven::readInvenFile(void)
{
	//맨 첨에 읽기
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
	//top좌표(끝점)를 가져와서 currentItemNum이 증가할 수록 맞는 점만큼 뒤로 더해준다.
	int currentTopPoint;
	currentTopPoint = top - 7 + ((( bottom - top) / _totalItemNum) * _currentItemNum);
	
	return currentTopPoint;
}
