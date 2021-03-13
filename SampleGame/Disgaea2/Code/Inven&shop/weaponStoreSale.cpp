
#include "stdafx.h"
#include "weaponStoreSale.h"


weaponStoreSale::weaponStoreSale()
{

}

weaponStoreSale::~weaponStoreSale()
{

}

HRESULT weaponStoreSale::init(void)
{
	_currentItemNum = 0;
	_isPushDownX = 0;
	_totalItemNum = 16;

	_totalMoney = 0;

	//커서 시작점 -> Item 탭이든 status 탭이든 인벤이 나오면 항상 인벤이 기준점
	_cursorTurnOver = false;
	_choiceItem = false; //아이템을 아직 선택하지 않은 상태

	_isPushDownC = false;
	_fileLode = false;

	//파일 읽기-> 파일이 있을 때는 그걸로 읽고 파일이 없을 때는 전부 Idle로 초기화
	readInStoreInvenFile();

	_shopImage = IMAGEMANAGER->findImage("shopimage");
	_hellslot = IMAGEMANAGER->findImage("hellslot");

	//롤 이미지 등록
	_rollimage = IMAGEMANAGER->findImage("progressbarimage");

	//일반 커서 초기화 부분
	_cursor = IMAGEMANAGER->findImage("cursorImage");
	_cursorRc = RectMake(_inStoreinven[_currentItemNum].itemRc.left-15, _inStoreinven[_currentItemNum].itemRc.top+10, 50, 50);
	
	//움직일 랙트 틀에 대한 정의(이걸로 충돌처리 해서 슬라이드를 넘긴다)
	_extentRc = RectMake(_inStoreinven[0].itemRc.left, _inStoreinven[0].itemRc.top, 300, 400);

	_info = new itemInfo;
	_info->init();

	return S_OK;
}

void weaponStoreSale::release(void)
{
}

