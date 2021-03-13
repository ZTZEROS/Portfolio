#include "stdafx.h"
#include "townmap.h"


townmap::townmap()
{
}


townmap::~townmap()
{
}

HRESULT townmap::init(void)
{
	_tileRock = IMAGEMANAGER->addImage("T_돌타일", "image/maptool/돌타일.bmp", TOWN_TILEWIDTH + 5, TOWN_TILEHEIGHT * 2, true, RGB(255, 0, 255));
	_tileGraass = IMAGEMANAGER->addImage("T_풀타일", "image/maptool/풀타일.bmp", TOWN_TILEWIDTH + 5, TOWN_TILEHEIGHT * 2, true, RGB(255, 0, 255));
	_tileStones = IMAGEMANAGER->addImage("T_자갈타일", "image/maptool/자갈타일.bmp", TOWN_TILEWIDTH + 5, TOWN_TILEHEIGHT * 2, true, RGB(255, 0, 255));
	_tileRockTop = IMAGEMANAGER->addImage("T_돌타일상단", "image/maptool/돌타일상단.bmp", TOWN_TILEWIDTH + 5, TOWN_TILEHEIGHT * 2, true, RGB(255, 0, 255));
	_tileGraassTop = IMAGEMANAGER->addImage("T_풀타일상단", "image/maptool/풀타일상단.bmp", TOWN_TILEWIDTH + 5, TOWN_TILEHEIGHT * 2, true, RGB(255, 0, 255));
	_tileStonesTop = IMAGEMANAGER->addImage("T_자갈타일상단", "image/maptool/자갈타일상단.bmp", TOWN_TILEWIDTH + 5, TOWN_TILEHEIGHT * 2, true, RGB(255, 0, 255));
	_tileWater = IMAGEMANAGER->addImage("T_물타일", "image/maptool/물타일.bmp", TOWN_TILEWIDTH, TOWN_TILEHEIGHT, true, RGB(255, 0, 255), true);
	
	_tileDeepTile = IMAGEMANAGER->addImage("T_마을타일1", "image/maptool/tile_town1.bmp", TOWN_TILEWIDTH + 5, TOWN_TILEHEIGHT * 2, true, RGB(255, 0, 255));
	_tileLightTile = IMAGEMANAGER->addImage("T_마을타일2", "image/maptool/tile_town2.bmp", TOWN_TILEWIDTH + 5, TOWN_TILEHEIGHT * 2, true, RGB(255, 0, 255));
	_tileGraveTile = IMAGEMANAGER->addImage("T_마을타일3", "image/maptool/tile_town3.bmp", TOWN_TILEWIDTH + 5, TOWN_TILEHEIGHT * 2, true, RGB(255, 0, 255));
	_obj = IMAGEMANAGER->addFrameImage("T_object2", "image/maptool/오브젝트.bmp", TOWN_TILEWIDTH * 2, TOWN_TILEHEIGHT * 6, 2, 3, true, RGB(255, 0, 255), false);

	load();

	//IMAGEMANAGER->addImage("npc_weaponshop", "image/character/npcs/weaponshop.bmp", 79, 108, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("npc_weaponshop", "image/character/npcs/weaponshop1.bmp", 85 * 0.8, 146 * 0.8, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("npc_armorshop", "image/character/npcs/armorshop.bmp", 102 * 0.8, 138 * 0.8, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("npc_asagi", "image/character/npcs/asagi.bmp", 88 * 0.8, 150 * 0.8, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("npc_medic", "image/character/npcs/medic.bmp", 65 * 0.8, 157 * 0.8, true, RGB(255, 0, 255));
	
	
	npc[NPC_WEAPONSHOP].img = IMAGEMANAGER->findImage("npc_weaponshop");
	npc[NPC_WEAPONSHOP].x = 3;
	npc[NPC_WEAPONSHOP].y = 7;
	npc[NPC_WEAPONSHOP].z = 0;
	npc[NPC_WEAPONSHOP].state = 1;

	npc[NPC_ARMORSHOP].img = IMAGEMANAGER->findImage("npc_armorshop");
	npc[NPC_ARMORSHOP].x = 3;
	npc[NPC_ARMORSHOP].y = 8;
	npc[NPC_ARMORSHOP].z = 0;
	npc[NPC_ARMORSHOP].state = 1;

	npc[NPC_WARP].img = IMAGEMANAGER->findImage("npc_asagi");
	npc[NPC_WARP].img_uz = IMAGEMANAGER->addFrameImage("npc_asagi_uz", "image/character/npcs/asagi_uz.bmp", 528 * 0.8, 150 * 0.8, 6, 1, true, RGB(255, 0, 255));
	npc[NPC_WARP].x = 7;
	npc[NPC_WARP].y = 0;
	npc[NPC_WARP].z = 3;
	npc[NPC_WARP].state = 1;
	npc[NPC_WARP].timer = 0;
	npc[NPC_WARP].currentX = 0;
	npc[NPC_WARP].currentY = 0;

	npc[NPC_HOSPITAL].img = IMAGEMANAGER->findImage("npc_medic");
	npc[NPC_HOSPITAL].img_uz = IMAGEMANAGER->addFrameImage("npc_medic_uz", "image/character/npcs/medic_uz.bmp", 325 * 0.8, 157 * 0.8, 5, 1, true, RGB(255, 0, 255));
	npc[NPC_HOSPITAL].x = 9;
	npc[NPC_HOSPITAL].y = 8;
	npc[NPC_HOSPITAL].z = 2;
	npc[NPC_HOSPITAL].state = 1;
	npc[NPC_HOSPITAL].timer = 0;
	npc[NPC_HOSPITAL].currentX = 0;
	npc[NPC_HOSPITAL].currentY = 0;

	for (int i = 0; i < NPC_END; i++)
	{
		npc[i].rc = RectMake((npc[i].x - npc[i].y) * TOWN_TILEWIDTH * 0.5 + BACKBUFFERSIZEX * 0.5 - (TOWN_TILEWIDTH * 0.5) + ((TOWN_TILEWIDTH - npc[i].img->getWidth()) / 2) - REVX,
			(npc[i].x + npc[i].y) * TOWN_TILEHEIGHT * 0.5 - npc[i].z*TOWN_TILEHEIGHT / 2.1 + BACKBUFFERSIZEY / 2.5 - (npc[i].img->getHeight() - TOWN_TILEHEIGHT),
			npc[i].img->getWidth(),
			npc[i].img->getHeight());
	}

	// TOWN MENU ------------------------------------ ★
	whatShowing = NOW_NONE;
	townMenu = IMAGEMANAGER->addImage("town_ui", "UI_IMAGE/maeulUI.bmp", 244, 119, true, RGB(255, 0, 255));

		// WARP CHAT
	ui_chat = IMAGEMANAGER->addImage("ui_chat", "UI_IMAGE/textBoxUI.bmp", WINSIZEX - 100, 112 * 1.5, true, RGB(255, 0, 255));
	warp_face = IMAGEMANAGER->addImage("asagi_faceSmall", "image/character/npcs/AsagiFace.bmp", 127, 130, true, RGB(255, 0, 255));
	hospital_face = IMAGEMANAGER->addImage("medic_faceSmall", "image/character/npcs/MedicFace.bmp", 127, 130, true, RGB(255, 0, 255));

	_asv = new armorStoreView;
	_asv->init();

	_wsv = new weaponStoreView;
	_wsv->init();

	_uistat = new UIstat;
	_uistat->init();

	openStore = false;

	money = 15000; //최초 프로그램 켰을 때 가지고있는 돈

	num = 0;
	return S_OK;
}

