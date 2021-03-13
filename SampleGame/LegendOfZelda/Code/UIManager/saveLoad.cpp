#include "stdafx.h"
#include "saveLoad.h"

saveLoad::saveLoad() {}
saveLoad::~saveLoad() {}

HRESULT saveLoad::Initialize()
{
	Image = IMAGEMANAGER->findImage("TextBox");

	Cursor.SetImage(IMAGEMANAGER->findImage("LinkHead"));
	Cursor.Initialize();
	Cursor.SetCenterX(185);
	Cursor.SetCenterY(50 + WINSIZEY * 5 / 7);

	return S_OK;
}

void saveLoad::Release()
{}

void saveLoad::Update()
{
	Cursor.Update();
}

void saveLoad::Render()
{
	Image->render(getMemDC(), 135, WINSIZEY * 5 / 7);
	
	HFONT InnerFont = CreateFont(25, 0, 0, 0, 100, 0, 0, 0, HANGUL_CHARSET, 0, 0, 0, 0, TEXT("�޸ձ���ü"));
	//HFONT OuterFont = CreateFont(25, 0, 0, 0, 1000, 0, 0, 0, HANGUL_CHARSET, 0, 0, 0, 0, TEXT("�޸ձ���ü"));
	
	//SetTextColor(getMemDC(), RGB(0, 0, 255));
	//SelectObject(getMemDC(), OuterFont);
	//for���� center ���� rectangle�� ���� ����
	//DrawText(getMemDC(), "�����ϰ� ����", strlen("�����ϰ� ����"), &RectMake(200, 50 + WINSIZEY * 3 / 4, Image->getWidth(), Image->getHeight()), DT_LEFT | DT_WORDBREAK);
	//DrawText(getMemDC(), "�������� �ʰ� ����", strlen("�������� �ʰ� ����"), &RectMake(200, 100 + WINSIZEY * 3 / 4, Image->getWidth(), Image->getHeight()), DT_LEFT | DT_WORDBREAK);
	SetTextColor(getMemDC(), RGB(255, 255, 255));
	SelectObject(getMemDC(), InnerFont);
	DrawText(getMemDC(), "�����ϰ� ����", strlen("�����ϰ� ����"), &RectMake(235, 50 + WINSIZEY * 5 / 7, Image->getWidth(), Image->getHeight()), DT_LEFT | DT_WORDBREAK);
	DrawText(getMemDC(), "�������� �ʰ� ����", strlen("�������� �ʰ� ����"), &RectMake(235, 100 + WINSIZEY * 5 / 7, Image->getWidth(), Image->getHeight()), DT_LEFT | DT_WORDBREAK);
	//DeleteObject(OuterFont);
	DeleteObject(InnerFont);
	
	Cursor.Render();
}