void weaponStoreSale::update(int money)
{
	_totalMoney = money;
	if(_fileLode)
	{
		readInStoreInvenFile();
		_fileLode = false;
	}

	if(KEYMANAGER->isOnceKeyDown(VK_DOWN))
	{
		SOUNDMANAGER->play("menuMove", 1.0f);
		//랙트 사이즈만큼 올린다.
		if(_cursorRc.bottom >= _extentRc.bottom)
		{
			//전부 1칸씩 올린다.
			for(int i = 0; i < _totalItemNum; i++)
			{
				if(_currentItemNum < 15)
				{
					_inStoreinven[i].itemRc.top -= 50;
					_inStoreinven[i].itemRc.bottom -= 50;
				}
			}
		}
		if(_currentItemNum < 15) _currentItemNum++;
	}

	if(KEYMANAGER->isOnceKeyDown(VK_UP))
	{
		SOUNDMANAGER->play("menuMove", 1.0f);
		//랙트 사이즈만큼 올린다.
		if(_cursorRc.top < _extentRc.top + 50)
		{
			//전부 1칸씪 올린다.
			for(int i = 0; i < _totalItemNum; i++)
			{
				if(_currentItemNum > 0)
				{
					_inStoreinven[i].itemRc.top += 50;
					_inStoreinven[i].itemRc.bottom += 50;
				}
			}
		}
		if(_currentItemNum > 0) _currentItemNum--;
	}

	if(KEYMANAGER->isOnceKeyDown('X'))
	{
		SOUNDMANAGER->play("menuselect", 1.0f);
		if(_inStoreinven[_currentItemNum].name != "( IDLE )")
		{
			tagItem temp;
			temp.img = IMAGEMANAGER->findImage("backitem");
			temp.itemClass = (itemRarity)1;
			temp.itemHell = NULL;
			temp.itemIndex = NULL;
			temp.itemKind = NULL;
			temp.itemRc = RectMake(50, 140 + ( 50 * _currentItemNum), 300, 50);
			temp.name = "( IDLE )";
			temp.recoveryHP = NULL;
			temp.recoverySP = NULL;
			temp.regulaterAttack = NULL;
			temp.regulaterDefence = NULL;
			temp.regulaterHit = NULL;
			temp.regulaterHP = NULL;
			temp.regulaterMagic = NULL;
			temp.regulaterResistance = NULL;
			temp.regulaterSP = NULL;
			temp.regulaterSpeed = NULL;
			sprintf(temp.str, "NULL");

			_totalMoney += _inStoreinven[_currentItemNum].itemHell;

			_inStoreinven[_currentItemNum] = temp;
		}
	}

	//평상시에는 예비커서랑 일반커서가 함께 움직인다.
	_cursorRc = RectMake(_inStoreinven[_currentItemNum].itemRc.left-15, _inStoreinven[_currentItemNum].itemRc.top+10, 50, 50);

	sprintf(str, "%d/%d", _currentItemNum + 1, _totalItemNum);


	//파일 저장
	if(_isPushDownC) //KEYMANAGER->isOnceKeyDown('C'))
	{
		for(int i = 0; i < _totalItemNum; i++)
		{
			_inStoreinven[i].itemRc = RectMake(50, 140 + ( 50 * i ), 300, 50);
		}
		writeInStoreInvenFile(_inStoreinven);
		_isPushDownC = false;
	}

	sprintf(arrHell, "%d", _totalMoney);

	//아이템 인포를 위한 처리(함수 만들기 귀찮다)
	sprintf(_itemInfo.recoveryHP, "%d", _inStoreinven[_currentItemNum].recoveryHP);
	sprintf(_itemInfo.recoverySP, "%d", _inStoreinven[_currentItemNum].recoverySP);
	sprintf(_itemInfo.regulaterAttack, "%d", _inStoreinven[_currentItemNum].regulaterAttack);
	sprintf(_itemInfo.regulaterDefence, "%d", _inStoreinven[_currentItemNum].regulaterDefence);
	sprintf(_itemInfo.regulaterHit, "%d", _inStoreinven[_currentItemNum].regulaterHit);
	sprintf(_itemInfo.regulaterMagic, "%d", _inStoreinven[_currentItemNum].regulaterMagic);
	sprintf(_itemInfo.regulaterSpeed, "%d", _inStoreinven[_currentItemNum].regulaterSpeed);
	sprintf(_itemInfo.regulaterResistance, "%d", _inStoreinven[_currentItemNum].regulaterResistance);
	sprintf(_itemInfo.itemName, "%s", _inStoreinven[_currentItemNum].name);
	sprintf(_itemInfo.itemIndex, "%s", _inStoreinven[_currentItemNum].itemIndex);

	_info->update(_itemInfo);
}