void townmap::release(void)
{

}

void townmap::update(void)
{
	CAMERA->update();
	CAMERA->setFocusY(BACKBUFFERSIZEY * 0.5-100);
	CAMERA->setFocusX(BACKBUFFERSIZEX * 0.5);

	// TOWN MENU-----------------------------★
	menuRc[TOWNMENU_ITEM] = RectMake(CAMERA->getCameraX() + WINSIZEX - 300, CAMERA->getCameraY() + 50, townMenu->getWidth(), townMenu->getHeight() / 2);
	menuRc[TOWNMENU_EQUIP] = RectMake(menuRc[TOWNMENU_ITEM].left, menuRc[TOWNMENU_ITEM].top + townMenu->getHeight() / 2, townMenu->getWidth(), townMenu->getHeight() / 2);

	//여기서는 걍 루프를 돌면서 실행되는 부분이 실행될 부분
	switch (whatShowing)
	{
	case NOW_NONE:
		openStore = false;
		if (KEYMANAGER->isOnceKeyDown('V')) whatShowing = NOW_MENU;		
		//클릭 했을 때 상태를 결정짓는 스위칭 부분.
		if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			for (int i = 0; i < NPC_END; i++)
			{
				if (PtInRect(&npc[i].rc, PointMake(_ptMouse.x + CAMERA->getCameraX() + CAMERA->getWheelValue() / 2, _ptMouse.y + CAMERA->getCameraY() + CAMERA->getWheelValue() / 4)))
				{
					switch (i)
					{
						case NPC_WEAPONSHOP:
							whatShowing = NOW_WEAPONSHOP;
						break;

						case NPC_ARMORSHOP:
							whatShowing = NOW_ARMORSHOP;
						break;

						case NPC_WARP:
							//npc[NPC_WARP].state = 0; //아사기는 움직이는게 어색해;;
							whatShowing = NOW_WARPCHAT;
						break;

						case NPC_HOSPITAL:
							npc[NPC_HOSPITAL].state = 0;
							whatShowing = NOW_HOSPITALCHAT;
						break;

					}
				}
			}
		}
	break;

	case NOW_WEAPONSHOP:
		openStore = true;
		_wsv->update(money);
		money = _wsv->getCurrentMoney();
	break;

	case NOW_ARMORSHOP:
		openStore = true;
		_asv->update(money);
		money = _asv->getCurrentMoney();			
	break;

	case NOW_WARPCHAT:
	if (KEYMANAGER->isOnceKeyDown('X'))
	{
		_uistat->statSave();
		SOUNDMANAGER->stop("townBgm");
		SOUNDMANAGER->play("battleBgm", 0.5f);
		SCENEMANAGER->changeScene("battle");
	}
	openStore = false;
	break;

	case NOW_HOSPITALCHAT:
		if (KEYMANAGER->isOnceKeyDown('X'))
		{
			for (int i = 0; i < CHAR_END; i++)
			{
				_uistat->setCurrectHP(i, _uistat->getCharStatIndex(i).hp);
				_uistat->setCurrectSP(i, _uistat->getCharStatIndex(i).sp);
			}
			whatShowing = NOW_NONE;
		}
	openStore = false;
	break;

	case NOW_MENU:
		if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			for (int i = 0; i < TOWNMENU_END; i++)
			{
				if (PtInRect(&menuRc[i], PointMake(_ptMouse.x + CAMERA->getCameraX() + CAMERA->getWheelValue() / 2, _ptMouse.y + CAMERA->getCameraY() + CAMERA->getWheelValue() / 4)))
				{
					switch (i)
					{
					case TOWNMENU_ITEM:
						whatShowing = NOW_MENU_ITEM;
						break;
					case TOWNMENU_EQUIP:
						whatShowing = NOW_MENU_EQUIP;
						break;
					}
				}
			}
		}
	openStore = false;
	break;

	case NOW_MENU_ITEM:

	break;

	case NOW_MENU_EQUIP:

		if (KEYMANAGER->isOnceKeyDown(VK_LEFT))
		{
			if(num >= 0) num--;
		}

		if (KEYMANAGER->isOnceKeyDown(VK_RIGHT))
		{
			if (num < CHAR_END - 1) num++;
		}

		_uistat->setCharStatIndex(num);
	openStore = false;
	break;
	}

	if (KEYMANAGER->isOnceKeyDown('M'))
	{
		whatShowing = NOW_NONE;
	}

	// 캐릭터 상태값 랜덤으로 바꿔주기
	
	//if (RND->getFromIntTo(0, 10) == 0) npc[NPC_WEAPONSHOP].state = 0;
	//if (RND->getFromIntTo(0, 10) == 0) npc[NPC_ARMORSHOP].state = 0;
	//if (RND->getFromIntTo(0, 10) == 0) npc[NPC_WARP].state = 0;

}

