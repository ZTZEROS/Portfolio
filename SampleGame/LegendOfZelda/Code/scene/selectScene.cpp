#include "stdafx.h"
#include "../stdafx.h"
#include "selectScene.h"


selectScene::selectScene()
	:_fairyY(230), _slot(SLOT1),_selectionScreen(CHARACTER_SELECT), _alphabetLoopX(0)
	, _horizontaliLineLoopY(480), _alphabetLevel(TOP), _letterCount(0),_deleteScreen(QUIT)
{
}


selectScene::~selectScene()
{
}
HRESULT selectScene::init()
{
	fairyInit();
	return S_OK;
}
void selectScene::release()
{
	SAFE_DELETE(_fairyAni);
	//SAFE_DELETE_ARRAY(_rcCapitalLetter);
}
void selectScene::update()
{
	selectionUpdate();

}
void selectScene::render()
{
	selectionRender();
	char str[123];
	sprintf(str, "%d", _selectionScreen);
	TextOut(getMemDC(), 100, 100, str, strlen(str));
}
void selectScene:: fairyInit(void)
{
	//****************************//
	//			요정애니 등록		  //
	//****************************//
	_fairyAni = new animation;
	_fairyAni->init(32 * IMAGESCALE, 17 * IMAGESCALE, 16 * IMAGESCALE, 17 * IMAGESCALE);
	_fairyAni->setDefPlayFrame(true, true);
	_fairyAni->setFPS(3);
	_fairyAni->start();
	_vName = TXTDATA->txtLoad("UIManager/SlotName0.txt");
	_vName2 = TXTDATA->txtLoad("UIManager/SlotName1.txt");
	_vName3 = TXTDATA->txtLoad("UIManager/SlotName2.txt");
	if(_vName.size() ==NULL)_slots[SLOT1].slotCharacter = SLOTEMPTY;
	else _slots[SLOT1].slotCharacter = SLOTOCCUPIED;
	if (_vName2.size() == NULL)_slots[SLOT2].slotCharacter = SLOTEMPTY;
	else _slots[SLOT2].slotCharacter = SLOTOCCUPIED;
	if (_vName3.size() == NULL)_slots[SLOT3].slotCharacter = SLOTEMPTY;
	else _slots[SLOT3].slotCharacter = SLOTOCCUPIED;
	_slots[COPYPLAYER].slotCharacter = NONE;
	_slots[DELETEPLAYER].slotCharacter = NONE;
}
void selectScene:: fairyUpdate(void)
{
	_fairyAni->frameUpdate(0.1f); //요정 날개 파닥파닥프레임 업댓
	switch (_selectionScreen)
	{
	case CHARACTER_SELECT:
		switch (_slot)
		{
		case SLOT1:
			_fairyY = 230;
			if (KEYMANAGER->isOnceKeyDown(VK_DOWN))_slot = SLOT2;
			if (KEYMANAGER->isOnceKeyDown(VK_UP))_slot = DELETEPLAYER;
			break;
		case SLOT2:
			_fairyY = 340;
			if (KEYMANAGER->isOnceKeyDown(VK_DOWN))_slot = SLOT3;
			if (KEYMANAGER->isOnceKeyDown(VK_UP))_slot = SLOT1;
			break;
		case SLOT3:
			_fairyY = 450;
			if (KEYMANAGER->isOnceKeyDown(VK_DOWN))_slot = COPYPLAYER;
			if (KEYMANAGER->isOnceKeyDown(VK_UP))_slot = SLOT2;
			break;
		case COPYPLAYER:
			_fairyY = 580;
			if (KEYMANAGER->isOnceKeyDown(VK_DOWN))_slot = DELETEPLAYER;
			if (KEYMANAGER->isOnceKeyDown(VK_UP))_slot = SLOT3;
			break;
		case DELETEPLAYER:
			_fairyY = 650;
			if (KEYMANAGER->isOnceKeyDown(VK_DOWN))_slot = SLOT1;
			if (KEYMANAGER->isOnceKeyDown(VK_UP))_slot = COPYPLAYER;
			break;
		}
		break;
	case CHARACTER_DELETE:
		switch (_deleteScreen)
		{
		case D_SLOT1:
			_fairyY = 340;
			if (KEYMANAGER->isOnceKeyDown(VK_DOWN)) _deleteScreen = D_SLOT2;
			if (KEYMANAGER->isOnceKeyDown(VK_UP)) _deleteScreen = QUIT;
			break;
		case D_SLOT2:
			_fairyY = 430;
			if (KEYMANAGER->isOnceKeyDown(VK_DOWN)) _deleteScreen = D_SLOT3;
			if (KEYMANAGER->isOnceKeyDown(VK_UP)) _deleteScreen = D_SLOT1;
			break;
		case D_SLOT3:
			_fairyY = 520;
			if (KEYMANAGER->isOnceKeyDown(VK_DOWN)) _deleteScreen = QUIT;
			if (KEYMANAGER->isOnceKeyDown(VK_UP))	_deleteScreen = D_SLOT2;
			break;
		case QUIT:
			_fairyY = 650;
			if (KEYMANAGER->isOnceKeyDown(VK_DOWN)) _deleteScreen = D_SLOT1;
			if (KEYMANAGER->isOnceKeyDown(VK_UP))	_deleteScreen = D_SLOT3;
			break;
		}
		break;
	}
		

}