void weaponStoreSale::render(void)
{
	IMAGEMANAGER->findImage("storebg2")->render(getIMemDC(), 0, 0);
	IMAGEMANAGER->findImage("shopimage")->render(getIMemDC(), _extentRc.left, _extentRc.top - 50);
	IMAGEMANAGER->findImage("progressbarimage")->render(getIMemDC(), _extentRc.right + 268, progressbarSaleInven(_extentRc.top, _extentRc.bottom));
	IMAGEMANAGER->findImage("hellslot")->render(getIMemDC(), 50, 590);

	HFONT font, oldfont;
	font = CreateFont(35, 0, 0, 0, 1000, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH, TEXT("맑은 고딕"));
	oldfont = (HFONT)SelectObject(getIMemDC(), font);
	SetTextColor(getIMemDC(), RGB(255, 255, 255));
	SetBkMode(getIMemDC(), TRANSPARENT);
	for(int i = 0; i < _totalItemNum; i++)
	{
		if(_inStoreinven[i].itemRc.top >= _extentRc.top && _inStoreinven[i].itemRc.bottom <= _extentRc.bottom)
		{
			//Rectangle(getMemDC(), _inven[i].itemRc.left, _inven[i].itemRc.top, _inven[i].itemRc.right, _inven[i].itemRc.bottom);
			//_inStoreinven[i].img->render(getMemDC(), _inStoreinven[i].itemRc.left+30, _inStoreinven[i].itemRc.top);
			TextOut(getIMemDC(), _inStoreinven[i].itemRc.left+90, _inStoreinven[i].itemRc.top, _inStoreinven[i].name, lstrlen(_inStoreinven[i].name));
			if(_inStoreinven[i].name != "( IDLE )")
			{
				TextOut(getIMemDC(), _inStoreinven[i].itemRc.left+230, _inStoreinven[i].itemRc.top, _inStoreinven[i].str, lstrlen(_inStoreinven[i].str));
			}
		}
	}
	if(_inStoreinven[_currentItemNum].name != "( IDLE )")
	{
		_info->render();
	}
	TextOut(getIMemDC(), 250 - (unitNum(_totalMoney) * 14), 595, arrHell, lstrlen(arrHell));
	TextOut(getIMemDC(), _extentRc.left + 520, _extentRc.top-40, str, lstrlen(str));
	SelectObject(getIMemDC(), oldfont);
	DeleteObject(font);

	if(_cursorRc.top >= _extentRc.top && _cursorRc.bottom <= _extentRc.bottom + 50)
	{
		IMAGEMANAGER->findImage("cursorImage")->render(getIMemDC(), _cursorRc.left, _cursorRc.top);
	}
}

void weaponStoreSale::readInStoreInvenFile(void)
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
			_inStoreinven[i].img = IMAGEMANAGER->findImage("backitem");
			_inStoreinven[i].itemClass = (itemRarity)1;
			_inStoreinven[i].itemHell = NULL;
			_inStoreinven[i].itemIndex = NULL;
			_inStoreinven[i].itemKind = NULL;
			_inStoreinven[i].itemRc = RectMake(50, 140+(50*i), 300, 50);
			_inStoreinven[i].name = "( IDLE )";
			_inStoreinven[i].recoveryHP = NULL;
			_inStoreinven[i].recoverySP = NULL;
			_inStoreinven[i].regulaterAttack = NULL;
			_inStoreinven[i].regulaterDefence = NULL;
			_inStoreinven[i].regulaterHit = NULL;
			_inStoreinven[i].regulaterHP = NULL;
			_inStoreinven[i].regulaterMagic = NULL;
			_inStoreinven[i].regulaterResistance = NULL;
			_inStoreinven[i].regulaterSP = NULL;
			_inStoreinven[i].regulaterSpeed = NULL;
			sprintf(_inStoreinven[i].str, "NULL");
		}
	}
	else
	{
		for(int j = 0; j < 16; j++)
		{
			_inStoreinven[j] = strItem[j];
			_inStoreinven[j].itemRc = RectMake(50, 140+(50*j), 300, 50);
			wsprintf(_inStoreinven[j].str, "                 HL           %d", _inStoreinven[j].itemHell);
		}	
	}

	CloseHandle(file);

}
void weaponStoreSale::writeInStoreInvenFile(tagItem* arrayItem)
{
	//현재는 계속 쓰지만, 나가는 버튼이 생기면 나가는 버튼에 넣어서 상점 들올떄 1번읽고 나갈때 1번쓰는 방식
	HANDLE file;
	DWORD write;

	file = CreateFile("inventory.inv", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	WriteFile(file, arrayItem, sizeof(tagItem) * 16, &write, NULL);

	CloseHandle(file);
}

int weaponStoreSale::progressbarSaleInven(long top, long bottom)
{
	//top좌표(끝점)를 가져와서 currentItemNum이 증가할 수록 맞는 점만큼 뒤로 더해준다.
	int currentTopPoint;
	currentTopPoint = top + (((bottom - top - 20) / _totalItemNum) * _currentItemNum);
	
	return currentTopPoint;
}