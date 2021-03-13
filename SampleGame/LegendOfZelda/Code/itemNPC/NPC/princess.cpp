#include "stdafx.h"
#include "../../stdafx.h"
#include "princess.h"


princess::princess()
{
}

princess::~princess()
{
}

HRESULT princess::init(void)
{
	npc::init();
	_nState = STATE_ECO;
	speed = 0;
	pause = true;
	procedure = 0;
	rt = { 165, 600, 860, 700 };
	//ECO1
	_mList.insert(make_pair("1_01", "�����ּ���. ���� ���� ���ϰ����� ������ �ֽ��ϴ�."));
	_mList.insert(make_pair("1_02", "�� �̸��� ����...."));
	_mList.insert(make_pair("1_03", "�̹� ������ �������� ��������, ���� ������ �ϳ��Դϴ�."));
	_mList.insert(make_pair("1_04", "���� �ĵ��� ���� �Ʊ״��� �������� �̿���, �ϰ� ������ ������ ������ �մϴ�. �����ּ���."));
	//ECO2
	_mList.insert(make_pair("1_05", "�����ּ���. �� �� ���� ���� �ȿ� �����ֽ��ϴ�. ������ ������ �� �� �ִ� ��� ��ΰ� �ִٰ� �˰� �ֽ��ϴ�."));
	//NONE_MASTER_KEY
	_mList.insert(make_pair("1_06", "�� ���� �����! Ŀ�ٶ� ���踦 ������ ������ �� �� �־��!"));
	//CASTEL ZELDA
	_mList.insert(make_pair("1_07", "������ ��ũ. ����� ������ ���� �ִ� ���� �������."));
	_mList.insert(make_pair("1_08", "�� �������� ��ũ. �����簡 �� ���� ���ε��� �����ϰ� �־��. �� �ƹ������� ���� ���� ���� ������ ������. "));
	_mList.insert(make_pair("1_09", "���� �Ʊ״��� ������ ������ ���� ����� Ż�� �� �Ǹ��Դϴ�!"));
	_mList.insert(make_pair("1_10", "�� ���� �����ϼ̳���?"));
	_mList.insert(make_pair("1_11", "���ƿ�. �Ʊ״��� ��ġä�� ���� �̰��� ����������. ���� ��� ��θ� �˰� �־��. ���� �� ������ �����ؿ�."));

	return S_OK;
}
void princess::release(void)
{
	npc::release();
}
void princess::update(void)
{
	npc::update();
	//chat();
	speed++;

}
void princess::render(void)
{
	npc::render();
	HFONT normal = CreateFont(25, 0, 0, 0, 600, 0, 0, 0, HANGUL_CHARSET, 0, 0, 0, 0, TEXT("HY����L"));
	SetTextColor(getMemDC(), RGB(255, 255, 255));
	SelectObject(getMemDC(), normal);
	if(CurrentMenu == 0) chat();
}

void princess::framePlay()
{
	npc::framePlay();
}

void princess::move(void)
{
	switch (_nState)
	{
		case STATE_ECO:
		break;
		case STATE_UP:
		break;
		case STATE_DOWN:
		break;
		case STATE_LEFT:
		break;
		case STATE_RIGHT:
		break;
	}
}
void princess::chat(void)
{
	switch (_nState)
	{
	case STATE_ECO:
			
		if (KEYMANAGER->isOnceKeyDown('X'))
		{
			procedure++;
			textframe = 0;// strlen(_mIter->second.c_str());
		}

		if (pause && procedure==0)
		{
			Talking = 1;
			_mIter = _mList.find("1_01");
		//	DrawText(getMemDC(), 160, WINSIZEY - 150, _mIter->second.c_str(), textframe);		
			DrawText(getMemDC(), _mIter->second.c_str(), textframe, &rt, DT_LEFT | DT_WORDBREAK);
		}
		if (pause && procedure == 1) //*&& KEYMANAGER->isOnceKeyDown('Z')*/&& textframe > strlen(_mIter->second.c_str()))
		{
			_mIter = _mList.find("1_02");
			DrawText(getMemDC(), _mIter->second.c_str(), textframe, &rt, DT_LEFT | DT_WORDBREAK);
		}
		if (pause &&procedure == 2) // _mIter == _mList.find("1_02") /*&& KEYMANAGER->isOnceKeyDown('Z')*/ && textframe > strlen(_mIter->second.c_str()))
		{
		//	textframe = 0;
			_mIter = _mList.find("1_03");
			DrawText(getMemDC(), _mIter->second.c_str(), textframe, &rt, DT_LEFT | DT_WORDBREAK);
		}
		if (pause && procedure == 3) //_mIter == _mList.find("1_03") /*&& KEYMANAGER->isOnceKeyDown('Z')*/ && textframe > strlen(_mIter->second.c_str()))
		{
		//	textframe = 0;
			_mIter = _mList.find("1_04");
			DrawText(getMemDC(), _mIter->second.c_str(), textframe, &rt, DT_LEFT | DT_WORDBREAK);
		}
		if (pause &&procedure == 4) // _mIter == _mList.find("1_04") /*&& KEYMANAGER->isOnceKeyDown('Z')*/ && textframe > strlen(_mIter->second.c_str()))
		{
		//	textframe = 0;
			pause = false;
			_nState = STATE_NONE;
			procedure = 0;
			Talking = 0;
		}
		if (speed >= 5 && textframe < strlen(_mIter->second.c_str()))
		{
			textframe += 2;
			speed = 0;
		}
		break;
	case STATE_NONE:
		break;
	case STATE_ECO2:

		if (KEYMANAGER->isOnceKeyDown('X'))
		{
			procedure++;
			textframe = 0;// strlen(_mIter->second.c_str());
		}
		if (procedure == 0)
		{
			Talking = 1;
			_mIter = _mList.find("1_05");
			//	DrawText(getMemDC(), 160, WINSIZEY - 150, _mIter->second.c_str(), textframe);		
			DrawText(getMemDC(), _mIter->second.c_str(), textframe, &rt, DT_LEFT | DT_WORDBREAK);
		}
		if (procedure == 1)
		{
			Talking = 0;
			_nState = STATE_NONE;
			procedure = 0;
		}
		if (speed >= 5 && textframe < strlen(_mIter->second.c_str()))
		{
			textframe += 2;
			speed = 0;
		}
		break;
	case STATE_UP:
		break;
	case STATE_DOWN:
		break;
	case STATE_LEFT:
		break;
	case STATE_RIGHT:
		break;
	}
}