void selectScene::adjustCharacterInfo(void)
{
	_slots[DELETEPLAYER].slotCharacter = NONE;
	if (KEYMANAGER->isOnceKeyDown(VK_RETURN))
	{
		switch (_selectionScreen)
		{
		case CHARACTER_SELECT:
			switch (_slots[_slot].slotCharacter)
			{
			case NONE:
				if (_slot == DELETEPLAYER)
				{
					_selectionScreen = CHARACTER_DELETE;
				}
				break;
			case SLOTEMPTY:
				_selectionScreen = CHARACTER_NAMING;
				_rcCapitalLetter = new RECT[30];
				for (int i = 0; i < 3; i++)
				{
					for (int j = 0; j < 10; j++)
					{
						_rcCapitalLetter[j + i * 10] = RectMakeCenter(130 + j * 64, 455 + i * 60, 20, 50);
					}
				}
				_alphabetLoopX = 0;
				for (int i = 0; i < 6; i++)
				{
					_nameRect[i] = RectMakeCenter(130 + i * 64, 340, 50, 70);
				}
				_endRect = RectMakeCenter(685, 650, 100, 50);
				break;
			case SLOTOCCUPIED:
				SCENEMANAGER->changeScene("게임");
				break;
			}
			break;
		case CHARACTER_DELETE:
			switch (_deleteScreen)
			{
			case D_SLOT1:
				_vName.clear();
				_vStr.clear();
				TXTDATA->txtSave("UIManager/SlotName0.txt", _vStr);
				_slots[SLOT1].slotCharacter = SLOTEMPTY;
				break;
			case D_SLOT2:
				_vName2.clear();
				_vStr.clear();
				TXTDATA->txtSave("UIManager/SlotName1.txt", _vStr);
				_slots[SLOT2].slotCharacter = SLOTEMPTY;
				break;
			case D_SLOT3:
				_vName3.clear();
				_vStr.clear();
				TXTDATA->txtSave("UIManager/SlotName2.txt", _vStr);
				_slots[SLOT3].slotCharacter = SLOTEMPTY;
				break;
			case QUIT:
				_selectionScreen = CHARACTER_SELECT;
				break;
			}
			break;
		}
	
			

	}
}


void selectScene::selectionUpdate(void)
{
	switch (_selectionScreen)
	{
	case CHARACTER_SELECT:
		fairyUpdate();
		adjustCharacterInfo();
		break;
	case CHARACTER_NAMING:
		if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
		{
			_alphabetLoopX += 3;
			for (int i = 0; i < 30; i++)
			{
				_rcCapitalLetter[i].left -= 3;
				_rcCapitalLetter[i].right -= 3;
				
			}
			_endRect.left -= 3;
			_endRect.right -= 3;
		}
		if (KEYMANAGER->isStayKeyDown(VK_LEFT))
		{
			_alphabetLoopX -= 3;
			for (int i = 0; i < 30; i++)
			{
				_rcCapitalLetter[i].left += 3;
				_rcCapitalLetter[i].right += 3;
				
			}
			_endRect.left += 3;
			_endRect.right += 3;
		}
		switch (_alphabetLevel)
		{
		case TOP:
			if (_horizontaliLineLoopY >= 460)_horizontaliLineLoopY -= 5;
			if (KEYMANAGER->isOnceKeyDown(VK_DOWN))_alphabetLevel = MIDDLE;
			_alphabetSniper = PointMake(WINSIZEX / 2, 460);
			break;
		case MIDDLE:
			if (_horizontaliLineLoopY <= 520)_horizontaliLineLoopY += 5;
			if (_horizontaliLineLoopY >= 520)_horizontaliLineLoopY -= 5;
			if (KEYMANAGER->isOnceKeyDown(VK_DOWN))_alphabetLevel = BOTTOM;
			if (KEYMANAGER->isOnceKeyDown(VK_UP))_alphabetLevel = TOP;
			_alphabetSniper = PointMake(WINSIZEX / 2, 520);
			break;
		case BOTTOM:
			if (_horizontaliLineLoopY <= 580)_horizontaliLineLoopY += 5;
			if (_horizontaliLineLoopY >= 580)_horizontaliLineLoopY -= 5;
			if (KEYMANAGER->isOnceKeyDown(VK_UP))_alphabetLevel = MIDDLE;
			if (KEYMANAGER->isOnceKeyDown(VK_DOWN))_alphabetLevel = ROCKBOTTOM;
			_alphabetSniper = PointMake(WINSIZEX / 2, 580);
			break;
		case ROCKBOTTOM:
			if (_horizontaliLineLoopY <= 650)_horizontaliLineLoopY += 5;
			if (KEYMANAGER->isOnceKeyDown(VK_UP)) _alphabetLevel = BOTTOM;
			_alphabetSniper = PointMake(WINSIZEX / 2, 650);
			break;
		}
		break;
	case CHARACTER_DELETE:
		fairyUpdate();
		adjustCharacterInfo();
	}
}

