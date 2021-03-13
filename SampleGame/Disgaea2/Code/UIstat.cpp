#include "stdafx.h"
#include "UIstat.h"


UIstat::UIstat()
{
}


UIstat::~UIstat()
{
}


HRESULT UIstat::init()
{
	
	//// stat.txt ÃÊ±â°ª ÀúÀåÇÏ·Á°í ¸¸µê
 //	charsStat[CHAR_ADELL].sName = "Adell";
	//charsStat[CHAR_ADELL].sClass = "";
	//charsStat[CHAR_ADELL].lv = 1;
	//charsStat[CHAR_ADELL].hp = 500;
	//charsStat[CHAR_ADELL].sp = 100;
	//charsStat[CHAR_ADELL].currentHp = 500;
	//charsStat[CHAR_ADELL].currentSp = 100;
	//charsStat[CHAR_ADELL].atk = 30;
	//charsStat[CHAR_ADELL].def = 30;
	//charsStat[CHAR_ADELL].inte = 10;
	//charsStat[CHAR_ADELL].res = 30;
	//charsStat[CHAR_ADELL].hit = 10;
	//charsStat[CHAR_ADELL].spd = 10;
	//charsStat[CHAR_ADELL].exp = 0;
	//charsStat[CHAR_ADELL].next = 100;

	//charsStat[CHAR_AXEL].sName = "Axel";
	//charsStat[CHAR_AXEL].sClass = "";
	//charsStat[CHAR_AXEL].lv = 1;
	//charsStat[CHAR_AXEL].hp = 700;
	//charsStat[CHAR_AXEL].sp = 70;
	//charsStat[CHAR_AXEL].currentHp = 700;
	//charsStat[CHAR_AXEL].currentSp = 70;
	//charsStat[CHAR_AXEL].atk = 10;
	//charsStat[CHAR_AXEL].def = 100;
	//charsStat[CHAR_AXEL].inte = 10;
	//charsStat[CHAR_AXEL].res = 100;
	//charsStat[CHAR_AXEL].hit = 10;
	//charsStat[CHAR_AXEL].spd = 5;
	//charsStat[CHAR_AXEL].exp = 0;
	//charsStat[CHAR_AXEL].next = 100;

	//charsStat[CHAR_FENRICH].sName = "Fenrich";
	//charsStat[CHAR_FENRICH].sClass = "";
	//charsStat[CHAR_FENRICH].lv = 1;
	//charsStat[CHAR_FENRICH].hp = 200;
	//charsStat[CHAR_FENRICH].sp = 200;
	//charsStat[CHAR_FENRICH].currentHp = 200;
	//charsStat[CHAR_FENRICH].currentSp = 200;
	//charsStat[CHAR_FENRICH].atk = 70;
	//charsStat[CHAR_FENRICH].def = 10;
	//charsStat[CHAR_FENRICH].inte = 20;
	//charsStat[CHAR_FENRICH].res = 10;
	//charsStat[CHAR_FENRICH].hit = 70;
	//charsStat[CHAR_FENRICH].spd = 20;
	//charsStat[CHAR_FENRICH].exp = 0;
	//charsStat[CHAR_FENRICH].next = 100;

	//charsStat[CHAR_PRIER].sName = "Prier";
	//charsStat[CHAR_PRIER].sClass = "";
	//charsStat[CHAR_PRIER].lv = 1;
	//charsStat[CHAR_PRIER].hp = 150;
	//charsStat[CHAR_PRIER].sp = 300;
	//charsStat[CHAR_PRIER].currentHp = 150;
	//charsStat[CHAR_PRIER].currentSp = 300;
	//charsStat[CHAR_PRIER].atk = 10;
	//charsStat[CHAR_PRIER].def = 15;
	//charsStat[CHAR_PRIER].inte = 70;
	//charsStat[CHAR_PRIER].res = 20;
	//charsStat[CHAR_PRIER].hit = 30;
	//charsStat[CHAR_PRIER].spd = 10;
	//charsStat[CHAR_PRIER].exp = 0;
	//charsStat[CHAR_PRIER].next = 100;

	//statSave();
	//

	statLoad();
	charStat = charsStat[0];

	ui = IMAGEMANAGER->addImage("Uistat", "UI_IMAGE/StatusUi2.bmp", 449 * 1.3, 358 * 1.3, true, RGB(255, 0, 255));
	//charStat = stat;
	
	IMAGEMANAGER->addImage("Face_Adell", "image/character/Adell/AdellFace2.bmp", 105 * 1.3, 338 * 1.3, false, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("Face_AKF", "image/character/ArmorKnightFemale/ArmorKnightFemaleFace2.bmp", 105 * 1.3, 338 * 1.3, false, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("Face_Axel", "image/character/Axel/AxelFace2.bmp", 105 * 1.3, 338 * 1.3, false, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("Face_Fenrich", "image/character/Fenrich/FenrichFace2.bmp", 105 * 1.3, 338 * 1.3, false, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("Face_HealerMale", "image/character/HealerMale/HealerMaleFace2.bmp", 105 * 1.3, 338 * 1.3, false, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("Face_Prier", "image/character/Prier/PrierFace2.bmp", 105 * 1.3, 338 * 1.3, false, RGB(255, 0, 255));

	return S_OK;
}
void UIstat::release(void)
{

}
void UIstat::update(void)
{
	
}
void UIstat::render(int x, int y)
{

	string name;
	name = "Face_" + charStat.sName;
	charFace = IMAGEMANAGER->findImage(name);
	ui->render(getMemDC(), x, y);
	charFace->render(getMemDC(), x + 15, y + 13);

	HFONT font, oldfont;
	font = CreateFont(35, 0, 0, 0, 1000, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH, TEXT("¸¼Àº °íµñ"));
	oldfont = (HFONT)SelectObject(getMemDC(), font);
	SetTextColor(getMemDC(), RGB(255, 255, 255));
	SelectObject(getMemDC(), oldfont);

	// STATÃâ·Â
	TextOut(getMemDC(), x + 180, y + 18, charStat.sName.c_str(), strlen(charStat.sName.c_str()));
	TextOut(getMemDC(), x + 180, y + 60, charStat.sClass.c_str(), strlen(charStat.sClass.c_str()));
	
	sprintf(str, "%d", charStat.lv);
	TextOut(getMemDC(), x + 460, y + 30, str, strlen(str));

	sprintf(str, "%d", charStat.hp);
	TextOut(getMemDC(), x + 220, y + 268, str, strlen(str));

	sprintf(str, "%d", charStat.sp);
	TextOut(getMemDC(), x + 220, y + 290, str, strlen(str));

	sprintf(str, "%d", charStat.atk);
	TextOut(getMemDC(), x + 220, y + 315, str, strlen(str));

	sprintf(str, "%d", charStat.def);
	TextOut(getMemDC(), x + 400, y + 315, str, strlen(str));

	sprintf(str, "%d", charStat.inte);
	TextOut(getMemDC(), x + 220, y + 335, str, strlen(str));

	sprintf(str, "%d", charStat.res);
	TextOut(getMemDC(), x + 400, y + 335, str, strlen(str));

	sprintf(str, "%d", charStat.hit);
	TextOut(getMemDC(), x + 220, y + 355, str, strlen(str));

	sprintf(str, "%d", charStat.spd);
	TextOut(getMemDC(), x + 400, y + 355, str, strlen(str));
	
	sprintf(str, "%d", charStat.exp);
	TextOut(getMemDC(), x + 230, y + 410, str, strlen(str));

	sprintf(str, "%d", charStat.next);
	TextOut(getMemDC(), x + 230, y + 430, str, strlen(str));
	DeleteObject(font);
}

void UIstat::statSave()
{
	HANDLE file;
	DWORD write;

	file = CreateFile("stat.txt", GENERIC_WRITE, 0, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	WriteFile(file, charsStat, sizeof(tagStat) * CHAR_END, &write, NULL);

	CloseHandle(file);
}

void UIstat::statLoad()
{
	HANDLE file;
	DWORD read;

	file = CreateFile("stat.txt", GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	ReadFile(file, charsStat, sizeof(tagStat) * CHAR_END, &read, NULL);

	CloseHandle(file);
}