void townmap::render(void)
{
	for (int i = 0; i < TILENUMX;i++)
	{
		for (int j = 0; j < TILENUMY; j++)
		{
			if (_tiles[j][i].terrain == TR_GRASS)_tileGraass->render(getMemDC(), (j - i) * TOWN_TILEWIDTH * 0.5 + BACKBUFFERSIZEX * 0.5 - (TOWN_TILEWIDTH * 0.5) - REVX, (j + i) * TOWN_TILEHEIGHT * 0.5 + BACKBUFFERSIZEY / 2.5);
			else if (_tiles[j][i].terrain == TR_STONES)_tileStones->render(getMemDC(), (j - i) * TOWN_TILEWIDTH * 0.5 + BACKBUFFERSIZEX * 0.5 - (TOWN_TILEWIDTH * 0.5) - REVX, (j + i) * TOWN_TILEHEIGHT * 0.5 + BACKBUFFERSIZEY / 2.5);
			else if (_tiles[j][i].terrain == TR_ROCK)_tileRock->render(getMemDC(), (j - i) * TOWN_TILEWIDTH * 0.5 + BACKBUFFERSIZEX * 0.5 - (TOWN_TILEWIDTH * 0.5) - REVX, (j + i) * TOWN_TILEHEIGHT * 0.5 + BACKBUFFERSIZEY / 2.5);
			else if (_tiles[j][i].terrain == TR_WATER)_tileWater->alphaRender(getMemDC(), (j - i) * TOWN_TILEWIDTH * 0.5 + BACKBUFFERSIZEX * 0.5 - (TOWN_TILEWIDTH * 0.5) - REVX, (j + i) * TOWN_TILEHEIGHT * 0.5 + TOWN_TILEHEIGHT / 2 - _tiles[j][i].altitude*TOWN_TILEHEIGHT / 2.1 + BACKBUFFERSIZEY / 3, 150);
			else if (_tiles[j][i].terrain == TR_DEEPTILE)_tileDeepTile->render(getMemDC(), (j - i) * TOWN_TILEWIDTH * 0.5 + BACKBUFFERSIZEX * 0.5 - (TOWN_TILEWIDTH * 0.5) - REVX, (j + i) * TOWN_TILEHEIGHT * 0.5 + BACKBUFFERSIZEY / 2.5);
			else if (_tiles[j][i].terrain == TR_LIGHTTILE)_tileLightTile->render(getMemDC(), (j - i) * TOWN_TILEWIDTH * 0.5 + BACKBUFFERSIZEX * 0.5 - (TOWN_TILEWIDTH * 0.5) - REVX, (j + i) * TOWN_TILEHEIGHT * 0.5 + BACKBUFFERSIZEY / 2.5);
			else if (_tiles[j][i].terrain == TR_GRAVELTILE)_tileGraveTile->render(getMemDC(), (j - i) * TOWN_TILEWIDTH * 0.5 + BACKBUFFERSIZEX * 0.5 - (TOWN_TILEWIDTH * 0.5) - REVX, (j + i) * TOWN_TILEHEIGHT * 0.5 + BACKBUFFERSIZEY / 2.5);

			if (_tiles[j][i].altitude > 0)
			{
				for (int k = 1; k < _tiles[j][i].altitude + 1; k++)
				{
					if (_tiles[j][i].terrain == TR_GRASS)_tileGraassTop->render(getMemDC(), (j - i) * TOWN_TILEWIDTH * 0.5 + BACKBUFFERSIZEX * 0.5 - (TOWN_TILEWIDTH * 0.5) - REVX, (j + i) * TOWN_TILEHEIGHT * 0.5 - TOWN_TILEHEIGHT / 2.1 *k + BACKBUFFERSIZEY / 2.5);
					else if (_tiles[j][i].terrain == TR_STONES)_tileStonesTop->render(getMemDC(), (j - i) * TOWN_TILEWIDTH * 0.5 + BACKBUFFERSIZEX * 0.5 - (TOWN_TILEWIDTH * 0.5) - REVX, (j + i) * TOWN_TILEHEIGHT * 0.5 - TOWN_TILEHEIGHT / 2.1 * k + BACKBUFFERSIZEY / 2.5);
					else if (_tiles[j][i].terrain == TR_ROCK)_tileRockTop->render(getMemDC(), (j - i) * TOWN_TILEWIDTH * 0.5 + BACKBUFFERSIZEX * 0.5 - (TOWN_TILEWIDTH * 0.5) - REVX, (j + i) * TOWN_TILEHEIGHT * 0.5 - TOWN_TILEHEIGHT / 2.1 * k + BACKBUFFERSIZEY / 2.5);
					else if (_tiles[j][i].terrain == TR_DEEPTILE)_tileDeepTile->render(getMemDC(), (j - i) * TOWN_TILEWIDTH * 0.5 + BACKBUFFERSIZEX * 0.5 - (TOWN_TILEWIDTH * 0.5) - REVX, (j + i) * TOWN_TILEHEIGHT * 0.5 - TOWN_TILEHEIGHT / 2.1 *k + BACKBUFFERSIZEY / 2.5);
					else if (_tiles[j][i].terrain == TR_LIGHTTILE)_tileLightTile->render(getMemDC(), (j - i) * TOWN_TILEWIDTH * 0.5 + BACKBUFFERSIZEX * 0.5 - (TOWN_TILEWIDTH * 0.5) - REVX, (j + i) * TOWN_TILEHEIGHT * 0.5 - TOWN_TILEHEIGHT / 2.1 * k + BACKBUFFERSIZEY / 2.5);
					else if (_tiles[j][i].terrain == TR_GRAVELTILE)_tileGraveTile->render(getMemDC(), (j - i) * TOWN_TILEWIDTH * 0.5 + BACKBUFFERSIZEX * 0.5 - (TOWN_TILEWIDTH * 0.5) - REVX, (j + i) * TOWN_TILEHEIGHT * 0.5 - TOWN_TILEHEIGHT / 2.1 * k + BACKBUFFERSIZEY / 2.5);
				}
			}

			if (_tiles[j][i].obj == OBJ_PUPLE_FLOWER)_obj->frameRender(getMemDC(), (j - i) * TOWN_TILEWIDTH * 0.5 + BACKBUFFERSIZEX * 0.5 - (TOWN_TILEWIDTH * 0.5) - REVX, (j + i) * TOWN_TILEHEIGHT * 0.5 - _tiles[j][i].altitude*TOWN_TILEHEIGHT / 2.1 + BACKBUFFERSIZEY / 2.5, 0, 0);
			else if (_tiles[j][i].obj == OBJ_WHITE_FLOWER)_obj->frameRender(getMemDC(), (j - i) * TOWN_TILEWIDTH * 0.5 + BACKBUFFERSIZEX * 0.5 - (TOWN_TILEWIDTH * 0.5) - REVX, (j + i) * TOWN_TILEHEIGHT * 0.5 - _tiles[j][i].altitude*TOWN_TILEHEIGHT / 2.1 + BACKBUFFERSIZEY / 2.5, 1, 0);
			else if (_tiles[j][i].obj == OBJ_WEEDS)_obj->frameRender(getMemDC(), (j - i) * TOWN_TILEWIDTH * 0.5 + BACKBUFFERSIZEX * 0.5 - (TOWN_TILEWIDTH * 0.5) - REVX, (j + i) * TOWN_TILEHEIGHT * 0.5 - _tiles[j][i].altitude*TOWN_TILEHEIGHT / 2.1 - 57 + BACKBUFFERSIZEY / 2.5, 0, 1);
			else if (_tiles[j][i].obj == OBJ_PEBBLE)_obj->frameRender(getMemDC(), (j - i) * TOWN_TILEWIDTH * 0.5 + BACKBUFFERSIZEX * 0.5 - (TOWN_TILEWIDTH * 0.5) - REVX, (j + i) * TOWN_TILEHEIGHT * 0.5 - _tiles[j][i].altitude*TOWN_TILEHEIGHT / 2.1 - 50 + BACKBUFFERSIZEY / 2.5, 1, 1);
			else if (_tiles[j][i].obj == OBJ_YELLOW_FLOWER)_obj->frameRender(getMemDC(), (j - i) * TOWN_TILEWIDTH * 0.5 + BACKBUFFERSIZEX * 0.5 - (TOWN_TILEWIDTH * 0.5) - REVX, (j + i) * TOWN_TILEHEIGHT * 0.5 - _tiles[j][i].altitude*TOWN_TILEHEIGHT / 2.1 - 50 + BACKBUFFERSIZEY / 2.5, 0, 2);
		
			for (int k = 0; k < NPC_END; k++)
			{
				if (npc[k].x == j && npc[k].y == i && npc[k].state == 0)
				{
					npc[k].timer++;
					if (npc[k].timer % 8 == 0)
					{
						npc[k].currentX++;
						if (npc[k].currentX >= npc[k].img_uz->getMaxFrameX())
						{
							npc[k].timer = 0;
							npc[k].currentX = 0;
							npc[k].state = 1;
						}
					}
					npc[k].img_uz->frameRender(getMemDC(),
						(j - i) * TOWN_TILEWIDTH * 0.5 + BACKBUFFERSIZEX * 0.5 - (TOWN_TILEWIDTH * 0.5) + ((TOWN_TILEWIDTH - npc[k].img->getWidth()) / 2) - REVX,
						(j + i) * TOWN_TILEHEIGHT * 0.5 - npc[k].z*TOWN_TILEHEIGHT / 2.1 + BACKBUFFERSIZEY / 2.5 - (npc[k].img->getHeight() - TOWN_TILEHEIGHT),
						npc[k].currentX,
						npc[k].currentY);
				}

				if (npc[k].x == j && npc[k].y == i && npc[k].state == 1)
				{
					npc[k].img->render(getMemDC(),
						(j - i) * TOWN_TILEWIDTH * 0.5 + BACKBUFFERSIZEX * 0.5 - (TOWN_TILEWIDTH * 0.5) + ((TOWN_TILEWIDTH - npc[k].img->getWidth()) / 2) - REVX,
						(j + i) * TOWN_TILEHEIGHT * 0.5 - npc[k].z*TOWN_TILEHEIGHT / 2.1 + BACKBUFFERSIZEY / 2.5 - (npc[k].img->getHeight() - TOWN_TILEHEIGHT));
				}

					//Rectangle(getMemDC(), npc[k].rc.left, npc[k].rc.top, npc[k].rc.right, npc[k].rc.bottom);
			}
			
		}
	}

	switch (whatShowing)
	{
		case NOW_NONE:
		break;
		case NOW_WEAPONSHOP:
			_wsv->render();
		break;
		case NOW_ARMORSHOP:
			_asv->render();
		break;
		case NOW_WARPCHAT:

			POINT pt;
			chat = "필드로 이동합니다 ★";

			pt.x = CAMERA->getCameraX() + 50;
			pt.y = CAMERA->getCameraY() + WINSIZEY - ui_chat->getHeight();
			ui_chat->render(getMemDC(), pt.x, pt.y);
			warp_face->render(getMemDC(), pt.x + 62, pt.y + 18);

			HFONT font, oldfont;
			font = CreateFont(35, 0, 0, 0, 1000, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH, TEXT("맑은 고딕"));
			oldfont = (HFONT)SelectObject(getMemDC(), font);
			SetTextColor(getMemDC(), RGB(0, 0, 0));
			SelectObject(getMemDC(), oldfont);

			TextOut(getMemDC(), pt.x + 220, pt.y + 40, chat.c_str(), strlen(chat.c_str()));
			
			

		break;

		case NOW_HOSPITALCHAT:

			SetTextColor(getMemDC(), RGB(0, 0, 0));
			SelectObject(getMemDC(), oldfont);
			chat = "HP와 SP 모두 회복됩니다 ★";
			pt.x = CAMERA->getCameraX() + 50;
			pt.y = CAMERA->getCameraY() + WINSIZEY - ui_chat->getHeight();
			ui_chat->render(getMemDC(), pt.x, pt.y);
			hospital_face->render(getMemDC(), pt.x + 62, pt.y + 18);

			TextOut(getMemDC(), pt.x + 220, pt.y + 40, chat.c_str(), strlen(chat.c_str()));

			DeleteObject(font);
			break;

		case NOW_MENU:
			townMenu->render(getMemDC(), menuRc[TOWNMENU_ITEM].left, menuRc[TOWNMENU_ITEM].top);
		break;
		case NOW_MENU_ITEM:
			
			break;
		case NOW_MENU_EQUIP:
			_uistat->render(CAMERA->getCameraX() + 100, CAMERA->getCameraY() + 50);
			break;
		case NOW_END:
		break;
		default:
		break;
	}

	char strr[128];
	sprintf(strr, "whatShowing : %d", whatShowing);
	TextOut(getMemDC(), CAMERA->getCameraX() + 150, CAMERA->getCameraY(), strr, strlen(strr));
}

void townmap::load(void)
{
	HANDLE file;
	DWORD read;

	file = CreateFile("townmap.map", GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	ReadFile(file, _tiles, sizeof(tagTile) * TILENUMX * TILENUMY, &read, NULL);

	CloseHandle(file);
}