void selectScene::selectionRender(void)
{
	switch (_selectionScreen)
	{
	case CHARACTER_SELECT:
		IMAGEMANAGER->findImage("선택화면")->render(getMemDC());
		IMAGEMANAGER->findImage("요정")->aniRender(getMemDC(), 100, _fairyY, _fairyAni);
		
		for (int i = 0; i < _vName.size(); i++)
		{
			IMAGEMANAGER->findImage(_vName[i])->render(getMemDC(), 350+i*50, 230);
		}
		
		for (int i = 0; i < _vName2.size(); i++)
		{
			IMAGEMANAGER->findImage(_vName2[i])->render(getMemDC(), 350 + i * 50, 340);
		}
		
		for (int i = 0; i < _vName3.size(); i++)
		{
			IMAGEMANAGER->findImage(_vName3[i])->render(getMemDC(), 350 + i * 50, 450);
		}
		break;
	case CHARACTER_NAMING:
		IMAGEMANAGER->findImage("이름")->render(getMemDC());
		for (int i = 0; i < 30; ++i)Rectangle(getMemDC(), _rcCapitalLetter[i].left, _rcCapitalLetter[i].top, _rcCapitalLetter[i].right, _rcCapitalLetter[i].bottom);
		IMAGEMANAGER->findImage("알파벳")->loopRender(getMemDC(), &RectMake(100, 432, 800, 247), _alphabetLoopX, 0);
		IMAGEMANAGER->findImage("세로선")->render(getMemDC(), WINSIZEX / 2, WINSIZEY / 2);
		IMAGEMANAGER->findImage("가로선")->render(getMemDC(), 100, _horizontaliLineLoopY);
		Rectangle(getMemDC(), _alphabetSniper.x, _alphabetSniper.y,_alphabetSniper.x+10,_alphabetSniper.y+10 );
		Rectangle(getMemDC(), _endRect.left, _endRect.top, _endRect.right, _endRect.bottom);
		for (int i = 0; i < 6; i++) Rectangle(getMemDC(), _nameRect[i].left, _nameRect[i].top, _nameRect[i].right, _nameRect[i].bottom);
		makeName();
		break;
	case CHARACTER_DELETE:
		IMAGEMANAGER->findImage("이름지우기")->render(getMemDC());
		IMAGEMANAGER->findImage("요정")->aniRender(getMemDC(), 100, _fairyY, _fairyAni);
		for (int i = 0; i < _vName.size(); i++)
		{
			IMAGEMANAGER->findImage(_vName[i])->render(getMemDC(), 350 + i * 50, 340);
		}

		for (int i = 0; i < _vName2.size(); i++)
		{
			IMAGEMANAGER->findImage(_vName2[i])->render(getMemDC(), 350 + i * 50, 430);
		}

		for (int i = 0; i < _vName3.size(); i++)
		{
			IMAGEMANAGER->findImage(_vName3[i])->render(getMemDC(), 350 + i * 50, 520);
		}
		break;
	}
	char str[123];
	sprintf(str, "%d", _slot);
	TextOut(getMemDC(), 30, 300, str, strlen(str));
}

void selectScene::makeName(void)
{
	if (KEYMANAGER->isOnceKeyDown('Z'))
	{
		
		for (int i = 0; i < 30; i++)
		{
			if (PtInRect(&_rcCapitalLetter[i], _alphabetSniper))
			{
				char str[2];
				sprintf(str, "%c", i + 65);
				_letterImage = new image;
				_letterImage = IMAGEMANAGER->findImage(str);
				_vTempImage.push_back(_letterImage);
				_vStr.push_back(str);
				_letterCount++;
				break;
			}
			if (PtInRect(&_endRect, _alphabetSniper))
			{
				_selectionScreen = CHARACTER_SELECT;
				_slots[_slot].slotCharacter = SLOTOCCUPIED;
				_vTempImage.clear();
				if(_slot== SLOT1)TXTDATA->txtSave("UIManager/SlotName0.txt", _vStr), _vName = TXTDATA->txtLoad("UIManager/SlotName0.txt");;
				if (_slot == SLOT2)TXTDATA->txtSave("UIManager/SlotName1.txt", _vStr), _vName2 = TXTDATA->txtLoad("UIManager/SlotName1.txt");;
				if (_slot == SLOT3)TXTDATA->txtSave("UIManager/SlotName2.txt", _vStr), _vName3 = TXTDATA->txtLoad("UIManager/SlotName2.txt");;
				break;
			}
		}
	}
	for (int i = 0; i < _vTempImage.size(); i++)
	{
		_vTempImage[i]->render(getMemDC(), _nameRect[i].left, _nameRect[i].top);
	}
}
